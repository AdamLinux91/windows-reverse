"""
Capstone-backed x86-64 Control-Flow Graph (CFG) disassembler for Windows PE32+ binaries.

Combines:
  1. .pdata RUNTIME_FUNCTION unwind bounds (authoritative on Windows x64)
  2. PDB public/procedure symbols (S_PUB32 / S_GPROC32)
  3. PE export table & direct CALL target discovery
  4. Basic-block leader splitting & branch edge linking
"""

from __future__ import annotations

import capstone
from capstone import x86

from ntrev.models import (
    BasicBlock,
    BinaryImage,
    DisasmInsn,
    FunctionCFG,
    InsnOperand,
    PDBDatabase,
)

UNCOND_JUMPS = {"jmp"}
COND_JUMPS = {
    "je", "jz", "jne", "jnz",
    "jl", "jnge", "jge", "jnl", "jle", "jng", "jg", "jnle",
    "jb", "jc", "jnae", "jae", "jnb", "jnc", "jbe", "jna", "ja", "jnbe",
    "js", "jns", "jo", "jno", "jp", "jpe", "jnp", "jpo",
    "jrcxz", "jecxz",
}
RET_MNEMONICS = {"ret", "retq", "retn", "retf", "lretq", "iret", "iretd", "iretq", "sysret", "sysretq"}
PADDING_MNEMONICS = {"int3", "nop"}

PRIVILEGED_MNEMONICS = {
    "swapgs", "sysret", "sysretq", "syscall", "sysenter", "sysexit",
    "iret", "iretd", "iretq",
    "rdmsr", "wrmsr", "rdpmc",
    "lgdt", "sgdt", "lidt", "sidt", "lldt", "sldt", "ltr", "str",
    "invlpg", "invpcid", "wbinvd", "invd",
    "cli", "sti", "hlt", "clts", "lmsw", "smsw",
    "in", "out", "insb", "insw", "insd", "outsb", "outsw", "outsd",
    "fxsave", "fxsave64", "fxrstor", "fxrstor64",
    "xsave", "xsave64", "xsaveopt", "xrstor", "xrstor64", "xsetbv",
}

SPECIAL_REGS = {
    "cr0", "cr2", "cr3", "cr4", "cr8",
    "dr0", "dr1", "dr2", "dr3", "dr6", "dr7",
}


class X86Disassembler:
    """Disassembles PE32+ executable sections into FunctionCFG objects."""

    def __init__(self) -> None:
        self._md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_64)
        self._md.detail = True

    def disassemble_image(
        self,
        image: BinaryImage,
        pdb: PDBDatabase | None = None,
        target_functions: list[str] | None = None,
    ) -> list[FunctionCFG]:
        """Discover and build CFGs for all (or selected) functions in `image`."""
        bounds = self._discover_function_bounds(image, pdb)
        cfgs: list[FunctionCFG] = []

        for rva_start, rva_end, name, has_pdata, sec_name in bounds:
            if target_functions and name not in target_functions:
                continue
            va_start = image.image_base + rva_start
            size = rva_end - rva_start
            code_bytes = image.read_va(va_start, size)
            if not code_bytes:
                continue
            cfg = self.build_function_cfg(
                name=name,
                code=code_bytes,
                base_va=va_start,
                has_pdata_unwind=has_pdata,
                section_name=sec_name,
            )
            if cfg.blocks:
                cfgs.append(cfg)
        return cfgs

    def _discover_function_bounds(
        self,
        image: BinaryImage,
        pdb: PDBDatabase | None,
    ) -> list[tuple[int, int, str, bool, str]]:
        """Merge .pdata bounds, PDB symbols, and exports into non-overlapping [start, end) RVAs."""
        exec_sections = [s for s in image.sections if s.is_executable]
        if not exec_sections:
            return []

        def section_for_rva(rva: int) -> tuple[str, int] | None:
            for sec in exec_sections:
                if sec.virtual_address <= rva < sec.virtual_address + len(sec.raw_data):
                    return sec.name, sec.virtual_address + len(sec.raw_data)
            return None

        pdata_map: dict[int, int] = {}
        for rf in image.pdata_functions:
            if section_for_rva(rf.begin_rva) is not None:
                pdata_map[rf.begin_rva] = rf.end_rva

        name_map: dict[int, str] = {}
        size_hints: dict[int, int] = {}

        for rva, exp_name in image.exports.items():
            if section_for_rva(rva) is not None:
                name_map[rva] = exp_name

        if pdb is not None:
            for rva, sym in pdb.symbols_by_rva.items():
                if section_for_rva(rva) is not None and sym.is_function:
                    name_map[rva] = sym.name
                    if sym.size > 0:
                        size_hints[rva] = sym.size

        all_starts = sorted(set(pdata_map.keys()) | set(name_map.keys()))
        if not all_starts and image.entry_point_rva:
            if section_for_rva(image.entry_point_rva) is not None:
                all_starts = [image.entry_point_rva]
                name_map[image.entry_point_rva] = "KiSystemStartup"

        results: list[tuple[int, int, str, bool, str]] = []
        for idx, start_rva in enumerate(all_starts):
            sec_info = section_for_rva(start_rva)
            if sec_info is None:
                continue
            sec_name, sec_end = sec_info
            next_start = all_starts[idx + 1] if idx + 1 < len(all_starts) else sec_end
            bound_limit = min(next_start, sec_end)

            if start_rva in pdata_map:
                end_rva = min(pdata_map[start_rva], bound_limit)
                has_pdata = True
            elif start_rva in size_hints:
                end_rva = min(start_rva + size_hints[start_rva], bound_limit)
                has_pdata = False
            else:
                end_rva = self._trim_trailing_padding(image, start_rva, bound_limit)
                has_pdata = False

            if end_rva <= start_rva:
                continue
            fn_name = name_map.get(start_rva, f"sub_{image.image_base + start_rva:x}")
            results.append((start_rva, end_rva, fn_name, has_pdata, sec_name))

        return results

    def _trim_trailing_padding(self, image: BinaryImage, start_rva: int, end_rva: int) -> int:
        """Trim trailing 0xCC (int3) or 0x90 alignment padding between functions."""
        va = image.image_base + start_rva
        raw = image.read_va(va, end_rva - start_rva)
        if not raw:
            return end_rva
        trimmed_len = len(raw)
        while trimmed_len > 1 and raw[trimmed_len - 1] in (0xCC, 0x00):
            trimmed_len -= 1
        # Also trim multi-byte alignment NOPs right after a RET
        insns = list(self._md.disasm(raw[:trimmed_len], va))
        last_real_end = start_rva + trimmed_len
        for idx in range(len(insns) - 1, -1, -1):
            mnem = insns[idx].mnemonic.lower()
            if mnem in PADDING_MNEMONICS:
                last_real_end = int(insns[idx].address - image.image_base)
            else:
                break
        return max(last_real_end, start_rva + 1)

    def build_function_cfg(
        self,
        name: str,
        code: bytes,
        base_va: int,
        has_pdata_unwind: bool = False,
        section_name: str = ".text",
    ) -> FunctionCFG:
        """Disassemble `code` starting at `base_va` and construct a basic-block CFG."""
        end_va = base_va + len(code)
        raw_insns: list[DisasmInsn] = []

        for cs_insn in self._md.disasm(code, base_va):
            d_insn = self._convert_insn(cs_insn)
            raw_insns.append(d_insn)

        # Strip trailing alignment padding (int3 / nop after final terminator)
        while len(raw_insns) > 1 and raw_insns[-1].mnemonic in PADDING_MNEMONICS:
            prev = raw_insns[-2].mnemonic
            if prev in RET_MNEMONICS or prev in UNCOND_JUMPS or prev in PADDING_MNEMONICS:
                raw_insns.pop()
            else:
                break

        if not raw_insns:
            return FunctionCFG(
                name=name,
                address=base_va,
                size=len(code),
                entry_address=base_va,
                has_pdata_unwind=has_pdata_unwind,
                section_name=section_name,
            )

        valid_addrs = {ins.address for ins in raw_insns}
        leaders: set[int] = {raw_insns[0].address}

        for i, ins in enumerate(raw_insns):
            mnem = ins.mnemonic
            if mnem in UNCOND_JUMPS or mnem in COND_JUMPS:
                target = self._branch_target(ins)
                if target is not None and base_va <= target < end_va and target in valid_addrs:
                    leaders.add(target)
                if i + 1 < len(raw_insns):
                    leaders.add(raw_insns[i + 1].address)
            elif mnem in RET_MNEMONICS:
                if i + 1 < len(raw_insns):
                    leaders.add(raw_insns[i + 1].address)

        sorted_leaders = sorted(leaders)
        leader_set = set(sorted_leaders)

        blocks: dict[int, BasicBlock] = {}
        cur_block: BasicBlock | None = None
        for ins in raw_insns:
            if ins.address in leader_set or cur_block is None:
                cur_block = BasicBlock(address=ins.address)
                blocks[ins.address] = cur_block
            cur_block.instructions.append(ins)

        # Populate control-flow edges
        block_addrs = sorted(blocks.keys())
        for idx, b_addr in enumerate(block_addrs):
            blk = blocks[b_addr]
            if not blk.instructions:
                continue
            term = blk.instructions[-1]
            mnem = term.mnemonic
            next_blk_addr = block_addrs[idx + 1] if idx + 1 < len(block_addrs) else None

            if mnem in RET_MNEMONICS:
                pass
            elif mnem in UNCOND_JUMPS:
                target = self._branch_target(term)
                if target is not None and target in blocks:
                    blk.branch_target = target
                    blk.successors.append(target)
            elif mnem in COND_JUMPS:
                target = self._branch_target(term)
                if target is not None and target in blocks:
                    blk.branch_target = target
                    blk.successors.append(target)
                if next_blk_addr is not None and next_blk_addr == term.end_address:
                    blk.fallthrough = next_blk_addr
                    if next_blk_addr not in blk.successors:
                        blk.successors.append(next_blk_addr)
            else:
                if next_blk_addr is not None and next_blk_addr == term.end_address:
                    blk.fallthrough = next_blk_addr
                    blk.successors.append(next_blk_addr)

        for b_addr, blk in blocks.items():
            for succ in blk.successors:
                if succ in blocks and b_addr not in blocks[succ].predecessors:
                    blocks[succ].predecessors.append(b_addr)

        actual_size = raw_insns[-1].end_address - base_va
        return FunctionCFG(
            name=name,
            address=base_va,
            size=actual_size,
            entry_address=raw_insns[0].address,
            blocks=blocks,
            has_pdata_unwind=has_pdata_unwind,
            section_name=section_name,
        )

    def _branch_target(self, ins: DisasmInsn) -> int | None:
        if len(ins.operands) == 1 and ins.operands[0].kind == "imm":
            return ins.operands[0].imm
        return None

    def _convert_insn(self, cs_insn: capstone.CsInsn) -> DisasmInsn:
        mnem = cs_insn.mnemonic.lower()
        ops: list[InsnOperand] = []
        has_special_reg = False

        for op in cs_insn.operands:
            if op.type == x86.X86_OP_REG:
                rname = cs_insn.reg_name(op.reg).lower()
                if rname in SPECIAL_REGS:
                    has_special_reg = True
                ops.append(InsnOperand(kind="reg", size=op.size, reg=rname))
            elif op.type == x86.X86_OP_IMM:
                ops.append(InsnOperand(kind="imm", size=op.size, imm=int(op.imm)))
            elif op.type == x86.X86_OP_MEM:
                base = cs_insn.reg_name(op.mem.base).lower() if op.mem.base != 0 else None
                index = cs_insn.reg_name(op.mem.index).lower() if op.mem.index != 0 else None
                seg = cs_insn.reg_name(op.mem.segment).lower() if op.mem.segment != 0 else None
                ops.append(
                    InsnOperand(
                        kind="mem",
                        size=op.size,
                        base=base,
                        index=index,
                        scale=int(op.mem.scale),
                        disp=int(op.mem.disp),
                        segment=seg,
                    )
                )

        regs_r_ids, regs_w_ids = cs_insn.regs_access()
        regs_read = [cs_insn.reg_name(r).lower() for r in regs_r_ids]
        regs_write = [cs_insn.reg_name(r).lower() for r in regs_w_ids]
        groups = [cs_insn.group_name(g) for g in cs_insn.groups]

        is_priv = mnem in PRIVILEGED_MNEMONICS or has_special_reg
        return DisasmInsn(
            address=int(cs_insn.address),
            size=int(cs_insn.size),
            raw_bytes=bytes(cs_insn.bytes),
            mnemonic=mnem,
            op_str=cs_insn.op_str,
            operands=ops,
            regs_read=regs_read,
            regs_write=regs_write,
            groups=groups,
            is_privileged=is_priv,
        )
