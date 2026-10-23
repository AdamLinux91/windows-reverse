"""Structural standalone-PE link gate for generated whole-kernel objects."""

from __future__ import annotations

import json
import subprocess
import sys
from pathlib import Path

from ntrev.frontend import load_binary_image
from ntrev.symbols import load_pdb_database
from ntrev.whole_kernel import WholeKernelCLifter


def test_standalone_whole_kernel_link_has_no_host_fallback(
    kernel_fixture_paths: tuple[Path, Path],
    tmp_path: Path,
) -> None:
    pe_path, pdb_path = kernel_fixture_paths
    image = load_binary_image(pe_path)
    pdb = load_pdb_database(pdb_path, binary=image)
    out_dir = tmp_path / "whole"
    WholeKernelCLifter().emit_whole_kernel_tree(image, pdb, out_dir)
    compile_result = WholeKernelCLifter.compile_tree(out_dir)
    assert not compile_result.failed_units
    assert compile_result.compiled_units == compile_result.translation_units

    standalone_path = tmp_path / "ntoskrnl_standalone.exe"
    builder = Path(__file__).resolve().parents[1] / "decompiled_nt_kernel" / "scripts" / "link_standalone_whole_kernel.py"
    objects = sorted(str(path) for path in (out_dir / "obj").glob("*.o"))
    result = subprocess.run(
        [
            sys.executable,
            str(builder),
            "--input-ntoskrnl", str(pe_path),
            "--whole-manifest", str(out_dir / "specs" / "nt_whole_kernel_rvas.json"),
            "--entry-symbol", "KiSystemCall64Entry",
            "--out", str(standalone_path),
            *objects,
        ],
        check=True,
        capture_output=True,
        text=True,
    )
    standalone_image = load_binary_image(standalone_path)
    assert standalone_image.format == "PE32+"
    assert standalone_image.machine == "AMD64"
    assert standalone_path.read_bytes()[:2] == b"MZ"
    report = json.loads(Path(str(standalone_path) + ".standalone.json").read_text(encoding="utf-8"))
    assert standalone_image.entry_point_rva == report["entry_rva"]
    assert report["mode"] == "standalone-generated-whole-kernel"
    assert report["host_fallback"] is False
    assert report["original_executable_bytes_are_guards"] is True
    assert "generated_functions" in report
    assert result.stdout
