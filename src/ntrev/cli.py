"""
Command-Line Interface (`ntrev`) for the Windows NT Decompilation & Z3 Equivalence Engine.

Commands:
  - `ntrev decompile <binary> [--pdb <pdb>] [--ghidra-json <json>] [-o <dir>]`
  - `ntrev inspect-pe <binary>`
  - `ntrev inspect-pdb <pdb>`
  - `ntrev demo [-o <dir>]`
"""

from __future__ import annotations

import argparse
import sys
from pathlib import Path

from rich.console import Console
from rich.panel import Panel
from rich.syntax import Syntax
from rich.table import Table

from ntrev.frontend import load_binary_image
from ntrev.pipeline import NtDecompilationPipeline
from ntrev.symbols import load_pdb_database
from ntrev.whole_kernel import WholeKernelCLifter

console = Console()


def _cmd_inspect_pe(args: argparse.Namespace) -> int:
    img = load_binary_image(args.binary)
    console.print(
        Panel(
            f"[bold cyan]Binary:[/bold cyan] {img.path}\n"
            f"[bold cyan]Format:[/bold cyan] {img.format} ({img.machine})\n"
            f"[bold cyan]ImageBase:[/bold cyan] 0x{img.image_base:x}\n"
            f"[bold cyan]EntryPoint RVA:[/bold cyan] 0x{img.entry_point_rva:x}\n"
            f"[bold cyan].pdata Unwind Entries:[/bold cyan] {len(img.pdata_functions)}\n"
            + (
                f"[bold cyan]RSDS PDB70 GUID:[/bold cyan] {img.rsds.guid} (Age {img.rsds.age})\n"
                f"[bold cyan]Symbol Server URL:[/bold cyan] {img.rsds.symbol_server_url}"
                if img.rsds
                else "[dim]No RSDS CodeView Debug Directory[/dim]"
            ),
            title="PE32+ / Binary Inspection",
        )
    )

    sec_table = Table(title="Loaded Sections")
    sec_table.add_column("Section", style="bold")
    sec_table.add_column("RVA")
    sec_table.add_column("Size")
    sec_table.add_column("Executable")
    sec_table.add_column("Writable")
    for s in img.sections:
        sec_table.add_row(
            s.name,
            f"0x{s.virtual_address:x}",
            f"0x{s.virtual_size:x}",
            "Yes" if s.is_executable else "No",
            "Yes" if s.is_writable else "No",
        )
    console.print(sec_table)
    return 0


def _cmd_inspect_pdb(args: argparse.Namespace) -> int:
    db = load_pdb_database(args.pdb)
    console.print(
        Panel(
            f"[bold cyan]PDB Path:[/bold cyan] {db.pdb_path}\n"
            f"[bold cyan]GUID:[/bold cyan] {db.guid} (Age {db.age})\n"
            f"[bold cyan]TPI Structures:[/bold cyan] {len(db.structs)}\n"
            f"[bold cyan]Public/Global Symbols:[/bold cyan] {len(db.symbols_by_name)}",
            title="MSF 7.00 PDB Database",
        )
    )

    if db.structs:
        st_table = Table(title=f"TPI Structures (showing up to {args.limit} of {len(db.structs)})")
        st_table.add_column("Struct Name", style="bold green")
        st_table.add_column("Size")
        st_table.add_column("Fields")
        for st in list(db.structs.values())[: args.limit]:
            fld_preview = ", ".join(f"{f.name}@+0x{f.offset:x}" for f in st.fields[:5])
            st_table.add_row(st.name, f"0x{st.size:x}", fld_preview)
        console.print(st_table)

    if db.symbols_by_rva:
        sym_table = Table(title=f"Symbols (showing up to {args.limit} of {len(db.symbols_by_rva)})")
        sym_table.add_column("RVA")
        sym_table.add_column("Size")
        sym_table.add_column("Name", style="bold yellow")
        sym_table.add_column("Prototype Hint")
        for rva in sorted(db.symbols_by_rva.keys())[: args.limit]:
            s = db.symbols_by_rva[rva]
            proto = (
                f"{s.return_type or '?'} ({', '.join(t for _, t in s.param_types)})"
                if (s.return_type or s.param_types)
                else "-"
            )
            sym_table.add_row(f"0x{rva:x}", str(s.size), s.name, proto)
        console.print(sym_table)
    return 0


def _cmd_decompile(args: argparse.Namespace) -> int:
    pipeline = NtDecompilationPipeline(verify_smt=not args.no_smt)
    targets = args.functions.split(",") if args.functions else None
    report = pipeline.run(
        binary_path=args.binary,
        pdb_path=args.pdb,
        ghidra_json_path=args.ghidra_json,
        target_functions=targets,
        output_dir=None if args.modular_tree else args.output_dir,
    )
    if args.modular_tree and args.output_dir:
        pipeline.export_subsystem_tree(report, args.output_dir)

    _render_report_summary(report)
    if args.show_c:
        console.print(Syntax(report.source_code, "c", theme="monokai", line_numbers=True))
    if args.output_dir:
        console.print(f"\n[bold green]Deliverables written to:[/bold green] {Path(args.output_dir).resolve()}")
    return 0


def _cmd_whole_kernel(args: argparse.Namespace) -> int:
    """Run the bounded-memory two-pass whole-kernel lifter on every PDB/.pdata function."""
    out_dir = Path(args.output_dir)
    console.print(Panel(
        f"Binary: [bold]{Path(args.binary).resolve()}[/bold]\n"
        f"PDB: [bold]{Path(args.pdb).resolve()}[/bold]\n"
        f"Output: [bold]{out_dir.resolve()}[/bold]",
        title="Whole-Kernel Streaming Decompilation",
    ))
    lifter = WholeKernelCLifter()
    _, summary = lifter.run(args.binary, args.pdb, out_dir)
    compile_result = lifter.compile_tree(out_dir) if args.compile else None
    table = Table(title="Whole-Kernel Coverage")
    table.add_column("Metric", style="bold cyan")
    table.add_column("Value", style="green")
    for key, value in summary.__dict__.items():
        if key != "shard_files":
            table.add_row(key, f"{value:,}" if isinstance(value, int) else str(value))
    table.add_row("shards", str(len(summary.shard_files)))
    console.print(table)
    if compile_result is not None:
        console.print(
            f"Native compile gate: {compile_result.compiled_units}/{compile_result.translation_units} units; "
            f"unexpected unresolved symbols: {len(compile_result.unexpected_unresolved_symbols)}"
        )
        if compile_result.failed_units or compile_result.unexpected_unresolved_symbols:
            return 2
    console.print("[bold green]Whole-kernel C/ASM shards, reconstructed data sections, prototypes, and RVA manifest written.[/bold green]")
    return 0


def _cmd_demo(args: argparse.Namespace) -> int:
    # Build synthetic PE32+ ntoskrnl fixture + MSF 7.00 PDB on the fly & run full pipeline
    sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "examples"))
    from build_ntoskrnl_fixture import build_ntoskrnl_mini_fixture  # type: ignore

    out_dir = Path(args.output_dir)
    out_dir.mkdir(parents=True, exist_ok=True)
    pe_path, pdb_path = build_ntoskrnl_mini_fixture(out_dir)

    console.print(
        Panel(
            f"Built realistic Windows x64 (`ms_abi`) PE32+ kernel fixture + MSF 7.00 PDB:\n"
            f"  • Binary : [bold]{pe_path}[/bold]\n"
            f"  • PDB    : [bold]{pdb_path}[/bold]",
            title="Stage 0 — Kernel Fixture Synthesis",
        )
    )

    pipeline = NtDecompilationPipeline(verify_smt=True)
    report = pipeline.run(
        binary_path=pe_path,
        pdb_path=pdb_path,
        output_dir=out_dir / "decompiled",
    )
    _render_report_summary(report)
    console.print(Syntax(report.source_code, "c", theme="monokai", line_numbers=True))
    console.print(
        f"\n[bold green]Full C replica, header, LLVM 22 IR, and Z3 proofs saved to:[/bold green] "
        f"{(out_dir / 'decompiled').resolve()}"
    )
    return 0


def _render_report_summary(report) -> None:
    tbl = Table(title=f"Decompilation & Z3 Equivalence Report — {Path(report.binary_path).name}")
    tbl.add_column("VA", style="cyan")
    tbl.add_column("Function", style="bold")
    tbl.add_column("Kind")
    tbl.add_column("De-Optimizations")
    tbl.add_column("Z3 Status")
    tbl.add_column("Solve (ms)", justify="right")

    for fn in report.functions:
        kind = "[magenta]RAW ASM[/magenta]" if fn.is_raw_asm else (
            "[yellow]HYBRID C+ASM[/yellow]" if fn.has_inline_asm else "[green]VERIFIED C[/green]"
        )
        deopts = [rl.kind for rl in fn.rerolled_loops] + [
            f"div/{rd.divisor}" for rd in fn.recovered_divs
        ]
        deopt_str = ", ".join(deopts) if deopts else "-"
        status_style = (
            "bold green"
            if fn.proof.status in ("VERIFIED_EQUIVALENT", "PRESERVED_RAW_ASM")
            else "bold red"
        )
        tbl.add_row(
            f"0x{fn.address:x}",
            fn.name,
            kind,
            deopt_str,
            f"[{status_style}]{fn.proof.status}[/{status_style}]",
            f"{fn.proof.solve_time_ms:.2f}",
        )
    console.print(tbl)


def build_arg_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        prog="ntrev",
        description="Automated Windows NT Binary Decompilation & Z3 Semantic Equivalence Engine",
    )
    sub = parser.add_subparsers(dest="command", required=True)

    p_dec = sub.add_parser("decompile", help="Run full 4-stage decompilation & Z3 proof pipeline")
    p_dec.add_argument("binary", help="Path to PE32+ (.exe/.sys/.dll) or ELF64 x86-64 binary")
    p_dec.add_argument("--pdb", help="Path to matching MSF 7.00 .pdb or .json symbol file")
    p_dec.add_argument("--ghidra-json", help="Path to exported Ghidra analysis JSON")
    p_dec.add_argument("-f", "--functions", help="Comma-separated list of function names")
    p_dec.add_argument("-o", "--output-dir", default="output", help="Directory for C/IR/Proof deliverables")
    p_dec.add_argument(
        "--modular-tree",
        action="store_true",
        help="Export modular per-subsystem C kernel tree (include/, src/{ke,ps,mm,ob,io,ex,rtl,se}/, ir/, proofs/)",
    )
    p_dec.add_argument("--no-smt", action="store_true", help="Skip Z3 SMT equivalence proofs")
    p_dec.add_argument("--show-c", action="store_true", help="Print reconstructed C source to stdout")
    p_dec.set_defaults(func=_cmd_decompile)

    p_whole = sub.add_parser(
        "whole-kernel",
        help="Stream all exported and non-exported ntoskrnl.exe functions into verified C/ASM subsystem shards",
    )
    p_whole.add_argument("binary", help="Path to the real PE32+ ntoskrnl.exe")
    p_whole.add_argument("--pdb", required=True, help="Path to the matching MSF 7.00 ntkrnlmp.pdb")
    p_whole.add_argument("-o", "--output-dir", default="decompiled_whole_kernel", help="Self-contained output tree")
    p_whole.add_argument("--compile", action="store_true", help="Compile every shard and link a relocatable whole-kernel object")
    p_whole.set_defaults(func=_cmd_whole_kernel)

    p_pe = sub.add_parser("inspect-pe", help="Inspect PE32+ sections, .pdata unwinds, and RSDS PDB70")
    p_pe.add_argument("binary", help="Path to PE32+ binary")
    p_pe.set_defaults(func=_cmd_inspect_pe)

    p_pdb = sub.add_parser("inspect-pdb", help="Inspect binary MSF 7.00 .pdb TPI structs and symbols")
    p_pdb.add_argument("pdb", help="Path to .pdb file")
    p_pdb.add_argument("--limit", type=int, default=40, help="Max structs/symbols to display")
    p_pdb.set_defaults(func=_cmd_inspect_pdb)

    p_demo = sub.add_parser(
        "demo",
        help="Synthesize a compiled Windows x64 PE32+ kernel fixture + MSF 7.00 PDB and verify end-to-end",
    )
    p_demo.add_argument("-o", "--output-dir", default="output/demo", help="Demo workspace directory")
    p_demo.set_defaults(func=_cmd_demo)

    return parser


def main(argv: list[str] | None = None) -> int:
    parser = build_arg_parser()
    args = parser.parse_args(argv)
    return args.func(args)


if __name__ == "__main__":
    raise SystemExit(main())
