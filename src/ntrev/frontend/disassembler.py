"""
Capstone-backed x86-64 Control-Flow Graph (CFG) disassembler for Windows PE32+ binaries.

Combines:
  1. .pdata RUNTIME_FUNCTION unwind bounds + UNW_FLAG_CHAININFO PGO cold chunks
  2. PDB public/procedure symbols (S_PUB32 / S_GPROC32) & /OPT:ICF alias support
  3. Accurate leaf-function terminator boundary discovery
  4. Recursive out-of-line MSVC PGO cold-block stitching & tailcall detection
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
RET_MNEMONICS = {
    "ret", "retq", "retn", "retf", "lretq",
    "iret", "iretd", "iretq", "sysret", "sysretq",
}
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
    "ldmxcsr", "stmxcsr",
}

SPECIAL_REGS = {
    "cr0", "cr2", "cr3", "cr4", "cr8",
    "dr0", "dr1", "dr2", "dr3", "dr6", "dr7",
}


def pdb_matches_image_symbols(image: BinaryImage, pdb: PDBDatabase | None) -> bool:
    """
    Check whether `pdb`'s RVA symbol table matches `image`.
    If both have non-zero GUIDs that differ, TPI structs are still usable for same-OS-build
    layout recovery, while RVAs must come from `image.exports` / `.pdata`.
    """
    if pdb is None:
        return False
    zero_guid = "00000000-0000-0000-0000-000000000000"
    if image.rsds and pdb.guid != zero_guid:
        return image.rsds.guid.upper() == pdb.guid.upper()
    return True


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
        use_pdb_rvas = pdb_matches_image_symbols(image, pdb)
        known_entry_vas = self._collect_known_function_entry_vas(image, pdb if use_pdb_rvas else None)
        pdata_by_begin = {rf.begin_rva: rf for rf in image.pdata_functions}

        if target_functions:
            return self._disassemble_targeted_functions(
                image=image,
                pdb=pdb if use_pdb_rvas else None,
                target_functions=target_functions,
                pdata_by_begin=pdata_by_begin,
                known_entry_vas=known_entry_vas,
            )

        bounds = self._discover_function_bounds(image, pdb if use_pdb_rvas else None)
        cfgs: list[FunctionCFG] = []

        for rva_start, rva_end, name, has_pdata, sec_name in bounds:
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
                image=image,
                known_entry_vas=known_entry_vas,
                pdata_by_begin=pdata_by_begin,
            )
            if cfg.blocks:
                cfgs.append(cfg)
        return cfgs

    def _collect_known_function_entry_vas(
        self, image: BinaryImage, pdb: PDBDatabase | None
    ) -> set[int]:
        vas: set[int] = set()
        base = image.image_base
        for rva in image.exports:
            vas.add(base + rva)
        for rf in image.pdata_functions:
            if rf.chained_parent_rva is None:
                vas.add(base + rf.begin_rva)
        if pdb is not None:
            for rva, sym in pdb.symbols_by_rva.items():
                if sym.is_function:
                    vas.add(base + rva)
        return vas

    def _disassemble_targeted_functions(
        self,
        image: BinaryImage,
        pdb: PDBDatabase | None,
        target_functions: list[str],
        pdata_by_begin: dict[int, object],
        known_entry_vas: set[int],
    ) -> list[FunctionCFG]:
        cfgs: list[FunctionCFG] = []
        exec_sections = [s for s in image.sections if s.is_executable]

        def section_for_rva(rva: int) -> tuple[str, int] | None:
            for sec in exec_sections:
                if sec.virtual_address <= rva < sec.virtual_address + len(sec.raw_data):
                    return sec.name, sec.virtual_address + len(sec.raw_data)
            return None

        for fn_name in target_functions:
            rva: int | None = None
            size_hint = 0
            if fn_name in image.export_by_name:
                rva = image.export_by_name[fn_name]
            elif pdb is not None and fn_name in pdb.symbols_by_name:
                sym = pdb.symbols_by_name[fn_name]
                if sym.is_function:
                    rva = sym.rva
                    size_hint = sym.size
            if rva is None:
                continue

            sec_info = section_for_rva(rva)
            if sec_info is None:
                continue
            sec_name, sec_end = sec_info

            rf = pdata_by_begin.get(rva)
            if rf is not None:
                end_rva = min(rf.end_rva, sec_end)  # type: ignore[attr-defined]
                has_pdata = True
            elif size_hint > 0:
                end_rva = min(rva + size_hint, sec_end)
                has_pdata = False
            else:
                end_rva = self._scan_leaf_function_end(image, rva, sec_end)
                has_pdata = False

            va_start = image.image_base + rva
            code_bytes = image.read_va(va_start, max(1, end_rva - rva))
            if not code_bytes:
                continue

            cfg = self.build_function_cfg(
                name=fn_name,
                code=code_bytes,
                base_va=va_start,
                has_pdata_unwind=has_pdata,
                section_name=sec_name,
                image=image,
                known_entry_vas=known_entry_vas,
                pdata_by_begin=pdata_by_begin,
            )
            if cfg.blocks:
                cfgs.append(cfg)
        return cfgs

    def _scan_leaf_function_end(self, image: BinaryImage, start_rva: int, sec_end: int) -> int:
        """Scan forward from a leaf function entry (without .pdata) until final reachable terminator."""
        va_start = image.image_base + start_rva
        max_scan = min(512, sec_end - start_rva)
        raw = image.read_va(va_start, max_scan)
        if not raw:
            return min(start_rva + 16, sec_end)

        furthest_target = va_start
        for cs_insn in self._md.disasm(raw, va_start):
            mnem = cs_insn.mnemonic.lower()
            end_va = int(cs_insn.address + cs_insn.size)
            if mnem in COND_JUMPS or mnem in UNCOND_JUMPS:
                if len(cs_insn.operands) == 1 and cs_insn.operands[0].type == x86.X86_OP_IMM:
                    tgt = int(cs_insn.operands[0].imm)
                    if va_start <= tgt < va_start + max_scan:
                        furthest_target = max(furthest_target, tgt)
            if (mnem in RET_MNEMONICS or mnem in UNCOND_JUMPS) and cs_insn.address >= furthest_target:
                return int(end_va - image.image_base)
            if mnem == "int3" and cs_insn.address > va_start and cs_insn.address >= furthest_target:
                return int(cs_insn.address - image.image_base)

        return min(start_rva + max_scan, sec_end)

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
            if rf.chained_parent_rva is not None:
                continue  # Out-of-line PGO chunk belonging to parent function
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
                    name_map.setdefault(rva, sym.name)
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
        image: BinaryImage | None = None,
        known_entry_vas: set[int] | None = None,
        pdata_by_begin: dict[int, object] | None = None,
    ) -> FunctionCFG:
        """
        Disassemble `code` starting at `base_va`, stitch separated MSVC PGO cold chunks
        when `image` is supplied, and construct a complete basic-block `FunctionCFG`.
        """
        primary_insns: list[DisasmInsn] = [
            self._convert_insn(cs_i) for cs_i in self._md.disasm(code, base_va)
        ]

        # Strip trailing alignment padding (int3 / nop after final terminator)
        while len(primary_insns) > 1 and primary_insns[-1].mnemonic in PADDING_MNEMONICS:
            prev = primary_insns[-2].mnemonic
            if prev in RET_MNEMONICS or prev in UNCOND_JUMPS or prev in PADDING_MNEMONICS:
                primary_insns.pop()
            else:
                break

        if not primary_insns:
            return FunctionCFG(
                name=name,
                address=base_va,
                size=len(code),
                entry_address=base_va,
                has_pdata_unwind=has_pdata_unwind,
                section_name=section_name,
            )

        primary_size = primary_insns[-1].end_address - base_va
        insn_by_addr: dict[int, DisasmInsn] = {ins.address: ins for ins in primary_insns}
        chunk_end_addrs: set[int] = {primary_insns[-1].end_address}
        entry_vas = known_entry_vas or set()

        # Discover separated MSVC PGO cold basic blocks reachable via conditional/unconditional jumps
        if image is not None:
            worklist: list[int] = []
            visited_chunks: set[int] = set()
            for ins in primary_insns:
                tgt = self._branch_target(ins)
                if tgt is not None and tgt not in insn_by_addr:
                    if ins.mnemonic in COND_JUMPS or (
                        ins.mnemonic in UNCOND_JUMPS and (tgt not in entry_vas or tgt == base_va)
                    ):
                        worklist.append(tgt)

            while worklist and len(visited_chunks) < 16:
                cold_va = worklist.pop(0)
                if cold_va in insn_by_addr or cold_va in visited_chunks:
                    continue
                sec = image.section_for_va(cold_va)
                if sec is None or not sec.is_executable:
                    continue
                visited_chunks.add(cold_va)

                cold_rva = cold_va - image.image_base
                sec_end_rva = sec.virtual_address + len(sec.raw_data)
                cold_end_rva = self._find_cold_chunk_end_rva(
                    image, cold_rva, sec_end_rva, pdata_by_begin or {}
                )
                cold_bytes = image.read_va(cold_va, max(1, cold_end_rva - cold_rva))
                if not cold_bytes:
                    continue

                cold_insns = [
                    self._convert_insn(cs_i) for cs_i in self._md.disasm(cold_bytes, cold_va)
                ]
                while len(cold_insns) > 1 and cold_insns[-1].mnemonic in PADDING_MNEMONICS:
                    prev = cold_insns[-2].mnemonic
                    if prev in RET_MNEMONICS or prev in UNCOND_JUMPS or prev in PADDING_MNEMONICS:
                        cold_insns.pop()
                    else:
                        break
                if not cold_insns:
                    continue

                for cins in cold_insns:
                    insn_by_addr.setdefault(cins.address, cins)
                chunk_end_addrs.add(cold_insns[-1].end_address)

                for cins in cold_insns:
                    ctgt = self._branch_target(cins)
                    if ctgt is not None and ctgt not in insn_by_addr:
                        if cins.mnemonic in COND_JUMPS or (
                            cins.mnemonic in UNCOND_JUMPS and ctgt not in entry_vas
                        ):
                            worklist.append(ctgt)

        raw_insns = [insn_by_addr[a] for a in sorted(insn_by_addr.keys())]
        valid_addrs = set(insn_by_addr.keys())
        leaders: set[int] = {primary_insns[0].address}

        # Every non-contiguous chunk start is also a basic-block leader
        for i in range(1, len(raw_insns)):
            if raw_insns[i].address != raw_insns[i - 1].end_address:
                leaders.add(raw_insns[i].address)

        for i, ins in enumerate(raw_insns):
            mnem = ins.mnemonic
            if mnem in UNCOND_JUMPS or mnem in COND_JUMPS:
                target = self._branch_target(ins)
                if target is not None and target in valid_addrs:
                    leaders.add(target)
                if i + 1 < len(raw_insns):
                    leaders.add(raw_insns[i + 1].address)
            elif mnem in RET_MNEMONICS:
                if i + 1 < len(raw_insns):
                    leaders.add(raw_insns[i + 1].address)

        blocks: dict[int, BasicBlock] = {}
        cur_block: BasicBlock | None = None
        for ins in raw_insns:
            if ins.address in leaders or cur_block is None:
                cur_block = BasicBlock(address=ins.address)
                blocks[ins.address] = cur_block
            cur_block.instructions.append(ins)

        # Populate control-flow edges (only allowing fallthrough across contiguous chunk bytes!)
        block_addrs = sorted(blocks.keys())
        for idx, b_addr in enumerate(block_addrs):
            blk = blocks[b_addr]
            if not blk.instructions:
                continue
            term = blk.instructions[-1]
            mnem = term.mnemonic
            next_blk_addr = block_addrs[idx + 1] if idx + 1 < len(block_addrs) else None
            can_fallthrough = (
                next_blk_addr is not None
                and next_blk_addr == term.end_address
                and term.end_address not in chunk_end_addrs
            )

            if mnem in RET_MNEMONICS:
                pass
            elif mnem in UNCOND_JUMPS:
                target = self._branch_target(term)
                if target is not None and target in blocks:
                    blk.branch_target = target
                    blk.successors.append(target)
                elif target is not None:
                    blk.tailcall_target = target
            elif mnem in COND_JUMPS:
                target = self._branch_target(term)
                if target is not None and target in blocks:
                    blk.branch_target = target
                    blk.successors.append(target)
                if can_fallthrough and next_blk_addr is not None:
                    blk.fallthrough = next_blk_addr
                    if next_blk_addr not in blk.successors:
                        blk.successors.append(next_blk_addr)
            else:
                if can_fallthrough and next_blk_addr is not None:
                    blk.fallthrough = next_blk_addr
                    blk.successors.append(next_blk_addr)

        for b_addr, blk in blocks.items():
            for succ in blk.successors:
                if succ in blocks and b_addr not in blocks[succ].predecessors:
                    blocks[succ].predecessors.append(b_addr)

        return FunctionCFG(
            name=name,
            address=base_va,
            size=primary_size,
            entry_address=primary_insns[0].address,
            blocks=blocks,
            has_pdata_unwind=has_pdata_unwind,
            section_name=section_name,
        )

    def _find_cold_chunk_end_rva(
        self,
        image: BinaryImage,
        cold_rva: int,
        sec_end_rva: int,
        pdata_by_begin: dict[int, object],
    ) -> int:
        # Check if cold_rva lies inside an explicit .pdata chunk
        rf = pdata_by_begin.get(cold_rva)
        if rf is not None:
            return min(rf.end_rva, sec_end_rva)  # type: ignore[attr-defined]
        for entry in image.pdata_functions:
            if entry.begin_rva <= cold_rva < entry.end_rva:
                # Scan within this .pdata chunk up to its first unconditional terminator from cold_rva
                return self._scan_leaf_function_end(
                    image, cold_rva, min(entry.end_rva, sec_end_rva)
                )
        return self._scan_leaf_function_end(image, cold_rva, min(cold_rva + 256, sec_end_rva))

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
