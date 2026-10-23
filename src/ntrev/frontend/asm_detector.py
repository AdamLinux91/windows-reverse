"""
Hand-Written / Non-C NT Kernel Assembly Detector & Preserver.

Parts of ntoskrnl.exe (e.g., KiSystemCall64, KiSwapContext, KiPageFault, trap stubs,
MSR/CR3/TLB routines) are written directly in x86-64 assembly and violate C semantics
or execute ring-0 instructions with no standard C representation.

This module classifies both:
  1. Whole-function assembly routines (system call/interrupt entry stubs, raw RSP swaps,
     sysretq/iretq terminators, KVASCODE kernel-user shadow stubs) -> preserved as
     complete naked __asm__ __volatile__ functions.
  2. Inline privileged blocks inside C functions (e.g. invlpg, mov cr3, wrmsr, cli/sti
     surrounded by standard ms_abi C code) -> preserved as inline __asm__ statements
     spliced cleanly into the decompiled C AST.
"""

from __future__ import annotations

from ntrev.models import AsmBlockClassification, DisasmInsn, FunctionCFG

# Instructions that strictly belong to non-C system entry/exit stubs (whole function is raw ASM)
WHOLE_FUNC_ASM_MNEMONICS = {
    "swapgs",
    "sysret",
    "sysretq",
    "sysexit",
    "iret",
    "iretd",
    "iretq",
    "retf",
    "lretq",
}

# Ring-0 / CPU-state instructions that have no pure C equivalent (require inline asm)
INLINE_ASM_MNEMONICS = {
    "rdmsr", "wrmsr", "rdpmc", "rdtsc", "rdtscp", "cpuid",
    "lgdt", "sgdt", "lidt", "sidt", "lldt", "sldt", "ltr", "str",
    "invlpg", "invpcid", "wbinvd", "invd",
    "cli", "sti", "hlt", "clts", "lmsw", "smsw",
    "in", "out", "insb", "insw", "insd", "outsb", "outsw", "outsd",
    "fxsave", "fxsave64", "fxrstor", "fxrstor64",
    "xsave", "xsave64", "xsaveopt", "xrstor", "xrstor64", "xsetbv", "xgetbv",
    "pushfq", "popfq",
}

CONTROL_OR_DEBUG_REGS = {
    "cr0", "cr2", "cr3", "cr4", "cr8",
    "dr0", "dr1", "dr2", "dr3", "dr6", "dr7",
}

CANONICAL_GPR64 = {
    "rax": "rax", "eax": "rax", "ax": "rax", "al": "rax", "ah": "rax",
    "rbx": "rbx", "ebx": "rbx", "bx": "rbx", "bl": "rbx", "bh": "rbx",
    "rcx": "rcx", "ecx": "rcx", "cx": "rcx", "cl": "rcx", "ch": "rcx",
    "rdx": "rdx", "edx": "rdx", "dx": "rdx", "dl": "rdx", "dh": "rdx",
    "rsi": "rsi", "esi": "rsi", "si": "rsi", "sil": "rsi",
    "rdi": "rdi", "edi": "rdi", "di": "rdi", "dil": "rdi",
    "rbp": "rbp", "ebp": "rbp", "bp": "rbp", "bpl": "rbp",
    "rsp": "rsp", "esp": "rsp", "sp": "rsp", "spl": "rsp",
    "r8": "r8", "r8d": "r8", "r8w": "r8", "r8b": "r8",
    "r9": "r9", "r9d": "r9", "r9w": "r9", "r9b": "r9",
    "r10": "r10", "r10d": "r10", "r10w": "r10", "r10b": "r10",
    "r11": "r11", "r11d": "r11", "r11w": "r11", "r11b": "r11",
    "r12": "r12", "r12d": "r12", "r12w": "r12", "r12b": "r12",
    "r13": "r13", "r13d": "r13", "r13w": "r13", "r13b": "r13",
    "r14": "r14", "r14d": "r14", "r14w": "r14", "r14b": "r14",
    "r15": "r15", "r15d": "r15", "r15w": "r15", "r15b": "r15",
}


class AsmBlockDetector:
    """Detects hand-written NT kernel assembly functions and inline privileged blocks."""

    def classify_function(self, cfg: FunctionCFG) -> AsmBlockClassification:
        reasons: list[str] = []
        priv_insns: list[str] = []
        inline_block_addrs: list[int] = []
        force_whole_asm = False

        if cfg.section_name.upper() in ("KVASCODE", ".TRAP"):
            force_whole_asm = True
            reasons.append(f"Located in kernel shadow/trap assembly section '{cfg.section_name}'")

        all_insns = cfg.all_instructions()
        priv_count = 0

        for blk in cfg.sorted_blocks():
            blk_has_priv = False
            blk_reason = None
            for ins in blk.instructions:
                mnem = ins.mnemonic
                if mnem in WHOLE_FUNC_ASM_MNEMONICS:
                    force_whole_asm = True
                    priv_count += 1
                    desc = f"0x{ins.address:x}: {ins.text}"
                    priv_insns.append(desc)
                    blk_has_priv = True
                    blk_reason = f"Non-C kernel entry/exit instruction '{mnem}'"
                    reasons.append(f"{blk_reason} at 0x{ins.address:x}")

                elif mnem in INLINE_ASM_MNEMONICS or self._uses_control_or_debug_reg(ins):
                    priv_count += 1
                    desc = f"0x{ins.address:x}: {ins.text}"
                    priv_insns.append(desc)
                    blk_has_priv = True
                    blk_reason = f"Privileged CPU/CSR instruction '{ins.text}'"
                    reasons.append(f"{blk_reason} at 0x{ins.address:x}")

                elif self._is_non_abi_stack_pivot(ins):
                    force_whole_asm = True
                    priv_count += 1
                    desc = f"0x{ins.address:x}: {ins.text}"
                    priv_insns.append(desc)
                    blk_has_priv = True
                    blk_reason = f"Non-ABI kernel stack pivot '{ins.text}'"
                    reasons.append(f"{blk_reason} at 0x{ins.address:x}")

            if blk_has_priv:
                blk.is_raw_asm_block = True
                blk.raw_asm_reason = blk_reason
                inline_block_addrs.append(blk.address)

        # If majority of instructions are privileged/system state manipulations, or no C ABI
        # return exists while privileged ops exist, preserve the whole routine as naked ASM.
        has_standard_ret = any(i.mnemonic in ("ret", "retq", "retn") for i in all_insns)
        if priv_count > 0 and (not has_standard_ret or priv_count * 2 >= len(all_insns)):
            force_whole_asm = True
            if "High density of privileged kernel instructions" not in reasons:
                reasons.append(
                    f"Privileged/system instruction density ({priv_count}/{len(all_insns)} insns) "
                    "indicates hand-written kernel assembly routine"
                )

        inputs, outputs, clobbers = self._compute_register_interface(all_insns)

        classification = AsmBlockClassification(
            is_raw_asm=force_whole_asm,
            has_inline_asm_blocks=(not force_whole_asm and len(inline_block_addrs) > 0),
            reasons=reasons,
            privileged_instructions=priv_insns,
            inline_block_addrs=inline_block_addrs,
            inputs=inputs,
            outputs=outputs,
            clobbers=clobbers,
        )
        cfg.asm_classification = classification
        return classification

    def format_raw_asm_function(self, cfg: FunctionCFG) -> str:
        """Render a whole-function hand-written NT assembly routine as preserved C inline asm."""
        cls = cfg.asm_classification
        lines: list[str] = []
        lines.append("/*")
        lines.append(f" * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] {cfg.name} @ 0x{cfg.address:x}")
        lines.append(" * Automated non-C detection prevented destructive C structural forcing:")
        for reason in cls.reasons:
            lines.append(f" *   - {reason}")
        if cls.inputs:
            lines.append(f" *   Live-In Registers : {', '.join(cls.inputs)}")
        if cls.clobbers:
            lines.append(f" *   Clobbered State   : {', '.join(cls.clobbers)}")
        lines.append(" */")
        lines.append(f"__attribute__((naked)) void {cfg.name}(void) {{")
        lines.append("    __asm__ __volatile__ (")
        lines.append('        ".intel_syntax noprefix\\n\\t"')

        label_addrs = {
            blk.address
            for blk in cfg.sorted_blocks()
            if blk.predecessors and blk.address != cfg.entry_address
        }

        for blk in cfg.sorted_blocks():
            if blk.address in label_addrs:
                lines.append(f'        ".L_{cfg.name}_{blk.address:x}:\\n\\t"')
            for ins in blk.instructions:
                formatted = self._format_intel_insn(ins, cfg.name, label_addrs)
                lines.append(f'        "{formatted}\\n\\t"')

        lines.append('        ".att_syntax prefix"')
        lines.append("    );")
        lines.append("}")
        return "\n".join(lines)

    def format_inline_asm_insn(self, ins: DisasmInsn) -> str:
        """Render a single privileged instruction inside an otherwise-C function."""
        clobbers = ['"memory"', '"cc"']
        for wreg in ins.regs_write:
            canon = CANONICAL_GPR64.get(wreg)
            if canon and f'"{canon}"' not in clobbers:
                clobbers.append(f'"{canon}"')
        clobber_str = ", ".join(clobbers)
        return (
            f'__asm__ __volatile__ (".intel_syntax noprefix; {ins.text}; .att_syntax prefix" '
            f": : : {clobber_str});"
        )

    def _uses_control_or_debug_reg(self, ins: DisasmInsn) -> bool:
        return any(
            op.kind == "reg" and op.reg in CONTROL_OR_DEBUG_REGS
            for op in ins.operands
        )

    def _is_non_abi_stack_pivot(self, ins: DisasmInsn) -> bool:
        """Detect e.g. `mov rsp, gs:[0x1a8]` or `xchg rsp, ...` stack swaps in KiSystemCall64."""
        if ins.mnemonic in ("mov", "xchg") and ins.operands:
            dst = ins.operands[0]
            if dst.kind == "reg" and dst.reg == "rsp":
                if len(ins.operands) > 1:
                    src = ins.operands[1]
                    if src.kind == "mem" or (src.kind == "reg" and src.reg != "rbp"):
                        return True
        return False

    def _compute_register_interface(
        self, insns: list[DisasmInsn]
    ) -> tuple[list[str], list[str], list[str]]:
        defined: set[str] = set()
        live_in: list[str] = []
        written: list[str] = []

        for ins in insns:
            # Explicit + implicit reads before writes
            read_gprs: list[str] = []
            for r in ins.regs_read:
                canon = CANONICAL_GPR64.get(r)
                if canon:
                    read_gprs.append(canon)
            for op in ins.operands:
                if op.kind == "mem":
                    for mr in (op.base, op.index):
                        if mr and mr in CANONICAL_GPR64:
                            read_gprs.append(CANONICAL_GPR64[mr])
                elif op.kind == "reg" and ins.operands.index(op) > 0:
                    if op.reg and op.reg in CANONICAL_GPR64:
                        read_gprs.append(CANONICAL_GPR64[op.reg])

            # xor eax, eax is a zeroing write, not a true live-in read
            is_self_xor = (
                ins.mnemonic == "xor"
                and len(ins.operands) == 2
                and ins.operands[0].kind == "reg"
                and ins.operands[1].kind == "reg"
                and ins.operands[0].reg == ins.operands[1].reg
            )

            if not is_self_xor:
                for gpr in read_gprs:
                    if gpr != "rsp" and gpr not in defined and gpr not in live_in:
                        live_in.append(gpr)

            for r in ins.regs_write:
                canon = CANONICAL_GPR64.get(r)
                if canon:
                    defined.add(canon)
                    if canon not in written:
                        written.append(canon)
            if ins.operands and ins.operands[0].kind == "reg":
                r0 = ins.operands[0].reg
                if r0 and r0 in CANONICAL_GPR64 and ins.mnemonic not in ("cmp", "test"):
                    canon = CANONICAL_GPR64[r0]
                    defined.add(canon)
                    if canon not in written:
                        written.append(canon)

        outputs = ["rax"] if "rax" in written else []
        clobbers = [r for r in written if r != "rsp"] + ["memory", "cc"]
        return live_in, outputs, clobbers

    def _format_intel_insn(
        self, ins: DisasmInsn, fn_name: str, label_addrs: set[int]
    ) -> str:
        if len(ins.operands) == 1 and ins.operands[0].kind == "imm":
            target = ins.operands[0].imm
            if target in label_addrs:
                return f"{ins.mnemonic} .L_{fn_name}_{target:x}"
        return ins.text
