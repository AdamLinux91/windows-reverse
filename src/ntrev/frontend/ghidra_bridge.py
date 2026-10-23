"""
Programmatic Bridge to Ghidra's Python / Java Headless Analysis API.

Provides:
  1. `run_headless_analysis`: Invokes Ghidra's `analyzeHeadless` with our bundled
     `ghidra_scripts/ExportNtAnalysis.py` (or `GhidraNtBridge.java`) post-script to extract
     DataTypeManager structs, DecompInterface HighFunction / PcodeOpAST graphs, and symbols.
  2. `load_ghidra_export`: Imports a JSON analysis bundle produced by Ghidra into
     `PDBDatabase` structures & symbols so the Symbol/Structural Mapper and Z3 Verifier
     consume Ghidra's Programmatic API outputs seamlessly.
"""

from __future__ import annotations

import json
import os
import shutil
import subprocess
import tempfile
from pathlib import Path

from ntrev.models import PDBDatabase, PDBSymbol, StructDef, StructField


class GhidraHeadlessBridge:
    """Adapter for Ghidra's `analyzeHeadless` Python/Java programmatic analysis APIs."""

    def __init__(self, ghidra_install_dir: str | Path | None = None) -> None:
        env_dir = os.environ.get("GHIDRA_INSTALL_DIR")
        resolved = ghidra_install_dir or env_dir
        self.ghidra_dir = Path(resolved) if resolved else None
        self.scripts_dir = Path(__file__).resolve().parents[3] / "ghidra_scripts"

    def is_available(self) -> bool:
        if self.ghidra_dir:
            headless = self.ghidra_dir / "support" / "analyzeHeadless"
            if headless.exists():
                return True
        return shutil.which("analyzeHeadless") is not None

    def headless_binary(self) -> str:
        if self.ghidra_dir:
            candidate = self.ghidra_dir / "support" / "analyzeHeadless"
            if candidate.exists():
                return str(candidate)
        found = shutil.which("analyzeHeadless")
        if not found:
            raise FileNotFoundError(
                "Ghidra 'analyzeHeadless' not found. Set GHIDRA_INSTALL_DIR or pass "
                "--ghidra-json <exported_analysis.json>."
            )
        return found

    def run_headless_analysis(
        self,
        binary_path: str | Path,
        output_json_path: str | Path,
        timeout_sec: int = 300,
    ) -> PDBDatabase:
        """Execute Ghidra headless analyzer + ExportNtAnalysis.py and parse results."""
        headless_bin = self.headless_binary()
        out_path = Path(output_json_path).resolve()

        with tempfile.TemporaryDirectory(prefix="ntrev_ghidra_") as proj_dir:
            cmd = [
                headless_bin,
                proj_dir,
                "NtRevProject",
                "-import",
                str(Path(binary_path).resolve()),
                "-scriptPath",
                str(self.scripts_dir),
                "-postScript",
                "ExportNtAnalysis.py",
                str(out_path),
                "-deleteProject",
            ]
            subprocess.run(cmd, check=True, timeout=timeout_sec)

        return self.load_ghidra_export(out_path)

    @staticmethod
    def load_ghidra_export(json_path: str | Path) -> PDBDatabase:
        """Import a JSON bundle produced by `ghidra_scripts/ExportNtAnalysis.py`."""
        data = json.loads(Path(json_path).read_text(encoding="utf-8"))
        image_base = int(data.get("image_base", "0x140000000"), 0)

        db = PDBDatabase(
            pdb_path=str(json_path),
            guid=data.get("program_guid", "GHIDRA-EXPORT"),
            age=1,
        )

        for st in data.get("structures", []):
            fields = [
                StructField(
                    name=f["name"],
                    offset=int(f["offset"]),
                    size=int(f["size"]),
                    c_type=f["c_type"],
                    is_pointer=bool(f.get("is_pointer", False)),
                    pointee_type=f.get("pointee_type"),
                )
                for f in st.get("fields", [])
            ]
            sdef = StructDef(
                name=st["name"],
                size=int(st["size"]),
                fields=fields,
                typedef_name=st["name"].lstrip("_"),
                pointer_typedef=f"P{st['name'].lstrip('_')}",
                is_union=bool(st.get("is_union", False)),
                source="ghidra",
            )
            db.structs[sdef.name] = sdef

        for fn in data.get("functions", []):
            entry_va = int(fn["entry_va"], 0) if isinstance(fn["entry_va"], str) else int(fn["entry_va"])
            rva = entry_va - image_base if entry_va >= image_base else entry_va
            params = [(p["name"], p["c_type"]) for p in fn.get("parameters", [])]
            sym = PDBSymbol(
                name=fn["name"],
                rva=rva,
                size=int(fn.get("size", 0)),
                is_function=True,
                return_type=fn.get("return_type"),
                param_types=params,
            )
            db.symbols_by_rva[rva] = sym
            db.symbols_by_name[sym.name] = sym

        return db
