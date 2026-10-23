"""
Hand-Written / Non-C NT Kernel Assembly Detector & Preserver.

Parts of ntoskrnl.exe (e.g., KiSystemCall64, KiFlushCurrentTbWorker, KeRaiseIrqlToDpcLevel,
ExpInterlockedPopEntrySList, RtlCompareMemory, trap stubs, Zw* SSDT stubs, retpolines) are
written in x86-64 assembly or execute ring-0 / 128-bit atomic / CPU-specific instructions
with no portable C representation.

This module classifies both:
  1. Whole-function assembly routines -> emitted as `__attribute__((naked, ms_abi))`
     C-callable assembly functions or `__attribute__((naked)) void fn(void)` entry stubs,
     with all RIP-relative data & branch/call targets rewritten to relocatable symbols.
  2. Inline privileged blocks inside mixed C functions -> preserved as inline `__asm__`
     statements when requested by single-function C emission.
"""

from __future__ import annotations

import re

from ntrev.models import (
    AsmBlockClassification,
    DisasmInsn,
    FunctionCFG,
    InferredFuncSignature,
)

# Instructions that strictly belong to non-C system entry/exit stubs
ENTRY_STUB_ASM_MNEMONICS = {
    "swapgs",
    "sysret",
    "sysretq",
    "sysexit",
    "syscall",
    "sysenter",
    "iret",
    "iretd",
    "iretq",
    "retf",
    "retfq",
    "lretq",
    "encls",
    "vmcall",
    "vmmcall",
    "vmfunc",
}

# Hand-coded assembly / special-CPU idioms preserved whole-function in Ring-0 kernel builds
HAND_CODED_ASM_MNEMONICS = {
    "cmpxchg16b",
    "lock cmpxchg16b",
    "repe cmpsb",
    "repe cmpsw",
    "repe cmpsd",
    "repe cmpsq",
    "repne cmpsb",
    "repne cmpsw",
    "repne cmpsd",
    "repne cmpsq",
    "rep movsb",
    "rep movsw",
    "rep movsd",
    "rep movsq",
    "rep stosb",
    "rep stosw",
    "rep stosd",
    "rep stosq",
    "repne scasb",
    "repne scasw",
    "repne scasd",
    "repne scasq",
    "repe scasb",
    "repe scasw",
    "repe scasd",
    "repe scasq",
    "scasb",
    "scasw",
    "scasd",
    "scasq",
    "cmpsb",
    "cmpsw",
    "lodsb",
    "lodsw",
    "lodsd",
    "lodsq",
    "movsb",
    "movsw",
    "stosb",
    "stosw",
    "stosd",
    "stosq",
    "pushfq",
    "popfq",
    "std",
    "cld",
    "cmc",
    "stc",
    "clc",
    "lahf",
    "sahf",
    "xlatb",
    "enter",
    "rcl",
    "rcr",
    "lock adc",
    "xbegin",
    "xend",
    "rdrand",
    "int1",
}

WHOLE_FUNC_ASM_MNEMONICS = ENTRY_STUB_ASM_MNEMONICS | HAND_CODED_ASM_MNEMONICS

# Ring-0 / CPU-state instructions that have no pure C equivalent
INLINE_ASM_MNEMONICS = {
    "rdmsr", "wrmsr", "rdpmc", "rdtsc", "rdtscp", "cpuid",
    "lgdt", "sgdt", "lidt", "sidt", "lldt", "sldt", "ltr", "str", "lsl",
    "invlpg", "invpcid", "wbinvd", "invd", "clflush", "clflushopt", "clwb", "clzero",
    "cli", "sti", "hlt", "clts", "lmsw", "smsw", "wait",
    "in", "out", "insb", "insw", "insd", "outsb", "outsw", "outsd",
    "fxsave", "fxsave64", "fxrstor", "fxrstor64",
    "xsave", "xsave64", "xsaveopt", "xsavec", "xsaves",
    "xrstor", "xrstor64", "xrstors", "xsetbv", "xgetbv",
    "ldmxcsr", "stmxcsr",
    "pushfq", "popfq",
}

X87_FPU_MNEMONICS = {
    "fninit", "fnclex", "fnsave", "fnstcw", "fnstsw", "fldcw",
    "fbld", "fild", "fadd", "fmul", "fdiv", "fdivr", "fsub", "fsubr",
    "fisub", "ficomp", "fucompi", "ffree",
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

    def classify_function(
        self, cfg: FunctionCFG, whole_kernel_mode: bool = False
    ) -> AsmBlockClassification:
        reasons: list[str] = []
        priv_insns: list[str] = []
        inline_block_addrs: list[int] = []
        force_whole_asm = False
        has_non_abi_entry_or_pivot = False

        if cfg.section_name.upper() in ("KVASCODE", "RETPOL", ".TRAP"):
            force_whole_asm = True
            has_non_abi_entry_or_pivot = True
            reasons.append(f"Located in kernel shadow/retpoline/trap section '{cfg.section_name}'")

        all_insns = cfg.all_instructions()
        all_insn_addrs = {i.address for i in all_insns}
        priv_count = 0

        for blk in cfg.sorted_blocks():
            blk_has_priv = False
            blk_reason = None
            for ins in blk.instructions:
                mnem = ins.mnemonic
                if mnem in ENTRY_STUB_ASM_MNEMONICS:
                    force_whole_asm = True
                    has_non_abi_entry_or_pivot = True
                    priv_count += 1
                    desc = f"0x{ins.address:x}: {ins.text}"
                    priv_insns.append(desc)
                    blk_has_priv = True
                    blk_reason = f"Non-C kernel entry/exit instruction '{mnem}'"
                    reasons.append(f"{blk_reason} at 0x{ins.address:x}")

                elif (
                    mnem in HAND_CODED_ASM_MNEMONICS
                    or mnem in X87_FPU_MNEMONICS
                    or mnem.startswith(("v", "sha256"))
                    or (mnem == "cmpsd" and "ptr" in ins.op_str and "xmm" not in ins.op_str)
                    or "cmpxchg16b" in ins.text
                ):
                    force_whole_asm = True
                    priv_count += 1
                    desc = f"0x{ins.address:x}: {ins.text}"
                    priv_insns.append(desc)
                    blk_has_priv = True
                    blk_reason = f"Hand-coded/ISA-specific x86-64 instruction '{mnem}'"
                    reasons.append(f"{blk_reason} at 0x{ins.address:x}")

                elif mnem in INLINE_ASM_MNEMONICS or self._uses_control_or_debug_reg(ins):
                    priv_count += 1
                    desc = f"0x{ins.address:x}: {ins.text}"
                    priv_insns.append(desc)
                    blk_has_priv = True
                    blk_reason = f"Privileged CPU/CSR instruction '{ins.text}'"
                    reasons.append(f"{blk_reason} at 0x{ins.address:x}")
                    if whole_kernel_mode:
                        # Standard MSVC <intrin.h> / wdm.h C intrinsics (__readcr8/__writecr8/__readcr2/__readcr3/_disable/_enable/__rdtsc)
                        # map cleanly to C helpers; all other privileged/MSR/IO/XSave/CR0/CR4/DRx ops stay raw ASM.
                        is_c_intrin = mnem in (
                            "rdtsc",
                            "clflush",
                            "clflushopt",
                            "cli",
                            "sti",
                        ) or (
                            mnem == "mov"
                            and any(op.kind == "reg" and op.reg in ("cr2", "cr3", "cr8") for op in ins.operands)
                        )
                        if not is_c_intrin:
                            force_whole_asm = True

                elif self._is_non_abi_stack_pivot(ins):
                    force_whole_asm = True
                    has_non_abi_entry_or_pivot = True
                    priv_count += 1
                    desc = f"0x{ins.address:x}: {ins.text}"
                    priv_insns.append(desc)
                    blk_has_priv = True
                    blk_reason = f"Non-ABI kernel stack pivot '{ins.text}'"
                    reasons.append(f"{blk_reason} at 0x{ins.address:x}")

                elif (
                    mnem == "jmp"
                    and ins.operands
                    and ins.operands[0].kind != "imm"
                    and ins.address not in cfg.jump_tables
                ):
                    force_whole_asm = True
                    priv_count += 1
                    desc = f"0x{ins.address:x}: {ins.text}"
                    priv_insns.append(desc)
                    blk_has_priv = True
                    blk_reason = f"Indirect dispatch / IAT tail-jump '{ins.text}'"
                    reasons.append(f"{blk_reason} at 0x{ins.address:x}")

                elif (
                    whole_kernel_mode
                    and mnem == "call"
                    and len(ins.operands) == 1
                    and ins.operands[0].kind == "imm"
                    and ins.operands[0].imm in all_insn_addrs
                    and ins.operands[0].imm != cfg.entry_address
                ):
                    # Intra-function call (e.g. Spectre RSB stuffing `call next; add rsp, 8`)
                    force_whole_asm = True
                    has_non_abi_entry_or_pivot = True
                    reasons.append(f"Intra-function RSB/local call at 0x{ins.address:x}")

            if blk_has_priv:
                blk.is_raw_asm_block = True
                blk.raw_asm_reason = blk_reason
                inline_block_addrs.append(blk.address)

        if whole_kernel_mode and not force_whole_asm:
            non_std = self._check_non_standard_entry_regs(cfg)
            if non_std:
                force_whole_asm = True
                reasons.append(f"Non-standard entry register live-in ({', '.join(non_std)})")

        has_standard_ret = any(i.mnemonic in ("ret", "retq", "retn") for i in all_insns)
        has_cr_gpr_flow = (not whole_kernel_mode) and any(
            self._uses_control_or_debug_reg(i) for i in all_insns
        )
        if priv_count > 0 and (
            not has_standard_ret or priv_count * 2 >= len(all_insns) or has_cr_gpr_flow
        ):
            force_whole_asm = True
            if "High density of privileged kernel instructions" not in reasons:
                reasons.append(
                    f"Privileged/control-register routine ({priv_count}/{len(all_insns)} insns) "
                    "preserved as native x86-64 assembly"
                )

        has_indirect_jmp = any(
            i.mnemonic == "jmp"
            and i.operands
            and i.operands[0].kind != "imm"
            and i.address not in cfg.jump_tables
            for i in all_insns
        )
        is_c_callable = (
            force_whole_asm
            and (has_standard_ret or has_indirect_jmp)
            and not has_non_abi_entry_or_pivot
        )

        inputs, outputs, clobbers = self._compute_register_interface(all_insns)

        classification = AsmBlockClassification(
            is_raw_asm=force_whole_asm,
            has_inline_asm_blocks=(not force_whole_asm and len(inline_block_addrs) > 0),
            reasons=reasons[:8],
            privileged_instructions=priv_insns[:16],
            inline_block_addrs=inline_block_addrs,
            inputs=inputs,
            outputs=outputs,
            clobbers=clobbers,
            is_c_callable_naked=is_c_callable,
        )
        cfg.asm_classification = classification
        return classification

    def _check_non_standard_entry_regs(self, cfg: FunctionCFG) -> list[str]:
        """Detect routines reading RAX, R10, R11, or XMM0..XMM3 before writing them on entry."""
        entry_blk = cfg.blocks.get(cfg.entry_address)
        if entry_blk is None:
            return []
        written: set[str] = set()
        live_in: list[str] = []
        for ins in entry_blk.instructions:
            mn = ins.mnemonic
            if mn.startswith(("nop", "prefetch")):
                continue
            ops = ins.operands
            if len(ops) == 2 and ops[0].kind == "reg":
                r0 = ops[0].reg or ""
                c0 = CANONICAL_GPR64.get(r0, r0)
                if (
                    mn in ("xor", "sub", "sbb", "pxor", "xorps", "xorpd")
                    and ops[1].kind == "reg"
                    and ops[0].reg == ops[1].reg
                ):
                    written.add(c0)
                    continue
                if mn == "or" and ops[1].kind == "imm" and (ops[1].imm & 0xFFFFFFFF) == 0xFFFFFFFF:
                    written.add(c0)
                    continue
                if mn == "and" and ops[1].kind == "imm" and ops[1].imm == 0:
                    written.add(c0)
                    continue
                if mn.startswith("mov"):
                    for rr in ins.regs_read:
                        cr = CANONICAL_GPR64.get(rr, rr)
                        if cr != c0 and (cr in ("rax", "r10", "r11") or cr in ("xmm0", "xmm1", "xmm2", "xmm3")):
                            if cr not in written and cr not in live_in:
                                live_in.append(cr)
                    written.add(c0)
                    continue
            for rr in ins.regs_read:
                cr = CANONICAL_GPR64.get(rr, rr)
                if cr in ("rax", "r10", "r11", "xmm0", "xmm1", "xmm2", "xmm3"):
                    if cr not in written and cr not in live_in:
                        live_in.append(cr)
            for wr in ins.regs_write:
                cw = CANONICAL_GPR64.get(wr, wr)
                written.add(cw)
            if mn == "call" or mn.startswith(("j", "ret")):
                break
        return live_in

    def format_raw_asm_function(
        self,
        cfg: FunctionCFG,
        signature: InferredFuncSignature | None = None,
        va_symbol_map: dict[int, str] | None = None,
        whole_kernel_mode: bool = False,
        exported_mid_labels: dict[int, str] | None = None,
        mid_label_owners: dict[int, int] | None = None,
    ) -> str:
        """Render a whole-function hand-written NT assembly routine as preserved C inline asm."""
        cls = cfg.asm_classification
        lines: list[str] = []
        lines.append(f"/* [PRESERVED NT KERNEL ASM] {cfg.name} @ 0x{cfg.address:x} */")

        if (cls.is_c_callable_naked or whole_kernel_mode) and signature is not None:
            param_str = (
                ", ".join(f"{ct} {pn}" for pn, ct in signature.params)
                if signature.params
                else "void"
            )
            lines.append(
                f"{signature.return_type} __attribute__((naked, ms_abi)) {cfg.name}({param_str}) {{"
            )
        elif whole_kernel_mode:
            lines.append(f"uint64_t __attribute__((naked, ms_abi)) {cfg.name}(void) {{")
        else:
            lines.append(f"__attribute__((naked)) void {cfg.name}(void) {{")

        lines.append("    __asm__ __volatile__ (")
        lines.append('        ".intel_syntax noprefix\\n\\t"')

        valid_block_addrs = set(cfg.blocks.keys())
        all_insn_addrs = {
            ins.address for blk in cfg.blocks.values() for ins in blk.instructions
        }
        label_addrs: set[int] = set()
        ext_targets: list[int] = []
        ext_seen: set[int] = set()
        ext_rip_syms: list[int] = []
        ext_rip_seen: set[int] = set()
        rip_pat = re.compile(r"\[rip(?:\s*([+-])\s*(0x[0-9a-fA-F]+|\d+))?\]")
        vmap = va_symbol_map or {}

        for blk in cfg.sorted_blocks():
            if blk.predecessors and blk.address != cfg.entry_address:
                label_addrs.add(blk.address)
            for ins in blk.instructions:
                if len(ins.operands) == 1 and ins.operands[0].kind == "imm":
                    tgt = ins.operands[0].imm
                    if tgt in valid_block_addrs or tgt in all_insn_addrs:
                        label_addrs.add(tgt)
                    elif (ins.mnemonic == "call" or ins.mnemonic.startswith(("j", "loop"))) and tgt not in ext_seen:
                        ext_seen.add(tgt)
                        ext_targets.append(tgt)
                m_rip = rip_pat.search(ins.text)
                if m_rip:
                    disp = 0
                    if m_rip.group(2) is not None:
                        disp = int(m_rip.group(2), 0)
                        if m_rip.group(1) == "-":
                            disp = -disp
                    rip_tgt = (ins.address + ins.size + disp) & 0xFFFFFFFFFFFFFFFF
                    if rip_tgt in all_insn_addrs:
                        label_addrs.add(rip_tgt)
                    elif rip_tgt not in ext_rip_seen:
                        ext_rip_seen.add(rip_tgt)
                        ext_rip_syms.append(rip_tgt)

        ext_stubs: list[str] = []
        for tgt in ext_targets:
            sym_n = vmap.get(tgt, f"__nt_asm_ext_{tgt:x}")
            base_sym = sym_n.split()[0]
            if base_sym != cfg.name and not whole_kernel_mode:
                ext_stubs.append(f"extern void {base_sym}(void);")
            elif base_sym.startswith("__nt_asm_ext_"):
                ext_stubs.append(f"extern void {base_sym}(void);")
        for rtgt in ext_rip_syms:
            sym_n = vmap.get(rtgt, f"__nt_asm_sym_{rtgt:x}")
            base_sym = sym_n.split()[0]
            if base_sym != cfg.name and (not whole_kernel_mode or base_sym.startswith("__nt_asm_sym_")):
                ext_stubs.append(f"extern unsigned char {base_sym}[];")

        mid_lbls = exported_mid_labels or {}
        mid_owners = mid_label_owners or {}
        body_lines: list[str] = []
        emitted_labels: set[int] = set()
        for blk in cfg.sorted_blocks():
            if blk.address in label_addrs and blk.address not in emitted_labels:
                emitted_labels.add(blk.address)
                body_lines.append(f'        ".L_{cfg.name}_{blk.address:x}:\\n\\t"')
            for ins in blk.instructions:
                if (
                    ins.address in mid_lbls
                    and ins.address in all_insn_addrs
                    and mid_owners.get(ins.address, cfg.address) == cfg.address
                ):
                    ml = mid_lbls[ins.address]
                    body_lines.append(f'        ".globl {ml}\\n\\t{ml}:\\n\\t"')
                if ins.address in label_addrs and ins.address not in emitted_labels:
                    emitted_labels.add(ins.address)
                    body_lines.append(f'        ".L_{cfg.name}_{ins.address:x}:\\n\\t"')
                formatted = self._format_intel_insn(ins, cfg.name, label_addrs, vmap)
                body_lines.append(f'        "{formatted}\\n\\t"')

        lines_out: list[str] = []
        if ext_stubs:
            # Deduplicate extern declarations
            lines_out.extend(dict.fromkeys(ext_stubs))
        lines_out.extend(lines)
        lines_out.extend(body_lines)
        lines_out.append('        ".att_syntax prefix"')
        lines_out.append("    );")
        lines_out.append("}")
        return "\n".join(lines_out)

    def format_inline_asm_insn(self, ins: DisasmInsn) -> str:
        """Render a single privileged instruction inside an otherwise-C function."""
        clobbers = ['"memory"', '"cc"']
        for wreg in ins.regs_write:
            canon = CANONICAL_GPR64.get(wreg)
            if canon and f'"{canon}"' not in clobbers:
                clobbers.append(f'"{canon}"')
        clobber_str = ", ".join(clobbers)
        norm_txt = self._normalize_gas_text(ins.text)
        return (
            f'__asm__ __volatile__ (".intel_syntax noprefix; {norm_txt}; .att_syntax prefix" '
            f": : : {clobber_str});"
        )

    def _uses_control_or_debug_reg(self, ins: DisasmInsn) -> bool:
        return any(
            op.kind == "reg" and op.reg in CONTROL_OR_DEBUG_REGS
            for op in ins.operands
        )

    def _is_non_abi_stack_pivot(self, ins: DisasmInsn) -> bool:
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

    def _normalize_gas_text(self, text: str) -> str:
        """Normalize Capstone Intel disassembly quirks to strict GNU `as` `.intel_syntax noprefix`."""
        t = text.replace("oword ptr", "xmmword ptr")
        for no_ptr in (
            "fxsave64", "fxrstor64", "fxsave", "fxrstor",
            "xsave64", "xrstor64", "xsave", "xrstor", "xsaveopt", "xsavec", "xsaves", "xrstors",
        ):
            if t.startswith(no_ptr + " ptr "):
                t = no_ptr + t[len(no_ptr) + 4 :]
                break
        if t.startswith("retfq"):
            return "lretq" + t[5:]
        if t.startswith("retf") and not t.startswith("retfq"):
            return "lret" + t[4:]
        # Strip explicit operands on string / port ops where GAS .intel_syntax prefers bare mnemonic
        for bare in (
            "repe cmpsb", "repe cmpsw", "repe cmpsd", "repe cmpsq",
            "repne cmpsb", "repne cmpsw", "repne cmpsd", "repne cmpsq",
            "rep movsb", "rep movsw", "rep movsd", "rep movsq",
            "rep stosb", "rep stosw", "rep stosd", "rep stosq",
            "repne scasb", "repne scasw", "repne scasd", "repne scasq",
            "repe scasb", "repe scasw", "repe scasd", "repe scasq",
            "scasb", "scasw", "scasd", "scasq",
            "cmpsb", "cmpsw",
            "lodsb", "lodsw", "lodsd", "lodsq",
            "movsb", "movsw",
            "stosb", "stosw", "stosd", "stosq",
            "insb", "insw", "insd",
            "outsb", "outsw", "outsd",
            "iretq", "iretd", "sysretq", "swapgs", "xlatb",
        ):
            if t == bare or t.startswith(bare + " "):
                return bare
        if t.startswith("cmpsd ") and "ptr" in t and "xmm" not in t:
            return "cmpsd"
        return t

    def _format_intel_insn(
        self,
        ins: DisasmInsn,
        fn_name: str,
        label_addrs: set[int],
        va_symbol_map: dict[int, str] | None = None,
    ) -> str:
        vmap = va_symbol_map or {}
        if ins.mnemonic == "ret" and ins.op_str.strip() == "0":
            return "ret"
        # Raw byte fallback for instructions whose operand syntax varies in older GAS
        if (
            ins.mnemonic
            in (
                "movabs",
                "enter",
                "lsl",
                "encls",
                "vmfunc",
                "fbld",
                "fnsave",
                "frstor",
                "fldenv",
                "fnstenv",
                "fstenv",
                "retf",
                "retfq",
                "lret",
                "lretq",
            )
            and "[rip" not in ins.text
        ) or (
            ins.mnemonic
            in (
                "mov",
                "test",
                "cmp",
                "and",
                "or",
                "add",
                "sub",
                "inc",
                "dec",
                "xchg",
                "lock xadd",
                "lock cmpxchg",
                "lock bts",
                "lock btr",
                "lock and",
                "lock or",
                "lock inc",
                "lock dec",
            )
            and "gs:[" in ins.text
            and "[rip" not in ins.text
        ):
            hex_bytes = ", ".join(f"0x{b:02x}" for b in ins.raw_bytes)
            return f".byte {hex_bytes}"
        if len(ins.operands) == 1 and ins.operands[0].kind == "imm":
            target = ins.operands[0].imm
            if target in label_addrs:
                return f"{ins.mnemonic} .L_{fn_name}_{target:x}"
            if ins.mnemonic == "call" or ins.mnemonic.startswith(("j", "loop")):
                sym_n = vmap.get(target, f"__nt_asm_ext_{target:x}")
                return f"{ins.mnemonic} {sym_n}"
        norm = self._normalize_gas_text(ins.text)
        m_rip = re.search(r"\[rip(?:\s*([+-])\s*(0x[0-9a-fA-F]+|\d+))?\]", norm)
        if m_rip:
            disp = 0
            if m_rip.group(2) is not None:
                disp = int(m_rip.group(2), 0)
                if m_rip.group(1) == "-":
                    disp = -disp
            rip_tgt = (ins.address + ins.size + disp) & 0xFFFFFFFFFFFFFFFF
            if rip_tgt in label_addrs:
                sym_ref = f".L_{fn_name}_{rip_tgt:x}"
            else:
                sym_ref = vmap.get(rip_tgt, f"__nt_asm_sym_{rip_tgt:x}")
            norm = norm[: m_rip.start()] + f"[rip + {sym_ref}]" + norm[m_rip.end() :]
        elif any(
            op.kind == "mem" and op.base is None and op.index is None
            for op in ins.operands
        ):
            # Absolute displacement operand without RIP (e.g. ds:[0x...]) -> emit exact bytes
            hex_bytes = ", ".join(f"0x{b:02x}" for b in ins.raw_bytes)
            return f".byte {hex_bytes}"
        return norm
