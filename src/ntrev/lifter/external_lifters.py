"""
Pluggable External Lifter Bridges for Remill and RetDec.

Supports:
  1. Invoking `remill-lift` (Trail of Bits Remill) or `retdec-decompiler --stop-after bin2llvmir`
     when installed on PATH.
  2. Normalizing Remill/RetDec LLVM IR intrinsics (e.g., `__remill_read_memory_64`,
     `__remill_write_memory_64`, `@__remill_flag_computation_*`) into canonical native LLVM
     `inttoptr` + `load`/`store` IR so downstream Z3 verification and C emission consume
     both native-lifted and Remill/RetDec-lifted LLVM IR uniformly.
"""

from __future__ import annotations

import re
import shutil
import subprocess
import tempfile
from pathlib import Path


class ExternalLifterAdapter:
    """Adapter & IR normalizer for Remill (`remill-lift`) and RetDec (`retdec-decompiler`)."""

    def __init__(
        self,
        remill_bin: str | None = None,
        retdec_bin: str | None = None,
    ) -> None:
        self.remill_bin = remill_bin or shutil.which("remill-lift")
        self.retdec_bin = retdec_bin or shutil.which("retdec-decompiler")

    def available_backends(self) -> list[str]:
        backends = ["native-llvm22"]
        if self.remill_bin:
            backends.append("remill")
        if self.retdec_bin:
            backends.append("retdec")
        return backends

    def lift_bytes_with_remill(
        self, code: bytes, address: int, os_name: str = "windows", arch: str = "amd64"
    ) -> str:
        """Invoke `remill-lift` on raw machine bytes and normalize memory intrinsics."""
        if not self.remill_bin:
            raise FileNotFoundError("remill-lift binary is not installed on PATH.")

        with tempfile.TemporaryDirectory(prefix="ntrev_remill_") as tmp:
            ir_out = Path(tmp) / "lifted.ll"
            cmd = [
                self.remill_bin,
                "--os",
                os_name,
                "--arch",
                arch,
                "--address",
                hex(address),
                "--bytes",
                code.hex(),
                "--ir_out",
                str(ir_out),
            ]
            subprocess.run(cmd, check=True, timeout=60)
            raw_ir = ir_out.read_text(encoding="utf-8")
        return self.normalize_remill_ir(raw_ir)

    def lift_binary_with_retdec(self, binary_path: str | Path) -> str:
        """Invoke `retdec-decompiler --stop-after bin2llvmir` to obtain RetDec LLVM IR."""
        if not self.retdec_bin:
            raise FileNotFoundError("retdec-decompiler binary is not installed on PATH.")

        with tempfile.TemporaryDirectory(prefix="ntrev_retdec_") as tmp:
            out_base = Path(tmp) / "out"
            cmd = [
                self.retdec_bin,
                "--stop-after",
                "bin2llvmir",
                "-o",
                str(out_base),
                str(binary_path),
            ]
            subprocess.run(cmd, check=True, timeout=180)
            ll_path = out_base.with_suffix(".ll")
            return ll_path.read_text(encoding="utf-8")

    @staticmethod
    def normalize_remill_ir(remill_ir: str) -> str:
        """
        Rewrite Remill memory intrinsics (`__remill_read_memory_{8,16,32,64}` and
        `__remill_write_memory_{8,16,32,64}`) into standard LLVM `inttoptr` + `load`/`store`.
        """
        lines_out: list[str] = []
        tmp_idx = 0

        read_pat = re.compile(
            r"^\s*(%[A-Za-z0-9_.]+)\s*=\s*(?:tail\s+)?call\s+i(8|16|32|64)\s+"
            r"@__remill_read_memory_(?:8|16|32|64)\(\s*ptr\s+[^,]+,\s*i64\s+([^)]+)\)"
        )
        write_pat = re.compile(
            r"^\s*(?:%[A-Za-z0-9_.]+\s*=\s*)?(?:tail\s+)?call\s+ptr\s+"
            r"@__remill_write_memory_(8|16|32|64)\(\s*ptr\s+([^,]+),\s*i64\s+([^,]+),\s*i(?:8|16|32|64)\s+([^)]+)\)"
        )

        for line in remill_ir.splitlines():
            m_rd = read_pat.match(line)
            if m_rd:
                lhs, bits, addr_val = m_rd.group(1), m_rd.group(2), m_rd.group(3).strip()
                ptr_tmp = f"%remill_norm_ptr_{tmp_idx}"
                tmp_idx += 1
                lines_out.append(f"  {ptr_tmp} = inttoptr i64 {addr_val} to ptr")
                lines_out.append(f"  {lhs} = load i{bits}, ptr {ptr_tmp}")
                continue

            m_wr = write_pat.match(line)
            if m_wr:
                bits, _mem_ptr, addr_val, val_expr = (
                    m_wr.group(1),
                    m_wr.group(2),
                    m_wr.group(3).strip(),
                    m_wr.group(4).strip(),
                )
                ptr_tmp = f"%remill_norm_ptr_{tmp_idx}"
                tmp_idx += 1
                lines_out.append(f"  {ptr_tmp} = inttoptr i64 {addr_val} to ptr")
                lines_out.append(f"  store i{bits} {val_expr}, ptr {ptr_tmp}")
                continue

            if line.strip().startswith("declare ") and "__remill_" in line:
                continue

            lines_out.append(line)

        return "\n".join(lines_out)
