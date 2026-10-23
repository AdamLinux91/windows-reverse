"""
Compiler De-Optimizer: Loop Reroller & Strength-Reduction Recovery Engine.

Handles heavy compiler optimizations found in production Windows NT binaries:
  1. Multi-Branch Unrolled Loop Collapse:
     Recognizes when a compiler unrolled a small/bounded loop into N distinct conditional
     branches, binary-search `cmp` trees, or Duff's-device fallthrough cascades (e.g.
     5 branches handling trip counts 1..5), and collapses all branches back into a single
     clean `for` loop.
  2. Linear Strided Loop Rerolling:
     Recognizes K >= 3 consecutive strided memory loads/stores/reductions (`arr[0]..arr[K-1]`)
     inside a basic block and collapses them into a clean counted `for` loop.
  3. Magic-Multiplier Constant Division / Modulo Recovery:
     Recovers clean `x / D` and `x % D` expressions from 64-bit & 32-bit widening
     `mul` / `imul` + bit-shift magic reciprocal sequences (including 65-bit Hacker's Delight
     add-shift unsigned reciprocals and signed reciprocals).
"""

from __future__ import annotations

from dataclasses import dataclass

from ntrev.frontend.asm_detector import CANONICAL_GPR64
from ntrev.models import (
    DisasmInsn,
    FunctionCFG,
    InferredFuncSignature,
    RecoveredDivMod,
    RerolledLoop,
)

WIN64_PARAM_REGS = ["rcx", "rdx", "r8", "r9"]


@dataclass
class StridedMemOp:
    insn_addr: int
    base_reg: str
    disp: int
    size: int
    op_kind: str  # "add" | "xor" | "or" | "store_imm"
    imm_val: int = 0


class CompilerDeoptimizer:
    """Recovers clean loops and division operators from compiler-optimized x86-64 CFGs."""

    def analyze_function(
        self,
        cfg: FunctionCFG,
        signature: InferredFuncSignature,
    ) -> tuple[list[RerolledLoop], list[RecoveredDivMod]]:
        if cfg.asm_classification.is_raw_asm:
            return [], []

        param_map: dict[str, str] = {}
        for idx, (pname, _) in enumerate(signature.params[:4]):
            param_map[WIN64_PARAM_REGS[idx]] = pname

        rerolled_loops: list[RerolledLoop] = []

        # 1. Check whole-CFG multi-branch unrolled loop (e.g. 5-branch compiler unrolling)
        mb_loop = self._detect_multibranch_unrolled_loop(cfg, param_map)
        if mb_loop is not None:
            rerolled_loops.append(mb_loop)
        else:
            # 2a. Check back-edge unrolled strided loop (e.g. 16x unrolled 4KB page scan in KeCheckForZeroPage)
            be_loop = self._detect_backedge_unrolled_strided_loop(cfg, param_map)
            if be_loop is not None:
                rerolled_loops.append(be_loop)
            else:
                # 2b. Check intra-block linear strided loop unrolling
                rerolled_loops.extend(self._detect_linear_strided_loops(cfg, param_map))

        # 3. Check magic-multiplier constant division / modulo strength reduction
        recovered_divs = self._detect_magic_divisions(cfg, param_map)

        return rerolled_loops, recovered_divs

    # ------------------------------------------------------------------
    # 1. Multi-Branch Unrolled Loop Collapse (N-way branches / Duff cascade)
    # ------------------------------------------------------------------
    def _detect_multibranch_unrolled_loop(
        self,
        cfg: FunctionCFG,
        param_map: dict[str, str],
    ) -> RerolledLoop | None:
        """
        Detect when a compiler unrolled a loop into multiple branches (e.g. 3..16 branches
        via linear `cmp` chain, binary-search `cmp` tree, or Duff's-device fallthrough blocks).
        """
        if len(cfg.blocks) < 4 or len(cfg.blocks) > 40:
            return None

        # Ensure CFG is acyclic (unrolled branch tree has no backedges)
        paths = self._enumerate_acyclic_paths(cfg, max_paths=32)
        if not paths or len(paths) < 4:
            return None

        # Identify which parameter register is compared against small integer trip counts
        count_info = self._find_count_dispatch_reg(cfg)
        if not count_info:
            return None
        count_reg, count_bits = count_info

        # Extract the strided memory op set along each path
        ops_by_len: dict[int, list[StridedMemOp]] = {}
        base_reg_seen: str | None = None
        op_kind_seen: str | None = None
        elem_size_seen: int | None = None

        for path in paths:
            mem_ops = self._extract_path_strided_ops(cfg, path)
            k = len(mem_ops)
            if k > 0:
                b0 = mem_ops[0].base_reg
                ok0 = mem_ops[0].op_kind
                sz0 = mem_ops[0].size
                if any(
                    m.base_reg != b0 or m.op_kind != ok0 or m.size != sz0
                    for m in mem_ops
                ):
                    return None
                if base_reg_seen is None:
                    base_reg_seen, op_kind_seen, elem_size_seen = b0, ok0, sz0
                elif (b0, ok0, sz0) != (base_reg_seen, op_kind_seen, elem_size_seen):
                    return None
            ops_by_len[k] = mem_ops

        # Require contiguous unrolled lengths 0, 1, 2, ..., N with N >= 3 (e.g., N = 4 or 5)
        max_k = max(ops_by_len.keys(), default=0)
        if max_k < 3 or base_reg_seen is None or op_kind_seen is None or elem_size_seen is None:
            return None
        for needed_k in range(0, max_k + 1):
            if needed_k not in ops_by_len:
                return None

        # Verify every branch k operates on exact arithmetic-progression offsets {d0 + i*stride}
        stride = elem_size_seen
        d0 = sorted(m.disp for m in ops_by_len[max_k])[0]
        for k in range(1, max_k + 1):
            disps = sorted(m.disp for m in ops_by_len[k])
            expected = [d0 + i * stride for i in range(k)]
            if disps != expected:
                return None

        # Determine how `count > max_k` behaves: clamps to `max_k` or returns 0
        clamps_at_max = self._detect_upper_clamp(cfg, count_reg, max_k)

        base_name = param_map.get(base_reg_seen, base_reg_seen)
        count_name = param_map.get(count_reg, count_reg)
        elem_c_type = {1: "uint8_t", 2: "uint16_t", 4: "uint32_t", 8: "uint64_t"}.get(
            elem_size_seen, "uint32_t"
        )
        c_op = {"add": "+=", "xor": "^=", "or": "|="}.get(op_kind_seen, "+=")

        ptr_expr = (
            f"(({elem_c_type}*)((uint8_t*){base_name} + 0x{d0:x}))"
            if d0 != 0
            else f"(({elem_c_type}*){base_name})"
        )

        cnt_cast = f"(uint64_t)(uint32_t){count_name}" if count_bits == 32 else f"(uint64_t){count_name}"
        if clamps_at_max:
            bound_expr = f"({cnt_cast} < {max_k}ULL ? {cnt_cast} : {max_k}ULL)"
        else:
            bound_expr = f"({cnt_cast} <= {max_k}ULL ? {cnt_cast} : 0ULL)"

        body_stmt = f"acc {c_op} {ptr_expr}[i];"
        c_loop_lines = [
            f"uint64_t bound = {bound_expr};",
            "uint64_t acc = 0;",
            "for (uint64_t i = 0; i < bound; ++i) {",
            f"    {body_stmt}",
            "}",
            "return acc;",
        ]

        all_addrs = [ins.address for ins in cfg.all_instructions()]
        return RerolledLoop(
            kind="multi_branch_dispatch",
            header_addr=cfg.entry_address,
            trip_count_expr=bound_expr,
            max_unroll_factor=max_k,
            induction_var="i",
            base_expr=base_name,
            elem_c_type=elem_c_type,
            stride_bytes=stride,
            op_kind=op_kind_seen,
            accumulator_init="0",
            body_c_code=body_stmt,
            c_loop_code="\n".join(c_loop_lines),
            covered_addrs=all_addrs,
            replaces_entire_function=True,
            description=(
                f"Collapsed {max_k}-branch compiler-unrolled dispatch on '{count_name}' "
                f"(stride={stride}B, op={op_kind_seen}) into single counted for-loop"
            ),
        )

    def _enumerate_acyclic_paths(
        self, cfg: FunctionCFG, max_paths: int = 32
    ) -> list[list[int]] | None:
        paths: list[list[int]] = []

        def dfs(cur: int, stack_set: set[int], cur_path: list[int]) -> bool:
            if len(paths) > max_paths:
                return False
            blk = cfg.blocks[cur]
            if not blk.successors:
                paths.append(list(cur_path))
                return True
            for succ in blk.successors:
                if succ in stack_set:
                    return False  # Cycle detected
                stack_set.add(succ)
                cur_path.append(succ)
                if not dfs(succ, stack_set, cur_path):
                    return False
                cur_path.pop()
                stack_set.remove(succ)
            return True

        if cfg.entry_address not in cfg.blocks:
            return None
        ok = dfs(cfg.entry_address, {cfg.entry_address}, [cfg.entry_address])
        return paths if ok else None

    def _find_count_dispatch_reg(self, cfg: FunctionCFG) -> tuple[str, int] | None:
        counts: dict[str, int] = {}
        widths: dict[str, int] = {}
        for ins in cfg.all_instructions():
            if ins.mnemonic in ("cmp", "test", "sub", "dec") and ins.operands:
                op0 = ins.operands[0]
                if op0.kind == "reg" and op0.reg:
                    canon = CANONICAL_GPR64.get(op0.reg)
                    if canon in WIN64_PARAM_REGS:
                        counts[canon] = counts.get(canon, 0) + 1
                        widths[canon] = (op0.size or 8) * 8
        if not counts:
            return None
        best_reg = max(counts.items(), key=lambda kv: kv[1])[0]
        return best_reg, widths.get(best_reg, 64)

    def _extract_path_strided_ops(
        self, cfg: FunctionCFG, block_path: list[int]
    ) -> list[StridedMemOp]:
        ops: list[StridedMemOp] = []
        reg_aliases: dict[str, tuple[str, int]] = {r: (r, 0) for r in WIN64_PARAM_REGS}
        loaded_mem: dict[str, tuple[str, int, int]] = {}

        for b_addr in block_path:
            blk = cfg.blocks[b_addr]
            for ins in blk.instructions:
                m = ins.mnemonic
                # Track `mov reg, param` or `lea reg, [param + disp]`
                if m == "mov" and len(ins.operands) == 2:
                    d, s = ins.operands
                    if d.kind == "reg" and s.kind == "reg":
                        dc = CANONICAL_GPR64.get(d.reg or "")
                        sc = CANONICAL_GPR64.get(s.reg or "")
                        if dc and sc and sc in reg_aliases:
                            reg_aliases[dc] = reg_aliases[sc]
                    elif (
                        d.kind == "reg"
                        and s.kind == "mem"
                        and s.base
                        and s.index is None
                        and s.segment is None
                    ):
                        dc = CANONICAL_GPR64.get(d.reg or "")
                        bc = CANONICAL_GPR64.get(s.base)
                        if dc and bc and bc in reg_aliases:
                            orig_b, base_off = reg_aliases[bc]
                            loaded_mem[dc] = (orig_b, base_off + s.disp, s.size or 4)
                            # Note: in branch `count == 1`, a single `mov eax, [rcx]` IS the 1-elem sum!

                elif m == "lea" and len(ins.operands) == 2:
                    d, s = ins.operands
                    if d.kind == "reg" and s.kind == "mem" and s.base and s.index is None:
                        dc = CANONICAL_GPR64.get(d.reg or "")
                        bc = CANONICAL_GPR64.get(s.base)
                        if dc and bc and bc in reg_aliases:
                            orig_b, base_off = reg_aliases[bc]
                            reg_aliases[dc] = (orig_b, base_off + s.disp)

                elif m in ("add", "xor", "or") and len(ins.operands) == 2:
                    d, s = ins.operands
                    dc = CANONICAL_GPR64.get(d.reg or "") if d.kind == "reg" else None
                    # Direct memory op: `add eax, [rcx + disp]`
                    if dc and s.kind == "mem" and s.base and s.index is None and s.segment is None:
                        bc = CANONICAL_GPR64.get(s.base)
                        if bc and bc in reg_aliases:
                            # If destination itself was initialized via `mov eax, [rcx + d0]`, include that first load!
                            if dc in loaded_mem:
                                lb, ld, lsz = loaded_mem.pop(dc)
                                ops.append(StridedMemOp(ins.address, lb, ld, lsz, m))
                            orig_b, base_off = reg_aliases[bc]
                            ops.append(
                                StridedMemOp(
                                    ins.address, orig_b, base_off + s.disp, s.size or 4, m
                                )
                            )
                    # Register-loaded op: `mov edx, [rcx + disp]; add eax, edx`
                    elif dc and s.kind == "reg":
                        sc = CANONICAL_GPR64.get(s.reg or "")
                        if dc in loaded_mem:
                            lb, ld, lsz = loaded_mem.pop(dc)
                            ops.append(StridedMemOp(ins.address, lb, ld, lsz, m))
                        if sc and sc in loaded_mem:
                            lb, ld, lsz = loaded_mem.pop(sc)
                            ops.append(StridedMemOp(ins.address, lb, ld, lsz, m))

        # Handle the `k == 1` branch where `rax` was simply `mov eax, [rcx + d0]; ret`
        if not ops and "rax" in loaded_mem:
            lb, ld, lsz = loaded_mem["rax"]
            ops.append(StridedMemOp(block_path[-1], lb, ld, lsz, "add"))

        return ops

    def _detect_upper_clamp(self, cfg: FunctionCFG, count_reg: str, max_k: int) -> bool:
        """
        Determine whether inputs `count > max_k` execute the `max_k` path (clamp)
        or fall into the 0-op default path (exact switch).

        Follows the concrete execution path from `entry_address` with `count = max_k + 1`!
        """
        cur_addr = cfg.entry_address
        test_val = max_k + 1
        visited: set[int] = set()
        default_path: list[int] = []
        last_cmp_imm: int | None = None

        while cur_addr in cfg.blocks and cur_addr not in visited:
            visited.add(cur_addr)
            default_path.append(cur_addr)
            blk = cfg.blocks[cur_addr]
            if not blk.instructions:
                break

            for ins in blk.instructions:
                if (
                    ins.mnemonic == "cmp"
                    and len(ins.operands) == 2
                    and ins.operands[0].kind == "reg"
                    and CANONICAL_GPR64.get(ins.operands[0].reg or "") == count_reg
                    and ins.operands[1].kind == "imm"
                ):
                    last_cmp_imm = ins.operands[1].imm
                elif (
                    ins.mnemonic == "test"
                    and len(ins.operands) == 2
                    and ins.operands[0].kind == "reg"
                    and CANONICAL_GPR64.get(ins.operands[0].reg or "") == count_reg
                ):
                    last_cmp_imm = 0

            term = blk.instructions[-1]
            mn = term.mnemonic
            if mn in ("ret", "retq", "retn"):
                break
            if mn == "jmp" and blk.branch_target is not None:
                cur_addr = blk.branch_target
                continue
            if mn.startswith("j") and last_cmp_imm is not None:
                cc = mn[1:]
                take = False
                if cc in ("e", "z"):
                    take = test_val == last_cmp_imm
                elif cc in ("ne", "nz"):
                    take = test_val != last_cmp_imm
                elif cc in ("b", "c", "nae", "l", "nge"):
                    take = test_val < last_cmp_imm
                elif cc in ("be", "na", "le", "ng"):
                    take = test_val <= last_cmp_imm
                elif cc in ("a", "nbe", "g", "nle"):
                    take = test_val > last_cmp_imm
                elif cc in ("ae", "nb", "nc", "ge", "nl"):
                    take = test_val >= last_cmp_imm
                nxt = blk.branch_target if take else blk.fallthrough
                if nxt is not None:
                    cur_addr = nxt
                    continue
            if blk.fallthrough is not None:
                cur_addr = blk.fallthrough
            else:
                break

        ops_on_overflow = self._extract_path_strided_ops(cfg, default_path)
        return len(ops_on_overflow) == max_k

    # ------------------------------------------------------------------
    # 2a. Back-Edge Unrolled Strided Loop Collapse (e.g. KeCheckForZeroPage 16x unroll)
    # ------------------------------------------------------------------
    def _detect_backedge_unrolled_strided_loop(
        self,
        cfg: FunctionCFG,
        param_map: dict[str, str],
    ) -> RerolledLoop | None:
        """
        Detect a counted pointer-bump loop whose body was unrolled K >= 4 times
        (e.g. `KeCheckForZeroPage` in retail `ntoskrnl.exe`: 16x 8-byte `or` loads per
        128-byte outer iteration across a 4096-byte page -> 512-element single loop).
        """
        if len(cfg.blocks) != 3 or cfg.entry_address not in cfg.blocks:
            return None

        entry_blk = cfg.blocks[cfg.entry_address]
        if len(entry_blk.successors) != 1:
            return None
        loop_addr = entry_blk.successors[0]
        if loop_addr not in cfg.blocks or loop_addr == cfg.entry_address:
            return None
        loop_blk = cfg.blocks[loop_addr]
        if loop_addr not in loop_blk.successors or len(loop_blk.successors) != 2:
            return None
        exit_addrs = [s for s in loop_blk.successors if s != loop_addr]
        if len(exit_addrs) != 1 or exit_addrs[0] not in cfg.blocks:
            return None
        exit_blk = cfg.blocks[exit_addrs[0]]
        if len(exit_blk.instructions) != 1 or exit_blk.instructions[0].mnemonic not in ("ret", "retq", "retn"):
            return None

        # Preheader: `lea end_reg, [base_reg + total_bytes]` and `xor eax, eax`
        base_reg: str | None = None
        end_reg: str | None = None
        total_bytes: int | None = None
        zeroed_rax = False
        for ins in entry_blk.instructions:
            if ins.mnemonic == "lea" and len(ins.operands) == 2:
                d, s = ins.operands
                if d.kind == "reg" and s.kind == "mem" and s.base and s.index is None and s.disp > 0:
                    end_reg = CANONICAL_GPR64.get(d.reg or "")
                    base_reg = CANONICAL_GPR64.get(s.base)
                    total_bytes = s.disp
            elif ins.mnemonic == "xor" and len(ins.operands) == 2:
                d, s = ins.operands
                if d.kind == "reg" and s.kind == "reg" and d.reg == s.reg:
                    if CANONICAL_GPR64.get(d.reg or "") == "rax":
                        zeroed_rax = True

        if not (base_reg and end_reg and total_bytes and zeroed_rax):
            return None

        # Extract the unrolled strided memory reductions in the loop block
        ops = self._extract_path_strided_ops(cfg, [loop_addr])
        k = len(ops)
        if k < 4:
            return None
        ok0 = ops[0].op_kind
        sz0 = ops[0].size
        if any(m.base_reg != base_reg or m.op_kind != ok0 or m.size != sz0 for m in ops):
            return None
        if [m.disp for m in ops] != [i * sz0 for i in range(k)]:
            return None

        outer_stride = k * sz0
        if total_bytes <= 0 or total_bytes % outer_stride != 0:
            return None

        # Verify pointer increment by `outer_stride` and unsigned `jb` back-edge against `end_reg`
        has_step = False
        has_cmp = False
        for ins in loop_blk.instructions:
            if ins.mnemonic in ("lea", "add") and len(ins.operands) == 2:
                d, s = ins.operands
                if d.kind == "reg" and CANONICAL_GPR64.get(d.reg or "") == base_reg:
                    if ins.mnemonic == "lea" and s.kind == "mem" and CANONICAL_GPR64.get(s.base or "") == base_reg and s.disp == outer_stride:
                        has_step = True
                    elif ins.mnemonic == "add" and s.kind == "imm" and s.imm == outer_stride:
                        has_step = True
            elif ins.mnemonic == "cmp" and len(ins.operands) == 2:
                o0, o1 = ins.operands
                if (
                    o0.kind == "reg"
                    and o1.kind == "reg"
                    and CANONICAL_GPR64.get(o0.reg or "") == base_reg
                    and CANONICAL_GPR64.get(o1.reg or "") == end_reg
                ):
                    has_cmp = True

        term = loop_blk.instructions[-1]
        if not (has_step and has_cmp and term.mnemonic in ("jb", "jc", "jnae", "jne", "jnz") and loop_blk.branch_target == loop_addr):
            return None

        total_elems = total_bytes // sz0
        base_name = param_map.get(base_reg, base_reg)
        elem_c_type = {1: "uint8_t", 2: "uint16_t", 4: "uint32_t", 8: "uint64_t"}.get(sz0, "uint64_t")
        c_op = {"add": "+=", "xor": "^=", "or": "|="}.get(ok0, "|=")
        ptr_expr = f"((const {elem_c_type} *)(uintptr_t){base_name})"
        body_stmt = f"acc {c_op} {ptr_expr}[i];"
        loop_lines = [
            "uint64_t acc = 0ULL;",
            f"for (size_t i = 0; i < {total_elems}ULL; ++i) {{",
            f"    {body_stmt}",
            "}",
            "return acc;",
        ]
        all_addrs = [ins.address for ins in cfg.all_instructions()]
        return RerolledLoop(
            kind="unrolled_page_loop",
            header_addr=loop_addr,
            trip_count_expr=str(total_elems),
            max_unroll_factor=k,
            induction_var="i",
            base_expr=base_name,
            elem_c_type=elem_c_type,
            stride_bytes=sz0,
            op_kind=ok0,
            accumulator_init="0",
            body_c_code=body_stmt,
            c_loop_code="\n".join(loop_lines),
            covered_addrs=all_addrs,
            replaces_entire_function=True,
            description=(
                f"Collapsed {k}x-unrolled strided loop ({total_bytes}B = {total_elems} x {sz0}B, "
                f"op={ok0}) into single counted for-loop"
            ),
        )

    # ------------------------------------------------------------------
    # 2b. Intra-Block Linear Strided Unrolled Loop Collapse
    # ------------------------------------------------------------------
    def _detect_linear_strided_loops(
        self,
        cfg: FunctionCFG,
        param_map: dict[str, str],
    ) -> list[RerolledLoop]:
        found: list[RerolledLoop] = []

        for blk in cfg.sorted_blocks():
            ops = self._extract_path_strided_ops(cfg, [blk.address])
            if len(ops) < 4:
                continue

            # Partition into maximal runs with identical (base_reg, op_kind, size) & constant stride
            b0 = ops[0].base_reg
            ok0 = ops[0].op_kind
            sz0 = ops[0].size
            if any(m.base_reg != b0 or m.op_kind != ok0 or m.size != sz0 for m in ops):
                continue

            stride = ops[1].disp - ops[0].disp
            if stride != sz0:
                continue
            if any(ops[i].disp - ops[i - 1].disp != stride for i in range(2, len(ops))):
                continue

            k = len(ops)
            d0 = ops[0].disp
            base_name = param_map.get(b0, b0)
            elem_c_type = {1: "uint8_t", 2: "uint16_t", 4: "uint32_t", 8: "uint64_t"}.get(
                sz0, "uint32_t"
            )
            c_op = {"add": "+=", "xor": "^=", "or": "|="}.get(ok0, "+=")
            ptr_expr = (
                f"(({elem_c_type}*)((uint8_t*){base_name} + 0x{d0:x}))"
                if d0 != 0
                else f"(({elem_c_type}*){base_name})"
            )
            body_stmt = f"acc {c_op} {ptr_expr}[i];"
            replaces_whole = len(cfg.blocks) == 1
            loop_lines = [
                "uint64_t acc = 0;",
                f"for (size_t i = 0; i < {k}; ++i) {{",
                f"    {body_stmt}",
                "}",
            ]
            if replaces_whole:
                loop_lines.append("return acc;")

            found.append(
                RerolledLoop(
                    kind="linear_strided",
                    header_addr=blk.address,
                    trip_count_expr=str(k),
                    max_unroll_factor=k,
                    induction_var="i",
                    base_expr=base_name,
                    elem_c_type=elem_c_type,
                    stride_bytes=stride,
                    op_kind=ok0,
                    accumulator_init="0",
                    body_c_code=body_stmt,
                    c_loop_code="\n".join(loop_lines),
                    covered_addrs=[m.insn_addr for m in ops],
                    replaces_entire_function=replaces_whole,
                    description=(
                        f"Collapsed {k}x linearly unrolled strided {ok0} chain on '{base_name}' "
                        f"(stride={stride}B) into counted for-loop"
                    ),
                )
            )
        return found

    # ------------------------------------------------------------------
    # 3. Magic-Multiplier Constant Division / Modulo Recovery
    # ------------------------------------------------------------------
    def _detect_magic_divisions(
        self,
        cfg: FunctionCFG,
        param_map: dict[str, str],
    ) -> list[RecoveredDivMod]:
        results: list[RecoveredDivMod] = []

        for blk in cfg.sorted_blocks():
            insns = blk.instructions
            const_regs: dict[str, int] = {}
            rax_src_reg: str | None = None

            for idx, ins in enumerate(insns):
                m = ins.mnemonic
                if m in ("mov", "movabs") and len(ins.operands) == 2:
                    d, s = ins.operands
                    if d.kind == "reg" and s.kind == "imm" and d.reg:
                        canon = CANONICAL_GPR64.get(d.reg)
                        if canon:
                            mask = (1 << ((d.size or 8) * 8)) - 1
                            const_regs[canon] = s.imm & mask
                    elif d.kind == "reg" and s.kind == "reg":
                        dc = CANONICAL_GPR64.get(d.reg or "")
                        sc = CANONICAL_GPR64.get(s.reg or "")
                        if dc == "rax" and sc:
                            rax_src_reg = sc

                # Detect 1-operand widening `mul reg` or `imul reg` by magic constant.
                # Note: compilers put the magic constant in EITHER `mul_reg` (`mov rdx, M; mul rdx`)
                # OR directly in `rax` (`movabs rax, M; mul rcx` by commutativity)!
                if m in ("mul", "imul") and len(ins.operands) == 1 and ins.operands[0].kind == "reg":
                    mul_reg = CANONICAL_GPR64.get(ins.operands[0].reg or "")
                    bits = (ins.operands[0].size or 8) * 8
                    if not mul_reg or bits not in (32, 64):
                        continue
                    if mul_reg in const_regs:
                        magic = const_regs[mul_reg]
                        dividend_reg = rax_src_reg or "rcx"
                    elif "rax" in const_regs:
                        magic = const_regs["rax"]
                        dividend_reg = mul_reg
                    else:
                        continue
                    is_signed = m == "imul"

                    rec = self._analyze_post_mul_window(
                        insns=insns,
                        mul_idx=idx,
                        magic=magic,
                        bits=bits,
                        is_signed=is_signed,
                        dividend_reg=dividend_reg,
                        param_map=param_map,
                    )
                    if rec is not None:
                        results.append(rec)

                # Detect MSVC x64 32-bit unsigned magic reciprocal via 64-bit 3-operand
                # `imul r64, src64, imm32` (`imm > 0x10000`) followed by `shr r64, s` (`s >= 32`)!
                elif (
                    m == "imul"
                    and len(ins.operands) == 3
                    and ins.operands[0].kind == "reg"
                    and ins.operands[1].kind == "reg"
                    and ins.operands[2].kind == "imm"
                    and (ins.operands[0].size or 8) == 8
                ):
                    imm_m = ins.operands[2].imm & 0xFFFFFFFF
                    if imm_m > 0x10000:
                        dst_c = CANONICAL_GPR64.get(ins.operands[0].reg or "") or "rax"
                        src_c = CANONICAL_GPR64.get(ins.operands[1].reg or "") or "rcx"
                        win = insns[idx + 1 : min(len(insns), idx + 8)]
                        sh_amt: int | None = None
                        for w_ins in win:
                            if (
                                w_ins.mnemonic == "shr"
                                and len(w_ins.operands) == 2
                                and w_ins.operands[0].kind == "reg"
                                and CANONICAL_GPR64.get(w_ins.operands[0].reg or "") == dst_c
                                and w_ins.operands[1].kind == "imm"
                                and w_ins.operands[1].imm >= 32
                            ):
                                sh_amt = w_ins.operands[1].imm
                                break
                        if sh_amt is not None:
                            divisor = round((1 << sh_amt) / imm_m)
                            if divisor > 1:
                                is_mod = any(
                                    w_ins.mnemonic == "imul"
                                    and len(w_ins.operands) == 3
                                    and w_ins.operands[2].kind == "imm"
                                    and (w_ins.operands[2].imm & 0xFFFFFFFF) == divisor
                                    for w_ins in win
                                )
                                div_name = param_map.get(src_c, src_c)
                                op_sym = "%" if is_mod else "/"
                                results.append(
                                    RecoveredDivMod(
                                        insn_addrs=[ins.address] + [w.address for w in win],
                                        dividend_reg=src_c,
                                        dividend_c_expr=div_name,
                                        divisor=divisor,
                                        is_signed=False,
                                        is_modulo=is_mod,
                                        bit_width=32,
                                        magic_constant=imm_m,
                                        shift_amount=sh_amt - 32,
                                        c_expr=f"((uint32_t){div_name} {op_sym} (uint32_t){divisor})",
                                    )
                                )

        return results

    def _analyze_post_mul_window(
        self,
        insns: list[DisasmInsn],
        mul_idx: int,
        magic: int,
        bits: int,
        is_signed: bool,
        dividend_reg: str,
        param_map: dict[str, str],
    ) -> RecoveredDivMod | None:
        window = insns[mul_idx : min(len(insns), mul_idx + 12)]
        shifts: list[tuple[str, int, int]] = []
        has_overflow_add_trick = False
        has_signed_add_adj = False
        sign_bit_regs: set[str] = set()

        for w_ins in window[1:]:
            wm = w_ins.mnemonic
            if wm in ("shr", "sar") and len(w_ins.operands) == 2 and w_ins.operands[1].kind == "imm":
                sh_amt = w_ins.operands[1].imm
                if wm == "shr" and sh_amt in (31, 63) and w_ins.operands[0].kind == "reg":
                    rc = CANONICAL_GPR64.get(w_ins.operands[0].reg or "")
                    if rc:
                        sign_bit_regs.add(rc)
                else:
                    shifts.append((wm, sh_amt, w_ins.address))
            elif wm == "sub" and not is_signed:
                # `sub rcx, rdx; shr rcx, 1; add rdx, rcx; shr rdx, s` (65-bit unsigned magic)
                has_overflow_add_trick = True
            elif wm == "add" and is_signed and len(w_ins.operands) == 2:
                src_rc = (
                    CANONICAL_GPR64.get(w_ins.operands[1].reg or "")
                    if w_ins.operands[1].kind == "reg"
                    else None
                )
                if src_rc not in sign_bit_regs:
                    has_signed_add_adj = True
            elif wm in ("ret", "retq", "jmp"):
                break

        if not is_signed:
            if has_overflow_add_trick and len(shifts) >= 2 and shifts[0][1] == 1:
                s2 = shifts[1][1]
                eff_magic = (1 << bits) + magic
                total_shift = bits + 1 + s2
                divisor = round((1 << total_shift) / eff_magic)
                shift_used = 1 + s2
            elif shifts:
                s = shifts[0][1]
                total_shift = bits + s
                divisor = round((1 << total_shift) / magic) if magic else 0
                shift_used = s
            else:
                divisor = round((1 << bits) / magic) if magic else 0
                shift_used = 0
        else:
            # Signed reciprocal: effective multiplier M_eff in [1, 2^(bits+1))
            sign_bit = 1 << (bits - 1)
            signed_magic = magic - (1 << bits) if (magic & sign_bit) else magic
            eff_magic = signed_magic + (1 << bits) if (has_signed_add_adj and signed_magic < 0) else signed_magic
            sar_shifts = [amt for mn, amt, _ in shifts if mn == "sar"]
            s = sar_shifts[0] if sar_shifts else 0
            if eff_magic <= 0:
                return None
            divisor = round((1 << (bits + s)) / eff_magic)
            shift_used = s

        if divisor <= 1:
            return None

        is_mod = any(
            w_ins.mnemonic == "imul"
            and len(w_ins.operands) == 3
            and w_ins.operands[2].kind == "imm"
            and w_ins.operands[2].imm == divisor
            for w_ins in window[1:]
        )
        op_sym = "%" if is_mod else "/"
        div_name = param_map.get(dividend_reg, dividend_reg)
        cast_ty = f"int{bits}_t" if is_signed else f"uint{bits}_t"
        c_expr = f"(({cast_ty}){div_name} {op_sym} ({cast_ty}){divisor})"
        covered = [w.address for w in window]

        return RecoveredDivMod(
            insn_addrs=covered,
            dividend_reg=dividend_reg,
            dividend_c_expr=div_name,
            divisor=divisor,
            is_signed=is_signed,
            is_modulo=is_mod,
            bit_width=bits,
            magic_constant=magic,
            shift_amount=shift_used,
            c_expr=c_expr,
        )
