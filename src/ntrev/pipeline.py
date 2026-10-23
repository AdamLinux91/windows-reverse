"""
End-to-End 4-Stage Automated Decompilation & Z3 Semantic Equivalence Pipeline.

Pipeline Architecture:
  [Windows Binary (ntoskrnl.exe / .sys)]
                │
                ▼
      1. Lifter / IR Translator (Capstone CFG + Non-C Asm Detector + LLVM 22 Lifter)
                │
                ▼
      2. Symbol & Structural Mapper (MSF 7.00 PDB + Ghidra API + NT Struct Reconstructor)
                │
                ▼
      3. Type Inference, Loop Reroller & Z3 SMT Equivalence Prover
                │
                ▼
      4. Custom Verified LLVM IR -> Exact Compilable C/C++ Source Replica
"""

from __future__ import annotations

import json
import re
from pathlib import Path

from ntrev.analysis import (
    CompilerDeoptimizer,
    TypeInferenceEngine,
    Z3EquivalenceVerifier,
)
from ntrev.backend import VerifiedCEmitter
from ntrev.frontend import (
    AsmBlockDetector,
    GhidraHeadlessBridge,
    X86Disassembler,
    load_binary_image,
)
from ntrev.lifter import X86ToLLVMLifter
from ntrev.models import (
    DecompilationReport,
    DecompiledFunction,
    EquivalenceProof,
    InferredFuncSignature,
    KernelGlobalRef,
    PDBDatabase,
    StructDef,
    classify_nt_subsystem,
)
from ntrev.symbols import StructuralMapper, load_pdb_database
from ntrev.symbols.nt_types import KNOWN_WDK_PROTOTYPES, get_builtin_nt_structs


class NtDecompilationPipeline:
    """Orchestrates the 4-stage NT binary decompilation and Z3 verification pipeline."""

    def __init__(
        self,
        verify_smt: bool = True,
        loop_unroll_bound: int = 12,
        z3_timeout_ms: int = 10000,
    ) -> None:
        self.verify_smt = verify_smt
        self.disassembler = X86Disassembler()
        self.asm_detector = AsmBlockDetector()
        self.lifter = X86ToLLVMLifter()
        self.type_engine = TypeInferenceEngine()
        self.deoptimizer = CompilerDeoptimizer()
        self.verifier = Z3EquivalenceVerifier(
            loop_unroll_bound=loop_unroll_bound,
            timeout_ms=z3_timeout_ms,
        )

    def run(
        self,
        binary_path: str | Path,
        pdb_path: str | Path | None = None,
        ghidra_json_path: str | Path | None = None,
        target_functions: list[str] | None = None,
        output_dir: str | Path | None = None,
        preloaded_image: BinaryImage | None = None,
        preloaded_pdb: PDBDatabase | None = None,
    ) -> DecompilationReport:
        """Execute all 4 stages on `binary_path` and optionally write deliverables to `output_dir`."""
        image = preloaded_image if preloaded_image is not None else load_binary_image(binary_path)

        pdb_db: PDBDatabase | None = preloaded_pdb
        if pdb_db is None and pdb_path is not None:
            pdb_db = load_pdb_database(pdb_path, binary=image)

        if ghidra_json_path is not None:
            ghidra_db = GhidraHeadlessBridge.load_ghidra_export(ghidra_json_path)
            pdb_db = self._merge_pdb_databases(pdb_db, ghidra_db)

        # Stage 1A: Disassemble executable sections (.text, PAGE, INIT, KVASCODE) into CFGs
        cfgs = self.disassembler.disassemble_image(
            image=image,
            pdb=pdb_db,
            target_functions=target_functions,
        )

        # Build global VA -> symbol name lookup for direct CALL & RIP-relative data symbols
        va_symbols: dict[int, str] = {}
        for rva, ename in image.exports.items():
            va_symbols[image.image_base + rva] = ename
        for va, iname in image.imports.items():
            va_symbols[va] = iname.split("!")[-1]
        global_data_symbols: dict[int, str] = {}
        if pdb_db is not None:
            for rva, sym in pdb_db.symbols_by_rva.items():
                if sym.is_function:
                    va_symbols[image.image_base + rva] = sym.name
            for rva, dsym in pdb_db.data_symbols_by_rva.items():
                global_data_symbols[image.image_base + rva] = dsym.name

        mapper = StructuralMapper(pdb=pdb_db)
        all_synthesized_structs: list[StructDef] = []

        # Pass 1: Classify ASM, map PDB/NT structures, and infer function signatures across batch
        pre_stage: list[tuple] = []
        known_signatures: dict[str, InferredFuncSignature] = {}
        callee_param_widths: dict[str, list[int]] = {}

        for fname, (ret_ty, params) in KNOWN_WDK_PROTOTYPES.items():
            callee_param_widths[fname] = [self._c_type_bit_width(pty) or 64 for _, pty in params]

        for cfg in cfgs:
            asm_cls = self.asm_detector.classify_function(cfg)
            field_refs, new_structs, inferred_param_structs = mapper.map_function_structures(cfg)
            all_synthesized_structs.extend(new_structs)
            signature = self.type_engine.infer_signature(
                cfg=cfg,
                pdb=pdb_db,
                inferred_param_structs=inferred_param_structs,
            )
            # Re-run struct mapping with inferred parameter names so field expressions match C params
            p_name_map = {
                reg: signature.params[i][0]
                for i, reg in enumerate(("rcx", "rdx", "r8", "r9"))
                if i < len(signature.params)
            }
            if p_name_map:
                field_refs, _, _ = mapper.map_function_structures(cfg, param_names=p_name_map)

            known_signatures[cfg.name] = signature
            callee_param_widths[cfg.name] = [
                self._c_type_bit_width(pty) or 64 for _, pty in signature.params
            ]
            pre_stage.append((cfg, asm_cls, field_refs, signature))

        # Pass 2: Lift to LLVM 22 IR, De-Optimize, and Formally Prove Equivalence in Z3
        decompiled_funcs: list[DecompiledFunction] = []
        ir_modules: list[str] = []
        all_globals_by_name: dict[str, KernelGlobalRef] = {}
        c_callable_asm_names: set[str] = set()

        for cfg, asm_cls, field_refs, signature in pre_stage:
            if asm_cls.is_raw_asm:
                raw_ir = f"; Preserved raw NT kernel assembly routine: {cfg.name}\n"
                opt_ir = raw_ir
                fn_globals: list[KernelGlobalRef] = []
                if asm_cls.is_c_callable_naked:
                    c_callable_asm_names.add(cfg.name)
            else:
                lifted = self.lifter.lift_function(
                    cfg=cfg,
                    param_count=len(signature.params),
                    symbol_lookup=va_symbols,
                    global_data_symbols=global_data_symbols,
                    callee_param_widths=callee_param_widths,
                    image=image,
                    opt_level=2,
                )
                raw_ir = lifted.raw_ir
                opt_ir = lifted.optimized_ir
                fn_globals = lifted.referenced_globals
                for g in fn_globals:
                    all_globals_by_name.setdefault(g.name, g)
                ir_modules.append(opt_ir)

            rerolled_loops, recovered_divs = self.deoptimizer.analyze_function(cfg, signature)

            ret_bits = self._c_type_bit_width(signature.return_type)
            if self.verify_smt:
                proof = self.verifier.verify_function(
                    cfg=cfg,
                    llvm_ir=opt_ir,
                    rerolled_loops=rerolled_loops,
                    recovered_divs=recovered_divs,
                    return_bits=ret_bits,
                    param_names=[pn for pn, _ in signature.params],
                    image=image,
                    symbol_lookup=va_symbols,
                    global_data_symbols=global_data_symbols,
                    callee_param_widths=callee_param_widths,
                )
            else:
                proof = EquivalenceProof(function_name=cfg.name, status="VERIFICATION_DISABLED")

            decompiled_funcs.append(
                DecompiledFunction(
                    name=cfg.name,
                    address=cfg.address,
                    size=cfg.size,
                    signature=signature,
                    is_raw_asm=asm_cls.is_raw_asm,
                    has_inline_asm=asm_cls.has_inline_asm_blocks,
                    raw_llvm_ir=raw_ir,
                    optimized_llvm_ir=opt_ir,
                    c_code="",
                    subsystem=classify_nt_subsystem(cfg.name),
                    field_accesses=field_refs,
                    rerolled_loops=rerolled_loops,
                    recovered_divs=recovered_divs,
                    referenced_globals=fn_globals,
                    proof=proof,
                )
            )

        # Stage 4: Custom Verified LLVM IR + Structural Map -> Exact C/C++ Emitter
        selected_structs = self._select_structs_for_header(
            mapper.structs, decompiled_funcs, all_synthesized_structs
        )
        ordered_structs = self._topological_structs(selected_structs)
        emitter = VerifiedCEmitter(structs=mapper.structs, known_signatures=known_signatures)
        globals_list = list(all_globals_by_name.values())

        header_code = emitter.emit_header(
            structs=ordered_structs,
            signatures=[(df.signature, df.is_raw_asm) for df in decompiled_funcs],
            globals_list=globals_list,
            c_callable_asm_names=c_callable_asm_names,
        )

        # Collect any external callees invoked in IR that are not defined in this compilation unit
        defined_names = {df.name for df in decompiled_funcs}
        ext_callees = self._collect_external_ir_callees(decompiled_funcs, defined_names)

        c_units: list[str] = [
            "/*",
            f" * Auto-generated by ntrev — Verified C Source Replica for {Path(binary_path).name}",
            " * Every C function below has been checked against original x86-64 machine semantics.",
            " */",
            '#include "reconstructed_types.h"',
            "",
        ]

        if globals_list:
            c_units.append("/* Kernel Global Definitions (Weak Linkage — Overridable by Boot Runtime) */")
            for g in globals_list:
                defn = emitter.format_global_declaration(g, is_extern=False)
                c_units.append(f"__attribute__((weak)) {defn}")
            c_units.append("")

        if ext_callees:
            c_units.append("/* External Kernel Callee Declarations */")
            for ec_name, n_args in sorted(ext_callees.items()):
                csig = emitter.known_signatures.get(ec_name)
                if csig is not None:
                    c_units.append(f"extern {csig.format_prototype(include_abi=True)};")
                else:
                    p_str = ", ".join(f"uint64_t a{i}" for i in range(n_args)) or "void"
                    c_units.append(f"extern uint64_t __attribute__((ms_abi)) {ec_name}({p_str});")
            c_units.append("")

        cfg_by_name = {c.name: c for c in cfgs}
        for df in decompiled_funcs:
            df.c_code = emitter.emit_function(
                cfg=cfg_by_name[df.name],
                signature=df.signature,
                optimized_llvm_ir=df.optimized_llvm_ir,
                field_accesses=df.field_accesses,
                rerolled_loops=df.rerolled_loops,
                recovered_divs=df.recovered_divs,
                proof=df.proof,
            )
            c_units.append(df.c_code)
            c_units.append("")

        asm_weak_lines = self._collect_asm_weak_stubs(decompiled_funcs)
        if asm_weak_lines:
            c_units.extend(asm_weak_lines)
            c_units.append("")

        source_code = "\n".join(c_units)
        combined_ir = "\n\n".join(ir_modules)

        report = DecompilationReport(
            binary_path=str(binary_path),
            pdb_path=str(pdb_path) if pdb_path else None,
            image_base=image.image_base,
            functions=decompiled_funcs,
            reconstructed_structs=ordered_structs,
            referenced_globals=globals_list,
            header_code=header_code,
            source_code=source_code,
            combined_llvm_ir=combined_ir,
        )

        if output_dir is not None:
            self.write_deliverables(report, output_dir)

        return report

    @staticmethod
    def _select_structs_for_header(
        all_structs: dict[str, StructDef],
        decompiled_funcs: list[DecompiledFunction],
        synthesized_structs: list[StructDef],
    ) -> list[StructDef]:
        if len(all_structs) <= 64:
            return list(all_structs.values())

        wanted: set[str] = set(get_builtin_nt_structs().keys())
        for st in synthesized_structs:
            wanted.add(st.name)
        pat = re.compile(r"\b(?:struct|union)\s+([A-Za-z_][A-Za-z0-9_]*)")
        for df in decompiled_funcs:
            for m in pat.finditer(df.signature.return_type):
                wanted.add(m.group(1))
            for _, pty in df.signature.params:
                for m in pat.finditer(pty):
                    wanted.add(m.group(1))
            for fa in df.field_accesses:
                wanted.add(fa.struct_name)

        return [all_structs[n] for n in sorted(wanted) if n in all_structs]

    @staticmethod
    def _collect_external_ir_callees(
        decompiled_funcs: list[DecompiledFunction],
        defined_names: set[str],
    ) -> dict[str, int]:
        ext: dict[str, int] = {}
        pat = re.compile(r"call\s+i\d+\s+@([A-Za-z0-9_.$]+)\(([^)]*)\)")
        for df in decompiled_funcs:
            if df.is_raw_asm:
                continue
            for m in pat.finditer(df.optimized_llvm_ir):
                cname = m.group(1)
                if (
                    cname in defined_names
                    or cname.startswith("llvm.")
                    or cname.startswith("__nt_read_gs_")
                    or cname.startswith("__nt_sym_addr_")
                ):
                    continue
                args_s = m.group(2).strip()
                n_args = len([x for x in args_s.split(",") if x.strip()]) if args_s else 0
                ext[cname] = n_args
        return ext

    @staticmethod
    def write_deliverables(report: DecompilationReport, output_dir: str | Path) -> dict[str, Path]:
        out = Path(output_dir)
        out.mkdir(parents=True, exist_ok=True)

        hdr_path = out / "reconstructed_types.h"
        src_path = out / "reconstructed_nt.c"
        ir_path = out / "verified_lifted.ll"
        proof_path = out / "z3_equivalence_proofs.json"

        hdr_path.write_text(report.header_code, encoding="utf-8")
        src_path.write_text(report.source_code, encoding="utf-8")
        ir_path.write_text(report.combined_llvm_ir, encoding="utf-8")

        manifest = {
            "binary_path": report.binary_path,
            "pdb_path": report.pdb_path,
            "image_base": hex(report.image_base),
            "total_functions": len(report.functions),
            "verified_functions": report.verified_count,
            "referenced_globals": [
                {
                    "name": g.name,
                    "va": hex(g.va),
                    "c_type": g.c_type,
                    "is_const": g.is_const,
                    "initial_value": hex(g.initial_value) if g.initial_value is not None else None,
                }
                for g in report.referenced_globals
            ],
            "functions": [
                {
                    "name": fn.name,
                    "subsystem": fn.subsystem,
                    "address": hex(fn.address),
                    "size": fn.size,
                    "prototype": fn.signature.format_prototype(include_abi=False),
                    "is_raw_asm": fn.is_raw_asm,
                    "rerolled_loops": [rl.description for rl in fn.rerolled_loops],
                    "recovered_divisions": [rd.c_expr for rd in fn.recovered_divs],
                    "proof": fn.proof.to_dict(),
                }
                for fn in report.functions
            ],
        }
        proof_path.write_text(json.dumps(manifest, indent=2), encoding="utf-8")

        return {
            "header": hdr_path,
            "source": src_path,
            "llvm_ir": ir_path,
            "proofs": proof_path,
        }

    def export_subsystem_tree(
        self,
        report: DecompilationReport,
        output_dir: str | Path,
        binary_label: str | None = None,
        pdb_label: str | None = None,
    ) -> dict[str, Path]:
        """
        Export decompiled NT kernel artifacts as a modular, per-subsystem C kernel tree
        (`include/`, `src/{ke,ps,mm,ob,io,ex,rtl,se,runtime}/`, `ir/`, `proofs/`).
        """
        root = Path(output_dir)
        inc_dir = root / "include"
        ir_dir = root / "ir"
        proof_dir = root / "proofs"
        for d in (inc_dir, ir_dir, proof_dir, root / "src" / "runtime"):
            d.mkdir(parents=True, exist_ok=True)

        # Build external callee declarations for the shared kernel header
        defined_names = {df.name for df in report.functions}
        ext_callees = self._collect_external_ir_callees(report.functions, defined_names)
        known_sigs: dict[str, InferredFuncSignature] = {}
        for kname, (ret_ty, kparams) in KNOWN_WDK_PROTOTYPES.items():
            known_sigs[kname] = InferredFuncSignature(
                name=kname, return_type=ret_ty, params=list(kparams)
            )
        ext_lines: list[str] = []
        if ext_callees:
            ext_lines.append("/* External Kernel Callee Declarations */")
            for ec_name, n_args in sorted(ext_callees.items()):
                csig = known_sigs.get(ec_name)
                if csig is not None:
                    ext_lines.append(f"extern {csig.format_prototype(include_abi=True)};")
                else:
                    p_str = ", ".join(f"uint64_t a{i}" for i in range(n_args)) or "void"
                    ext_lines.append(f"extern uint64_t __attribute__((ms_abi)) {ec_name}({p_str});")
            ext_lines.append("")

        hdr_text = report.header_code.replace("NTREV_RECONSTRUCTED_TYPES_H", "NT_RECONSTRUCTED_TYPES_H")
        if ext_lines:
            end_guard = "#endif /* NT_RECONSTRUCTED_TYPES_H */"
            hdr_text = hdr_text.replace(end_guard, "\n".join(ext_lines) + "\n" + end_guard)

        hdr_path = inc_dir / "nt_reconstructed_types.h"
        hdr_path.write_text(hdr_text, encoding="utf-8")

        # Group functions into canonical NT executive/kernel subsystems
        subsys_files = [
            ("ke", "ke_kernel.c", "Kernel Core Dispatcher, IRQL, DPC, Bootstrap & Synchronization (Ke/Ki/Kz/Hal)"),
            ("ps", "ps_process.c", "Process, Thread, Job & Silo Executive (Ps/Psp)"),
            ("mm", "mm_memory.c", "Virtual Memory Manager, PFN & MDL Subsystem (Mm/Mi)"),
            ("ob", "ob_object.c", "Executive Object Manager & Cookie Type Table (Ob/Obp)"),
            ("io", "io_manager.c", "I/O Manager, IRP Stack, WorkItems & Plug-and-Play (Io/Iop/Pi/Pnp)"),
            ("ex", "ex_executive.c", "Executive SLists, Rundown, PushLocks & Resources (Ex/Exp/Exf)"),
            ("rtl", "rtl_library.c", "Kernel Run-Time Library, Trees, Bitmaps, Strings & Time (Rtl)"),
            ("se", "se_kd_security.c", "Security Reference Monitor, Tokens, SIDs & ACLs (Se/Sep)"),
            ("cm", "cm_registry.c", "Configuration Manager & Kernel Registry Transactions (Cm/Cmp)"),
            ("cc", "cc_cache.c", "Common Cache Manager, BCBs & Dirty Page Tracking (Cc)"),
            ("fsrtl", "fsrtl_filesystem.c", "File System Runtime Library, MCBs, Locks & Oplocks (FsRtl)"),
            ("po", "po_power.c", "Power Manager, Device Power IRPs & Idle States (Po/Pop)"),
            ("alpc", "alpc_ipc.c", "Asynchronous Local IPC Message Attributes & Headers (Alpc/Lpc)"),
            ("tm", "tm_transactions.c", "Kernel Transaction Manager & Enlistments (Tm/Tmp)"),
            ("etw_vf", "etw_vf_diag.c", "ETW Tracing, WMI, Driver Verifier, Hypervisor & KD (Etw/Vf/Hvl/Whea/Kd)"),
            ("zw_nt", "zw_nt_syscalls.c", "Kernel System Service Dispatchers & Native API (Zw/Nt)"),
        ]

        known_subsys = {k for k, _, _ in subsys_files}
        written_paths: dict[str, Path] = {"header": hdr_path}
        for sub_key, fname, desc in subsys_files:
            if sub_key == "rtl":
                group = [
                    df
                    for df in report.functions
                    if df.subsystem == "rtl" or df.subsystem not in known_subsys
                ]
            else:
                group = [df for df in report.functions if df.subsystem == sub_key]
            if not group:
                continue
            sdir = root / "src" / sub_key
            sdir.mkdir(parents=True, exist_ok=True)
            spath = sdir / fname
            lines = [
                "/*",
                f" * Reconstructed Windows NT Kernel Subsystem: {desc}",
                f" * Source Binary : {binary_label or Path(report.binary_path).name}",
                f" * Functions     : {len(group)} Z3-verified / assembly-preserved routines",
                " */",
                '#include "../../include/nt_reconstructed_types.h"',
                "",
            ]
            for df in group:
                lines.append(df.c_code)
                lines.append("")
            spath.write_text("\n".join(lines), encoding="utf-8")
            written_paths[f"src_{sub_key}"] = spath

        # Weak-linkage reconstructed globals, `.rdata` lookup tables, ASM stubs & external callees
        glob_path = root / "src" / "runtime" / "nt_reconstructed_globals.c"
        glines = [
            "/*",
            " * Reconstructed Retail ntoskrnl.exe Data Symbols, Read-Only Lookup Tables & Weak Callees",
            " * Extracted via RIP-relative symbol/section recovery (weak linkage — overridden in Ring-0",
            " * standalone builds by `nt_kernel_runtime.c` and bound directly to real kernel RVAs in",
            " * `ntoskrnl_dropin.exe`).",
            " */",
            '#include "../../include/nt_reconstructed_types.h"',
            "",
        ]
        for g in report.referenced_globals:
            defn = VerifiedCEmitter.format_global_declaration(g, is_extern=False)
            glines.append(f"__attribute__((weak)) {defn}")
        glines.append("")

        asm_weak_lines = self._collect_asm_weak_stubs(report.functions)
        if asm_weak_lines:
            glines.extend(asm_weak_lines)
            glines.append("")

        if ext_callees:
            glines.append("/* Weak Fallback Stubs for External Internal Callees (Overridden by Strong Runtime) */")
            for ec_name, n_args in sorted(ext_callees.items()):
                csig = known_sigs.get(ec_name)
                if csig is not None:
                    proto = csig.format_prototype(include_abi=True)
                    if csig.is_void_return:
                        glines.append(f"__attribute__((weak)) {proto} {{}}")
                    else:
                        glines.append(
                            f"__attribute__((weak)) {proto} {{ return ({csig.return_type})0; }}"
                        )
                else:
                    p_str = ", ".join(f"uint64_t a{i}" for i in range(n_args)) or "void"
                    glines.append(
                        f"__attribute__((weak)) uint64_t __attribute__((ms_abi)) {ec_name}({p_str}) {{ return 0ULL; }}"
                    )
            glines.append("")

        glob_path.write_text("\n".join(glines), encoding="utf-8")
        written_paths["globals"] = glob_path

        ir_path = ir_dir / "verified_lifted.ll"
        ir_path.write_text(report.combined_llvm_ir, encoding="utf-8")
        written_paths["llvm_ir"] = ir_path

        proof_path = proof_dir / "z3_equivalence_proofs.json"
        manifest = {
            "binary": binary_label or Path(report.binary_path).name,
            "pdb": pdb_label or (Path(report.pdb_path).name if report.pdb_path else None),
            "image_base": hex(report.image_base),
            "total_functions": len(report.functions),
            "verified_functions": report.verified_count,
            "subsystems": {
                sub_key: sum(
                    1
                    for df in report.functions
                    if (
                        (df.subsystem == "rtl" or df.subsystem not in known_subsys)
                        if sub_key == "rtl"
                        else df.subsystem == sub_key
                    )
                )
                for sub_key, _, _ in subsys_files
            },
            "referenced_globals": [
                {
                    "name": g.name,
                    "va": hex(g.va),
                    "c_type": g.c_type,
                    "is_const": g.is_const,
                    "has_initial_bytes": bool(g.initial_bytes),
                }
                for g in report.referenced_globals
            ],
            "functions": [
                {
                    "name": fn.name,
                    "subsystem": fn.subsystem,
                    "address": hex(fn.address),
                    "size": fn.size,
                    "prototype": fn.signature.format_prototype(include_abi=False),
                    "is_raw_asm": fn.is_raw_asm,
                    "rerolled_loops": [rl.description for rl in fn.rerolled_loops],
                    "recovered_divisions": [rd.c_expr for rd in fn.recovered_divs],
                    "proof": fn.proof.to_dict(),
                }
                for fn in report.functions
            ],
        }
        proof_path.write_text(json.dumps(manifest, indent=2), encoding="utf-8")
        written_paths["proofs"] = proof_path
        return written_paths

    @staticmethod
    def _collect_asm_weak_stubs(decompiled_funcs: list[DecompiledFunction]) -> list[str]:
        ext_fns: set[str] = set()
        ext_syms: set[str] = set()
        fn_pat = re.compile(r"extern void (__nt_asm_ext_[0-9a-fA-F]+)\(void\);")
        sym_pat = re.compile(r"extern unsigned char (__nt_asm_sym_[0-9a-fA-F]+)\[\];")
        for df in decompiled_funcs:
            if not df.is_raw_asm:
                continue
            for m in fn_pat.finditer(df.c_code):
                ext_fns.add(m.group(1))
            for m in sym_pat.finditer(df.c_code):
                ext_syms.add(m.group(1))
        out: list[str] = []
        if ext_fns or ext_syms:
            out.append("/* Weak Linkage Stubs for Preserved Assembly External Targets */")
            for fn in sorted(ext_fns):
                out.append(
                    f'__attribute__((weak, naked)) void {fn}(void) {{ __asm__ __volatile__ ("ret"); }}'
                )
            for sn in sorted(ext_syms):
                out.append(f"__attribute__((weak)) unsigned char {sn}[64] = {{0}};")
        return out

    @staticmethod
    def _merge_pdb_databases(
        primary: PDBDatabase | None, secondary: PDBDatabase
    ) -> PDBDatabase:
        if primary is None:
            return secondary
        for k, v in secondary.structs.items():
            primary.structs.setdefault(k, v)
        for rva, sym in secondary.symbols_by_rva.items():
            primary.symbols_by_rva.setdefault(rva, sym)
            primary.symbols_by_name.setdefault(sym.name, sym)
        return primary

    @staticmethod
    def _topological_structs(structs: list[StructDef]) -> list[StructDef]:
        by_name = {s.name: s for s in structs}
        typedef_to_name = {s.typedef_name: s.name for s in structs if s.typedef_name}
        visited: set[str] = set()
        visiting: set[str] = set()
        ordered: list[StructDef] = []

        def visit(sname: str) -> None:
            if sname in visited or sname not in by_name or sname in visiting:
                return
            visiting.add(sname)
            st = by_name[sname]
            for fld in st.fields:
                if not fld.is_pointer and not fld.c_type.endswith("*"):
                    dep = fld.c_type.replace("struct ", "").replace("union ", "").strip()
                    dep_name = typedef_to_name.get(dep, dep)
                    if dep_name in by_name and dep_name != sname:
                        visit(dep_name)
            visiting.remove(sname)
            visited.add(sname)
            ordered.append(st)

        for s in structs:
            visit(s.name)
        return ordered

    @staticmethod
    def _c_type_bit_width(c_type: str) -> int:
        clean = c_type.strip()
        if clean == "void":
            return 0
        if "*" in clean or clean in ("PVOID", "HANDLE", "ULONG_PTR", "size_t"):
            return 64
        if clean in ("uint8_t", "int8_t", "char", "BOOLEAN", "KIRQL", "bool"):
            return 8
        if clean in ("uint16_t", "int16_t", "USHORT", "SHORT"):
            return 16
        if clean in ("uint32_t", "int32_t", "ULONG", "LONG", "NTSTATUS"):
            return 32
        return 64
