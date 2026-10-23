"""Shared pytest fixtures and inline GNU assembler helper for x86-64 CFG testing."""

from __future__ import annotations

import subprocess
import sys
import tempfile
from pathlib import Path

import pytest

# Make `examples/` importable for fixture synthesis
ROOT_DIR = Path(__file__).resolve().parents[1]
if str(ROOT_DIR / "examples") not in sys.path:
    sys.path.insert(0, str(ROOT_DIR / "examples"))

from build_ntoskrnl_fixture import _extract_elf_functions, build_ntoskrnl_mini_fixture  # noqa: E402
from ntrev.frontend import X86Disassembler  # noqa: E402
from ntrev.models import FunctionCFG  # noqa: E402


def assemble_function_cfg(
    asm_body: str,
    func_name: str = "test_fn",
    base_va: int = 0x140001000,
) -> FunctionCFG:
    """Assemble an Intel-syntax x86-64 snippet with GNU `as`/`gcc` and return its `FunctionCFG`."""
    full_asm = (
        ".intel_syntax noprefix\n"
        ".text\n"
        f".globl {func_name}\n"
        f".type {func_name}, @function\n"
        f"{func_name}:\n"
        f"{asm_body}\n"
        f".size {func_name}, .-{func_name}\n"
    )
    with tempfile.TemporaryDirectory(prefix="ntrev_asm_") as tmp:
        s_path = Path(tmp) / "snippet.s"
        o_path = Path(tmp) / "snippet.o"
        s_path.write_text(full_asm, encoding="utf-8")
        subprocess.run(["gcc", "-c", str(s_path), "-o", str(o_path)], check=True)
        extracted = _extract_elf_functions(o_path)
        assert extracted, f"No function extracted for {func_name}"
        _, code_bytes = extracted[0]

    disasm = X86Disassembler()
    return disasm.build_function_cfg(
        name=func_name,
        code=code_bytes,
        base_va=base_va,
        has_pdata_unwind=True,
    )


@pytest.fixture(scope="session")
def kernel_fixture_paths(tmp_path_factory: pytest.TempPathFactory) -> tuple[Path, Path]:
    out_dir = tmp_path_factory.mktemp("ntoskrnl_fixture")
    return build_ntoskrnl_mini_fixture(out_dir)
