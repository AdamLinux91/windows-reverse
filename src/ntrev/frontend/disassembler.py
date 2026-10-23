"""
Capstone-backed x86-64 Control-Flow Graph (CFG) disassembler for Windows PE32+ binaries.

Combines:
  1. .pdata RUNTIME_FUNCTION unwind bounds + UNW_FLAG_CHAININFO PGO cold chunks
  2. PDB public/procedure symbols (S_PUB32 / S_GPROC32) & /OPT:ICF alias support
  3. Accurate leaf-function terminator boundary discovery
  4. Recursive out-of-line MSVC PGO cold-block stitching, switch jump-table recovery & tailcall detection
"""

from __future__ import annotations

import bisect
import struct

import capstone
from capstone import x86

from ntrev.models import (
    BasicBlock,
    BinaryImage,
    DisasmInsn,
    FunctionCFG,
    InsnOperand,
    JumpTableInfo,
    PDBDatabase,
    RuntimeFunctionEntry,
)

UNCOND_JUMPS = {"jmp"}
COND_JUMPS = {
    "je", "jz", "jne", "jnz",
    "jl", "jnge", "jge", "jnl", "jle", "jng", "jg", "jnle",
    "jb", "jc", "jnae", "jae", "jnb", "jnc", "jbe", "jna", "ja", "jnbe",
    "js", "jns", "jo", "jno", "jp", "jpe", "jnp", "jpo",
    "jrcxz", "jecxz",
    "loop", "loope", "loopz", "loopne", "loopnz",
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

_GPR_CANON = {
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


class _PdataIndex:
    """Fast O(log N) lookup index over sorted .pdata RUNTIME_FUNCTION entries."""

    def __init__(self, entries: list[RuntimeFunctionEntry]) -> None:
        sorted_entries = sorted(entries, key=lambda e: e.begin_rva)
        self.begins = [e.begin_rva for e in sorted_entries]
        self.entries = sorted_entries
        self.by_begin: dict[int, RuntimeFunctionEntry] = {e.begin_rva: e for e in sorted_entries}
        self.chained_by_parent: dict[int, list[RuntimeFunctionEntry]] = {}
        for e in sorted_entries:
            if e.chained_parent_rva is not None:
                self.chained_by_parent.setdefault(e.chained_parent_rva, []).append(e)

    def containing(self, rva: int) -> RuntimeFunctionEntry | None:
        idx = bisect.bisect_right(self.begins, rva) - 1
        if 0 <= idx < len(self.entries):
            e = self.entries[idx]
            if e.begin_rva <= rva < e.end_rva:
                return e
        return None


class X86Disassembler:
    """Disassembles PE32+ executable sections into FunctionCFG objects."""

    def __init__(self) -> None:
        self._md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_64)
        self._md.detail = True

    def iter_disassemble_image(
        self,
        image: BinaryImage,
        pdb: PDBDatabase | None = None,
    ):
        """Stream `FunctionCFG` objects one at a time across the entire binary with minimal RAM."""
        use_pdb_rvas = pdb_matches_image_symbols(image, pdb)
        known_entry_vas = self._collect_known_function_entry_vas(image, pdb if use_pdb_rvas else None)
        pdata_idx = _PdataIndex(image.pdata_functions)
        bounds = self._discover_function_bounds(image, pdb if use_pdb_rvas else None)

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
                pdata_by_begin=pdata_idx.by_begin,
                pdata_index=pdata_idx,
            )
            if cfg.blocks:
                yield cfg

    def disassemble_image(
        self,
        image: BinaryImage,
        pdb: PDBDatabase | None = None,
        target_functions: list[str] | None = None,
    ) -> list[FunctionCFG]:
        """Discover and build CFGs for all (or selected) functions in `image`."""
        if target_functions:
            use_pdb_rvas = pdb_matches_image_symbols(image, pdb)
            known_entry_vas = self._collect_known_function_entry_vas(image, pdb if use_pdb_rvas else None)
            pdata_idx = _PdataIndex(image.pdata_functions)
            return self._disassemble_targeted_functions(
                image=image,
                pdb=pdb if use_pdb_rvas else None,
                target_functions=target_functions,
                pdata_idx=pdata_idx,
                known_entry_vas=known_entry_vas,
            )
        return list(self.iter_disassemble_image(image=image, pdb=pdb))

    def _collect_known_function_entry_vas(
        self, image: BinaryImage, pdb: PDBDatabase | None
    ) -> set[int]:
        vas: set[int] = set()
        base = image.image_base
        exec_sections = [s for s in image.sections if s.is_executable]

        def in_exec(rva: int) -> bool:
            return any(
                s.virtual_address <= rva < s.virtual_address + len(s.raw_data)
                for s in exec_sections
            )

        for rva in image.exports:
            if in_exec(rva):
                vas.add(base + rva)
        for rf in image.pdata_functions:
            if rf.chained_parent_rva is None and in_exec(rf.begin_rva):
                vas.add(base + rf.begin_rva)
        if pdb is not None:
            for rva, sym in pdb.symbols_by_rva.items():
                if sym.is_function and in_exec(rva):
                    vas.add(base + rva)
        return vas

    def _disassemble_targeted_functions(
        self,
        image: BinaryImage,
        pdb: PDBDatabase | None,
        target_functions: list[str],
        pdata_idx: _PdataIndex,
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

            rf = pdata_idx.by_begin.get(rva)
            if rf is not None:
                end_rva = min(rf.end_rva, sec_end)
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
                pdata_by_begin=pdata_idx.by_begin,
                pdata_index=pdata_idx,
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
        chained_starts: set[int] = set()
        for rf in image.pdata_functions:
            if rf.chained_parent_rva is not None:
                chained_starts.add(rf.begin_rva)
                continue  # Out-of-line PGO chunk belonging to parent function
            if section_for_rva(rf.begin_rva) is not None:
                pdata_map[rf.begin_rva] = rf.end_rva

        name_map: dict[int, str] = {}
        size_hints: dict[int, int] = {}

        if pdb is not None:
            for rva, sym in pdb.symbols_by_rva.items():
                if rva in chained_starts:
                    continue
                if section_for_rva(rva) is not None and sym.is_function:
                    name_map.setdefault(rva, sym.name)
                    if sym.size > 0:
                        size_hints[rva] = sym.size

        for rva, exp_name in image.exports.items():
            if rva in chained_starts:
                continue
            if section_for_rva(rva) is not None:
                # Only treat export RVA as code if backed by .pdata, PDB code symbol, or non-PDB binary
                if pdb is None or not pdb.symbols_by_rva or rva in pdata_map or rva in name_map:
                    name_map[rva] = exp_name  # Prefer official exported name on exact RVA

        all_starts = sorted(set(pdata_map.keys()) | set(name_map.keys()))
        if not all_starts and image.entry_point_rva:
            if section_for_rva(image.entry_point_rva) is not None:
                all_starts = [image.entry_point_rva]
                name_map[image.entry_point_rva] = "KiSystemStartup"

        # Deduplicate any colliding function names across distinct RVAs
        used_names: set[str] = set()
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
                end_rva = min(
                    self._scan_leaf_function_end(image, start_rva, bound_limit),
                    bound_limit,
                )
                has_pdata = False

            if end_rva <= start_rva:
                continue
            fn_name = name_map.get(start_rva, f"sub_{image.image_base + start_rva:x}")
            if fn_name in used_names:
                fn_name = f"{fn_name}_{start_rva:x}"
            used_names.add(fn_name)
            results.append((start_rva, end_rva, fn_name, has_pdata, sec_name))

        return results

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
        pdata_index: _PdataIndex | None = None,
    ) -> FunctionCFG:
        """
        Disassemble `code` starting at `base_va`, stitch separated MSVC PGO cold chunks
        and RVA jump tables when `image` is supplied, and build a complete `FunctionCFG`.
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
        jump_tables: dict[int, JumpTableInfo] = {}

        if image is not None:
            pidx = pdata_index
            worklist: list[int] = []
            visited_chunks: set[int] = set()

            # Pre-seed explicit .pdata UNW_FLAG_CHAININFO cold chunks belonging to this function
            start_rva = base_va - image.image_base
            if pidx is not None and start_rva in pidx.chained_by_parent:
                for crf in pidx.chained_by_parent[start_rva]:
                    worklist.append(image.image_base + crf.begin_rva)

            def enqueue_branch_targets(ins_seq: list[DisasmInsn]) -> None:
                has_reg_jmp = False
                for ins in ins_seq:
                    if ins.mnemonic == "jmp" and ins.operands and ins.operands[0].kind == "reg":
                        has_reg_jmp = True
                    tgt = self._branch_target(ins)
                    if tgt is not None and tgt not in insn_by_addr:
                        if tgt not in entry_vas or tgt == base_va:
                            if ins.mnemonic in COND_JUMPS or ins.mnemonic in UNCOND_JUMPS:
                                worklist.append(tgt)
                if has_reg_jmp:
                    all_cur = [insn_by_addr[a] for a in sorted(insn_by_addr.keys())]
                    for jt in self._recover_jump_tables(all_cur, image, base_va, entry_vas):
                        if jt.jmp_address not in jump_tables:
                            jump_tables[jt.jmp_address] = jt
                            for case_va in jt.targets:
                                if case_va not in insn_by_addr and (case_va not in entry_vas or case_va == base_va):
                                    worklist.append(case_va)

            enqueue_branch_targets(primary_insns)

            while worklist and len(visited_chunks) < 256:
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
                    image, cold_rva, sec_end_rva, pdata_by_begin or {}, pidx
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
                enqueue_branch_targets(cold_insns)

        # Strip any inline `.text` jump-table data bytes that fell inside `.pdata` bounds
        if jump_tables:
            jt_ranges = [rng for jt in jump_tables.values() for rng in jt.data_ranges]
            if jt_ranges:
                to_drop = [
                    addr
                    for addr, ins in insn_by_addr.items()
                    if any(ins.address < r_end and ins.end_address > r_start for r_start, r_end in jt_ranges)
                ]
                for addr in to_drop:
                    if addr != primary_insns[0].address:
                        insn_by_addr.pop(addr, None)

        raw_insns = [insn_by_addr[a] for a in sorted(insn_by_addr.keys())]
        valid_addrs = set(insn_by_addr.keys())
        leaders: set[int] = {primary_insns[0].address}

        # Every non-contiguous chunk start is also a basic-block leader
        for i in range(1, len(raw_insns)):
            if raw_insns[i].address != raw_insns[i - 1].end_address:
                leaders.add(raw_insns[i].address)

        for jt in jump_tables.values():
            for t_va in jt.targets:
                if t_va in valid_addrs:
                    leaders.add(t_va)

        for i, ins in enumerate(raw_insns):
            mnem = ins.mnemonic
            if mnem in UNCOND_JUMPS or mnem in COND_JUMPS:
                target = self._branch_target(ins)
                if target is not None and target in valid_addrs:
                    leaders.add(target)
                if i + 1 < len(raw_insns):
                    leaders.add(raw_insns[i + 1].address)
            elif mnem in RET_MNEMONICS or mnem == "int3":
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

            if mnem in RET_MNEMONICS or mnem == "int3":
                pass
            elif mnem in UNCOND_JUMPS:
                if term.address in jump_tables:
                    jt = jump_tables[term.address]
                    for case_va in jt.targets:
                        if case_va in blocks and case_va not in blk.successors:
                            blk.successors.append(case_va)
                else:
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
                elif target is not None and target in entry_vas and target != base_va:
                    blk.cond_tailcall_target = target
                if can_fallthrough and next_blk_addr is not None:
                    blk.fallthrough = next_blk_addr
                    if next_blk_addr not in blk.successors:
                        blk.successors.append(next_blk_addr)
            else:
                if can_fallthrough and next_blk_addr is not None:
                    blk.fallthrough = next_blk_addr
                    blk.successors.append(next_blk_addr)

        # Prune unreachable alignment-padding / inline-table blocks that follow `int3`/`ret`/`jmp`
        entry_va = primary_insns[0].address
        if len(blocks) > 1 and entry_va in blocks:
            reachable: set[int] = {entry_va}
            q = [entry_va]
            while q:
                curr = q.pop()
                cb = blocks.get(curr)
                if cb is None:
                    continue
                for s in cb.successors:
                    if s in blocks and s not in reachable:
                        reachable.add(s)
                        q.append(s)
            referenced_targets = {
                t
                for b in blocks.values()
                for t in (b.fallthrough, b.branch_target)
                if t is not None
            }
            for b_addr in list(blocks.keys()):
                if b_addr not in reachable and b_addr not in referenced_targets:
                    ub = blocks[b_addr]
                    if all(i.mnemonic in PADDING_MNEMONICS for i in ub.instructions) or any(
                        i.mnemonic in ("hlt", "retf", "lret", "iret", "iretd", "in", "out", "insb", "outsb")
                        for i in ub.instructions
                    ):
                        del blocks[b_addr]

        for b_addr, blk in blocks.items():
            if blk.fallthrough is not None and blk.fallthrough not in blocks:
                blk.fallthrough = None
            if blk.branch_target is not None and blk.branch_target not in blocks:
                blk.branch_target = None
            blk.successors = [s for s in blk.successors if s in blocks]
            for succ in blk.successors:
                if b_addr not in blocks[succ].predecessors:
                    blocks[succ].predecessors.append(b_addr)

        return FunctionCFG(
            name=name,
            address=base_va,
            size=primary_size,
            entry_address=primary_insns[0].address,
            blocks=blocks,
            has_pdata_unwind=has_pdata_unwind,
            section_name=section_name,
            jump_tables=jump_tables,
        )

    def _recover_jump_tables(
        self,
        insns: list[DisasmInsn],
        image: BinaryImage,
        base_va: int,
        entry_vas: set[int],
    ) -> list[JumpTableInfo]:
        """
        Decode MSVC x64 RVA-based `switch` jump tables:
          1-level: mov reg32, [img_base + idx*4 + tbl_rva]; add reg64, img_base; jmp reg64
          2-level: movzx idx2, byte ptr [img_base + idx + btbl_rva];
                   mov reg32, [img_base + idx2*4 + tbl_rva]; add reg64, img_base; jmp reg64
        """
        recovered: list[JumpTableInfo] = []
        img_base = image.image_base

        for i, ins in enumerate(insns):
            if ins.mnemonic != "jmp" or len(ins.operands) != 1 or ins.operands[0].kind != "reg":
                continue
            jmp_reg = _GPR_CANON.get(ins.operands[0].reg or "")
            if not jmp_reg or i < 2:
                continue

            add_ins = insns[i - 1]
            mov_ins = insns[i - 2]
            if (
                add_ins.mnemonic != "add"
                or len(add_ins.operands) != 2
                or add_ins.operands[0].kind != "reg"
                or _GPR_CANON.get(add_ins.operands[0].reg or "") != jmp_reg
                or add_ins.operands[1].kind != "reg"
            ):
                continue
            base_reg = _GPR_CANON.get(add_ins.operands[1].reg or "")
            if not base_reg:
                continue

            if (
                mov_ins.mnemonic not in ("mov", "movsxd")
                or len(mov_ins.operands) != 2
                or mov_ins.operands[0].kind != "reg"
                or _GPR_CANON.get(mov_ins.operands[0].reg or "") != jmp_reg
                or mov_ins.operands[1].kind != "mem"
            ):
                continue

            mem = mov_ins.operands[1]
            if (
                _GPR_CANON.get(mem.base or "") != base_reg
                or mem.scale != 4
                or mem.disp <= 0
                or not mem.index
            ):
                continue

            tbl_rva = mem.disp
            idx_reg_raw = mem.index
            idx_canon = _GPR_CANON.get(idx_reg_raw, "")
            byte_tbl_rva: int | None = None
            setup_addrs = [mov_ins.address, add_ins.address]
            search_from = i - 3

            # Check optional 2-level byte index table right before mov_ins
            if i >= 3:
                b_ins = insns[i - 3]
                if (
                    b_ins.mnemonic == "movzx"
                    and len(b_ins.operands) == 2
                    and b_ins.operands[0].kind == "reg"
                    and _GPR_CANON.get(b_ins.operands[0].reg or "") == idx_canon
                    and b_ins.operands[1].kind == "mem"
                    and b_ins.operands[1].size == 1
                    and _GPR_CANON.get(b_ins.operands[1].base or "") == base_reg
                    and b_ins.operands[1].scale == 1
                    and b_ins.operands[1].disp > 0
                    and b_ins.operands[1].index
                ):
                    byte_tbl_rva = b_ins.operands[1].disp
                    idx_reg_raw = b_ins.operands[1].index
                    idx_canon = _GPR_CANON.get(idx_reg_raw, "")
                    setup_addrs.insert(0, b_ins.address)
                    search_from = i - 4

            # Walk backwards up to 16 instructions to trace `cdqe`/`movsxd`/`movzx` index copies
            # and find the dominating `cmp idx, MAX_CASE` + `ja/jae/jbe` or `and idx, MASK`!
            orig_idx_reg = idx_reg_raw
            orig_idx_width = 64 if _GPR_CANON.get(orig_idx_reg) == orig_idx_reg else 32
            tracked_canon = {idx_canon}
            num_cases: int | None = None
            default_va: int | None = None

            for k in range(search_from, max(-1, search_from - 18), -1):
                prev = insns[k]
                pm = prev.mnemonic
                if pm in ("cdqe", "cwde") and "rax" in tracked_canon:
                    continue
                if (
                    pm in ("movsxd", "movzx", "mov")
                    and len(prev.operands) == 2
                    and prev.operands[0].kind == "reg"
                    and prev.operands[1].kind == "reg"
                    and _GPR_CANON.get(prev.operands[0].reg or "") in tracked_canon
                ):
                    src_r = prev.operands[1].reg or ""
                    src_c = _GPR_CANON.get(src_r, "")
                    if src_c and src_c != base_reg:
                        tracked_canon.add(src_c)
                    continue
                if (
                    pm == "and"
                    and len(prev.operands) == 2
                    and prev.operands[0].kind == "reg"
                    and _GPR_CANON.get(prev.operands[0].reg or "") in tracked_canon
                    and prev.operands[1].kind == "imm"
                ):
                    mask = prev.operands[1].imm
                    if mask in (1, 3, 7, 15, 31, 63, 127, 255):
                        num_cases = mask + 1
                        break
                if (
                    pm == "cmp"
                    and len(prev.operands) == 2
                    and prev.operands[0].kind == "reg"
                    and _GPR_CANON.get(prev.operands[0].reg or "") in tracked_canon
                    and prev.operands[1].kind == "imm"
                ):
                    bound_imm = prev.operands[1].imm & 0xFFFFFFFF
                    if 0 < bound_imm <= 1024 and k + 1 < len(insns):
                        j_ins = insns[k + 1]
                        jm = j_ins.mnemonic
                        if jm in ("ja", "jnbe"):
                            num_cases = bound_imm + 1
                            default_va = self._branch_target(j_ins)
                            break
                        if jm in ("jae", "jnb", "jnc"):
                            num_cases = bound_imm
                            default_va = self._branch_target(j_ins)
                            break
                        if jm in ("jbe", "jna"):
                            num_cases = bound_imm + 1
                            break
                        if jm in ("jb", "jnae", "jc"):
                            num_cases = bound_imm
                            break

            if num_cases is None or num_cases <= 0 or num_cases > 1024:
                continue

            targets: list[int] = []
            data_ranges: list[tuple[int, int]] = []
            valid_table = True
            if byte_tbl_rva is not None:
                b_raw = image.read_va(img_base + byte_tbl_rva, num_cases)
                if not b_raw or len(b_raw) < num_cases:
                    continue
                max_slot = max(b_raw)
                data_ranges.append((img_base + byte_tbl_rva, img_base + byte_tbl_rva + num_cases))
                data_ranges.append((img_base + tbl_rva, img_base + tbl_rva + (max_slot + 1) * 4))
                for c_idx in range(num_cases):
                    slot = b_raw[c_idx]
                    ent_raw = image.read_va(img_base + tbl_rva + slot * 4, 4)
                    if not ent_raw or len(ent_raw) < 4:
                        valid_table = False
                        break
                    rva_val = struct.unpack("<I", ent_raw)[0]
                    t_va = img_base + rva_val
                    sec = image.section_for_va(t_va)
                    if sec is None or not sec.is_executable or (t_va in entry_vas and t_va != base_va):
                        valid_table = False
                        break
                    targets.append(t_va)
            else:
                t_raw = image.read_va(img_base + tbl_rva, num_cases * 4)
                if not t_raw or len(t_raw) < num_cases * 4:
                    continue
                data_ranges.append((img_base + tbl_rva, img_base + tbl_rva + num_cases * 4))
                for c_idx in range(num_cases):
                    rva_val = struct.unpack_from("<I", t_raw, c_idx * 4)[0]
                    t_va = img_base + rva_val
                    sec = image.section_for_va(t_va)
                    if sec is None or not sec.is_executable or (t_va in entry_vas and t_va != base_va):
                        valid_table = False
                        break
                    targets.append(t_va)

            if valid_table and targets:
                recovered.append(
                    JumpTableInfo(
                        jmp_address=ins.address,
                        index_reg=orig_idx_reg,
                        index_width=orig_idx_width,
                        targets=targets,
                        default_target=default_va,
                        setup_insn_addrs=setup_addrs,
                        data_ranges=data_ranges,
                    )
                )

        return recovered

    def _find_cold_chunk_end_rva(
        self,
        image: BinaryImage,
        cold_rva: int,
        sec_end_rva: int,
        pdata_by_begin: dict[int, object],
        pdata_index: _PdataIndex | None = None,
    ) -> int:
        rf = pdata_by_begin.get(cold_rva)
        if rf is not None:
            return min(rf.end_rva, sec_end_rva)  # type: ignore[attr-defined]
        if pdata_index is not None:
            containing = pdata_index.containing(cold_rva)
            if containing is not None:
                return self._scan_leaf_function_end(
                    image, cold_rva, min(containing.end_rva, sec_end_rva)
                )
        else:
            for entry in image.pdata_functions:
                if entry.begin_rva <= cold_rva < entry.end_rva:
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
