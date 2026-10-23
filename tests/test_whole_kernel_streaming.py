"""Whole-kernel streaming lifter coverage and generated-tree compilation tests."""

from __future__ import annotations

import subprocess
from pathlib import Path

from ntrev.frontend import load_binary_image
from ntrev.symbols import load_pdb_database
from ntrev.whole_kernel import (
    WholeKernelCLifter,
    export_exact_kernel,
    verify_exact_kernel,
)


def test_whole_kernel_two_pass_fixture_covers_all_functions_and_shards(
    kernel_fixture_paths: tuple[Path, Path],
    tmp_path: Path,
) -> None:
    pe_path, pdb_path = kernel_fixture_paths
    image = load_binary_image(pe_path)
    pdb = load_pdb_database(pdb_path, binary=image)

    lifter = WholeKernelCLifter()
    index = lifter.build_symbol_index(image, pdb)
    assert len(index.functions_by_rva) == 6
    assert len(index.functions_by_va) == 6
    assert len(index.shard_order) >= 2
    assert all(meta.c_name for meta in index.functions_by_rva.values())
    assert all(meta.shard_id for meta in index.functions_by_rva.values())

    _, summary = lifter.emit_whole_kernel_tree(image, pdb, tmp_path / "whole")
    assert summary.total_functions == 6
    assert summary.pure_c_functions == 5
    assert summary.preserved_asm_functions == 1
    assert summary.recovered_magic_divs >= 1
    assert summary.recovered_loops >= 1
    assert (tmp_path / "whole/include/nt_whole_kernel_runtime.h").is_file()
    assert (tmp_path / "whole/include/nt_whole_kernel_protos.h").is_file()
    assert (tmp_path / "whole/specs/nt_whole_kernel_rvas.json").is_file()
    assert len(summary.shard_files) >= 2
    compiled = lifter.compile_tree(tmp_path / "whole")
    assert compiled.compiled_units == compiled.translation_units
    assert not compiled.failed_units
    assert not compiled.unexpected_unresolved_symbols


def test_whole_kernel_generated_fixture_shards_compile(
    kernel_fixture_paths: tuple[Path, Path],
    tmp_path: Path,
) -> None:
    pe_path, pdb_path = kernel_fixture_paths
    image = load_binary_image(pe_path)
    pdb = load_pdb_database(pdb_path, binary=image)
    out_dir = tmp_path / "whole"
    WholeKernelCLifter().emit_whole_kernel_tree(image, pdb, out_dir)

    obj_dir = out_dir / "obj"
    obj_dir.mkdir()
    sources = sorted((out_dir / "src").glob("*/*.c"))
    sources += sorted((out_dir / "src/globals").glob("*.S"))
    assert sources
    for source in sources:
        subprocess.run(
            [
                "gcc",
                "-c",
                "-O1",
                "-std=gnu11",
                "-mabi=ms",
                "-ffreestanding",
                "-fno-builtin",
                "-fno-tree-loop-distribute-patterns",
                "-Wno-unused-variable",
                "-Wno-unused-label",
                f"-I{out_dir / 'include'}",
                str(source),
                "-o",
                str(obj_dir / f"{source.parent.name}__{source.stem}.o"),
            ],
            check=True,
            capture_output=True,
            text=True,
        )
