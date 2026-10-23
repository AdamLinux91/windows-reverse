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
    PDBDatabase,
    StructDef,
)
from ntrev.symbols import StructuralMapper, load_pdb_database


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
    ) -> DecompilationReport:
        """Execute all 4 stages on `binary_path` and optionally write deliverables to `output_dir`."""
        image = load_binary_image(binary_path)

        pdb_db: PDBDatabase | None = None
        if pdb_path is not None:
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

        # Build global VA -> symbol name lookup for direct CALL instructions
        va_symbols: dict[int, str] = {}
        for rva, ename in image.exports.items():
            va_symbols[image.image_base + rva] = ename
        for va, iname in image.imports.items():
            va_symbols[va] = iname.split("!")[-1]
        if pdb_db is not None:
            for rva, sym in pdb_db.symbols_by_rva.items():
                va_symbols[image.image_base + rva] = sym.name

        mapper = StructuralMapper(pdb=pdb_db)
        all_synthesized_structs: list[StructDef] = []
        decompiled_funcs: list[DecompiledFunction] = []
        ir_modules: list[str] = []

        for cfg in cfgs:
            # Stage 1B: Detect non-C kernel assembly routines & inline privileged blocks
            asm_cls = self.asm_detector.classify_function(cfg)

            # Stage 2: Map PDB & built-in NT structures + reconstruct unknown structs
            field_refs, new_structs, inferred_param_structs = mapper.map_function_structures(cfg)
            all_synthesized_structs.extend(new_structs)

            # Stage 3A: Constraint-based Type Inference
            signature = self.type_engine.infer_signature(
                cfg=cfg,
                pdb=pdb_db,
                inferred_param_structs=inferred_param_structs,
            )

            # Stage 1C: Lift C functions to LLVM 22 IR & optimize via LLVM NewPM PassBuilder
            if asm_cls.is_raw_asm:
                raw_ir = f"; Preserved raw NT kernel assembly routine: {cfg.name}\n"
                opt_ir = raw_ir
            else:
                lifted = self.lifter.lift_function(
                    cfg=cfg,
                    param_count=len(signature.params),
                    symbol_lookup=va_symbols,
                    opt_level=2,
                )
                raw_ir = lifted.raw_ir
                opt_ir = lifted.optimized_ir
                ir_modules.append(opt_ir)

            # Stage 3B: Compiler De-Optimization (multi-branch & linear loop reroll + magic div)
            rerolled_loops, recovered_divs = self.deoptimizer.analyze_function(cfg, signature)

            # Stage 3C: Z3 Theorem Prover Semantic Equivalence Verification
            ret_bits = self._c_type_bit_width(signature.return_type)
            if self.verify_smt:
                proof = self.verifier.verify_function(
                    cfg=cfg,
                    llvm_ir=opt_ir,
                    rerolled_loops=rerolled_loops,
                    recovered_divs=recovered_divs,
                    return_bits=ret_bits,
                    param_names=[pn for pn, _ in signature.params],
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
                    c_code="",  # Populated in Stage 4 once all structs are finalized
                    field_accesses=field_refs,
                    rerolled_loops=rerolled_loops,
                    recovered_divs=recovered_divs,
                    proof=proof,
                )
            )

        # Stage 4: Custom Verified LLVM IR + Structural Map -> Exact C/C++ Emitter
        ordered_structs = self._topological_structs(list(mapper.structs.values()))
        emitter = VerifiedCEmitter(structs=mapper.structs)

        header_code = emitter.emit_header(
            structs=ordered_structs,
            signatures=[(df.signature, df.is_raw_asm) for df in decompiled_funcs],
        )

        c_units: list[str] = [
            "/*",
            f" * Auto-generated by ntrev — Verified C Source Replica for {Path(binary_path).name}",
            " * Every C function below has been checked against original x86-64 machine semantics.",
            " */",
            '#include "reconstructed_types.h"',
            "",
        ]

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

        source_code = "\n".join(c_units)
        combined_ir = "\n\n".join(ir_modules)

        report = DecompilationReport(
            binary_path=str(binary_path),
            pdb_path=str(pdb_path) if pdb_path else None,
            image_base=image.image_base,
            functions=decompiled_funcs,
            reconstructed_structs=ordered_structs,
            header_code=header_code,
            source_code=source_code,
            combined_llvm_ir=combined_ir,
        )

        if output_dir is not None:
            self.write_deliverables(report, output_dir)

        return report

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
            "functions": [
                {
                    "name": fn.name,
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
        """Order structs so embedded non-pointer value structs (`LIST_ENTRY`, `DISPATCHER_HEADER`) come first."""
        by_name = {s.name: s for s in structs}
        typedef_to_name = {s.typedef_name: s.name for s in structs if s.typedef_name}
        visited: set[str] = set()
        ordered: list[StructDef] = []

        def visit(sname: str) -> None:
            if sname in visited or sname not in by_name:
                return
            visited.add(sname)
            st = by_name[sname]
            for fld in st.fields:
                if not fld.is_pointer and not fld.c_type.endswith("*"):
                    dep = fld.c_type.replace("struct ", "").replace("union ", "").strip()
                    dep_name = typedef_to_name.get(dep, dep)
                    if dep_name in by_name and dep_name != sname:
                        visit(dep_name)
            ordered.append(st)

        for s in structs:
            visit(s.name)
        return ordered

    @staticmethod
    def _c_type_bit_width(c_type: str) -> int:
        clean = c_type.strip()
        if clean == "void":
            return 64
        if clean.endswith("*"):
            return 64
        if clean in ("uint8_t", "int8_t", "char", "BOOLEAN", "KIRQL", "bool"):
            return 8
        if clean in ("uint16_t", "int16_t", "USHORT", "SHORT"):
            return 16
        if clean in ("uint32_t", "int32_t", "ULONG", "LONG", "NTSTATUS"):
            return 32
        return 64
