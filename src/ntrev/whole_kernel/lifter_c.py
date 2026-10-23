"""
Whole-Kernel Streaming C11 Lifter & Relocatable Symbol/Data Emitter (`lifter_c.py`).

Lifts all ~22,200 exported and internal functions in retail `ntoskrnl.exe` into
compilable C11 candidates plus preserved Ring-0 assembly shards. The generated C is
structurally/bit-width aware, but it is not an unconditional semantic or byte-identity
proof; use `whole_kernel.exact` for the byte-preserving execution artifact:
  - Pass 1 (`build_symbol_index`): Streams all `FunctionCFG`s (< 240 MiB RSS) to recover
    collision-free C identifiers, Win64 `ms_abi` parameter arities (callee stack reads +
    caller outgoing stack writes), `/OPT:ICF` RVA symbol aliases, IAT imports, PDB data
    symbols, PE `.reloc` DIR64 pointers, and balanced subsystem shard assignments.
  - Pass 2 (`emit_whole_kernel_tree`): Streams shard-by-shard (< 350 MiB RSS) to emit:
      * `include/nt_whole_kernel_runtime.h` & `include/nt_whole_kernel_protos.h`
      * `include/reconstructed_types.h` (exact PDB `_KPCR`, `_KTHREAD`, `_EPROCESS`, etc.)
      * `src/globals/nt_data_sections.S` & `src/globals/nt_reconstructed_globals.c`
      * ~32 subsystem `.c` shards (`src/<subsys>/<shard>.c`) covering 100% of functions.
"""

from __future__ import annotations

import gc
import hashlib
import json
import re
import struct
import subprocess
from collections import defaultdict
from dataclasses import dataclass, field
from pathlib import Path

from ntrev.analysis.deoptimizer import CompilerDeoptimizer
from ntrev.backend.c_emitter import VerifiedCEmitter
from ntrev.frontend.asm_detector import CANONICAL_GPR64, AsmBlockDetector
from ntrev.frontend.disassembler import COND_JUMPS, RET_MNEMONICS, UNCOND_JUMPS, X86Disassembler
from ntrev.models import (
    BinaryImage,
    DisasmInsn,
    FunctionCFG,
    InferredFuncSignature,
    InsnOperand,
    PDBDatabase,
    StructDef,
    classify_nt_subsystem,
)

# Reserved C11 keywords, local register names, and macro identifiers
_C_RESERVED_NAMES = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "inline", "int", "long", "register", "restrict", "return", "short",
    "signed", "sizeof", "static", "struct", "switch", "typedef", "union",
    "unsigned", "void", "volatile", "while", "_Alignas", "_Alignof",
    "_Atomic", "_Bool", "_Complex", "_Generic", "_Imaginary", "_Noreturn",
    "_Static_assert", "_Thread_local", "bool", "true", "false", "typeof",
    "alignas", "alignof", "static_assert", "thread_local",
    "rax", "rbx", "rcx", "rdx", "rsi", "rdi", "rbp", "rsp",
    "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15",
    "xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7",
    "xmm8", "xmm9", "xmm10", "xmm11", "xmm12", "xmm13", "xmm14", "xmm15",
    "zf", "sf", "cf", "of", "pf", "main",
}

_HIGH8_GPRS = {"ah": "rax", "bh": "rbx", "ch": "rcx", "dh": "rdx"}

_XMM_REGS = {f"xmm{i}" for i in range(16)}

_FLAG_READER_MNEMONICS = {
    "je", "jz", "jne", "jnz", "jb", "jnae", "jc", "jae", "jnb", "jnc",
    "ja", "jnbe", "jbe", "jna", "jl", "jnge", "jge", "jnl",
    "jle", "jng", "jg", "jnle", "js", "jns", "jo", "jno", "jp", "jpe", "jnp", "jpo",
    "loope", "loopne",
    "sete", "setz", "setne", "setnz", "setb", "setnae", "setc", "setae", "setnb", "setnc",
    "seta", "setnbe", "setbe", "setna", "setl", "setnge", "setge", "setnl",
    "setle", "setng", "setg", "setnle", "sets", "setns", "seto", "setno", "setp", "setpe", "setnp", "setpo",
    "cmove", "cmovz", "cmovne", "cmovnz", "cmovb", "cmovnae", "cmovc", "cmovae", "cmovnb", "cmovnc",
    "cmova", "cmovnbe", "cmovbe", "cmovna", "cmovl", "cmovnge", "cmovge", "cmovnl",
    "cmovle", "cmovng", "cmovg", "cmovnle", "cmovs", "cmovns", "cmovo", "cmovno", "cmovp", "cmovpe", "cmovnp", "cmovpo",
    "sbb", "adc", "lock adc",
}

_FULL_FLAG_KILLER_MNEMONICS = {
    "cmp", "test", "add", "sub", "and", "or", "xor", "neg",
    "lock add", "lock and", "lock or", "lock xor", "lock xadd", "lock cmpxchg", "cmpxchg",
    "comisd", "ucomisd", "comiss", "ucomiss",
}

# Well-known x86-64 Windows _KPCR / _KPRCB GS-segment offsets for readable structural comments
_KPCR_GS_FIELD_COMMENTS: dict[int, str] = {
    0x000: "KPCR.NtTib.ExceptionList",
    0x008: "KPCR.NtTib.StackBase",
    0x010: "KPCR.UserRsp",
    0x018: "KPCR.Self",
    0x020: "KPCR.CurrentPrcb",
    0x038: "KPCR.IdtBase",
    0x050: "KPCR.MajorVersion",
    0x054: "KPCR.Irql",
    0x180: "KPCR.Prcb",
    0x184: "KPCR.Prcb.Number",
    0x188: "KPCR.Prcb.CurrentThread (_KTHREAD*)",
    0x190: "KPCR.Prcb.NextThread (_KTHREAD*)",
    0x198: "KPCR.Prcb.IdleThread (_KTHREAD*)",
    0x1A4: "KPCR.Prcb.NestingLevel",
    0x1A8: "KPCR.Prcb.RspBase",
    0x853: "KPCR.Prcb.ShadowFlags",
    0x2E90: "KPCR.Prcb.KeSystemCalls",
    0x2E9C: "KPCR.Prcb.KeContextSwitches",
}


_SUBSYS_TITLES: dict[str, str] = {
    "ke": "Kernel Core Dispatcher, Scheduler, Traps & Interrupts (KE/KI/HAL)",
    "mm": "Virtual Memory Manager, PFN Database & Store Compression (MM/MI/SMKM)",
    "ps": "Process, Thread & Job Object Manager (PS/PSP)",
    "ob": "Executive Object Manager & Handle Tables (OB/OBP)",
    "io": "I/O Manager, IRP Dispatch & Plug-and-Play (IO/IOP/PNP)",
    "ex": "Executive Pool, Heap, PushLocks, WorkItems & Lookaside (EX/EXP)",
    "rtl": "Run-Time Library, AVL/RB Trees, NLS, Crypto & CRT (RTL)",
    "se": "Security Reference Monitor, Tokens, ACLs & Authz (SE/SEP)",
    "cm": "Configuration Manager & Registry Hive Engine (CM/CMP/HVP)",
    "cc": "Cache Manager & Shared Cache Maps (CC/CCP)",
    "fsrtl": "File System Run-Time Library, Oplocks & MCBs (FSRTL)",
    "po": "Power Manager, Processor Idle & Thermal Engine (PO/POP/PPM)",
    "alpc": "Advanced Local Procedure Call & IPC Ports (ALPC/ALPCP)",
    "tm": "Kernel Transaction Manager (TM/TMP)",
    "etw_vf": "Event Tracing for Windows, Hypervisor, WHEA, KD & Driver Verifier (ETW/HVL/VF/KD)",
    "zw_nt": "System Service Dispatch Stubs & Native Syscall Entrypoints (ZW/NT)",
}


def sanitize_c_ident(raw: str, prefix: str = "sym_") -> str:
    cleaned = re.sub(r"[^0-9A-Za-z_]", "_", raw)
    cleaned = re.sub(r"_+", "_", cleaned).strip("_")
    if not cleaned:
        cleaned = prefix + "anon"
    if cleaned[0].isdigit():
        cleaned = prefix + cleaned
    if cleaned in _C_RESERVED_NAMES or cleaned.startswith("__builtin_"):
        cleaned = "nt_" + cleaned
    return cleaned


def sec_ident_name(sec_name: str) -> str:
    s = re.sub(r"[^0-9A-Za-z_]", "_", sec_name.strip("\x00")).strip("_").lower()
    return f"__nt_sec_{s or 'sec'}"


@dataclass
class FunctionMeta:
    """Lightweight per-function metadata collected during Pass 1."""

    va: int
    rva: int
    size: int
    orig_name: str
    c_name: str
    section_name: str
    subsystem_key: str
    subsystem_title: str
    shard_id: str  # e.g. "ke/ke_0"
    is_exported: bool
    is_raw_asm: bool
    asm_reasons: list[str] = field(default_factory=list)
    arity: int = 4
    static_stack_bytes: int = 128
    has_dyn_alloca: bool = False
    nonstd_entry_regs: tuple[str, ...] = ()
    icf_aliases: list[str] = field(default_factory=list)
    export_aliases: list[str] = field(default_factory=list)
    block_count: int = 0
    insn_count: int = 0
    switch_count: int = 0
    original_machine_code_sha256: str = ""


@dataclass
class WholeKernelSymbolIndex:
    """Global symbol, section, relocation, and prototype index for whole-kernel emission."""

    image_base: int
    entry_rva: int
    rsds_guid: str
    rsds_age: int
    pdb_filename: str
    functions_by_rva: dict[int, FunctionMeta] = field(default_factory=dict)
    functions_by_va: dict[int, FunctionMeta] = field(default_factory=dict)
    data_syms_by_rva: dict[int, str] = field(default_factory=dict)  # RVA -> C ident
    sorted_data_rvas: list[int] = field(default_factory=list)
    imports_by_va: dict[int, tuple[str, str]] = field(default_factory=dict)  # IAT VA -> (c_imp_name, dll!fn)
    dir64_relocs: dict[int, int] = field(default_factory=dict)  # RVA -> target RVA
    emitted_data_sections: list[str] = field(default_factory=list)
    mid_asm_labels: dict[int, str] = field(default_factory=dict)  # mid-insn VA -> __nt_asm_ext_<va>
    asm_label_owners: dict[int, int] = field(default_factory=dict)  # mid-insn VA -> owning function entry VA
    asm_data_symbols: dict[int, bytes] = field(default_factory=dict)  # RIP data/table VA -> exact source bytes
    external_asm_stubs: set[str] = field(default_factory=set)
    shard_order: list[str] = field(default_factory=list)


@dataclass
class WholeKernelCompileResult:
    """Native compiler/linker verification of a generated whole-kernel tree."""

    translation_units: int
    compiled_units: int
    failed_units: list[str] = field(default_factory=list)
    relocatable_object: str | None = None
    unresolved_symbols: list[str] = field(default_factory=list)
    unexpected_unresolved_symbols: list[str] = field(default_factory=list)


@dataclass
class WholeKernelEmitSummary:
    """Complete telemetry of a whole-kernel decompilation run."""

    total_functions: int
    pure_c_functions: int
    preserved_asm_functions: int
    exported_functions: int
    internal_functions: int
    icf_alias_count: int
    total_blocks: int
    total_instructions: int
    recovered_switches: int
    recovered_magic_divs: int
    recovered_loops: int
    kpcr_struct_refs: int
    data_symbols_count: int
    import_symbols_count: int
    shard_files: list[str] = field(default_factory=list)


class WholeKernelCLifter:
    """Two-pass streaming whole-kernel C candidate + symbol-relocated ASM lifter.

    This class is an analysis/build pipeline. It intentionally does not label
    recompiled Microsoft C as behaviorally exact; :mod:`ntrev.whole_kernel.exact`
    provides the byte-preserving artifact when exact execution is required.
    """

    MAX_FUNCS_PER_SHARD = 750

    def __init__(self) -> None:
        self.disassembler = X86Disassembler()
        self.asm_detector = AsmBlockDetector()
        self.deoptimizer = CompilerDeoptimizer()

    def run(
        self,
        binary_path: str | Path,
        pdb_path: str | Path,
        out_root: str | Path,
    ) -> tuple[WholeKernelSymbolIndex, WholeKernelEmitSummary]:
        """Run both streaming passes on a real PE32+ kernel and write a self-contained tree."""
        from ntrev.frontend import load_binary_image
        from ntrev.symbols import load_pdb_database

        image = load_binary_image(binary_path)
        pdb = load_pdb_database(pdb_path, binary=image)
        return self.emit_whole_kernel_tree(image, pdb, Path(out_root))

    @staticmethod
    def compile_tree(
        out_root: str | Path,
        compiler: str = "gcc",
        linker: str = "ld",
    ) -> WholeKernelCompileResult:
        """Compile every generated shard and link a relocatable whole-kernel object.

        This is intentionally a separate gate from emission: a decompiler is not useful
        if one rare internal instruction produces uncompilable output. The generated
        tree supplies freestanding GCC 128-bit division helpers; the only normal
        unresolved symbol after `ld -r` is the platform GOT marker. All raw-ASM symbols
        must resolve inside the generated tree.
        """
        root = Path(out_root)
        include = root / "include"
        obj_dir = root / "obj"
        obj_dir.mkdir(parents=True, exist_ok=True)
        for stale_object in obj_dir.glob("*.o"):
            stale_object.unlink()
        sources = sorted((root / "src").glob("*/*.c")) + sorted((root / "src/globals").glob("*.S"))
        # `src/globals/*.c` is included by the first glob; preserve deterministic order.
        sources = sorted(set(sources))
        failures: list[str] = []
        compiled = 0
        for source in sources:
            obj = obj_dir / f"{source.parent.name}__{source.stem}.o"
            cmd = [
                compiler, "-c", "-O1", "-std=gnu11", "-mabi=ms", "-ffreestanding",
                "-fno-builtin", "-fno-tree-loop-distribute-patterns",
                "-Wno-unused-variable", "-Wno-unused-label", f"-I{include}",
                str(source), "-o", str(obj),
            ]
            result = subprocess.run(cmd, capture_output=True, text=True)
            if result.returncode != 0:
                failures.append(f"{source}: {result.stderr[-2000:]}")
            else:
                compiled += 1

        reloc_obj = root / "whole_kernel_reloc.o"
        unresolved: list[str] = []
        if not failures:
            objects = sorted(str(p) for p in obj_dir.glob("*.o"))
            link_result = subprocess.run(
                [linker, "-r", "-o", str(reloc_obj), *objects],
                capture_output=True,
                text=True,
            )
            if link_result.returncode != 0:
                failures.append(f"{linker} -r: {link_result.stderr[-2000:]}")
            else:
                nm = subprocess.run(["nm", "-u", str(reloc_obj)], capture_output=True, text=True)
                unresolved = sorted({line.split()[-1] for line in nm.stdout.splitlines() if line.strip()})

        allowed = {"_GLOBAL_OFFSET_TABLE_"}
        unexpected = [s for s in unresolved if s not in allowed]
        return WholeKernelCompileResult(
            translation_units=len(sources),
            compiled_units=compiled,
            failed_units=failures,
            relocatable_object=str(reloc_obj) if reloc_obj.is_file() else None,
            unresolved_symbols=unresolved,
            unexpected_unresolved_symbols=unexpected,
        )

    # ------------------------------------------------------------------
    # PASS 1: Streaming Symbol, Arity, Alias & Relocation Index
    # ------------------------------------------------------------------
    def build_symbol_index(
        self,
        image: BinaryImage,
        pdb: PDBDatabase | None = None,
    ) -> WholeKernelSymbolIndex:
        base = image.image_base
        rsds_guid = image.rsds.guid if image.rsds else (pdb.guid if pdb else "00000000-0000-0000-0000-000000000000")
        rsds_age = image.rsds.age if image.rsds else (pdb.age if pdb else 1)
        pdb_leaf = image.rsds.pdb_filename if image.rsds else "ntkrnlmp.pdb"

        idx = WholeKernelSymbolIndex(
            image_base=base,
            entry_rva=image.entry_point_rva,
            rsds_guid=rsds_guid,
            rsds_age=rsds_age,
            pdb_filename=pdb_leaf,
        )

        used_idents: set[str] = set(_C_RESERVED_NAMES)
        if pdb is not None:
            for st in pdb.structs.values():
                if st.typedef_name:
                    used_idents.add(st.typedef_name)
                if st.name.startswith("_") and len(st.name) > 1:
                    used_idents.add(st.name[1:])

        def alloc_ident(raw: str, rva_hint: int | None = None, prefix: str = "sym_") -> str:
            cand = sanitize_c_ident(raw, prefix=prefix)
            if cand not in used_idents:
                used_idents.add(cand)
                return cand
            if rva_hint is not None:
                cand2 = f"{cand}_{rva_hint:x}"
                if cand2 not in used_idents:
                    used_idents.add(cand2)
                    return cand2
            n = 2
            while f"{cand}_{n}" in used_idents:
                n += 1
            res = f"{cand}_{n}"
            used_idents.add(res)
            return res

        # 1. Register IAT imports (`__imp_<Func>`)
        for iat_va, full_imp in sorted(image.imports.items()):
            fn_part = full_imp.split("!", 1)[-1]
            c_imp = alloc_ident(f"__imp_{fn_part}", iat_va - base, prefix="__imp_")
            idx.imports_by_va[iat_va] = (c_imp, full_imp)

        # 2. Identify non-executable data/rodata sections to emit in `nt_data_sections.S`
        skip_nonexec = {".edata", ".idata", ".reloc", ".rsrc"}
        exec_rva_ranges: list[tuple[int, int]] = []
        nonexec_rva_ranges: list[tuple[int, int, str]] = []
        for sec in image.sections:
            s_start = sec.virtual_address
            s_end = sec.virtual_address + max(sec.virtual_size, len(sec.raw_data))
            if sec.is_executable:
                exec_rva_ranges.append((s_start, s_end))
            elif sec.name not in skip_nonexec:
                idx.emitted_data_sections.append(sec.name)
                used_idents.add(sec_ident_name(sec.name))
                nonexec_rva_ranges.append((s_start, s_end, sec.name))

        def in_emitted_nonexec(rva: int) -> bool:
            return any(a <= rva < b for a, b, _ in nonexec_rva_ranges)

        # 3. Parse PE `.reloc` IMAGE_REL_BASED_DIR64 entries for symbolic data relocation
        idx.dir64_relocs = self._parse_pe_dir64_relocs(image)

        # 4. Stream all disassembled functions, recording metadata & call-site stack arity
        caller_arity_votes: dict[int, int] = defaultdict(lambda: 4)
        asm_ext_targets: list[tuple[str, int]] = []
        asm_rip_targets: set[int] = set()
        all_insn_owner_va: dict[int, int] = {}

        # Group export names sharing the same RVA (`/OPT:ICF` aliases!)
        exports_at_rva: dict[int, list[str]] = defaultdict(list)
        for exp_name, exp_rva in sorted(image.export_by_name.items()):
            exports_at_rva[exp_rva].append(exp_name)

        for cfg in self.disassembler.iter_disassemble_image(image, pdb=pdb):
            rva = cfg.address - base
            cls = self.asm_detector.classify_function(cfg, whole_kernel_mode=True)
            nonstd = tuple(self.asm_detector._check_non_standard_entry_regs(cfg))
            sub_key = classify_nt_subsystem(cfg.name)
            sub_title = _SUBSYS_TITLES.get(sub_key, sub_key.upper())

            # Prefer official primary export name if exported at this exact RVA
            primary_raw = image.exports.get(rva, cfg.name)
            c_name = alloc_ident(primary_raw, rva, prefix="sub_")

            is_exp = rva in exports_at_rva
            exp_aliases: list[str] = []
            icf_c_aliases: list[str] = []
            for ename in exports_at_rva.get(rva, []):
                exp_aliases.append(ename)
                eclean = sanitize_c_ident(ename, prefix="exp_")
                if eclean != c_name and eclean not in used_idents:
                    used_idents.add(eclean)
                    icf_c_aliases.append(eclean)

            callee_arity, static_stack, has_dyn = self._analyze_stack_and_arity(cfg)
            n_insns = sum(len(b.instructions) for b in cfg.blocks.values())
            original_bytes = image.read_va(cfg.address, cfg.size)
            original_machine_code_sha256 = (
                hashlib.sha256(original_bytes).hexdigest()
                if original_bytes is not None and len(original_bytes) == cfg.size
                else ""
            )

            # Track instruction addresses inside raw-ASM functions for mid-stub calls
            if cls.is_raw_asm:
                local_addrs = {i.address for b in cfg.blocks.values() for i in b.instructions}
                for a in local_addrs:
                    all_insn_owner_va[a] = cfg.address
                for b in cfg.blocks.values():
                    for ins in b.instructions:
                        if (
                            (ins.mnemonic == "call" or ins.mnemonic.startswith(("j", "loop")))
                            and len(ins.operands) == 1
                            and ins.operands[0].kind == "imm"
                        ):
                            tgt = ins.operands[0].imm
                            if tgt not in local_addrs:
                                asm_ext_targets.append((c_name, tgt))
                        for op in ins.operands:
                            if op.kind == "mem" and op.base == "rip":
                                asm_rip_targets.add((ins.address + ins.size + op.disp) & 0xFFFFFFFFFFFFFFFF)
            else:
                # Scan outgoing call/tailcall argument writes in pure-C functions
                for b in cfg.blocks.values():
                    max_out_slot = -1
                    for ins in b.instructions:
                        # Check `[rsp + disp]` writes in `0x20 .. 0xB8`
                        if ins.operands and ins.operands[0].kind == "mem":
                            m = ins.operands[0]
                            if m.base == "rsp" and m.index is None and 0x20 <= m.disp <= 0xB8:
                                top_off = m.disp + (8 if m.size == 16 else 0)
                                slot = (top_off - 0x20) // 8
                                if slot > max_out_slot:
                                    max_out_slot = slot
                        if ins.mnemonic == "call" and len(ins.operands) == 1:
                            if ins.operands[0].kind == "imm" and max_out_slot >= 0:
                                tgt_va = ins.operands[0].imm
                                vote = min(20, 5 + max_out_slot)
                                if vote > caller_arity_votes[tgt_va]:
                                    caller_arity_votes[tgt_va] = vote
                            max_out_slot = -1

            meta = FunctionMeta(
                va=cfg.address,
                rva=rva,
                size=cfg.size,
                orig_name=cfg.name,
                c_name=c_name,
                section_name=cfg.section_name,
                subsystem_key=sub_key,
                subsystem_title=sub_title,
                shard_id=sub_key,
                is_exported=is_exp,
                is_raw_asm=cls.is_raw_asm,
                asm_reasons=list(cls.reasons[:3]),
                arity=callee_arity,
                static_stack_bytes=static_stack,
                has_dyn_alloca=has_dyn,
                nonstd_entry_regs=nonstd,
                icf_aliases=icf_c_aliases,
                export_aliases=exp_aliases,
                block_count=len(cfg.blocks),
                insn_count=n_insns,
                switch_count=len(cfg.jump_tables),
                original_machine_code_sha256=original_machine_code_sha256,
            )
            idx.functions_by_rva[rva] = meta
            idx.functions_by_va[cfg.address] = meta

        # Merge caller arity votes into callee arities
        for fva, vote in caller_arity_votes.items():
            fm = idx.functions_by_va.get(fva)
            if fm is not None and vote > fm.arity:
                fm.arity = min(20, vote)

        # Resolve mid-function ASM branch targets and RIP-relative code/data targets.
        for _, tgt_va in asm_ext_targets:
            if tgt_va not in idx.functions_by_va:
                lbl = f"__nt_asm_ext_{tgt_va:x}"
                if tgt_va in all_insn_owner_va:
                    idx.mid_asm_labels[tgt_va] = lbl
                    idx.asm_label_owners[tgt_va] = all_insn_owner_va[tgt_va]
                else:
                    idx.external_asm_stubs.add(lbl)

        for tgt_va in sorted(asm_rip_targets):
            if tgt_va in idx.functions_by_va or tgt_va in idx.imports_by_va:
                continue
            if tgt_va in all_insn_owner_va:
                idx.mid_asm_labels.setdefault(tgt_va, f"__nt_asm_ext_{tgt_va:x}")
                idx.asm_label_owners.setdefault(tgt_va, all_insn_owner_va[tgt_va])
                continue
            # A RIP-relative target that is neither a function entry nor a known data/IAT symbol
            # is an inline literal, jump table, import directory slot, or patchpoint. Preserve the
            # exact source bytes behind a global symbol so every raw-ASM relocation links cleanly.
            raw = image.read_va(tgt_va, 128)
            if raw:
                idx.asm_data_symbols[tgt_va] = raw

        # 5. Register named data globals (exports + PDB data symbols in emitted non-exec sections)
        iat_rvas = {va - base for va in idx.imports_by_va}
        for exp_rva, exp_name in sorted(image.exports.items()):
            if exp_rva not in idx.functions_by_rva and exp_rva not in iat_rvas and in_emitted_nonexec(exp_rva):
                c_sym = alloc_ident(exp_name, exp_rva, prefix="g_")
                idx.data_syms_by_rva[exp_rva] = c_sym

        if pdb is not None:
            for drva, dsym in sorted(pdb.data_symbols_by_rva.items()):
                if (
                    drva not in idx.functions_by_rva
                    and drva not in idx.data_syms_by_rva
                    and drva not in iat_rvas
                    and in_emitted_nonexec(drva)
                ):
                    c_sym = alloc_ident(dsym.name, drva, prefix="g_")
                    idx.data_syms_by_rva[drva] = c_sym

        idx.sorted_data_rvas = sorted(idx.data_syms_by_rva.keys())

        # 6. Partition functions into balanced subsystem shards (<= MAX_FUNCS_PER_SHARD each)
        by_subsys: dict[str, list[FunctionMeta]] = defaultdict(list)
        for rva in sorted(idx.functions_by_rva.keys()):
            fm = idx.functions_by_rva[rva]
            by_subsys[fm.subsystem_key].append(fm)

        shard_order: list[str] = []
        for sub_key in sorted(by_subsys.keys()):
            fn_list = by_subsys[sub_key]
            if len(fn_list) <= self.MAX_FUNCS_PER_SHARD:
                sid = f"{sub_key}/{sub_key}_kernel"
                shard_order.append(sid)
                for fm in fn_list:
                    fm.shard_id = sid
            else:
                n_parts = (len(fn_list) + self.MAX_FUNCS_PER_SHARD - 1) // self.MAX_FUNCS_PER_SHARD
                chunk_sz = (len(fn_list) + n_parts - 1) // n_parts
                for p_idx in range(n_parts):
                    part = fn_list[p_idx * chunk_sz : (p_idx + 1) * chunk_sz]
                    if not part:
                        continue
                    sid = f"{sub_key}/{sub_key}_part{p_idx:02d}"
                    shard_order.append(sid)
                    for fm in part:
                        fm.shard_id = sid
        idx.shard_order = shard_order

        gc.collect()
        return idx

    def _parse_pe_dir64_relocs(self, image: BinaryImage) -> dict[int, int]:
        """Extract IMAGE_REL_BASED_DIR64 entries from `.reloc` for symbolic data section emission."""
        relocs: dict[int, int] = {}
        reloc_sec = next((s for s in image.sections if s.name == ".reloc"), None)
        if reloc_sec is None or not reloc_sec.raw_data:
            return relocs
        buf = reloc_sec.raw_data
        pos = 0
        n = len(buf)
        base = image.image_base
        while pos + 8 <= n:
            page_rva, block_sz = struct.unpack_from("<II", buf, pos)
            if block_sz < 8 or pos + block_sz > n:
                break
            cnt = (block_sz - 8) // 2
            for i in range(cnt):
                entry = struct.unpack_from("<H", buf, pos + 8 + i * 2)[0]
                rtype = entry >> 12
                roff = entry & 0xFFF
                if rtype == 10:  # IMAGE_REL_BASED_DIR64
                    slot_rva = page_rva + roff
                    raw8 = image.read_va(base + slot_rva, 8)
                    if raw8 and len(raw8) == 8:
                        target_va = struct.unpack("<Q", raw8)[0]
                        if base <= target_va < base + 0x10000000:
                            relocs[slot_rva] = target_va - base
            pos += block_sz
        return relocs

    def _analyze_stack_and_arity(self, cfg: FunctionCFG) -> tuple[int, int, bool]:
        """Compute incoming parameter count (4..20), static stack frame bytes, and dynamic alloca."""
        entry_blk = cfg.blocks.get(cfg.entry_address)
        sp_delta = 0
        alias_deltas: dict[str, int] = {"rsp": 0}
        has_dyn = False
        max_sub_rsp = 0

        if entry_blk is not None:
            for ins in entry_blk.instructions[:24]:
                mn = ins.mnemonic
                ops = ins.operands
                if mn == "push" and len(ops) == 1:
                    sp_delta -= 8
                    alias_deltas["rsp"] = sp_delta
                elif (
                    mn == "sub"
                    and len(ops) == 2
                    and ops[0].kind == "reg"
                    and ops[0].reg == "rsp"
                ):
                    if ops[1].kind == "imm":
                        imm = ops[1].imm & 0xFFFFF
                        sp_delta -= imm
                        alias_deltas["rsp"] = sp_delta
                        max_sub_rsp = max(max_sub_rsp, -sp_delta)
                    else:
                        has_dyn = True
                elif (
                    mn == "mov"
                    and len(ops) == 2
                    and ops[0].kind == "reg"
                    and ops[1].kind == "reg"
                    and ops[1].reg == "rsp"
                    and ops[0].reg in ("rbp", "rax", "r11")
                ):
                    alias_deltas[ops[0].reg] = sp_delta
                elif (
                    mn == "lea"
                    and len(ops) == 2
                    and ops[0].kind == "reg"
                    and ops[0].reg == "rbp"
                    and ops[1].kind == "mem"
                    and ops[1].base == "rsp"
                    and ops[1].index is None
                ):
                    alias_deltas["rbp"] = sp_delta + ops[1].disp

        arity = 4
        push_count = 0
        for blk in cfg.blocks.values():
            tmp_aliases = dict(alias_deltas)
            for ins in blk.instructions:
                mn = ins.mnemonic
                ops = ins.operands
                if mn == "push":
                    push_count += 1
                elif (
                    mn == "sub"
                    and len(ops) == 2
                    and ops[0].kind == "reg"
                    and ops[0].reg == "rsp"
                ):
                    if ops[1].kind == "imm":
                        max_sub_rsp = max(max_sub_rsp, (ops[1].imm & 0xFFFFF) + 64)
                    else:
                        has_dyn = True
                for op in ops:
                    if op.kind == "mem" and op.base in tmp_aliases and op.index is None:
                        entry_off = tmp_aliases[op.base] + op.disp
                        if 0x28 <= entry_off <= 0xC0:
                            slot = (entry_off - 0x28) // 8
                            arity = max(arity, min(20, 5 + slot))
                # Kill volatile prologue frame aliases (`rax`/`r11`) once overwritten
                for wr in ins.regs_write:
                    cwr = CANONICAL_GPR64.get(wr, wr)
                    if cwr in ("rax", "r11") and cwr in tmp_aliases:
                        if not (
                            mn == "mov"
                            and len(ops) == 2
                            and ops[1].kind == "reg"
                            and ops[1].reg == "rsp"
                        ):
                            tmp_aliases.pop(cwr, None)

        static_bytes = max(160, max_sub_rsp + push_count * 8 + 128)
        static_bytes = (static_bytes + 15) & ~15
        if static_bytes > 32768:
            static_bytes = 32768
        return arity, static_bytes, has_dyn

    # ------------------------------------------------------------------
    # PASS 2: Streaming Whole-Kernel Emission
    # ------------------------------------------------------------------
    def emit_whole_kernel_tree(
        self,
        image: BinaryImage,
        pdb: PDBDatabase | None,
        out_root: Path,
        index: WholeKernelSymbolIndex | None = None,
    ) -> tuple[WholeKernelSymbolIndex, WholeKernelEmitSummary]:
        if index is None:
            index = self.build_symbol_index(image, pdb)

        inc_dir = out_root / "include"
        src_dir = out_root / "src"
        specs_dir = out_root / "specs"
        inc_dir.mkdir(parents=True, exist_ok=True)
        src_dir.mkdir(parents=True, exist_ok=True)
        specs_dir.mkdir(parents=True, exist_ok=True)

        # 1. Emit PDB reconstructed struct header (`include/reconstructed_types.h`)
        structs: dict[str, StructDef] = pdb.structs if pdb is not None else {}
        core_struct_names = [
            "_LIST_ENTRY", "_SINGLE_LIST_ENTRY", "_UNICODE_STRING", "_STRING",
            "_DISPATCHER_HEADER", "_KPCR", "_KPRCB", "_KTHREAD", "_ETHREAD",
            "_KPROCESS", "_EPROCESS", "_OBJECT_HEADER", "_OBJECT_TYPE",
            "_KDPC", "_KAPC", "_KTIMER", "_KEVENT", "_KMUTANT", "_KSEMAPHORE",
            "_IRP", "_IO_STACK_LOCATION", "_DEVICE_OBJECT", "_DRIVER_OBJECT",
            "_FILE_OBJECT", "_MDL", "_ERESOURCE", "_RTL_BALANCED_NODE",
            "_CONTEXT", "_KTRAP_FRAME", "_EXCEPTION_RECORD",
        ]
        selected_structs = [structs[n] for n in core_struct_names if n in structs]
        if not selected_structs and structs:
            selected_structs = list(structs.values())[:32]
        type_hdr = VerifiedCEmitter(structs=structs).emit_header(selected_structs, [])
        # Rename inline helpers in reconstructed_types.h that also exist as real exported kernel symbols
        type_hdr = type_hdr.replace(
            "static inline struct _KTHREAD* KeGetCurrentThread(",
            "static inline struct _KTHREAD* __ntrev_inline_KeGetCurrentThread(",
        ).replace(
            "static inline struct _KPRCB* KeGetCurrentPrcb(",
            "static inline struct _KPRCB* __ntrev_inline_KeGetCurrentPrcb(",
        )
        (inc_dir / "reconstructed_types.h").write_text(type_hdr, encoding="utf-8")

        # 2. Emit whole-kernel runtime & prototype headers
        (inc_dir / "nt_whole_kernel_runtime.h").write_text(
            self._emit_runtime_header(), encoding="utf-8"
        )
        (inc_dir / "nt_whole_kernel_protos.h").write_text(
            self._emit_protos_header(index), encoding="utf-8"
        )

        # 3. Emit `src/globals/nt_data_sections.S` and `src/globals/nt_reconstructed_globals.c`
        glob_dir = src_dir / "globals"
        glob_dir.mkdir(parents=True, exist_ok=True)
        (glob_dir / "nt_data_sections.S").write_text(
            self._emit_data_sections_asm(image, index), encoding="utf-8"
        )
        (glob_dir / "nt_reconstructed_globals.c").write_text(
            self._emit_globals_and_imports_c(index), encoding="utf-8"
        )
        runtime_dir = src_dir / "runtime"
        runtime_dir.mkdir(parents=True, exist_ok=True)
        (runtime_dir / "nt_int128_runtime.c").write_text(
            self._emit_int128_runtime_c(), encoding="utf-8"
        )

        # 4. Build fast VA -> GAS symbol map for raw-ASM functions
        asm_va_map = self._build_asm_va_symbol_map(image, index)

        # 5. Stream functions again and buffer per shard, flushing completed shards immediately!
        shard_expected_counts: dict[str, int] = defaultdict(int)
        for fm in index.functions_by_rva.values():
            shard_expected_counts[fm.shard_id] += 1

        shard_buffers: dict[str, list[str]] = defaultdict(list)
        shard_const_pools: dict[str, dict[int, bytes]] = defaultdict(dict)
        shard_written: list[str] = []

        total_c = 0
        total_asm = 0
        total_exp = 0
        total_icf = 0
        total_blks = 0
        total_insns = 0
        total_switches = 0
        total_mdivs = 0
        total_loops = 0
        total_kpcr_refs = 0

        for cfg in self.disassembler.iter_disassemble_image(image, pdb=pdb):
            rva = cfg.address - image.image_base
            fm = index.functions_by_rva.get(rva)
            if fm is None:
                continue

            cfg.name = fm.c_name
            total_blks += len(cfg.blocks)
            total_insns += fm.insn_count
            total_switches += len(cfg.jump_tables)
            if fm.is_exported:
                total_exp += 1
            total_icf += len(fm.icf_aliases)

            sig = InferredFuncSignature(
                name=fm.c_name,
                return_type="uint64_t",
                params=[(f"p{i}", "uint64_t") for i in range(1, fm.arity + 1)],
            )

            if fm.is_raw_asm:
                total_asm += 1
                self.asm_detector.classify_function(cfg, whole_kernel_mode=True)
                fn_code = self.asm_detector.format_raw_asm_function(
                    cfg,
                    signature=sig,
                    va_symbol_map=asm_va_map,
                    whole_kernel_mode=True,
                    exported_mid_labels=index.mid_asm_labels,
                    mid_label_owners=index.asm_label_owners,
                )
            else:
                total_c += 1
                fn_code, mdivs, loops, kpcr_refs = self.lift_function_to_c(
                    cfg,
                    fm,
                    image,
                    index,
                    shard_const_pools[fm.shard_id],
                )
                total_mdivs += mdivs
                total_loops += loops
                total_kpcr_refs += kpcr_refs

            if fm.icf_aliases:
                alias_lines = [fn_code]
                for al in fm.icf_aliases:
                    alias_lines.append(
                        f'extern __typeof__({fm.c_name}) {al} __attribute__((alias("{fm.c_name}")));'
                    )
                fn_code = "\n".join(alias_lines)

            shard_buffers[fm.shard_id].append(fn_code)
            shard_expected_counts[fm.shard_id] -= 1

            # Flush shard to disk and immediately release RAM as soon as its last function finishes!
            if shard_expected_counts[fm.shard_id] <= 0:
                rel_c = self._flush_shard_file(
                    src_dir,
                    fm.shard_id,
                    fm.subsystem_title,
                    shard_buffers.pop(fm.shard_id),
                    shard_const_pools.pop(fm.shard_id, {}),
                )
                shard_written.append(rel_c)
                gc.collect()

        # Flush any remaining partial buffers
        for sid in list(shard_buffers.keys()):
            if shard_buffers[sid]:
                rel_c = self._flush_shard_file(
                    src_dir,
                    sid,
                    sid.split("/")[0].upper(),
                    shard_buffers.pop(sid),
                    shard_const_pools.pop(sid, {}),
                )
                shard_written.append(rel_c)

        # 6. Emit `specs/nt_whole_kernel_rvas.json` for PE32+ & binary-grafted drop-in linker
        self._write_rva_manifest(specs_dir / "nt_whole_kernel_rvas.json", image, index)

        summary = WholeKernelEmitSummary(
            total_functions=total_c + total_asm,
            pure_c_functions=total_c,
            preserved_asm_functions=total_asm,
            exported_functions=total_exp,
            internal_functions=(total_c + total_asm) - total_exp,
            icf_alias_count=total_icf,
            total_blocks=total_blks,
            total_instructions=total_insns,
            recovered_switches=total_switches,
            recovered_magic_divs=total_mdivs,
            recovered_loops=total_loops,
            kpcr_struct_refs=total_kpcr_refs,
            data_symbols_count=len(index.data_syms_by_rva),
            import_symbols_count=len(index.imports_by_va),
            shard_files=sorted(shard_written),
        )
        (specs_dir / "whole_kernel_summary.json").write_text(
            json.dumps(summary.__dict__, indent=2), encoding="utf-8"
        )
        gc.collect()
        return index, summary

    def _flush_shard_file(
        self,
        src_dir: Path,
        shard_id: str,
        subsystem_title: str,
        fn_bodies: list[str],
        const_pool: dict[int, bytes],
    ) -> str:
        rel_path = f"src/{shard_id}.c"
        full_path = src_dir.parent / rel_path
        full_path.parent.mkdir(parents=True, exist_ok=True)

        header = [
            f"/* ==========================================================================",
            f" * Decompiled Windows NT Kernel Subsystem Shard: {shard_id}",
            f" * Subsystem: {subsystem_title} ({len(fn_bodies)} functions)",
            f" * Calling Convention: Windows x64 (__attribute__((ms_abi)))",
            f" * Generated automatically by ntrev WholeKernelCLifter",
            f" * ========================================================================== */",
            '#include "nt_whole_kernel_runtime.h"',
            '#include "nt_whole_kernel_protos.h"',
            "",
        ]
        if const_pool:
            header.append("/* Embedded .text constant lookup tables referenced via RIP-relative LEA */")
            for crva, cbytes in sorted(const_pool.items()):
                hex_b = ", ".join(f"0x{b:02x}" for b in cbytes)
                header.append(
                    f"static const uint8_t __nt_kconst_{crva:x}[{len(cbytes)}] __attribute__((aligned(16))) = {{ {hex_b} }};"
                )
            header.append("")

        full_path.write_text("\n".join(header) + "\n\n".join(fn_bodies) + "\n", encoding="utf-8")
        return rel_path

    # ------------------------------------------------------------------
    # Pure-C Function Lifter (All 143+ x86-64 Mnemonics + DFE + Switch)
    # ------------------------------------------------------------------
    def lift_function_to_c(
        self,
        cfg: FunctionCFG,
        fm: FunctionMeta,
        image: BinaryImage,
        index: WholeKernelSymbolIndex,
        shard_const_pool: dict[int, bytes],
    ) -> tuple[str, int, int, int]:
        """Lift a single pure-C `FunctionCFG` into bit-accurate, dead-flag-eliminated C11."""
        sig = InferredFuncSignature(
            name=fm.c_name,
            return_type="uint64_t",
            params=[(f"p{i}", "uint64_t") for i in range(1, fm.arity + 1)],
        )
        loops, mdivs = self.deoptimizer.analyze_function(cfg, sig)
        mdiv_by_addr = {d.insn_addrs[0]: d for d in mdivs if d.insn_addrs}
        loop_by_hdr = {lp.header_addr: lp for lp in loops}

        live_flags, live_out_map, uses_pf = self._compute_live_flags(cfg)
        used_gprs, used_xmms, max_call_extra, has_any_call = self._scan_used_regs_and_calls(cfg, index)

        params_sig = ", ".join(f"uint64_t p{i}" for i in range(1, fm.arity + 1))
        lines: list[str] = []
        exp_tag = "EXPORTED" if fm.is_exported else "INTERNAL"
        lines.append(
            f"/* [{exp_tag}] {fm.orig_name} @ 0x{fm.va:x} (RVA 0x{fm.rva:x}, {fm.insn_count}i/{len(cfg.blocks)}b) */"
        )
        lines.append(f"uint64_t __attribute__((ms_abi)) {fm.c_name}({params_sig}) {{")

        needs_frame = ("rsp" in used_gprs) or has_any_call or (fm.arity > 4) or fm.has_dyn_alloca
        if needs_frame:
            local_bytes = fm.static_stack_bytes + (8192 if fm.has_dyn_alloca else 0)
            local_bytes = (local_bytes + 15) & ~15
            in_extra = max(0, fm.arity - 4)
            upper_slots = max(in_extra, max_call_extra, 16)
            upper_bytes = ((0x28 + 8 * upper_slots) + 15) & ~15
            total_frame = local_bytes + upper_bytes
            lines.append(f"    uint8_t __frame[{total_frame}] __attribute__((aligned(16)));")
            lines.append(
                f"    uint64_t rax = 0, rcx = p1, rdx = p2, r8 = p3, r9 = p4, rsp = (uint64_t)(uintptr_t)&__frame[{local_bytes}];"
            )
        else:
            lines.append("    uint64_t rax = 0, rcx = p1, rdx = p2, r8 = p3, r9 = p4;")

        extra_gprs = [
            r
            for r in ("rbx", "rsi", "rdi", "rbp", "r10", "r11", "r12", "r13", "r14", "r15")
            if r in used_gprs
        ]
        if extra_gprs:
            lines.append("    uint64_t " + ", ".join(f"{r} = 0" for r in extra_gprs) + ";")

        if used_xmms:
            sorted_xmms = sorted(used_xmms, key=lambda x: int(x[3:]))
            lines.append("    nt_xmm_t " + ", ".join(f"{x} = {{0}}" for x in sorted_xmms) + ";")

        flag_vars = ["zf = 0", "sf = 0", "cf = 0", "of = 0"]
        if uses_pf:
            flag_vars.append("pf = 0")
        lines.append("    uint8_t " + ", ".join(flag_vars) + ";")

        if needs_frame:
            lines.append("    M64(rsp) = 0;")
            for p_idx in range(5, fm.arity + 1):
                off = 0x28 + 8 * (p_idx - 5)
                lines.append(f"    M64(rsp + 0x{off:x}) = p{p_idx};")
            for z_idx in range(fm.arity + 1, 5 + min(max_call_extra, 8)):
                off = 0x28 + 8 * (z_idx - 5)
                lines.append(f"    M64(rsp + 0x{off:x}) = 0;")

        sorted_blks = cfg.sorted_blocks()
        needed_labels: set[int] = set()
        if sorted_blks and sorted_blks[0].address != cfg.entry_address and cfg.entry_address in cfg.blocks:
            needed_labels.add(cfg.entry_address)
            lines.append(f"    goto L_{cfg.entry_address:x};")

        for b_idx, b_item in enumerate(sorted_blks):
            nxt_a = sorted_blks[b_idx + 1].address if b_idx + 1 < len(sorted_blks) else None
            if b_item.branch_target is not None and b_item.branch_target in cfg.blocks:
                if not (
                    b_item.instructions
                    and b_item.instructions[-1].mnemonic == "jmp"
                    and b_item.branch_target == nxt_a
                ):
                    needed_labels.add(b_item.branch_target)
            if b_item.fallthrough is not None and b_item.fallthrough in cfg.blocks and b_item.fallthrough != nxt_a:
                needed_labels.add(b_item.fallthrough)
        for jt in cfg.jump_tables.values():
            for t_va in jt.targets:
                if t_va in cfg.blocks:
                    needed_labels.add(t_va)
            if jt.default_target and jt.default_target in cfg.blocks:
                needed_labels.add(jt.default_target)

        # Collect subsumed setup instructions of switch jump tables
        jt_skip_addrs: set[int] = set()
        for jt in cfg.jump_tables.values():
            jt_skip_addrs.update(jt.setup_insn_addrs)

        kpcr_ref_count = 0

        for blk_idx, blk in enumerate(sorted_blks):
            next_blk_addr = sorted_blks[blk_idx + 1].address if blk_idx + 1 < len(sorted_blks) else None
            if blk.address in needed_labels:
                lines.append(f"L_{blk.address:x}:;")

            if blk.address in loop_by_hdr:
                lp = loop_by_hdr[blk.address]
                lines.append(f"    /* [DEOPT LOOP] {lp.kind} unroll={lp.max_unroll_factor}x @ 0x{blk.address:x}: {lp.description} */")

            if not blk.instructions:
                if blk.fallthrough is not None and blk.fallthrough in cfg.blocks and blk.fallthrough != next_blk_addr:
                    lines.append(f"    goto L_{blk.fallthrough:x};")
                continue

            # Check direct idiomatic C fusion of trailing `cmp`/`test` + `jcc` when `live_out == False`
            fused_pair_lines: list[str] | None = None
            if len(blk.instructions) >= 2 and not live_out_map.get(blk.address, False):
                fused_pair_lines = self._try_fuse_cmp_test_jcc(
                    blk.instructions[-2],
                    blk.instructions[-1],
                    blk,
                    cfg,
                    image,
                    index,
                    shard_const_pool,
                    next_blk_addr,
                )

            ins_limit = len(blk.instructions) - 2 if fused_pair_lines is not None else len(blk.instructions)

            for ins_idx in range(ins_limit):
                ins = blk.instructions[ins_idx]
                if ins.address in jt_skip_addrs:
                    continue
                if ins.address in mdiv_by_addr:
                    md = mdiv_by_addr[ins.address]
                    lines.append(
                        f"    /* [DEOPT DIV] compiler magic-multiply {'signed' if md.is_signed else 'unsigned'} division by {md.divisor} */"
                    )
                for op in ins.operands:
                    if op.kind == "mem" and op.segment == "gs":
                        kpcr_ref_count += 1
                        kcomment = _KPCR_GS_FIELD_COMMENTS.get(op.disp)
                        if kcomment:
                            lines.append(f"    /* [PDB STRUCT] gs:[0x{op.disp:x}] -> {kcomment} */")

                is_last = ins_idx == len(blk.instructions) - 1
                need_f = live_flags.get(ins.address, False)
                stmt_lines = self._lift_insn(
                    ins=ins,
                    blk=blk,
                    cfg=cfg,
                    image=image,
                    index=index,
                    shard_const_pool=shard_const_pool,
                    need_flags=need_f,
                    uses_pf=uses_pf,
                    is_last=is_last,
                    next_blk_addr=next_blk_addr,
                )
                for sl in stmt_lines:
                    lines.append("    " + sl)

            if fused_pair_lines is not None:
                for op in blk.instructions[-2].operands:
                    if op.kind == "mem" and op.segment == "gs":
                        kpcr_ref_count += 1
                for sl in fused_pair_lines:
                    lines.append("    " + sl)

            # Ensure explicit control transfer if block does not end in ret/jmp/cond-jmp/trap
            last_mn = blk.instructions[-1].mnemonic
            if (
                last_mn not in RET_MNEMONICS
                and last_mn not in UNCOND_JUMPS
                and last_mn not in COND_JUMPS
                and last_mn != "int3"
            ):
                if blk.fallthrough is not None and blk.fallthrough in cfg.blocks:
                    if blk.fallthrough != next_blk_addr:
                        lines.append(f"    goto L_{blk.fallthrough:x};")
                elif not blk.successors:
                    lines.append("    return rax;")

        lines.append("    return rax;")
        lines.append("}")
        return "\n".join(lines), len(mdivs), len(loops), kpcr_ref_count

    def _try_fuse_cmp_test_jcc(
        self,
        cmp_ins: DisasmInsn,
        jcc_ins: DisasmInsn,
        blk: object,
        cfg: FunctionCFG,
        image: BinaryImage,
        index: WholeKernelSymbolIndex,
        shard_const_pool: dict[int, bytes],
        next_blk_addr: int | None,
    ) -> list[str] | None:
        """Fuse trailing `cmp/test` + `jcc` into a single idiomatic C `if (...)` when flags die."""
        cm = cmp_ins.mnemonic
        jm = jcc_ins.mnemonic
        if cm not in ("cmp", "test") or len(cmp_ins.operands) != 2:
            return None
        if jm not in (
            "je", "jz", "jne", "jnz", "jb", "jnae", "jc", "jae", "jnb", "jnc",
            "ja", "jnbe", "jbe", "jna", "jl", "jnge", "jge", "jnl",
            "jle", "jng", "jg", "jnle", "js", "jns",
        ):
            return None

        bits = cmp_ins.operands[0].size * 8
        ut, st = self._u_type(bits), self._s_type(bits)
        a = self._read_op(cmp_ins, cmp_ins.operands[0], bits, image, index, shard_const_pool)
        b = self._read_op(cmp_ins, cmp_ins.operands[1], bits, image, index, shard_const_pool)

        cond: str | None = None
        if cm == "test":
            same_op = (
                cmp_ins.operands[0].kind == "reg"
                and cmp_ins.operands[1].kind == "reg"
                and cmp_ins.operands[0].reg == cmp_ins.operands[1].reg
            )
            lhs = a if same_op else f"(({ut})({a} & {b}))"
            if jm in ("je", "jz"):
                cond = f"{lhs} == 0"
            elif jm in ("jne", "jnz"):
                cond = f"{lhs} != 0"
            elif jm == "js":
                cond = f"({st}){lhs} < 0"
            elif jm == "jns":
                cond = f"({st}){lhs} >= 0"
            elif jm in ("jg", "jnle"):
                cond = f"({st}){lhs} > 0"
            elif jm in ("jle", "jng"):
                cond = f"({st}){lhs} <= 0"
        else:  # cmp
            if jm in ("je", "jz"):
                cond = f"{a} == {b}"
            elif jm in ("jne", "jnz"):
                cond = f"{a} != {b}"
            elif jm in ("jb", "jnae", "jc"):
                cond = f"{a} < {b}"
            elif jm in ("jae", "jnb", "nc", "jnc"):
                cond = f"{a} >= {b}"
            elif jm in ("ja", "jnbe"):
                cond = f"{a} > {b}"
            elif jm in ("jbe", "jna"):
                cond = f"{a} <= {b}"
            elif jm in ("jl", "jnge"):
                cond = f"({st}){a} < ({st}){b}"
            elif jm in ("jge", "jnl"):
                cond = f"({st}){a} >= ({st}){b}"
            elif jm in ("jg", "jnle"):
                cond = f"({st}){a} > ({st}){b}"
            elif jm in ("jle", "jng"):
                cond = f"({st}){a} <= ({st}){b}"

        if cond is None:
            return None

        btgt = getattr(blk, "branch_target", None)
        ctail = getattr(blk, "cond_tailcall_target", None)
        ft = getattr(blk, "fallthrough", None)
        out: list[str] = []
        if btgt is not None and btgt in cfg.blocks:
            out.append(f"if ({cond}) goto L_{btgt:x};")
        elif ctail is not None:
            tc_stmts = " ".join(self._lift_tailcall_insn(jcc_ins, ctail, image, index, shard_const_pool))
            out.append(f"if ({cond}) {{ {tc_stmts} }}")
        if ft is not None and ft in cfg.blocks and ft != next_blk_addr:
            out.append(f"goto L_{ft:x};")
        return out

    def _compute_live_flags(
        self, cfg: FunctionCFG
    ) -> tuple[dict[int, bool], dict[int, bool], bool]:
        """Exact backward dataflow liveness of x86-64 EFLAGS across CFG blocks."""
        uses_pf = False
        for blk in cfg.blocks.values():
            for ins in blk.instructions:
                if ins.mnemonic in ("jp", "jpe", "jnp", "jpo", "setp", "setpe", "setnp", "setpo", "cmovp", "cmovpe", "cmovnp", "cmovpo"):
                    uses_pf = True

        live_in: dict[int, bool] = {bva: False for bva in cfg.blocks}
        live_out: dict[int, bool] = {bva: False for bva in cfg.blocks}

        changed = True
        iters = 0
        while changed and iters < 32:
            changed = False
            iters += 1
            for bva, blk in cfg.blocks.items():
                out_f = any(live_in.get(s, False) for s in blk.successors)
                live_out[bva] = out_f
                cur_f = out_f
                for ins in reversed(blk.instructions):
                    mn = ins.mnemonic
                    if mn in RET_MNEMONICS or mn == "int3":
                        cur_f = False
                    elif mn == "call":
                        cur_f = False
                    elif mn in _FULL_FLAG_KILLER_MNEMONICS and mn not in _FLAG_READER_MNEMONICS:
                        cur_f = False
                    if mn in _FLAG_READER_MNEMONICS:
                        cur_f = True
                if cur_f != live_in[bva]:
                    live_in[bva] = cur_f
                    changed = True

        need_at_insn: dict[int, bool] = {}
        for bva, blk in cfg.blocks.items():
            cur_f = live_out[bva]
            for ins in reversed(blk.instructions):
                mn = ins.mnemonic
                need_at_insn[ins.address] = cur_f or (mn in ("lock cmpxchg", "cmpxchg"))
                if mn in RET_MNEMONICS or mn == "int3" or mn == "call":
                    cur_f = False
                elif mn in _FULL_FLAG_KILLER_MNEMONICS and mn not in _FLAG_READER_MNEMONICS:
                    cur_f = False
                if mn in _FLAG_READER_MNEMONICS:
                    cur_f = True
        return need_at_insn, live_out, uses_pf

    def _scan_used_regs_and_calls(
        self, cfg: FunctionCFG, index: WholeKernelSymbolIndex
    ) -> tuple[set[str], set[str], int, bool]:
        used_gprs: set[str] = set()
        used_xmms: set[str] = set()
        max_call_extra = 0
        has_any_call = False

        for blk in cfg.blocks.values():
            cur_out_extra = 0
            if blk.tailcall_target is not None or blk.cond_tailcall_target is not None:
                has_any_call = True
            for ins in blk.instructions:
                if ins.mnemonic in ("push", "pop"):
                    used_gprs.add("rsp")
                for r in ins.regs_read:
                    if r in CANONICAL_GPR64:
                        used_gprs.add(CANONICAL_GPR64[r])
                    elif r in _XMM_REGS:
                        used_xmms.add(r)
                for r in ins.regs_write:
                    if r in CANONICAL_GPR64:
                        used_gprs.add(CANONICAL_GPR64[r])
                    elif r in _XMM_REGS:
                        used_xmms.add(r)
                for op in ins.operands:
                    if op.kind == "reg" and op.reg:
                        if op.reg in CANONICAL_GPR64:
                            used_gprs.add(CANONICAL_GPR64[op.reg])
                        elif op.reg in _XMM_REGS:
                            used_xmms.add(op.reg)
                    elif op.kind == "mem":
                        for mr in (op.base, op.index):
                            if mr and mr in CANONICAL_GPR64:
                                used_gprs.add(CANONICAL_GPR64[mr])
                        if op.base == "rsp" and op.index is None and 0x20 <= op.disp <= 0xB8:
                            top_off = op.disp + (8 if op.size == 16 else 0)
                            cur_out_extra = max(cur_out_extra, ((top_off - 0x20) // 8) + 1)
                if ins.mnemonic in ("mul", "imul", "div", "idiv", "cqo", "cdq", "rdtsc"):
                    used_gprs.add("rdx")
                if ins.mnemonic in ("call", "jmp") and len(ins.operands) == 1:
                    if ins.mnemonic == "call":
                        has_any_call = True
                    op0 = ins.operands[0]
                    if op0.kind == "imm":
                        callee_m = index.functions_by_va.get(op0.imm)
                        if callee_m is not None:
                            max_call_extra = max(max_call_extra, max(0, callee_m.arity - 4))
                    max_call_extra = max(max_call_extra, cur_out_extra)
                    if ins.mnemonic == "call":
                        cur_out_extra = 0
        return used_gprs, used_xmms, max_call_extra, has_any_call

    # ------------------------------------------------------------------
    # Operand Read / Write Helpers
    # ------------------------------------------------------------------
    def _u_type(self, bits: int) -> str:
        return {8: "uint8_t", 16: "uint16_t", 32: "uint32_t", 64: "uint64_t"}[bits]

    def _s_type(self, bits: int) -> str:
        return {8: "int8_t", 16: "int16_t", 32: "int32_t", 64: "int64_t"}[bits]

    def _mem_addr_expr(
        self,
        ins: DisasmInsn,
        op: InsnOperand,
        image: BinaryImage,
        index: WholeKernelSymbolIndex,
        shard_const_pool: dict[int, bytes],
        is_lea: bool = False,
    ) -> str:
        if op.base == "rip":
            target_va = (ins.address + ins.size + op.disp) & 0xFFFFFFFFFFFFFFFF
            return self._resolve_va_c_expr(target_va, image, index, shard_const_pool, is_lea=is_lea)

        parts: list[str] = []
        if op.base:
            parts.append(self._read_gpr64(op.base))
        if op.index:
            idx_e = self._read_gpr64(op.index)
            if op.scale > 1:
                parts.append(f"({idx_e} * {op.scale}ULL)")
            else:
                parts.append(idx_e)
        if op.disp != 0 or not parts:
            if op.disp >= 0:
                parts.append(f"0x{op.disp:x}ULL")
            else:
                parts.append(f"- 0x{(-op.disp):x}ULL")

        expr = parts[0]
        for p in parts[1:]:
            if p.startswith("- "):
                expr = f"({expr} {p})"
            else:
                expr = f"({expr} + {p})"
        return expr

    def _resolve_va_c_expr(
        self,
        target_va: int,
        image: BinaryImage,
        index: WholeKernelSymbolIndex,
        shard_const_pool: dict[int, bytes],
        is_lea: bool = False,
    ) -> str:
        base = image.image_base
        if target_va == base:
            return "(uint64_t)(uintptr_t)__ImageBase"
        if target_va in index.imports_by_va:
            c_imp, _ = index.imports_by_va[target_va]
            return f"(uint64_t)(uintptr_t)&{c_imp}"

        rva = target_va - base
        if rva in index.functions_by_rva:
            return f"(uint64_t)(uintptr_t)&{index.functions_by_rva[rva].c_name}"
        if rva in index.data_syms_by_rva:
            return f"(uint64_t)(uintptr_t)&{index.data_syms_by_rva[rva]}"

        sec = image.section_for_va(target_va)
        if sec is not None:
            if not sec.is_executable and sec.name in index.emitted_data_sections:
                sec_off = rva - sec.virtual_address
                sname = sec_ident_name(sec.name)
                return f"(uint64_t)(uintptr_t)(&{sname}[0x{sec_off:x}])"
            if sec.is_executable:
                # Constant table or jump data embedded inside an executable section
                if rva not in shard_const_pool:
                    raw = image.read_va(target_va, 64) or (b"\x00" * 16)
                    shard_const_pool[rva] = raw
                return f"(uint64_t)(uintptr_t)__nt_kconst_{rva:x}"

        return f"0x{target_va:x}ULL"

    def _read_gpr64(self, reg: str) -> str:
        if reg in CANONICAL_GPR64 and CANONICAL_GPR64[reg] == reg:
            return reg
        canon = CANONICAL_GPR64.get(reg)
        if canon:
            if reg in _HIGH8_GPRS:
                return f"((uint64_t)({canon} >> 8) & 0xFFULL)"
            if reg.endswith("d") or reg in ("eax", "ebx", "ecx", "edx", "esi", "edi", "ebp", "esp"):
                return f"((uint64_t)(uint32_t){canon})"
            if reg.endswith("w") or reg in ("ax", "bx", "cx", "dx", "si", "di", "bp", "sp"):
                return f"((uint64_t)(uint16_t){canon})"
            return f"((uint64_t)(uint8_t){canon})"
        return "0ULL"

    def _read_op(
        self,
        ins: DisasmInsn,
        op: InsnOperand,
        bits: int,
        image: BinaryImage,
        index: WholeKernelSymbolIndex,
        shard_const_pool: dict[int, bytes],
    ) -> str:
        ut = self._u_type(bits)
        mask = (1 << bits) - 1
        if op.kind == "imm":
            val = op.imm & mask
            if bits == 64:
                return f"0x{val:x}ULL"
            return f"(({ut})0x{val:x}U)"
        if op.kind == "reg":
            r = op.reg or "rax"
            if r in ("cr2", "cr3", "cr8"):
                return f"(({ut})__nt_read{r}())"
            canon = CANONICAL_GPR64.get(r, r)
            if r in _HIGH8_GPRS:
                return f"(({ut})(({canon} >> 8) & 0xFFU))"
            if bits == 64:
                return canon
            return f"(({ut}){canon})"
        if op.kind == "mem":
            if op.base == "rip":
                tva = (ins.address + ins.size + op.disp) & 0xFFFFFFFFFFFFFFFF
                sec = image.section_for_va(tva)
                if (
                    sec is not None
                    and sec.is_executable
                    and (tva - image.image_base) not in index.functions_by_rva
                    and bits in (8, 16, 32, 64)
                ):
                    cval = image.read_readonly_scalar(tva, bits // 8)
                    if cval is not None:
                        return f"0x{cval:x}ULL" if bits == 64 else f"(({ut})0x{cval:x}U)"
            addr = self._mem_addr_expr(ins, op, image, index, shard_const_pool)
            if op.segment == "gs":
                return f"(*NT_GS_PTR({ut}, {addr}))"
            return f"M{bits}({addr})"
        return f"(({ut})0)"

    def _write_op(
        self,
        ins: DisasmInsn,
        op: InsnOperand,
        bits: int,
        val_expr: str,
        image: BinaryImage,
        index: WholeKernelSymbolIndex,
        shard_const_pool: dict[int, bytes],
    ) -> str:
        ut = self._u_type(bits)
        if op.kind == "reg":
            r = op.reg or "rax"
            if r == "cr8":
                return f"__nt_writecr8((uint64_t)({val_expr}));"
            if r in ("cr2", "cr3"):
                return f"(void)({val_expr});"
            canon = CANONICAL_GPR64.get(r, r)
            if bits == 64:
                return f"{canon} = (uint64_t)({val_expr});"
            if bits == 32:
                return f"{canon} = (uint64_t)(uint32_t)({val_expr});"
            if bits == 16:
                return f"{canon} = ({canon} & ~0xFFFFULL) | (uint16_t)({val_expr});"
            if r in _HIGH8_GPRS:
                return f"{canon} = ({canon} & ~0xFF00ULL) | (((uint64_t)(uint8_t)({val_expr})) << 8);"
            return f"{canon} = ({canon} & ~0xFFULL) | (uint8_t)({val_expr});"

        addr = self._mem_addr_expr(ins, op, image, index, shard_const_pool)
        if op.segment == "gs":
            return f"*NT_GS_PTR({ut}, {addr}) = ({ut})({val_expr});"
        return f"M{bits}({addr}) = ({ut})({val_expr});"

    def _mem_ptr_expr(
        self,
        ins: DisasmInsn,
        op: InsnOperand,
        bits: int,
        image: BinaryImage,
        index: WholeKernelSymbolIndex,
        shard_const_pool: dict[int, bytes],
    ) -> str:
        ut = self._u_type(bits)
        addr = self._mem_addr_expr(ins, op, image, index, shard_const_pool)
        if op.segment == "gs":
            return f"NT_GS_PTR({ut}, {addr})"
        return f"(({ut} *)(uintptr_t)({addr}))"

    # ------------------------------------------------------------------
    # Condition Code Expression Builder (`jcc`, `setcc`, `cmovcc`)
    # ------------------------------------------------------------------
    def _cond_expr(self, cc: str) -> str:
        c = cc.lower()
        if c in ("e", "z"):
            return "zf"
        if c in ("ne", "nz"):
            return "(!zf)"
        if c in ("b", "nae", "c"):
            return "cf"
        if c in ("ae", "nb", "nc"):
            return "(!cf)"
        if c in ("be", "na"):
            return "(cf || zf)"
        if c in ("a", "nbe"):
            return "(!cf && !zf)"
        if c == "s":
            return "sf"
        if c == "ns":
            return "(!sf)"
        if c == "o":
            return "of"
        if c == "no":
            return "(!of)"
        if c in ("p", "pe"):
            return "pf"
        if c in ("np", "po"):
            return "(!pf)"
        if c in ("l", "nge"):
            return "(sf != of)"
        if c in ("ge", "nl"):
            return "(sf == of)"
        if c in ("le", "ng"):
            return "(zf || (sf != of))"
        if c in ("g", "nle"):
            return "(!zf && (sf == of))"
        return "0"

    def _emit_logic_flags(self, res_var: str, bits: int, uses_pf: bool) -> list[str]:
        msb = bits - 1
        out = [
            f"zf = ({res_var} == 0); sf = (uint8_t)(({res_var} >> {msb}) & 1U); cf = 0; of = 0;"
        ]
        if uses_pf:
            out.append(f"pf = (uint8_t)(!__builtin_parity((uint8_t){res_var}));")
        return out

    # ------------------------------------------------------------------
    # Per-Instruction Translation (100% of Pure-C NT Kernel Mnemonics)
    # ------------------------------------------------------------------
    def _lift_insn(
        self,
        ins: DisasmInsn,
        blk: object,
        cfg: FunctionCFG,
        image: BinaryImage,
        index: WholeKernelSymbolIndex,
        shard_const_pool: dict[int, bytes],
        need_flags: bool,
        uses_pf: bool,
        is_last: bool,
        next_blk_addr: int | None,
    ) -> list[str]:
        mn = ins.mnemonic
        ops = ins.operands

        # 1. No-ops, fences, cache/prefetch hints, basic CPU intrinsics
        if mn.startswith(("nop", "prefetch")):
            return []
        if mn == "pause":
            return ["__builtin_ia32_pause();"]
        if mn in ("lfence", "sfence", "mfence"):
            return ['__asm__ __volatile__ ("" ::: "memory");']
        if mn in ("clflush", "clflushopt"):
            addr = self._mem_addr_expr(ins, ops[0], image, index, shard_const_pool)
            return [f"__nt_clflush((const void *)(uintptr_t)({addr}));"]
        if mn == "cli":
            return ["__nt_cli();"]
        if mn == "sti":
            return ["__nt_sti();"]
        if mn == "rdtsc":
            return [
                "{ uint64_t _tsc = __nt_rdtsc(); rax = (uint64_t)(uint32_t)_tsc; rdx = _tsc >> 32; }"
            ]
        if mn == "int3":
            return ["__nt_int3(); return rax;"]
        if mn == "int":
            ivec = ops[0].imm & 0xFF if ops else 0x29
            if ivec == 0x29:
                return ["__nt_int29(rcx); return rax;"]
            return [f"__nt_int_n(0x{ivec:x}U);"]

        # 2. Returns
        if mn in RET_MNEMONICS:
            if ops and ops[0].kind == "imm" and ops[0].imm > 0:
                return [f"rsp += 0x{ops[0].imm:x}ULL; return rax;"]
            return ["return rax;"]

        # 3. Stack push / pop
        if mn == "push":
            op0 = ops[0]
            if op0.kind == "imm":
                v = op0.imm & 0xFFFFFFFFFFFFFFFF
                return [f"rsp -= 8; M64(rsp) = 0x{v:x}ULL;"]
            bits = op0.size * 8 if op0.size in (2, 8) else 64
            ve = self._read_op(ins, op0, bits, image, index, shard_const_pool)
            if bits == 16:
                return [f"rsp -= 2; M16(rsp) = {ve};"]
            return [f"rsp -= 8; M64(rsp) = (uint64_t){ve};"]

        if mn == "pop":
            op0 = ops[0]
            bits = op0.size * 8 if op0.size in (2, 8) else 64
            if bits == 16:
                w = self._write_op(ins, op0, 16, "M16(rsp)", image, index, shard_const_pool)
                return [w, "rsp += 2;"]
            w = self._write_op(ins, op0, 64, "M64(rsp)", image, index, shard_const_pool)
            return [w, "rsp += 8;"]

        # 4. Data movement & sign/zero extension
        if mn in ("mov", "movabs", "movnti") and len(ops) == 2:
            # Handle XMM <-> GPR if Capstone ever emits `mov`
            bits = ops[0].size * 8
            ve = self._read_op(ins, ops[1], bits, image, index, shard_const_pool)
            return [self._write_op(ins, ops[0], bits, ve, image, index, shard_const_pool)]

        if mn == "lea" and len(ops) == 2:
            dbits = ops[0].size * 8
            ae = self._mem_addr_expr(ins, ops[1], image, index, shard_const_pool, is_lea=True)
            return [self._write_op(ins, ops[0], dbits, ae, image, index, shard_const_pool)]

        if mn == "movzx" and len(ops) == 2:
            dbits = ops[0].size * 8
            sbits = ops[1].size * 8
            se = self._read_op(ins, ops[1], sbits, image, index, shard_const_pool)
            return [self._write_op(ins, ops[0], dbits, f"({self._u_type(dbits)}){se}", image, index, shard_const_pool)]

        if mn in ("movsx", "movsxd") and len(ops) == 2:
            dbits = ops[0].size * 8
            sbits = ops[1].size * 8
            se = self._read_op(ins, ops[1], sbits, image, index, shard_const_pool)
            sexpr = f"({self._u_type(dbits)})({self._s_type(dbits)})({self._s_type(sbits)}){se}"
            return [self._write_op(ins, ops[0], dbits, sexpr, image, index, shard_const_pool)]

        if mn == "cdqe":
            return ["rax = (uint64_t)(int64_t)(int32_t)rax;"]
        if mn == "cwde":
            return ["rax = (uint64_t)(uint32_t)(int32_t)(int16_t)rax;"]
        if mn == "cqo":
            return ["rdx = ((int64_t)rax < 0) ? 0xFFFFFFFFFFFFFFFFULL : 0ULL;"]
        if mn == "cdq":
            return ["rdx = ((int32_t)rax < 0) ? 0xFFFFFFFFULL : 0ULL;"]
        if mn == "bswap" and len(ops) == 1:
            bits = ops[0].size * 8
            se = self._read_op(ins, ops[0], bits, image, index, shard_const_pool)
            bsfn = {16: "__builtin_bswap16", 32: "__builtin_bswap32", 64: "__builtin_bswap64"}[bits]
            return [self._write_op(ins, ops[0], bits, f"{bsfn}({se})", image, index, shard_const_pool)]

        # 5. XMM / SSE2 / Scalar Floating-Point Operations
        if any((op.reg or "") in _XMM_REGS for op in ops) or mn in (
            "movups", "movaps", "movapd", "movdqu", "movdqa", "movntdq",
            "movsd", "movss", "movq", "movd", "movhpd", "movlhps",
            "xorps", "xorpd", "pxor", "pand", "por", "psrldq", "psllq",
            "paddd", "paddq", "punpckldq", "punpcklwd", "punpcklqdq",
            "pshufd", "pextrw", "cvtsi2sd", "cvtsi2ss", "cvtps2pd",
            "cvtdq2pd", "cvtdq2ps", "cvtsd2ss", "addsd", "addss",
            "comisd", "ucomisd", "comiss", "ucomiss",
        ):
            return self._lift_xmm_insn(ins, image, index, shard_const_pool)

        # 6. Comparisons & Tests (`cmp`, `test`)
        if mn == "cmp" and len(ops) == 2:
            if not need_flags:
                return []
            bits = ops[0].size * 8
            ut, st = self._u_type(bits), self._s_type(bits)
            msb = bits - 1
            a = self._read_op(ins, ops[0], bits, image, index, shard_const_pool)
            b = self._read_op(ins, ops[1], bits, image, index, shard_const_pool)
            pf_s = " pf = (uint8_t)(!__builtin_parity((uint8_t)_r));" if uses_pf else ""
            return [
                f"{{ {ut} _a = {a}, _b = {b}, _r = ({ut})(_a - _b); "
                f"zf = (_r == 0); sf = (uint8_t)((_r >> {msb}) & 1U); cf = (_a < _b); "
                f"of = (uint8_t)((((_a ^ _b) & (_a ^ _r)) >> {msb}) & 1U);{pf_s} }}"
            ]

        if mn == "test" and len(ops) == 2:
            if not need_flags:
                return []
            bits = ops[0].size * 8
            ut = self._u_type(bits)
            a = self._read_op(ins, ops[0], bits, image, index, shard_const_pool)
            b = self._read_op(ins, ops[1], bits, image, index, shard_const_pool)
            fl = " ".join(self._emit_logic_flags("_r", bits, uses_pf))
            return [f"{{ {ut} _r = ({ut})({a} & {b}); {fl} }}"]

        # 7. Integer Arithmetic & Bitwise (`add`, `sub`, `sbb`, `adc`, `and`, `or`, `xor`, `inc`, `dec`, `neg`, `not`)
        if mn in ("add", "sub", "sbb", "adc", "and", "or", "xor") and len(ops) == 2:
            # Fast idiom: `xor reg, reg` / `sub reg, reg`
            if (
                mn in ("xor", "sub")
                and ops[0].kind == "reg"
                and ops[1].kind == "reg"
                and ops[0].reg == ops[1].reg
            ):
                w = self._write_op(ins, ops[0], ops[0].size * 8, "0U", image, index, shard_const_pool)
                if not need_flags:
                    return [w]
                pf_s = " pf = 1;" if uses_pf else ""
                return [f"{w} zf = 1; sf = 0; cf = 0; of = 0;{pf_s}"]

            bits = ops[0].size * 8
            ut = self._u_type(bits)
            msb = bits - 1
            a = self._read_op(ins, ops[0], bits, image, index, shard_const_pool)
            b = self._read_op(ins, ops[1], bits, image, index, shard_const_pool)

            if not need_flags and mn in ("add", "sub", "and", "or", "xor"):
                op_c = {"add": "+", "sub": "-", "and": "&", "or": "|", "xor": "^"}[mn]
                w = self._write_op(ins, ops[0], bits, f"({ut})({a} {op_c} {b})", image, index, shard_const_pool)
                return [w]

            pf_s = " pf = (uint8_t)(!__builtin_parity((uint8_t)_r));" if uses_pf else ""
            if mn == "add":
                w = self._write_op(ins, ops[0], bits, "_r", image, index, shard_const_pool)
                return [
                    f"{{ {ut} _a = {a}, _b = {b}, _r = ({ut})(_a + _b); {w} "
                    f"zf = (_r == 0); sf = (uint8_t)((_r >> {msb}) & 1U); cf = (_r < _a); "
                    f"of = (uint8_t)((~(_a ^ _b) & (_a ^ _r)) >> {msb}) & 1U;{pf_s} }}"
                ]
            if mn == "sub":
                w = self._write_op(ins, ops[0], bits, "_r", image, index, shard_const_pool)
                return [
                    f"{{ {ut} _a = {a}, _b = {b}, _r = ({ut})(_a - _b); {w} "
                    f"zf = (_r == 0); sf = (uint8_t)((_r >> {msb}) & 1U); cf = (_a < _b); "
                    f"of = (uint8_t)(((_a ^ _b) & (_a ^ _r)) >> {msb}) & 1U;{pf_s} }}"
                ]
            if mn == "sbb":
                w = self._write_op(ins, ops[0], bits, "_r", image, index, shard_const_pool)
                return [
                    f"{{ {ut} _a = {a}, _b = {b}, _c = ({ut})cf, _r = ({ut})(_a - _b - _c); {w} "
                    f"cf = (_a < _b) || (_c && (_a - _b == 0)); zf = (_r == 0); sf = (uint8_t)((_r >> {msb}) & 1U); "
                    f"of = (uint8_t)(((_a ^ _b) & (_a ^ _r)) >> {msb}) & 1U;{pf_s} }}"
                ]
            if mn == "adc":
                w = self._write_op(ins, ops[0], bits, "_r", image, index, shard_const_pool)
                return [
                    f"{{ {ut} _a = {a}, _b = {b}, _c = ({ut})cf, _r = ({ut})(_a + _b + _c); {w} "
                    f"cf = (_r < _a) || (_c && (_r == _a)); zf = (_r == 0); sf = (uint8_t)((_r >> {msb}) & 1U); "
                    f"of = (uint8_t)((~(_a ^ _b) & (_a ^ _r)) >> {msb}) & 1U;{pf_s} }}"
                ]
            op_c = {"and": "&", "or": "|", "xor": "^"}[mn]
            w = self._write_op(ins, ops[0], bits, "_r", image, index, shard_const_pool)
            fl = " ".join(self._emit_logic_flags("_r", bits, uses_pf))
            return [f"{{ {ut} _r = ({ut})({a} {op_c} {b}); {w} {fl} }}"]

        if mn in ("inc", "dec") and len(ops) == 1:
            bits = ops[0].size * 8
            ut = self._u_type(bits)
            msb = bits - 1
            a = self._read_op(ins, ops[0], bits, image, index, shard_const_pool)
            op_c = "+" if mn == "inc" else "-"
            if not need_flags:
                return [self._write_op(ins, ops[0], bits, f"({ut})({a} {op_c} 1U)", image, index, shard_const_pool)]
            w = self._write_op(ins, ops[0], bits, "_r", image, index, shard_const_pool)
            ov_chk = f"(_a == (({ut})1ULL << {msb}) - 1U)" if mn == "inc" else f"(_a == (({ut})1ULL << {msb}))"
            pf_s = " pf = (uint8_t)(!__builtin_parity((uint8_t)_r));" if uses_pf else ""
            # Note: x86 `inc`/`dec` preserve CF!
            return [
                f"{{ {ut} _a = {a}, _r = ({ut})(_a {op_c} 1U); {w} "
                f"zf = (_r == 0); sf = (uint8_t)((_r >> {msb}) & 1U); of = (uint8_t){ov_chk};{pf_s} }}"
            ]

        if mn == "neg" and len(ops) == 1:
            bits = ops[0].size * 8
            ut = self._u_type(bits)
            msb = bits - 1
            a = self._read_op(ins, ops[0], bits, image, index, shard_const_pool)
            if not need_flags:
                return [self._write_op(ins, ops[0], bits, f"({ut})(0U - {a})", image, index, shard_const_pool)]
            w = self._write_op(ins, ops[0], bits, "_r", image, index, shard_const_pool)
            pf_s = " pf = (uint8_t)(!__builtin_parity((uint8_t)_r));" if uses_pf else ""
            return [
                f"{{ {ut} _a = {a}, _r = ({ut})(0U - _a); {w} "
                f"cf = (_a != 0); zf = (_r == 0); sf = (uint8_t)((_r >> {msb}) & 1U); "
                f"of = (_a == (({ut})1ULL << {msb}));{pf_s} }}"
            ]

        if mn == "not" and len(ops) == 1:
            bits = ops[0].size * 8
            ut = self._u_type(bits)
            a = self._read_op(ins, ops[0], bits, image, index, shard_const_pool)
            return [self._write_op(ins, ops[0], bits, f"({ut})(~{a})", image, index, shard_const_pool)]

        # 8. Multiply & Divide (`mul`, `imul`, `div`, `idiv`)
        if mn == "mul" and len(ops) == 1:
            bits = ops[0].size * 8
            src = self._read_op(ins, ops[0], bits, image, index, shard_const_pool)
            if bits == 64:
                return [
                    f"{{ __uint128_t _p = (__uint128_t)rax * (__uint128_t){src}; "
                    f"rax = (uint64_t)_p; rdx = (uint64_t)(_p >> 64); cf = of = (rdx != 0); }}"
                ]
            if bits == 32:
                return [
                    f"{{ uint64_t _p = (uint64_t)(uint32_t)rax * (uint64_t){src}; "
                    f"rax = (uint64_t)(uint32_t)_p; rdx = (uint64_t)(uint32_t)(_p >> 32); cf = of = (rdx != 0); }}"
                ]
            if bits == 16:
                return [
                    f"{{ uint32_t _p = (uint32_t)(uint16_t)rax * (uint32_t){src}; "
                    f"rax = (rax & ~0xFFFFULL) | (_p & 0xFFFFU); rdx = (rdx & ~0xFFFFULL) | ((_p >> 16) & 0xFFFFU); cf = of = ((_p >> 16) != 0); }}"
                ]
            return [
                f"{{ uint16_t _p = (uint16_t)(uint8_t)rax * (uint16_t){src}; "
                f"rax = (rax & ~0xFFFFULL) | _p; cf = of = ((_p >> 8) != 0); }}"
            ]

        if mn == "imul":
            if len(ops) == 1:
                bits = ops[0].size * 8
                src = self._read_op(ins, ops[0], bits, image, index, shard_const_pool)
                if bits == 64:
                    return [
                        f"{{ __int128 _p = (__int128)(int64_t)rax * (__int128)(int64_t){src}; "
                        f"rax = (uint64_t)_p; rdx = (uint64_t)((__uint128_t)_p >> 64); "
                        f"cf = of = (_p != (__int128)(int64_t)rax); }}"
                    ]
                if bits == 32:
                    return [
                        f"{{ int64_t _p = (int64_t)(int32_t)rax * (int64_t)(int32_t){src}; "
                        f"rax = (uint64_t)(uint32_t)_p; rdx = (uint64_t)(uint32_t)((uint64_t)_p >> 32); "
                        f"cf = of = (_p != (int64_t)(int32_t)rax); }}"
                    ]
                return [
                    f"{{ int32_t _p = (int32_t)(int16_t)rax * (int32_t)(int16_t){src}; "
                    f"rax = (rax & ~0xFFFFULL) | ((uint32_t)_p & 0xFFFFU); rdx = (rdx & ~0xFFFFULL) | (((uint32_t)_p >> 16) & 0xFFFFU); "
                    f"cf = of = (_p != (int32_t)(int16_t)_p); }}"
                ]
            bits = ops[0].size * 8
            ut, st = self._u_type(bits), self._s_type(bits)
            s1 = self._read_op(ins, ops[1], bits, image, index, shard_const_pool)
            s2 = (
                self._read_op(ins, ops[2], bits, image, index, shard_const_pool)
                if len(ops) == 3
                else self._read_op(ins, ops[0], bits, image, index, shard_const_pool)
            )
            w = self._write_op(ins, ops[0], bits, "(_ut_res)", image, index, shard_const_pool).replace("(_ut_res)", "_r")
            if not need_flags:
                return [f"{{ {ut} _r = ({ut})((__int128)({st}){s1} * (__int128)({st}){s2}); {w} }}"]
            return [
                f"{{ __int128 _p = (__int128)({st}){s1} * (__int128)({st}){s2}; "
                f"{ut} _r = ({ut})_p; {w} cf = of = (_p != (__int128)({st})_r); }}"
            ]

        if mn == "div" and len(ops) == 1:
            bits = ops[0].size * 8
            src = self._read_op(ins, ops[0], bits, image, index, shard_const_pool)
            if bits == 64:
                return [
                    f"{{ __uint128_t _n = ((__uint128_t)rdx << 64) | rax; uint64_t _d = {src}; "
                    f"rax = (uint64_t)(_n / _d); rdx = (uint64_t)(_n % _d); }}"
                ]
            if bits == 32:
                return [
                    f"{{ uint64_t _n = ((uint64_t)(uint32_t)rdx << 32) | (uint32_t)rax; uint32_t _d = {src}; "
                    f"rax = (uint64_t)(uint32_t)(_n / _d); rdx = (uint64_t)(uint32_t)(_n % _d); }}"
                ]
            if bits == 16:
                return [
                    f"{{ uint32_t _n = (((uint32_t)(uint16_t)rdx) << 16) | (uint16_t)rax; uint16_t _d = {src}; "
                    f"rax = (rax & ~0xFFFFULL) | ((_n / _d) & 0xFFFFU); rdx = (rdx & ~0xFFFFULL) | ((_n % _d) & 0xFFFFU); }}"
                ]
            return [
                f"{{ uint16_t _n = (uint16_t)rax; uint8_t _d = {src}; "
                f"rax = (rax & ~0xFFFFULL) | (((uint16_t)(_n % _d) << 8) | (uint8_t)(_n / _d)); }}"
            ]

        if mn == "idiv" and len(ops) == 1:
            bits = ops[0].size * 8
            src = self._read_op(ins, ops[0], bits, image, index, shard_const_pool)
            if bits == 64:
                return [
                    f"{{ __int128 _n = (__int128)(((__uint128_t)rdx << 64) | rax); int64_t _d = (int64_t){src}; "
                    f"rax = (uint64_t)(int64_t)(_n / _d); rdx = (uint64_t)(int64_t)(_n % _d); }}"
                ]
            if bits == 32:
                return [
                    f"{{ int64_t _n = (int64_t)(((uint64_t)(uint32_t)rdx << 32) | (uint32_t)rax); int32_t _d = (int32_t){src}; "
                    f"rax = (uint64_t)(uint32_t)(int32_t)(_n / _d); rdx = (uint64_t)(uint32_t)(int32_t)(_n % _d); }}"
                ]
            return [
                f"{{ int32_t _n = (int32_t)((((uint32_t)(uint16_t)rdx) << 16) | (uint16_t)rax); int16_t _d = (int16_t){src}; "
                f"rax = (rax & ~0xFFFFULL) | ((uint16_t)(_n / _d)); rdx = (rdx & ~0xFFFFULL) | ((uint16_t)(_n % _d)); }}"
            ]

        # 9. Shifts & Rotates (`shl`, `shr`, `sar`, `rol`, `ror`, `shrd`, `shld`)
        if mn in ("shl", "sal", "shr", "sar", "rol", "ror") and len(ops) >= 1:
            bits = ops[0].size * 8
            ut, st = self._u_type(bits), self._s_type(bits)
            msb = bits - 1
            mask_cnt = 63 if bits == 64 else 31
            a = self._read_op(ins, ops[0], bits, image, index, shard_const_pool)
            cnt_e = (
                self._read_op(ins, ops[1], 8, image, index, shard_const_pool)
                if len(ops) >= 2
                else "((uint8_t)1U)"
            )
            w = self._write_op(ins, ops[0], bits, "_r", image, index, shard_const_pool)

            if mn in ("rol", "ror"):
                eff_mod = bits
                if mn == "rol":
                    rot_e = f"(_s == 0) ? _a : ({ut})((_a << _s) | (_a >> ({bits} - _s)))"
                    cf_e = "(_r & 1U)"
                    of_e = f"(uint8_t)(((_r >> {msb}) ^ cf) & 1U)"
                else:
                    rot_e = f"(_s == 0) ? _a : ({ut})((_a >> _s) | (_a << ({bits} - _s)))"
                    cf_e = f"((_r >> {msb}) & 1U)"
                    of_e = f"(uint8_t)(((_r >> {msb}) ^ (_r >> {msb - 1})) & 1U)"
                fl_s = f" if (_c != 0) {{ cf = (uint8_t){cf_e}; of = {of_e}; }}" if need_flags else ""
                return [
                    f"{{ {ut} _a = {a}; uint8_t _c = ({cnt_e}) & {mask_cnt}U, _s = _c % {eff_mod}U; "
                    f"{ut} _r = {rot_e}; {w}{fl_s} }}"
                ]

            if mn in ("shl", "sal"):
                sh_e = "(_c == 0) ? _a : ({ut})(_a << _c)".format(ut=ut)
                cf_e = f"((_a >> ({bits} - _c)) & 1U)"
                of_e = f"(uint8_t)(((_r >> {msb}) ^ cf) & 1U)"
            elif mn == "shr":
                sh_e = "(_c == 0) ? _a : ({ut})(_a >> _c)".format(ut=ut)
                cf_e = "((_a >> (_c - 1U)) & 1U)"
                of_e = f"(uint8_t)((_a >> {msb}) & 1U)"
            else:  # sar
                sh_e = f"(_c == 0) ? _a : ({ut})((({st})_a) >> _c)"
                cf_e = f"((((uint64_t)(({st})_a >> (_c - 1U))) & 1U))"
                of_e = "0"

            if not need_flags:
                return [
                    f"{{ {ut} _a = {a}; uint8_t _c = ({cnt_e}) & {mask_cnt}U; {ut} _r = {sh_e}; {w} }}"
                ]
            pf_s = " pf = (uint8_t)(!__builtin_parity((uint8_t)_r));" if uses_pf else ""
            return [
                f"{{ {ut} _a = {a}; uint8_t _c = ({cnt_e}) & {mask_cnt}U; {ut} _r = {sh_e}; {w} "
                f"if (_c != 0) {{ cf = (uint8_t){cf_e}; zf = (_r == 0); sf = (uint8_t)((_r >> {msb}) & 1U); of = {of_e};{pf_s} }} }}"
            ]

        if mn in ("shrd", "shld") and len(ops) == 3:
            bits = ops[0].size * 8
            ut = self._u_type(bits)
            mask_cnt = 63 if bits == 64 else 31
            a = self._read_op(ins, ops[0], bits, image, index, shard_const_pool)
            b = self._read_op(ins, ops[1], bits, image, index, shard_const_pool)
            c = self._read_op(ins, ops[2], 8, image, index, shard_const_pool)
            w = self._write_op(ins, ops[0], bits, "_r", image, index, shard_const_pool)
            if mn == "shrd":
                expr = f"(_c == 0) ? _a : ({ut})((_a >> _c) | (_b << ({bits} - _c)))"
            else:
                expr = f"(_c == 0) ? _a : ({ut})((_a << _c) | (_b >> ({bits} - _c)))"
            return [f"{{ {ut} _a = {a}, _b = {b}; uint8_t _c = ({c}) & {mask_cnt}U; {ut} _r = {expr}; {w} }}"]

        # 10. Bit Test & Bit Scan (`bt`, `bts`, `btr`, `btc`, `bsf`, `bsr`)
        if mn in ("bt", "bts", "btr", "btc") and len(ops) == 2:
            bits = ops[0].size * 8
            ut = self._u_type(bits)
            if ops[0].kind == "mem" and ops[1].kind == "reg":
                # Memory + register bit offset indexes signed bit offset across memory words
                base_addr = self._mem_addr_expr(ins, ops[0], image, index, shard_const_pool)
                bit_s = f"(int64_t){self._read_op(ins, ops[1], bits, image, index, shard_const_pool)}"
                word_bytes = bits // 8
                ptr_e = f"(({ut} *)(uintptr_t)({base_addr} + (uint64_t)((({bit_s}) >> {3 if bits == 8 else (5 if bits == 32 else 6)}) * {word_bytes})))"
                bidx_e = f"((uint8_t)(({bit_s}) & {bits - 1}))"
                if mn == "bt":
                    return [f"{{ {ut} *_p = {ptr_e}; uint8_t _bi = {bidx_e}; cf = (uint8_t)((*_p >> _bi) & 1U); }}"]
                mod = {"bts": "|= ((%s)1ULL << _bi)" % ut, "btr": "&= ~((%s)1ULL << _bi)" % ut, "btc": "^= ((%s)1ULL << _bi)" % ut}[mn]
                return [f"{{ {ut} *_p = {ptr_e}; uint8_t _bi = {bidx_e}; cf = (uint8_t)((*_p >> _bi) & 1U); *_p {mod}; }}"]

            a = self._read_op(ins, ops[0], bits, image, index, shard_const_pool)
            b = self._read_op(ins, ops[1], 8 if ops[1].kind == "imm" else bits, image, index, shard_const_pool)
            if mn == "bt":
                return [f"cf = (uint8_t)(({a} >> (({b}) & {bits - 1}U)) & 1U);"]
            w = self._write_op(ins, ops[0], bits, "_r", image, index, shard_const_pool)
            mod_op = {"bts": "|", "btr": "& ~", "btc": "^"}[mn]
            return [
                f"{{ {ut} _a = {a}; uint8_t _bi = (uint8_t)(({b}) & {bits - 1}U); "
                f"cf = (uint8_t)((_a >> _bi) & 1U); {ut} _r = ({ut})(_a {mod_op} (({ut})1ULL << _bi)); {w} }}"
            ]

        if mn in ("bsf", "bsr") and len(ops) == 2:
            bits = ops[0].size * 8
            ut = self._u_type(bits)
            s = self._read_op(ins, ops[1], bits, image, index, shard_const_pool)
            w = self._write_op(ins, ops[0], bits, "_r", image, index, shard_const_pool)
            if mn == "bsf":
                calc = "__builtin_ctzll((uint64_t)_s)"
            else:
                calc = f"(63 - __builtin_clzll((uint64_t)_s))"
            return [
                f"{{ {ut} _s = {s}; if (_s == 0) {{ zf = 1; }} else {{ zf = 0; {ut} _r = ({ut})({calc}); {w} }} }}"
            ]

        # 11. Atomic `lock` Operations & `xchg` / `cmpxchg`
        if mn.startswith("lock ") or mn in ("xchg", "cmpxchg"):
            return self._lift_atomic_insn(ins, image, index, shard_const_pool, need_flags, uses_pf)

        # 12. Conditional Byte Set (`setcc`) & Conditional Move (`cmovcc`)
        if mn.startswith("set") and len(ops) == 1:
            cond = self._cond_expr(mn[3:])
            return [self._write_op(ins, ops[0], 8, f"({cond} ? 1U : 0U)", image, index, shard_const_pool)]

        if mn.startswith("cmov") and len(ops) == 2:
            bits = ops[0].size * 8
            cond = self._cond_expr(mn[4:])
            cur_v = self._read_op(ins, ops[0], bits, image, index, shard_const_pool)
            src_v = self._read_op(ins, ops[1], bits, image, index, shard_const_pool)
            # Note: for 32-bit `cmovcc eax, ecx`, writing `cur_v` when false STILL zero-extends `eax` to `rax`!
            return [self._write_op(ins, ops[0], bits, f"({cond} ? {src_v} : {cur_v})", image, index, shard_const_pool)]

        # 13. Calls (`call`)
        if mn == "call" and len(ops) == 1:
            return self._lift_call_insn(ins, image, index, shard_const_pool)

        # 14. Unconditional & Switch Jumps (`jmp`)
        if mn == "jmp" and len(ops) == 1:
            if ins.address in cfg.jump_tables:
                jt = cfg.jump_tables[ins.address]
                idx_expr = self._read_gpr64(jt.index_reg)
                sw_lines = [f"switch ({idx_expr}) {{"]
                for case_i, tgt_va in enumerate(jt.targets):
                    if tgt_va in cfg.blocks:
                        sw_lines.append(f"    case {case_i}: goto L_{tgt_va:x};")
                if jt.default_target and jt.default_target in cfg.blocks:
                    sw_lines.append(f"    default: goto L_{jt.default_target:x};")
                else:
                    sw_lines.append("    default: return rax;")
                sw_lines.append("}")
                return sw_lines

            if getattr(blk, "branch_target", None) is not None and blk.branch_target in cfg.blocks:  # type: ignore[attr-defined]
                tgt = blk.branch_target  # type: ignore[attr-defined]
                if is_last and tgt == next_blk_addr:
                    return []
                return [f"goto L_{tgt:x};"]

            if getattr(blk, "tailcall_target", None) is not None:
                return self._lift_tailcall_insn(
                    ins, blk.tailcall_target, image, index, shard_const_pool  # type: ignore[attr-defined]
                )
            return ["return rax;"]

        # 15. Conditional Branches & Loop Instructions (`jcc`, `jrcxz`, `jecxz`, `loop*`)
        if mn in COND_JUMPS and len(ops) == 1:
            if mn == "jrcxz":
                cond = "(rcx == 0ULL)"
                pre = ""
            elif mn == "jecxz":
                cond = "((uint32_t)rcx == 0U)"
                pre = ""
            elif mn == "loop":
                pre = "rcx -= 1ULL; "
                cond = "(rcx != 0ULL)"
            elif mn in ("loope", "loopz"):
                pre = "rcx -= 1ULL; "
                cond = "(rcx != 0ULL && zf)"
            elif mn in ("loopne", "loopnz"):
                pre = "rcx -= 1ULL; "
                cond = "(rcx != 0ULL && !zf)"
            else:
                pre = ""
                cond = self._cond_expr(mn[1:])

            btgt = getattr(blk, "branch_target", None)
            ctail = getattr(blk, "cond_tailcall_target", None)
            ft = getattr(blk, "fallthrough", None)
            out_j: list[str] = []
            if btgt is not None and btgt in cfg.blocks:
                out_j.append(f"{pre}if ({cond}) goto L_{btgt:x};")
            elif ctail is not None:
                tc_stmts = " ".join(self._lift_tailcall_insn(ins, ctail, image, index, shard_const_pool))
                out_j.append(f"{pre}if ({cond}) {{ {tc_stmts} }}")
            if ft is not None and ft in cfg.blocks and ft != next_blk_addr:
                out_j.append(f"goto L_{ft:x};")
            return out_j

        raise ValueError(f"Unhandled pure-C instruction in {cfg.name} @ 0x{ins.address:x}: {ins.text}")

    # ------------------------------------------------------------------
    # XMM / SSE2 / Scalar Float Instruction Helper
    # ------------------------------------------------------------------
    def _lift_xmm_insn(
        self,
        ins: DisasmInsn,
        image: BinaryImage,
        index: WholeKernelSymbolIndex,
        shard_const_pool: dict[int, bytes],
    ) -> list[str]:
        mn = ins.mnemonic
        ops = ins.operands

        # 128-bit load / store / reg-to-reg move
        if mn in ("movups", "movaps", "movapd", "movdqu", "movdqa", "movntdq") and len(ops) == 2:
            if ops[0].kind == "reg" and ops[1].kind == "reg":
                return [f"{ops[0].reg} = {ops[1].reg};"]
            if ops[0].kind == "reg" and ops[1].kind == "mem":
                addr = self._mem_addr_expr(ins, ops[1], image, index, shard_const_pool)
                return [f"__builtin_memcpy(&{ops[0].reg}, (const void *)(uintptr_t)({addr}), 16);"]
            if ops[0].kind == "mem" and ops[1].kind == "reg":
                addr = self._mem_addr_expr(ins, ops[0], image, index, shard_const_pool)
                return [f"__builtin_memcpy((void *)(uintptr_t)({addr}), &{ops[1].reg}, 16);"]

        if mn == "movq" and len(ops) == 2:
            if ops[0].kind == "reg" and (ops[0].reg or "") in _XMM_REGS:
                d = ops[0].reg
                if ops[1].kind == "reg" and (ops[1].reg or "") in _XMM_REGS:
                    return [f"{d}.u64[0] = {ops[1].reg}.u64[0]; {d}.u64[1] = 0ULL;"]
                v = self._read_op(ins, ops[1], 64, image, index, shard_const_pool)
                return [f"{d}.u64[0] = {v}; {d}.u64[1] = 0ULL;"]
            return [self._write_op(ins, ops[0], 64, f"{ops[1].reg}.u64[0]", image, index, shard_const_pool)]

        if mn == "movd" and len(ops) == 2:
            if ops[0].kind == "reg" and (ops[0].reg or "") in _XMM_REGS:
                d = ops[0].reg
                v = self._read_op(ins, ops[1], 32, image, index, shard_const_pool)
                return [f"{d}.u64[0] = (uint64_t)(uint32_t){v}; {d}.u64[1] = 0ULL;"]
            return [self._write_op(ins, ops[0], 32, f"{ops[1].reg}.u32[0]", image, index, shard_const_pool)]

        if mn == "movsd" and len(ops) == 2:
            if ops[0].kind == "reg" and ops[1].kind == "reg":
                return [f"{ops[0].reg}.u64[0] = {ops[1].reg}.u64[0];"]
            if ops[0].kind == "reg" and ops[1].kind == "mem":
                v = self._read_op(ins, ops[1], 64, image, index, shard_const_pool)
                return [f"{ops[0].reg}.u64[0] = {v}; {ops[0].reg}.u64[1] = 0ULL;"]
            return [self._write_op(ins, ops[0], 64, f"{ops[1].reg}.u64[0]", image, index, shard_const_pool)]

        if mn == "movss" and len(ops) == 2:
            if ops[0].kind == "reg" and ops[1].kind == "reg":
                return [f"{ops[0].reg}.u32[0] = {ops[1].reg}.u32[0];"]
            if ops[0].kind == "reg" and ops[1].kind == "mem":
                v = self._read_op(ins, ops[1], 32, image, index, shard_const_pool)
                return [f"{ops[0].reg}.u64[0] = (uint64_t)(uint32_t){v}; {ops[0].reg}.u64[1] = 0ULL;"]
            return [self._write_op(ins, ops[0], 32, f"{ops[1].reg}.u32[0]", image, index, shard_const_pool)]

        if mn == "movhpd" and len(ops) == 2:
            if ops[0].kind == "reg":
                v = self._read_op(ins, ops[1], 64, image, index, shard_const_pool)
                return [f"{ops[0].reg}.u64[1] = {v};"]
            return [self._write_op(ins, ops[0], 64, f"{ops[1].reg}.u64[1]", image, index, shard_const_pool)]

        if mn == "movlhps" and len(ops) == 2:
            return [f"{ops[0].reg}.u64[1] = {ops[1].reg}.u64[0];"]

        if mn in ("xorps", "xorpd", "pxor", "pand", "por", "orps", "andps") and len(ops) == 2:
            d = ops[0].reg
            if ops[1].kind == "reg" and ops[0].reg == ops[1].reg and mn in ("xorps", "xorpd", "pxor"):
                return [f"{d}.u64[0] = 0ULL; {d}.u64[1] = 0ULL;"]
            op_c = "^" if "xor" in mn else ("&" if "and" in mn else "|")
            if ops[1].kind == "reg":
                s = ops[1].reg
                return [f"{d}.u64[0] {op_c}= {s}.u64[0]; {d}.u64[1] {op_c}= {s}.u64[1];"]
            addr = self._mem_addr_expr(ins, ops[1], image, index, shard_const_pool)
            return [
                f"{{ nt_xmm_t _m; __builtin_memcpy(&_m, (const void *)(uintptr_t)({addr}), 16); "
                f"{d}.u64[0] {op_c}= _m.u64[0]; {d}.u64[1] {op_c}= _m.u64[1]; }}"
            ]

        if mn == "psrldq" and len(ops) == 2:
            d = ops[0].reg
            sh = ops[1].imm & 0xFF
            if sh >= 16:
                return [f"{d}.u64[0] = 0ULL; {d}.u64[1] = 0ULL;"]
            if sh == 0:
                return []
            bsh = sh * 8
            return [
                f"{{ __uint128_t _v = (((__uint128_t){d}.u64[1] << 64) | {d}.u64[0]) >> {bsh}; "
                f"{d}.u64[0] = (uint64_t)_v; {d}.u64[1] = (uint64_t)(_v >> 64); }}"
            ]

        if mn == "psllq" and len(ops) == 2:
            d = ops[0].reg
            cnt_e = f"({ops[1].imm & 0xFF}ULL)" if ops[1].kind == "imm" else f"{ops[1].reg}.u64[0]"
            return [
                f"{{ uint64_t _c = {cnt_e}; if (_c >= 64ULL) {{ {d}.u64[0] = 0ULL; {d}.u64[1] = 0ULL; }} "
                f"else {{ {d}.u64[0] <<= _c; {d}.u64[1] <<= _c; }} }}"
            ]

        if mn == "paddd" and len(ops) == 2:
            d, s = ops[0].reg, ops[1].reg
            return [
                f"for (int _i = 0; _i < 4; ++_i) {d}.u32[_i] += {s}.u32[_i];"
            ]

        if mn == "paddq" and len(ops) == 2:
            d, s = ops[0].reg, ops[1].reg
            return [f"{d}.u64[0] += {s}.u64[0]; {d}.u64[1] += {s}.u64[1];"]

        if mn == "punpcklqdq" and len(ops) == 2:
            d, s = ops[0].reg, ops[1].reg
            return [f"{d}.u64[1] = {s}.u64[0];"]

        if mn == "punpckldq" and len(ops) == 2:
            d, s = ops[0].reg, ops[1].reg
            return [
                f"{{ nt_xmm_t _a = {d}, _b = {s}; "
                f"{d}.u32[0] = _a.u32[0]; {d}.u32[1] = _b.u32[0]; {d}.u32[2] = _a.u32[1]; {d}.u32[3] = _b.u32[1]; }}"
            ]

        if mn == "punpcklwd" and len(ops) == 2:
            d, s = ops[0].reg, ops[1].reg
            return [
                f"{{ nt_xmm_t _a = {d}, _b = {s}; "
                f"for (int _i = 0; _i < 4; ++_i) {{ {d}.u16[_i*2] = _a.u16[_i]; {d}.u16[_i*2+1] = _b.u16[_i]; }} }}"
            ]

        if mn == "pshufd" and len(ops) == 3:
            d = ops[0].reg
            imm = ops[2].imm & 0xFF
            src_load = (
                f"nt_xmm_t _s = {ops[1].reg};"
                if ops[1].kind == "reg"
                else f"nt_xmm_t _s; __builtin_memcpy(&_s, (const void *)(uintptr_t)({self._mem_addr_expr(ins, ops[1], image, index, shard_const_pool)}), 16);"
            )
            return [
                f"{{ {src_load} "
                f"{d}.u32[0] = _s.u32[{imm & 3}]; {d}.u32[1] = _s.u32[{(imm >> 2) & 3}]; "
                f"{d}.u32[2] = _s.u32[{(imm >> 4) & 3}]; {d}.u32[3] = _s.u32[{(imm >> 6) & 3}]; }}"
            ]

        if mn == "pextrw" and len(ops) == 3:
            lane = ops[2].imm & 7
            return [self._write_op(ins, ops[0], 32, f"(uint32_t){ops[1].reg}.u16[{lane}]", image, index, shard_const_pool)]

        if mn in ("cvtsi2sd", "cvtsi2ss") and len(ops) == 2:
            d = ops[0].reg
            sbits = ops[1].size * 8
            sv = f"({self._s_type(sbits)}){self._read_op(ins, ops[1], sbits, image, index, shard_const_pool)}"
            if mn == "cvtsi2sd":
                return [f"{d}.f64[0] = (double)({sv});"]
            return [f"{d}.f32[0] = (float)({sv});"]

        if mn == "cvtsd2ss" and len(ops) == 2:
            d = ops[0].reg
            sv = f"{ops[1].reg}.f64[0]" if ops[1].kind == "reg" else f"(*(const double *)(uintptr_t)({self._mem_addr_expr(ins, ops[1], image, index, shard_const_pool)}))"
            return [f"{d}.f32[0] = (float)({sv});"]

        if mn == "cvtps2pd" and len(ops) == 2:
            d, s = ops[0].reg, ops[1].reg
            return [f"{{ nt_xmm_t _s = {s}; {d}.f64[0] = (double)_s.f32[0]; {d}.f64[1] = (double)_s.f32[1]; }}"]

        if mn == "cvtdq2pd" and len(ops) == 2:
            d, s = ops[0].reg, ops[1].reg
            return [f"{{ nt_xmm_t _s = {s}; {d}.f64[0] = (double)(int32_t)_s.u32[0]; {d}.f64[1] = (double)(int32_t)_s.u32[1]; }}"]

        if mn == "cvtdq2ps" and len(ops) == 2:
            d, s = ops[0].reg, ops[1].reg
            return [f"{{ nt_xmm_t _s = {s}; for (int _i = 0; _i < 4; ++_i) {d}.f32[_i] = (float)(int32_t)_s.u32[_i]; }}"]

        if mn in ("addsd", "subsd", "mulsd", "divsd") and len(ops) == 2:
            d = ops[0].reg
            op_c = {"addsd": "+", "subsd": "-", "mulsd": "*", "divsd": "/"}[mn]
            if ops[1].kind == "reg":
                rhs = f"{ops[1].reg}.f64[0]"
            else:
                u64v = self._read_op(ins, ops[1], 64, image, index, shard_const_pool)
                rhs = f"__nt_u64_as_f64({u64v})"
            return [f"{d}.f64[0] {op_c}= {rhs};"]

        if mn in ("addss", "subss", "mulss", "divss") and len(ops) == 2:
            d = ops[0].reg
            op_c = {"addss": "+", "subss": "-", "mulss": "*", "divss": "/"}[mn]
            if ops[1].kind == "reg":
                rhs = f"{ops[1].reg}.f32[0]"
            else:
                u32v = self._read_op(ins, ops[1], 32, image, index, shard_const_pool)
                rhs = f"__nt_u32_as_f32({u32v})"
            return [f"{d}.f32[0] {op_c}= {rhs};"]

        if mn in ("comisd", "ucomisd") and len(ops) == 2:
            lhs = f"{ops[0].reg}.f64[0]"
            rhs = (
                f"{ops[1].reg}.f64[0]"
                if ops[1].kind == "reg"
                else f"__nt_u64_as_f64({self._read_op(ins, ops[1], 64, image, index, shard_const_pool)})"
            )
            return [
                f"{{ double _x = {lhs}, _y = {rhs}; "
                f"if (__builtin_isnan(_x) || __builtin_isnan(_y)) {{ zf = 1; cf = 1; of = 0; sf = 0; }} "
                f"else {{ zf = (_x == _y); cf = (_x < _y); of = 0; sf = 0; }} }}"
            ]

        if mn in ("comiss", "ucomiss") and len(ops) == 2:
            lhs = f"{ops[0].reg}.f32[0]"
            rhs = (
                f"{ops[1].reg}.f32[0]"
                if ops[1].kind == "reg"
                else f"__nt_u32_as_f32({self._read_op(ins, ops[1], 32, image, index, shard_const_pool)})"
            )
            return [
                f"{{ float _x = {lhs}, _y = {rhs}; "
                f"if (__builtin_isnan(_x) || __builtin_isnan(_y)) {{ zf = 1; cf = 1; of = 0; sf = 0; }} "
                f"else {{ zf = (_x == _y); cf = (_x < _y); of = 0; sf = 0; }} }}"
            ]

        raise ValueError(f"Unhandled XMM instruction @ 0x{ins.address:x}: {ins.text}")

    # ------------------------------------------------------------------
    # Atomic `lock *` & `xchg` / `cmpxchg` Helper
    # ------------------------------------------------------------------
    def _lift_atomic_insn(
        self,
        ins: DisasmInsn,
        image: BinaryImage,
        index: WholeKernelSymbolIndex,
        shard_const_pool: dict[int, bytes],
        need_flags: bool,
        uses_pf: bool,
    ) -> list[str]:
        mn = ins.mnemonic
        ops = ins.operands
        bits = ops[0].size * 8
        ut = self._u_type(bits)
        msb = bits - 1
        pf_s = " pf = (uint8_t)(!__builtin_parity((uint8_t)_r));" if uses_pf else ""

        if mn == "xchg" and len(ops) == 2:
            if ops[0].kind == "reg" and ops[1].kind == "reg":
                a = self._read_op(ins, ops[0], bits, image, index, shard_const_pool)
                b = self._read_op(ins, ops[1], bits, image, index, shard_const_pool)
                w0 = self._write_op(ins, ops[0], bits, "_b", image, index, shard_const_pool)
                w1 = self._write_op(ins, ops[1], bits, "_a", image, index, shard_const_pool)
                return [f"{{ {ut} _a = {a}, _b = {b}; {w0} {w1} }}"]
            mem_op, reg_op = (ops[0], ops[1]) if ops[0].kind == "mem" else (ops[1], ops[0])
            ptr = self._mem_ptr_expr(ins, mem_op, bits, image, index, shard_const_pool)
            rv = self._read_op(ins, reg_op, bits, image, index, shard_const_pool)
            wr = self._write_op(ins, reg_op, bits, "_old", image, index, shard_const_pool)
            return [f"{{ {ut} _old = __atomic_exchange_n({ptr}, {rv}, __ATOMIC_SEQ_CST); {wr} }}"]

        if mn in ("lock cmpxchg", "cmpxchg") and len(ops) == 2:
            src = self._read_op(ins, ops[1], bits, image, index, shard_const_pool)
            acc_op = InsnOperand(kind="reg", size=ops[0].size, reg={8: "al", 16: "ax", 32: "eax", 64: "rax"}[bits])
            w_acc = self._write_op(ins, acc_op, bits, "_exp", image, index, shard_const_pool)
            if ops[0].kind == "mem":
                ptr = self._mem_ptr_expr(ins, ops[0], bits, image, index, shard_const_pool)
                return [
                    f"{{ {ut} _exp = ({ut})rax; "
                    f"if (__atomic_compare_exchange_n({ptr}, &_exp, {src}, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) "
                    f"{{ zf = 1; cf = 0; of = 0; sf = 0; }} else {{ zf = 0; cf = ((({ut})rax) < _exp); {w_acc} }} }}"
                ]
            cur_d = self._read_op(ins, ops[0], bits, image, index, shard_const_pool)
            w_dst = self._write_op(ins, ops[0], bits, src, image, index, shard_const_pool)
            return [
                f"{{ {ut} _cur = {cur_d}, _exp = ({ut})rax; "
                f"if (_cur == _exp) {{ {w_dst} zf = 1; cf = 0; }} else {{ zf = 0; cf = (_exp < _cur); _exp = _cur; {w_acc} }} }}"
            ]

        ptr = self._mem_ptr_expr(ins, ops[0], bits, image, index, shard_const_pool)

        if mn == "lock xadd" and len(ops) == 2:
            rv = self._read_op(ins, ops[1], bits, image, index, shard_const_pool)
            wr = self._write_op(ins, ops[1], bits, "_old", image, index, shard_const_pool)
            if not need_flags:
                return [f"{{ {ut} _v = {rv}, _old = __atomic_fetch_add({ptr}, _v, __ATOMIC_SEQ_CST); {wr} }}"]
            return [
                f"{{ {ut} _v = {rv}, _old = __atomic_fetch_add({ptr}, _v, __ATOMIC_SEQ_CST), _r = ({ut})(_old + _v); "
                f"{wr} zf = (_r == 0); sf = (uint8_t)((_r >> {msb}) & 1U); cf = (_r < _old); "
                f"of = (uint8_t)((~(_old ^ _v) & (_old ^ _r)) >> {msb}) & 1U;{pf_s} }}"
            ]

        if mn in ("lock add", "lock sub", "lock and", "lock or", "lock xor") and len(ops) == 2:
            sub_mn = mn.split()[1]
            fn = {
                "add": "__atomic_fetch_add",
                "sub": "__atomic_fetch_sub",
                "and": "__atomic_fetch_and",
                "or": "__atomic_fetch_or",
                "xor": "__atomic_fetch_xor",
            }[sub_mn]
            v = self._read_op(ins, ops[1], bits, image, index, shard_const_pool)
            if not need_flags:
                return [f"(void){fn}({ptr}, {v}, __ATOMIC_SEQ_CST);"]
            op_c = {"add": "+", "sub": "-", "and": "&", "or": "|", "xor": "^"}[sub_mn]
            return [
                f"{{ {ut} _v = {v}, _old = {fn}({ptr}, _v, __ATOMIC_SEQ_CST), _r = ({ut})(_old {op_c} _v); "
                f"zf = (_r == 0); sf = (uint8_t)((_r >> {msb}) & 1U); cf = 0; of = 0;{pf_s} }}"
            ]

        if mn in ("lock inc", "lock dec") and len(ops) == 1:
            fn = "__atomic_fetch_add" if mn == "lock inc" else "__atomic_fetch_sub"
            op_c = "+" if mn == "lock inc" else "-"
            if not need_flags:
                return [f"(void){fn}({ptr}, ({ut})1U, __ATOMIC_SEQ_CST);"]
            return [
                f"{{ {ut} _old = {fn}({ptr}, ({ut})1U, __ATOMIC_SEQ_CST), _r = ({ut})(_old {op_c} 1U); "
                f"zf = (_r == 0); sf = (uint8_t)((_r >> {msb}) & 1U);{pf_s} }}"
            ]

        if mn in ("lock bts", "lock btr") and len(ops) == 2:
            bi = self._read_op(ins, ops[1], 8 if ops[1].kind == "imm" else bits, image, index, shard_const_pool)
            if mn == "lock bts":
                return [
                    f"{{ uint8_t _bi = (uint8_t)(({bi}) & {bits - 1}U); "
                    f"{ut} _old = __atomic_fetch_or({ptr}, (({ut})1ULL << _bi), __ATOMIC_SEQ_CST); "
                    f"cf = (uint8_t)((_old >> _bi) & 1U); }}"
                ]
            return [
                f"{{ uint8_t _bi = (uint8_t)(({bi}) & {bits - 1}U); "
                f"{ut} _old = __atomic_fetch_and({ptr}, ~( ({ut})1ULL << _bi ), __ATOMIC_SEQ_CST); "
                f"cf = (uint8_t)((_old >> _bi) & 1U); }}"
            ]

        raise ValueError(f"Unhandled atomic instruction @ 0x{ins.address:x}: {ins.text}")

    # ------------------------------------------------------------------
    # Direct, IAT, Indirect & Helper Call / Tail-Call Emitters
    # ------------------------------------------------------------------
    def _build_call_args_list(self, arity: int, stack_base_disp: int) -> str:
        """
        Build argument list `(rcx, rdx, r8, r9, M64(rsp+disp), ...)`.
        `stack_base_disp` is `0x20` for `call` and `0x28` for `jmp`/`jcc` tailcalls!
        """
        args = ["rcx", "rdx", "r8", "r9"][: max(4, arity)]
        for i in range(5, arity + 1):
            off = stack_base_disp + 8 * (i - 5)
            args.append(f"M64(rsp + 0x{off:x})")
        return ", ".join(args)

    def _lift_call_insn(
        self,
        ins: DisasmInsn,
        image: BinaryImage,
        index: WholeKernelSymbolIndex,
        shard_const_pool: dict[int, bytes],
    ) -> list[str]:
        op0 = ins.operands[0]
        if op0.kind == "imm":
            tgt_va = op0.imm
            fm = index.functions_by_va.get(tgt_va)
            if fm is not None:
                cname = fm.c_name
                orig = fm.orig_name
                # Compiler-emitted stack/security/CFG helpers with custom register preservation!
                if orig in ("__chkstk", "_alloca_probe"):
                    # Probes `rax` stack bytes and preserves ALL registers (including `rax`)!
                    return ["(void)rax; /* __chkstk stack probe */"]
                if orig == "__security_check_cookie":
                    # Validates `rcx == __security_cookie` without clobbering `rax`!
                    return ["__nt_check_cookie_preserve_rax(rcx);"]
                if orig in ("_guard_check_icall_nop", "_guard_check_icall"):
                    return ["(void)rcx; /* CFG check */"]
                if orig in ("_guard_dispatch_icall", "_guard_dispatch_icall_nop", "KeGuardDispatchICall"):
                    # CFG indirect dispatch calls target in `rax` with Win64 args!
                    arg_s = self._build_call_args_list(12, 0x20)
                    return [f"rax = ((nt_msabi_fn12_t)(uintptr_t)rax)({arg_s});"]
                if orig in ("sqrt", "_frnd"):
                    return [f"xmm0.f64[0] = ((nt_msabi_f64_1_t)(uintptr_t)&{cname})(xmm0.f64[0]);"]
                if orig == "sqrtf":
                    return [f"xmm0.f32[0] = ((nt_msabi_f32_1_t)(uintptr_t)&{cname})(xmm0.f32[0]);"]
                if orig == "_finite":
                    return [f"rax = ((nt_msabi_u64_f64_t)(uintptr_t)&{cname})(xmm0.f64[0]);"]

                arg_s = self._build_call_args_list(fm.arity, 0x20)
                return [f"rax = {cname}({arg_s});"]

        # Indirect call (`call reg` or `call qword ptr [mem]` including IAT `[rip + __imp_*]`)
        fn_ptr = self._read_op(ins, op0, 64, image, index, shard_const_pool)
        arg_s = self._build_call_args_list(12, 0x20)
        return [f"rax = ((nt_msabi_fn12_t)(uintptr_t)({fn_ptr}))({arg_s});"]

    def _lift_tailcall_insn(
        self,
        ins: DisasmInsn,
        target_va: int,
        image: BinaryImage,
        index: WholeKernelSymbolIndex,
        shard_const_pool: dict[int, bytes],
    ) -> list[str]:
        fm = index.functions_by_va.get(target_va)
        if fm is not None:
            if fm.orig_name in ("_guard_dispatch_icall", "_guard_dispatch_icall_nop", "KeGuardDispatchICall"):
                arg_s = self._build_call_args_list(12, 0x28)
                return [f"return ((nt_msabi_fn12_t)(uintptr_t)rax)({arg_s});"]
            arg_s = self._build_call_args_list(fm.arity, 0x28)
            return [f"return {fm.c_name}({arg_s});"]
        arg_s = self._build_call_args_list(12, 0x28)
        return [f"return ((nt_msabi_fn12_t)(uintptr_t)0x{target_va:x}ULL)({arg_s});"]

    # ------------------------------------------------------------------
    # Headers, Symbolic Data Sections & RVA Linker Manifest
    # ------------------------------------------------------------------
    def _build_asm_va_symbol_map(
        self, image: BinaryImage, index: WholeKernelSymbolIndex
    ) -> dict[int, str]:
        """Map exact VAs + section interior offsets to relocatable GAS expressions."""
        base = image.image_base
        vmap: dict[int, str] = {base: "__ImageBase"}
        for fva, fm in index.functions_by_va.items():
            vmap[fva] = fm.c_name
        for mva, mlbl in index.mid_asm_labels.items():
            vmap[mva] = mlbl
        for iat_va, (c_imp, _) in index.imports_by_va.items():
            vmap[iat_va] = c_imp
        for drva, dsym in index.data_syms_by_rva.items():
            vmap[base + drva] = dsym
        for va in index.asm_data_symbols:
            vmap.setdefault(va, f"__nt_asm_sym_{va:x}")

        # Populate section-offset fallback via a custom dict subclass so ANY RIP-relative
        # offset inside `.rdata`/`.data`/`ALMOSTRO`/etc. resolves dynamically without O(N) RAM!
        emitted_secs = [
            (s.virtual_address, s.virtual_address + max(s.virtual_size, len(s.raw_data)), sec_ident_name(s.name))
            for s in image.sections
            if not s.is_executable and s.name in index.emitted_data_sections
        ]

        class _RelocSymDict(dict):  # type: ignore[type-arg]
            def get(self, key: int, default: str | None = None) -> str | None:  # type: ignore[override]
                if key in self:
                    return super().__getitem__(key)
                rva = key - base
                for s_lo, s_hi, s_id in emitted_secs:
                    if s_lo <= rva < s_hi:
                        off = rva - s_lo
                        return s_id if off == 0 else f"{s_id} + 0x{off:x}"
                return default

        return _RelocSymDict(vmap)

    def _emit_runtime_header(self) -> str:
        lines = [
            "#ifndef NT_WHOLE_KERNEL_RUNTIME_H",
            "#define NT_WHOLE_KERNEL_RUNTIME_H",
            "",
            "#include <stdint.h>",
            "#include <stddef.h>",
            '#include "reconstructed_types.h"',
            "",
            "#define M8(a)  (*(uint8_t *)(uintptr_t)(a))",
            "#define M16(a) (*(uint16_t *)(uintptr_t)(a))",
            "#define M32(a) (*(uint32_t *)(uintptr_t)(a))",
            "#define M64(a) (*(uint64_t *)(uintptr_t)(a))",
            "",
            "typedef union nt_xmm_u {",
            "    uint64_t u64[2];",
            "    uint32_t u32[4];",
            "    uint16_t u16[8];",
            "    uint8_t  u8[16];",
            "    double   f64[2];",
            "    float    f32[4];",
            "} nt_xmm_t;",
            "",
            "static inline double __nt_u64_as_f64(uint64_t v) {",
            "    double d; __builtin_memcpy(&d, &v, 8); return d;",
            "}",
            "static inline float __nt_u32_as_f32(uint32_t v) {",
            "    float f; __builtin_memcpy(&f, &v, 4); return f;",
            "}",
            "",
            "/* Ring-0 vs Usermode-Harness GS segment (`_KPCR`), Control Registers & Intrinsics */",
            "#ifdef NT_USERMODE_HARNESS",
            "extern uint8_t __nt_usermode_gs_area[0x10000];",
            "extern uint64_t __nt_sim_cr2, __nt_sim_cr3, __nt_sim_cr8;",
            "#define NT_GS_PTR(T, off) ((T *)(void *)(&__nt_usermode_gs_area[(uintptr_t)(off) & 0xFFFFU]))",
            "static inline uint64_t __nt_readcr2(void) { return __nt_sim_cr2; }",
            "static inline uint64_t __nt_readcr3(void) { return __nt_sim_cr3; }",
            "static inline uint64_t __nt_readcr8(void) { return __nt_sim_cr8; }",
            "static inline void __nt_writecr8(uint64_t v) { __nt_sim_cr8 = v & 0xFULL; }",
            "static inline void __nt_cli(void) {}",
            "static inline void __nt_sti(void) {}",
            "#else",
            "#define NT_GS_PTR(T, off) ((T __seg_gs *)(uintptr_t)(off))",
            'static inline uint64_t __nt_readcr2(void) { uint64_t v; __asm__ __volatile__("mov %%cr2, %0" : "=r"(v)); return v; }',
            'static inline uint64_t __nt_readcr3(void) { uint64_t v; __asm__ __volatile__("mov %%cr3, %0" : "=r"(v)); return v; }',
            'static inline uint64_t __nt_readcr8(void) { uint64_t v; __asm__ __volatile__("mov %%cr8, %0" : "=r"(v)); return v; }',
            'static inline void __nt_writecr8(uint64_t v) { __asm__ __volatile__("mov %0, %%cr8" :: "r"(v) : "memory"); }',
            'static inline void __nt_cli(void) { __asm__ __volatile__("cli" ::: "memory"); }',
            'static inline void __nt_sti(void) { __asm__ __volatile__("sti" ::: "memory"); }',
            "#endif",
            "",
            "static inline uint64_t __nt_rdtsc(void) {",
            "    uint32_t lo, hi;",
            '    __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));',
            "    return ((uint64_t)hi << 32) | lo;",
            "}",
            'static inline void __nt_clflush(const void *p) { __asm__ __volatile__("clflush (%0)" :: "r"(p) : "memory"); }',
            "static inline void __nt_int3(void) { __builtin_trap(); }",
            "extern void __attribute__((ms_abi)) __nt_int29(uint64_t code);",
            "extern void __attribute__((ms_abi)) __nt_int_n(uint32_t vec);",
            "extern void __attribute__((ms_abi)) __nt_check_cookie_preserve_rax(uint64_t cookie);",
            "",
            "/* Typed Windows x64 ms_abi indirect function pointer types */",
        ]
        for n in range(4, 21):
            ps = ", ".join(["uint64_t"] * n)
            lines.append(f"typedef uint64_t (__attribute__((ms_abi)) *nt_msabi_fn{n}_t)({ps});")
        lines.extend([
            "typedef double (__attribute__((ms_abi)) *nt_msabi_f64_1_t)(double);",
            "typedef float  (__attribute__((ms_abi)) *nt_msabi_f32_1_t)(float);",
            "typedef uint64_t (__attribute__((ms_abi)) *nt_msabi_u64_f64_t)(double);",
            "",
            "#endif /* NT_WHOLE_KERNEL_RUNTIME_H */",
            "",
        ])
        return "\n".join(lines)

    def _emit_protos_header(self, index: WholeKernelSymbolIndex) -> str:
        lines = [
            "#ifndef NT_WHOLE_KERNEL_PROTOS_H",
            "#define NT_WHOLE_KERNEL_PROTOS_H",
            "",
            '#include "nt_whole_kernel_runtime.h"',
            "",
            "extern uint8_t __ImageBase[];",
        ]
        for sname in index.emitted_data_sections:
            lines.append(f"extern uint8_t {sec_ident_name(sname)}[];")
        lines.append("")

        lines.append("/* IAT Imported Module Slots (HAL.dll, PSHED.dll, BOOTVID.dll, kdcom.dll, CLFS, CI) */")
        for _, (c_imp, full_imp) in sorted(index.imports_by_va.items()):
            lines.append(f"extern void *{c_imp}; /* {full_imp} */")
        lines.append("")

        lines.append(f"/* Named Kernel Global Data Symbols ({len(index.data_syms_by_rva)} symbols) */")
        for drva in index.sorted_data_rvas:
            lines.append(f"extern uint8_t {index.data_syms_by_rva[drva]}[];")
        lines.append("")

        lines.append(f"/* Whole-Kernel Function Prototypes ({len(index.functions_by_rva)} functions) */")
        for rva in sorted(index.functions_by_rva.keys()):
            fm = index.functions_by_rva[rva]
            ps = ", ".join(f"uint64_t p{i}" for i in range(1, fm.arity + 1))
            lines.append(f"uint64_t __attribute__((ms_abi)) {fm.c_name}({ps});")
            for al in fm.icf_aliases:
                lines.append(f"uint64_t __attribute__((ms_abi)) {al}({ps});")

        lines.extend(["", "#endif /* NT_WHOLE_KERNEL_PROTOS_H */", ""])
        return "\n".join(lines)

    def _emit_data_sections_asm(
        self, image: BinaryImage, index: WholeKernelSymbolIndex
    ) -> str:
        """
        Emit exact `.rdata`, `.data`, `ALMOSTRO`, `PAGEDATA`, `INITDATA`, etc. sections
        with global symbol labels at exact RVA offsets and symbolic `.reloc` pointers!
        """
        out: list[str] = [
            "/* Exact Reconstructed Data & Read-Only Sections of ntoskrnl.exe */",
            "    .section .note.GNU-stack,\"\",@progbits",
            "",
        ]
        # Index data symbols by section
        syms_in_rva: dict[int, str] = dict(index.data_syms_by_rva)

        # Helper to map a DIR64 target RVA to a symbol expression
        emitted_secs = [
            (s.virtual_address, s.virtual_address + max(s.virtual_size, len(s.raw_data)), sec_ident_name(s.name))
            for s in image.sections
            if not s.is_executable and s.name in index.emitted_data_sections
        ]

        def resolve_reloc_sym(tgt_rva: int) -> str | None:
            if tgt_rva == 0:
                return "__ImageBase"
            if tgt_rva in index.functions_by_rva:
                return index.functions_by_rva[tgt_rva].c_name
            if tgt_rva in syms_in_rva:
                return syms_in_rva[tgt_rva]
            for s_lo, s_hi, s_id in emitted_secs:
                if s_lo <= tgt_rva < s_hi:
                    off = tgt_rva - s_lo
                    return s_id if off == 0 else f"{s_id} + {off}"
            return None

        for sec in image.sections:
            if sec.is_executable or sec.name not in index.emitted_data_sections:
                continue
            s_id = sec_ident_name(sec.name)
            elf_sec = ".data" if sec.is_writable else ".rodata"
            out.append(f"    .section {elf_sec}")
            out.append("    .balign 16")
            out.append(f"    .globl {s_id}")
            out.append(f"{s_id}:")

            raw = sec.raw_data
            raw_len = len(raw)
            vsize = max(sec.virtual_size, raw_len)
            s_rva = sec.virtual_address

            # Collect split points inside `[0 .. vsize]`: symbol RVAs and DIR64 reloc slots
            sec_sym_offs: dict[int, str] = {}
            for rva_k, sym_n in syms_in_rva.items():
                if s_rva <= rva_k <= s_rva + vsize:
                    sec_sym_offs[rva_k - s_rva] = sym_n

            sec_reloc_offs: dict[int, str] = {}
            for slot_rva, tgt_rva in index.dir64_relocs.items():
                if s_rva <= slot_rva <= s_rva + raw_len - 8:
                    rsym = resolve_reloc_sym(tgt_rva)
                    if rsym is not None:
                        sec_reloc_offs[slot_rva - s_rva] = rsym

            # Emit initialized `[0 .. raw_len)` cleanly in chunks between labels & relocs
            events = sorted(set(sec_sym_offs.keys()) | set(sec_reloc_offs.keys()) | {raw_len, vsize})
            pos = 0
            for ev in events:
                if ev < pos:
                    continue
                if pos < min(ev, raw_len):
                    self._emit_raw_bytes_directives(out, raw[pos : min(ev, raw_len)])
                    pos = min(ev, raw_len)
                if pos < ev and pos >= raw_len:
                    out.append(f"    .zero {ev - pos}")
                    pos = ev
                if pos in sec_sym_offs:
                    sn = sec_sym_offs[pos]
                    out.append(f"    .globl {sn}")
                    out.append(f"{sn}:")
                if pos in sec_reloc_offs and pos + 8 <= raw_len:
                    # Also check if any symbol sits inside `(pos, pos + 8)` — if so skip symbolic `.quad`
                    if not any(pos < so < pos + 8 for so in sec_sym_offs):
                        out.append(f"    .quad {sec_reloc_offs[pos]}")
                        pos += 8

            if pos < vsize:
                out.append(f"    .zero {vsize - pos}")
            out.append("")

        if index.asm_data_symbols:
            out.append("    .section .rodata")
            out.append("    .balign 16")
            out.append("    /* Exact inline literals, import-directory slots, and raw-ASM jump-table targets */")
            for target_va, raw_bytes in sorted(index.asm_data_symbols.items()):
                sym_name = f"__nt_asm_sym_{target_va:x}"
                out.append(f"    .globl {sym_name}")
                out.append(f"{sym_name}:")
                self._emit_raw_bytes_directives(out, raw_bytes)
            out.append("")

        return "\n".join(out)

    def _emit_raw_bytes_directives(self, out: list[str], chunk: bytes) -> None:
        n = len(chunk)
        i = 0
        while i < n:
            # Compress runs of >= 16 zero bytes into `.zero K`
            if chunk[i] == 0:
                j = i + 1
                while j < n and chunk[j] == 0:
                    j += 1
                if j - i >= 16:
                    out.append(f"    .zero {j - i}")
                    i = j
                    continue
            # Emit 8-byte `.quad` chunks where possible (compact & fast for GAS!)
            if i + 8 <= n:
                q_vals: list[str] = []
                while i + 8 <= n and len(q_vals) < 8:
                    if chunk[i : i + 16] == b"\x00" * 16:
                        break
                    qv = struct.unpack_from("<Q", chunk, i)[0]
                    q_vals.append(f"0x{qv:x}")
                    i += 8
                if q_vals:
                    out.append("    .quad " + ", ".join(q_vals))
                    continue
            b_vals: list[str] = []
            while i < n and len(b_vals) < 16:
                b_vals.append(f"0x{chunk[i]:02x}")
                i += 1
            out.append("    .byte " + ", ".join(b_vals))

    def _emit_int128_runtime_c(self) -> str:
        """Emit freestanding GCC 128-bit helpers instead of linking a C runtime.

        GCC may request these compiler-generated symbols from lifted arithmetic, but a
        standalone PE cannot leave them unresolved. This bitwise
        implementation has no division/modulo operators of its own and therefore does
        not recurse into the helpers it defines.
        """
        return r'''/* Freestanding GCC __int128 helpers for the generated kernel image. */
#include <stdint.h>

typedef unsigned __int128 nt_u128_t;
typedef __int128 nt_s128_t;

static nt_u128_t nt_udivmod128(nt_u128_t numerator, nt_u128_t denominator,
                               nt_u128_t *remainder) {
    if (denominator == 0) {
        __builtin_trap();
    }
    nt_u128_t quotient = 0;
    nt_u128_t rem = 0;
    for (int bit = 127; bit >= 0; --bit) {
        nt_u128_t shifted = (rem << 1) | ((numerator >> bit) & 1);
        if ((rem >> 127) != 0 || shifted >= denominator) {
            rem = shifted - denominator;
            quotient |= ((nt_u128_t)1 << bit);
        } else {
            rem = shifted;
        }
    }
    if (remainder != 0) {
        *remainder = rem;
    }
    return quotient;
}

nt_u128_t __attribute__((ms_abi)) __udivti3(nt_u128_t a, nt_u128_t b) {
    return nt_udivmod128(a, b, 0);
}

nt_u128_t __attribute__((ms_abi)) __umodti3(nt_u128_t a, nt_u128_t b) {
    nt_u128_t remainder;
    (void)nt_udivmod128(a, b, &remainder);
    return remainder;
}

nt_s128_t __attribute__((ms_abi)) __divti3(nt_s128_t a, nt_s128_t b) {
    int negative = (a < 0) != (b < 0);
    nt_u128_t ua = a < 0 ? (nt_u128_t)(0 - a) : (nt_u128_t)a;
    nt_u128_t ub = b < 0 ? (nt_u128_t)(0 - b) : (nt_u128_t)b;
    nt_u128_t quotient = nt_udivmod128(ua, ub, 0);
    return negative ? (nt_s128_t)(0 - quotient) : (nt_s128_t)quotient;
}

nt_s128_t __attribute__((ms_abi)) __modti3(nt_s128_t a, nt_s128_t b) {
    int negative = a < 0;
    nt_u128_t ua = a < 0 ? (nt_u128_t)(0 - a) : (nt_u128_t)a;
    nt_u128_t ub = b < 0 ? (nt_u128_t)(0 - b) : (nt_u128_t)b;
    nt_u128_t remainder;
    (void)nt_udivmod128(ua, ub, &remainder);
    return negative ? (nt_s128_t)(0 - remainder) : (nt_s128_t)remainder;
}
'''

    def _emit_globals_and_imports_c(self, index: WholeKernelSymbolIndex) -> str:
        lines = [
            "/* Whole-Kernel Runtime Helpers, IAT Import Dispatch Table & Weak Fallbacks */",
            '#include "nt_whole_kernel_runtime.h"',
            '#include "nt_whole_kernel_protos.h"',
            "",
            "uint8_t __attribute__((weak)) __ImageBase[16] = { 'M', 'Z' };",
            "",
            "#ifdef NT_USERMODE_HARNESS",
            "uint8_t __nt_usermode_gs_area[0x10000] __attribute__((aligned(64)));",
            "uint64_t __nt_sim_cr2 = 0ULL, __nt_sim_cr3 = 0x1AD000ULL, __nt_sim_cr8 = 0ULL;",
            "#endif",
            "",
            "void __attribute__((ms_abi)) __nt_int29(uint64_t code) {",
            "    (void)code;",
            "    __builtin_trap();",
            "}",
            "void __attribute__((ms_abi)) __nt_int_n(uint32_t vec) {",
            "    (void)vec;",
            "    __builtin_trap();",
            "}",
            "void __attribute__((ms_abi)) __nt_check_cookie_preserve_rax(uint64_t cookie) {",
            "    (void)cookie;",
            "    __builtin_trap();",
            "}",
            "",
            "/* Default standalone Ring-0 / HAL / KD / BootVid import stubs (overridden by Windows Loader IAT in PE builds) */",
            "static uint64_t __attribute__((ms_abi)) __nt_default_imp_stub(uint64_t rcx, uint64_t rdx, uint64_t r8, uint64_t r9) {",
            "    (void)rcx; (void)rdx; (void)r8; (void)r9;",
            "    return 0ULL;",
            "}",
            "static uint64_t __attribute__((ms_abi)) __nt_hal_query_perf_counter(uint64_t freq_out) {",
            "    if (freq_out) *(uint64_t *)(uintptr_t)freq_out = 10000000ULL;",
            "    return __nt_rdtsc();",
            "}",
            "",
        ]
        for _, (c_imp, full_imp) in sorted(index.imports_by_va.items()):
            target_stub = (
                "__nt_hal_query_perf_counter"
                if "KeQueryPerformanceCounter" in full_imp
                else "__nt_default_imp_stub"
            )
            lines.append(f"void *{c_imp} = (void *)&{target_stub}; /* {full_imp} */")

        if index.external_asm_stubs:
            lines.append("")
            lines.append("/* Weak fallback stubs for unreferenced shadow/patchguard labels */")
            for slbl in sorted(index.external_asm_stubs):
                lines.append(f"void __attribute__((weak)) {slbl}(void) {{}}")

        lines.append("")
        return "\n".join(lines)

    def _write_rva_manifest(
        self, out_json: Path, image: BinaryImage, index: WholeKernelSymbolIndex
    ) -> None:
        funcs_dict = {}
        for rva in sorted(index.functions_by_rva.keys()):
            fm = index.functions_by_rva[rva]
            entry = {
                "rva": rva,
                "size": fm.size,
                "orig_name": fm.orig_name,
                "section": fm.section_name,
                "subsystem": fm.subsystem_key,
                "shard": fm.shard_id,
                "is_exported": fm.is_exported,
                "is_raw_asm": fm.is_raw_asm,
                "arity": fm.arity,
                "original_machine_code_sha256": fm.original_machine_code_sha256,
                "icf_aliases": fm.icf_aliases,
                "export_aliases": fm.export_aliases,
            }
            funcs_dict[fm.c_name] = entry

        sec_rvas = {
            sec_ident_name(s.name): {
                "name": s.name,
                "rva": s.virtual_address,
                "virtual_size": s.virtual_size,
                "raw_size": len(s.raw_data),
                "writable": s.is_writable,
            }
            for s in image.sections
            if not s.is_executable and s.name in index.emitted_data_sections
        }

        manifest = {
            "image_base": image.image_base,
            "entry_point_rva": image.entry_point_rva,
            "rsds_guid": index.rsds_guid,
            "rsds_age": index.rsds_age,
            "pdb_filename": index.pdb_filename,
            "function_count": len(funcs_dict),
            "sections": sec_rvas,
            "imports": {
                c_imp: {"rva": iat_va - image.image_base, "target": full_imp}
                for iat_va, (c_imp, full_imp) in sorted(index.imports_by_va.items())
            },
            "data_symbols": {
                dsym: drva for drva, dsym in sorted(index.data_syms_by_rva.items())
            },
            "functions": funcs_dict,
        }
        out_json.write_text(json.dumps(manifest, separators=(",", ":")), encoding="utf-8")
