"""
Native x86-64 -> LLVM IR Lifter & LLVM 22 Core Pass Optimizer (`llvmlite`).

Key design highlights:
  - Targets `x86_64-pc-windows-msvc` (`ms_abi`: RCX, RDX, R8, R9 -> i64 parameters, RAX return).
  - Exact x86-64 sub-register aliasing (32-bit writes zero-extend to 64 bits; 16/8-bit writes
    merge into existing upper bits).
  - Non-escaping GEP stack-frame recovery (`%stack_frame = alloca [4096 x i8]`): `push`/`pop`
    and `[rsp + disp]` / `[rbp - disp]` accesses resolve to `getelementptr` into `%stack_frame`,
    enabling LLVM 22's SROA + GVN + DCE passes to eliminate prologue/epilogue spills.
  - RIP-relative `.rdata` constant folding + PDB named kernel global symbol lifting
    (`@__nt_sym_addr_<Symbol>()`) for position-independent bare-metal C emission.
  - Exact x86-64 memory bit-string indexing for `bt`/`bts`/`btr`/`btc`, `lock` atomics,
    `xadd`/`cmpxchg`, `bsf`/`bsr`/`tzcnt`/`lzcnt`/`popcnt`, and tailcalls.
"""

from __future__ import annotations

from dataclasses import dataclass, field

from llvmlite import binding as llvm
from llvmlite import ir

from ntrev.models import (
    BasicBlock,
    BinaryImage,
    DisasmInsn,
    FunctionCFG,
    InsnOperand,
    KernelGlobalRef,
)

llvm.initialize_all_targets()
llvm.initialize_all_asmprinters()

GPR64_LIST = [
    "rax", "rbx", "rcx", "rdx", "rsi", "rdi", "rbp", "rsp",
    "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15",
]

SUBREG_MAP: dict[str, tuple[str, int, int]] = {
    "rax": ("rax", 0, 64), "eax": ("rax", 0, 32), "ax": ("rax", 0, 16), "al": ("rax", 0, 8), "ah": ("rax", 8, 8),
    "rbx": ("rbx", 0, 64), "ebx": ("rbx", 0, 32), "bx": ("rbx", 0, 16), "bl": ("rbx", 0, 8), "bh": ("rbx", 8, 8),
    "rcx": ("rcx", 0, 64), "ecx": ("rcx", 0, 32), "cx": ("rcx", 0, 16), "cl": ("rcx", 0, 8), "ch": ("rcx", 8, 8),
    "rdx": ("rdx", 0, 64), "edx": ("rdx", 0, 32), "dx": ("rdx", 0, 16), "dl": ("rdx", 0, 8), "dh": ("rdx", 8, 8),
    "rsi": ("rsi", 0, 64), "esi": ("rsi", 0, 32), "si": ("rsi", 0, 16), "sil": ("rsi", 0, 8),
    "rdi": ("rdi", 0, 64), "edi": ("rdi", 0, 32), "di": ("rdi", 0, 16), "dil": ("rdi", 0, 8),
    "rbp": ("rbp", 0, 64), "ebp": ("rbp", 0, 32), "bp": ("rbp", 0, 16), "bpl": ("rbp", 0, 8),
    "rsp": ("rsp", 0, 64), "esp": ("rsp", 0, 32), "sp": ("rsp", 0, 16), "spl": ("rsp", 0, 8),
}
for _idx in range(8, 16):
    SUBREG_MAP[f"r{_idx}"] = (f"r{_idx}", 0, 64)
    SUBREG_MAP[f"r{_idx}d"] = (f"r{_idx}", 0, 32)
    SUBREG_MAP[f"r{_idx}w"] = (f"r{_idx}", 0, 16)
    SUBREG_MAP[f"r{_idx}b"] = (f"r{_idx}", 0, 8)

STACK_FRAME_BYTES = 1024
INITIAL_RSP_OFFSET = 768


@dataclass
class LiftedModuleResult:
    raw_ir: str
    optimized_ir: str
    verified: bool
    referenced_globals: list[KernelGlobalRef] = field(default_factory=list)


class X86ToLLVMLifter:
    """Translates a disassembled `FunctionCFG` into verified & optimized LLVM 22 IR."""

    def __init__(self, target_triple: str = "x86_64-pc-windows-msvc") -> None:
        self.target_triple = target_triple
        self.i1 = ir.IntType(1)
        self.i8 = ir.IntType(8)
        self.i16 = ir.IntType(16)
        self.i32 = ir.IntType(32)
        self.i64 = ir.IntType(64)
        self.i128 = ir.IntType(128)

    def lift_function(
        self,
        cfg: FunctionCFG,
        param_count: int = 4,
        symbol_lookup: dict[int, str] | None = None,
        global_data_symbols: dict[int, str] | None = None,
        callee_arg_counts: dict[str, int] | None = None,
        callee_param_widths: dict[str, list[int]] | None = None,
        image: BinaryImage | None = None,
        opt_level: int = 2,
    ) -> LiftedModuleResult:
        """Lift `cfg` to LLVM IR, verify with LLVM Core, and run NewPM optimization passes."""
        module = ir.Module(name=f"mod_{cfg.name}")
        module.triple = self.target_triple

        arg_names = ["rcx", "rdx", "r8", "r9"][: max(0, min(param_count, 4))]
        fn_ty = ir.FunctionType(self.i64, [self.i64] * len(arg_names))
        func = ir.Function(module, fn_ty, name=cfg.name)
        for arg, aname in zip(func.args, arg_names):
            arg.name = f"arg_{aname}"

        uses_rbp_as_frame = self._detect_frame_pointer(cfg)
        stack_base_regs = {"rsp"} | ({"rbp"} if uses_rbp_as_frame else set())
        has_escaped_stack_lea = self._detect_escaped_stack_lea(cfg, stack_base_regs)

        entry_bb = func.append_basic_block(name="lift_init")
        builder = ir.IRBuilder(entry_bb)

        stack_arr = builder.alloca(ir.ArrayType(self.i8, STACK_FRAME_BYTES), name="stack_frame")
        stack_arr.align = 16

        reg_slots: dict[str, ir.AllocaInstr] = {}
        for gpr in GPR64_LIST:
            slot = builder.alloca(self.i64, name=f"reg_{gpr}")
            reg_slots[gpr] = slot

        flag_slots: dict[str, ir.AllocaInstr] = {
            flg: builder.alloca(self.i1, name=f"flg_{flg}")
            for flg in ("zf", "cf", "sf", "of")
        }

        arg_map = {aname: func.args[i] for i, aname in enumerate(arg_names)}
        for gpr in GPR64_LIST:
            if gpr in arg_map:
                builder.store(arg_map[gpr], reg_slots[gpr])
            elif gpr == "rsp":
                builder.store(ir.Constant(self.i64, INITIAL_RSP_OFFSET), reg_slots[gpr])
            else:
                builder.store(ir.Constant(self.i64, 0), reg_slots[gpr])

        for flg_slot in flag_slots.values():
            builder.store(ir.Constant(self.i1, 0), flg_slot)

        llvm_blocks: dict[int, ir.Block] = {}
        for blk in cfg.sorted_blocks():
            llvm_blocks[blk.address] = func.append_basic_block(name=f"bb_{blk.address:x}")

        if cfg.entry_address in llvm_blocks:
            builder.branch(llvm_blocks[cfg.entry_address])
        else:
            builder.ret(ir.Constant(self.i64, 0))

        ext_helpers: dict[str, ir.Function] = {}
        ref_globals_map: dict[str, KernelGlobalRef] = {}
        g_syms = global_data_symbols or {}
        c_counts = callee_arg_counts or {}
        c_widths = callee_param_widths or {}

        for blk in cfg.sorted_blocks():
            bb_ir = llvm_blocks[blk.address]
            b = ir.IRBuilder(bb_ir)
            terminated = False

            for ins in blk.instructions:
                if terminated:
                    break
                terminated = self._lift_instruction(
                    b=b,
                    module=module,
                    ins=ins,
                    blk=blk,
                    reg_slots=reg_slots,
                    flag_slots=flag_slots,
                    stack_arr=stack_arr,
                    stack_base_regs=stack_base_regs,
                    has_escaped_stack_lea=has_escaped_stack_lea,
                    llvm_blocks=llvm_blocks,
                    ext_helpers=ext_helpers,
                    symbol_lookup=symbol_lookup or {},
                    global_data_symbols=g_syms,
                    callee_arg_counts=c_counts,
                    callee_param_widths=c_widths,
                    image=image,
                    ref_globals_map=ref_globals_map,
                )

            if not terminated:
                if blk.fallthrough is not None and blk.fallthrough in llvm_blocks:
                    b.branch(llvm_blocks[blk.fallthrough])
                elif blk.successors and blk.successors[0] in llvm_blocks:
                    b.branch(llvm_blocks[blk.successors[0]])
                else:
                    ret_val = b.load(reg_slots["rax"], name="ret_rax")
                    b.ret(ret_val)

        if image is not None:
            for g in ref_globals_map.values():
                if not g.is_array_or_addr and g.size in (1, 2, 4, 8):
                    ro_val = image.read_readonly_scalar(g.va, g.size)
                    if ro_val is not None:
                        g.is_const = True
                        g.initial_value = ro_val

        raw_ir = str(module)
        opt_ir, verified = self.optimize_and_verify_ir(raw_ir, opt_level=opt_level)
        return LiftedModuleResult(
            raw_ir=raw_ir,
            optimized_ir=opt_ir,
            verified=verified,
            referenced_globals=list(ref_globals_map.values()),
        )

    def optimize_and_verify_ir(self, raw_ir: str, opt_level: int = 2) -> tuple[str, bool]:
        """Parse IR via LLVM 22 Core (`llvmlite.binding`), verify, and run NewPM passes."""
        mod = llvm.parse_assembly(raw_ir)
        try:
            mod.verify()

            if opt_level > 0:
                try:
                    target = llvm.Target.from_triple(self.target_triple)
                except RuntimeError:
                    target = llvm.Target.from_default_triple()
                tm = target.create_target_machine()
                pto = llvm.create_pipeline_tuning_options(speed_level=opt_level)
                pb = llvm.create_pass_builder(tm, pto)
                pm = pb.getModulePassManager()
                try:
                    pm.run(mod, pb)
                    mod.verify()
                finally:
                    pm.close()
                    pb.close()
                    pto.close()
                    tm.close()

            return str(mod), True
        finally:
            mod.close()

    def _detect_frame_pointer(self, cfg: FunctionCFG) -> bool:
        if cfg.entry_address not in cfg.blocks:
            return False
        for ins in cfg.blocks[cfg.entry_address].instructions[:8]:
            if ins.mnemonic in ("mov", "lea") and len(ins.operands) == 2:
                d, s = ins.operands
                if d.kind == "reg" and d.reg == "rbp":
                    if s.kind == "reg" and s.reg == "rsp":
                        return True
                    if s.kind == "mem" and s.base == "rsp" and s.index is None:
                        return True
        return False

    def _detect_escaped_stack_lea(self, cfg: FunctionCFG, stack_base_regs: set[str]) -> bool:
        has_call = False
        has_stack_lea = False
        for blk in cfg.blocks.values():
            for ins in blk.instructions:
                if ins.mnemonic in ("call", "jmp"):
                    has_call = True
                elif ins.mnemonic == "lea" and len(ins.operands) == 2:
                    dst, src = ins.operands
                    if (
                        dst.kind == "reg"
                        and dst.reg not in ("rsp", "esp", "rbp", "ebp")
                        and src.kind == "mem"
                        and src.base in stack_base_regs
                        and src.segment is None
                    ):
                        has_stack_lea = True
                elif ins.mnemonic == "mov" and len(ins.operands) == 2:
                    dst, src = ins.operands
                    if (
                        dst.kind == "reg"
                        and dst.reg not in ("rsp", "esp", "rbp", "ebp")
                        and src.kind == "reg"
                        and src.reg in stack_base_regs
                    ):
                        has_stack_lea = True
        return has_call and has_stack_lea

    def _int_ty(self, bits: int) -> ir.IntType:
        return ir.IntType(bits)

    def _read_reg(
        self, b: ir.IRBuilder, reg_slots: dict[str, ir.AllocaInstr], reg_name: str
    ) -> ir.Value:
        if reg_name not in SUBREG_MAP:
            return ir.Constant(self.i64, 0)
        parent, bit_off, bit_sz = SUBREG_MAP[reg_name]
        full = b.load(reg_slots[parent], name=f"r_{parent}")
        if bit_off > 0:
            full = b.lshr(full, ir.Constant(self.i64, bit_off))
        if bit_sz < 64:
            return b.trunc(full, self._int_ty(bit_sz), name=f"r_{reg_name}")
        return full

    def _write_reg(
        self,
        b: ir.IRBuilder,
        reg_slots: dict[str, ir.AllocaInstr],
        reg_name: str,
        val: ir.Value,
    ) -> None:
        if reg_name not in SUBREG_MAP:
            return
        parent, bit_off, bit_sz = SUBREG_MAP[reg_name]
        val_bits = val.type.width

        if val_bits < bit_sz:
            val = b.zext(val, self._int_ty(bit_sz))
        elif val_bits > bit_sz:
            val = b.trunc(val, self._int_ty(bit_sz))

        if bit_sz == 64:
            b.store(val, reg_slots[parent])
        elif bit_sz == 32:
            b.store(b.zext(val, self.i64), reg_slots[parent])
        else:
            old = b.load(reg_slots[parent])
            mask_int = ((1 << bit_sz) - 1) << bit_off
            inv_mask = (~mask_int) & 0xFFFFFFFFFFFFFFFF
            cleared = b.and_(old, ir.Constant(self.i64, inv_mask))
            shifted = b.zext(val, self.i64)
            if bit_off > 0:
                shifted = b.shl(shifted, ir.Constant(self.i64, bit_off))
            b.store(b.or_(cleared, shifted), reg_slots[parent])

    def _emit_sym_addr_call(
        self,
        b: ir.IRBuilder,
        module: ir.Module,
        ext_helpers: dict[str, ir.Function],
        sym_name: str,
    ) -> ir.Value:
        helper_name = f"__nt_sym_addr_{sym_name}"
        if helper_name not in ext_helpers:
            fn = ir.Function(module, ir.FunctionType(self.i64, []), name=helper_name)
            fn.attributes.add("readnone")
            fn.attributes.add("nounwind")
            ext_helpers[helper_name] = fn
        return b.call(ext_helpers[helper_name], [], name=f"addr_{sym_name}")

    def _compute_ea(
        self,
        b: ir.IRBuilder,
        module: ir.Module,
        ins: DisasmInsn,
        op: InsnOperand,
        reg_slots: dict[str, ir.AllocaInstr],
        ext_helpers: dict[str, ir.Function],
        global_data_symbols: dict[int, str],
        ref_globals_map: dict[str, KernelGlobalRef],
        is_lea: bool = False,
        image: BinaryImage | None = None,
    ) -> ir.Value:
        if op.base == "rip" and op.index is None:
            target_va = (ins.end_address + op.disp) & 0xFFFFFFFFFFFFFFFF
            sec = image.section_for_va(target_va) if image is not None else None
            if target_va in global_data_symbols or sec is not None:
                default_sym = (
                    f"__nt_rodata_{target_va:x}"
                    if (is_lea or (sec is not None and not sec.is_writable))
                    else f"__nt_data_{target_va:x}"
                )
                sym_name = global_data_symbols.get(target_va, default_sym)
                byte_sz = op.size if (op.size and not is_lea) else 8
                c_ty = {1: "uint8_t", 2: "uint16_t", 4: "uint32_t", 8: "uint64_t"}.get(
                    byte_sz, "uint64_t"
                )
                prev = ref_globals_map.get(sym_name)
                is_arr = bool(is_lea or (prev and prev.is_array_or_addr))
                init_bytes = prev.initial_bytes if prev else None
                if is_arr and init_bytes is None and image is not None:
                    blob = image.read_va(target_va, 64)
                    if blob:
                        init_bytes = list(blob)
                ref_globals_map[sym_name] = KernelGlobalRef(
                    name=sym_name,
                    va=target_va,
                    size=byte_sz,
                    c_type=prev.c_type if (prev and prev.is_array_or_addr) else ("uint8_t" if is_arr else c_ty),
                    is_array_or_addr=is_arr,
                    is_const=bool(sec is not None and not sec.is_writable),
                    initial_bytes=init_bytes,
                )
                return self._emit_sym_addr_call(b, module, ext_helpers, sym_name)

        addr: ir.Value = ir.Constant(self.i64, 0)
        if op.base:
            if op.base == "rip":
                addr = ir.Constant(self.i64, ins.end_address)
            else:
                base_v = self._read_reg(b, reg_slots, op.base)
                if base_v.type.width < 64:
                    base_v = b.zext(base_v, self.i64)
                addr = base_v
        if op.index:
            idx_v = self._read_reg(b, reg_slots, op.index)
            if idx_v.type.width < 64:
                idx_v = b.sext(idx_v, self.i64)
            if op.scale > 1:
                idx_v = b.mul(idx_v, ir.Constant(self.i64, op.scale))
            addr = b.add(addr, idx_v)
        if op.disp != 0:
            disp_u64 = op.disp & 0xFFFFFFFFFFFFFFFF
            addr = b.add(addr, ir.Constant(self.i64, disp_u64))
        return addr

    def _mem_ptr(
        self,
        b: ir.IRBuilder,
        module: ir.Module,
        ins: DisasmInsn,
        op: InsnOperand,
        reg_slots: dict[str, ir.AllocaInstr],
        stack_arr: ir.AllocaInstr,
        stack_base_regs: set[str],
        ext_helpers: dict[str, ir.Function],
        global_data_symbols: dict[int, str],
        ref_globals_map: dict[str, KernelGlobalRef],
        bits: int,
        image: BinaryImage | None = None,
    ) -> ir.Value:
        ea = self._compute_ea(
            b,
            module,
            ins,
            op,
            reg_slots,
            ext_helpers,
            global_data_symbols,
            ref_globals_map,
            image=image,
        )
        elem_ty = self._int_ty(bits)
        if op.base in stack_base_regs and op.segment is None:
            raw_ptr = b.gep(stack_arr, [ir.Constant(self.i64, 0), ea])
            return b.bitcast(raw_ptr, elem_ty.as_pointer())
        return b.inttoptr(ea, elem_ty.as_pointer())

    def _read_operand(
        self,
        b: ir.IRBuilder,
        module: ir.Module,
        ins: DisasmInsn,
        op: InsnOperand,
        reg_slots: dict[str, ir.AllocaInstr],
        stack_arr: ir.AllocaInstr,
        stack_base_regs: set[str],
        ext_helpers: dict[str, ir.Function],
        global_data_symbols: dict[int, str],
        image: BinaryImage | None,
        ref_globals_map: dict[str, KernelGlobalRef],
        default_bits: int = 64,
    ) -> ir.Value:
        bits = (op.size * 8) if op.size else default_bits
        if bits not in (8, 16, 32, 64):
            bits = 64
        if op.kind == "reg" and op.reg:
            return self._read_reg(b, reg_slots, op.reg)
        if op.kind == "imm":
            mask = (1 << bits) - 1
            return ir.Constant(self._int_ty(bits), op.imm & mask)
        if op.kind == "mem":
            if op.segment == "gs":
                helper_name = f"__nt_read_gs_i{bits}"
                if helper_name not in ext_helpers:
                    hty = ir.FunctionType(self._int_ty(bits), [self.i64])
                    ext_helpers[helper_name] = ir.Function(module, hty, name=helper_name)
                ea = self._compute_ea(
                    b, module, ins, op, reg_slots, ext_helpers, global_data_symbols, ref_globals_map
                )
                return b.call(ext_helpers[helper_name], [ea], name="gs_val")

            # Fold RIP-relative read-only constants (`.rdata`) directly
            if op.base == "rip" and op.index is None and image is not None:
                target_va = (ins.end_address + op.disp) & 0xFFFFFFFFFFFFFFFF
                ro_val = image.read_readonly_scalar(target_va, bits // 8)
                if ro_val is not None:
                    return ir.Constant(self._int_ty(bits), ro_val)

            ptr = self._mem_ptr(
                b, module, ins, op, reg_slots, stack_arr, stack_base_regs,
                ext_helpers, global_data_symbols, ref_globals_map, bits, image=image,
            )
            return b.load(ptr, name="mem_ld")
        return ir.Constant(self._int_ty(bits), 0)

    def _write_operand(
        self,
        b: ir.IRBuilder,
        module: ir.Module,
        ins: DisasmInsn,
        op: InsnOperand,
        val: ir.Value,
        reg_slots: dict[str, ir.AllocaInstr],
        stack_arr: ir.AllocaInstr,
        stack_base_regs: set[str],
        ext_helpers: dict[str, ir.Function],
        global_data_symbols: dict[int, str],
        ref_globals_map: dict[str, KernelGlobalRef],
        image: BinaryImage | None = None,
    ) -> None:
        if op.kind == "reg" and op.reg:
            self._write_reg(b, reg_slots, op.reg, val)
        elif op.kind == "mem":
            bits = (op.size * 8) if op.size else val.type.width
            if val.type.width < bits:
                val = b.zext(val, self._int_ty(bits))
            elif val.type.width > bits:
                val = b.trunc(val, self._int_ty(bits))
            ptr = self._mem_ptr(
                b, module, ins, op, reg_slots, stack_arr, stack_base_regs,
                ext_helpers, global_data_symbols, ref_globals_map, bits, image=image,
            )
            b.store(val, ptr)

    def _update_flags_arith(
        self,
        b: ir.IRBuilder,
        flag_slots: dict[str, ir.AllocaInstr],
        lhs: ir.Value,
        rhs: ir.Value,
        res: ir.Value,
        is_sub: bool,
    ) -> None:
        w = res.type.width
        zero = ir.Constant(self._int_ty(w), 0)
        b.store(b.icmp_unsigned("==", res, zero), flag_slots["zf"])
        b.store(b.icmp_signed("<", res, zero), flag_slots["sf"])
        if is_sub:
            cf = b.icmp_unsigned("<", lhs, rhs)
            diff_sign = b.icmp_signed("<", b.xor(lhs, rhs), zero)
            res_flip = b.icmp_signed("<", b.xor(lhs, res), zero)
            of = b.and_(diff_sign, res_flip)
        else:
            cf = b.icmp_unsigned("<", res, lhs)
            same_sign = b.icmp_signed(">=", b.xor(lhs, rhs), zero)
            res_flip = b.icmp_signed("<", b.xor(lhs, res), zero)
            of = b.and_(same_sign, res_flip)
        b.store(cf, flag_slots["cf"])
        b.store(of, flag_slots["of"])

    def _update_flags_logic(
        self,
        b: ir.IRBuilder,
        flag_slots: dict[str, ir.AllocaInstr],
        res: ir.Value,
    ) -> None:
        w = res.type.width
        zero = ir.Constant(self._int_ty(w), 0)
        b.store(b.icmp_unsigned("==", res, zero), flag_slots["zf"])
        b.store(b.icmp_signed("<", res, zero), flag_slots["sf"])
        b.store(ir.Constant(self.i1, 0), flag_slots["cf"])
        b.store(ir.Constant(self.i1, 0), flag_slots["of"])

    def _eval_cc(
        self, b: ir.IRBuilder, flag_slots: dict[str, ir.AllocaInstr], cc: str
    ) -> ir.Value:
        zf = b.load(flag_slots["zf"])
        cf = b.load(flag_slots["cf"])
        sf = b.load(flag_slots["sf"])
        of = b.load(flag_slots["of"])

        if cc in ("e", "z"):
            return zf
        if cc in ("ne", "nz"):
            return b.not_(zf)
        if cc in ("b", "c", "nae"):
            return cf
        if cc in ("ae", "nb", "nc"):
            return b.not_(cf)
        if cc in ("be", "na"):
            return b.or_(cf, zf)
        if cc in ("a", "nbe"):
            return b.and_(b.not_(cf), b.not_(zf))
        if cc in ("l", "nge"):
            return b.xor(sf, of)
        if cc in ("ge", "nl"):
            return b.not_(b.xor(sf, of))
        if cc in ("le", "ng"):
            return b.or_(zf, b.xor(sf, of))
        if cc in ("g", "nle"):
            return b.and_(b.not_(zf), b.not_(b.xor(sf, of)))
        if cc == "s":
            return sf
        if cc == "ns":
            return b.not_(sf)
        if cc == "o":
            return of
        if cc == "no":
            return b.not_(of)
        return zf

    def _match_width(
        self, b: ir.IRBuilder, val: ir.Value, bits: int, signed: bool = False
    ) -> ir.Value:
        if val.type.width == bits:
            return val
        if val.type.width < bits:
            return b.sext(val, self._int_ty(bits)) if signed else b.zext(val, self._int_ty(bits))
        return b.trunc(val, self._int_ty(bits))

    def _emit_direct_call(
        self,
        b: ir.IRBuilder,
        module: ir.Module,
        target_va: int,
        reg_slots: dict[str, ir.AllocaInstr],
        ext_helpers: dict[str, ir.Function],
        symbol_lookup: dict[int, str],
        callee_arg_counts: dict[str, int],
        callee_param_widths: dict[str, list[int]],
        r_op_fn=None,
        stack_arg_base: int = 0x20,
    ) -> ir.Value:
        callee_name = symbol_lookup.get(target_va, f"sub_{target_va:x}")
        widths = callee_param_widths.get(callee_name)
        n_args = len(widths) if widths is not None else callee_arg_counts.get(callee_name, 4)
        n_args = max(0, min(n_args, 8))
        arg_vals: list[ir.Value] = []
        for idx, r in enumerate(("rcx", "rdx", "r8", "r9")[: min(n_args, 4)]):
            rv = self._read_reg(b, reg_slots, r)
            if widths is not None and idx < len(widths) and widths[idx] in (8, 16):
                rv = b.and_(rv, ir.Constant(self.i64, (1 << widths[idx]) - 1))
            arg_vals.append(rv)
        for idx in range(4, n_args):
            w = widths[idx] if (widths is not None and idx < len(widths)) else 64
            if w not in (8, 16, 32, 64):
                w = 64
            if r_op_fn is not None:
                sv = r_op_fn(
                    InsnOperand(
                        kind="mem",
                        size=w // 8,
                        base="rsp",
                        disp=stack_arg_base + 8 * (idx - 4),
                    ),
                    w,
                )
                arg_vals.append(self._match_width(b, sv, 64, signed=False))
            else:
                arg_vals.append(ir.Constant(self.i64, 0))
        if callee_name not in ext_helpers:
            callee_ty = ir.FunctionType(self.i64, [self.i64] * n_args)
            ext_helpers[callee_name] = ir.Function(module, callee_ty, name=callee_name)
        return b.call(ext_helpers[callee_name], arg_vals, name="call_res")

    def _lift_instruction(
        self,
        b: ir.IRBuilder,
        module: ir.Module,
        ins: DisasmInsn,
        blk: BasicBlock,
        reg_slots: dict[str, ir.AllocaInstr],
        flag_slots: dict[str, ir.AllocaInstr],
        stack_arr: ir.AllocaInstr,
        stack_base_regs: set[str],
        has_escaped_stack_lea: bool,
        llvm_blocks: dict[int, ir.Block],
        ext_helpers: dict[str, ir.Function],
        symbol_lookup: dict[int, str],
        global_data_symbols: dict[int, str],
        callee_arg_counts: dict[str, int],
        callee_param_widths: dict[str, list[int]],
        image: BinaryImage | None,
        ref_globals_map: dict[str, KernelGlobalRef],
    ) -> bool:
        raw_m = ins.mnemonic
        m = raw_m[5:].strip() if raw_m.startswith("lock ") else raw_m
        ops = ins.operands

        if m in ("nop", "endbr64", "lfence", "mfence", "sfence", "pause", "prefetchw", "prefetcht0", "cld"):
            return False

        if ins.is_privileged:
            asm_ty = ir.FunctionType(ir.VoidType(), [])
            iasm = ir.InlineAsm(
                asm_ty,
                ins.text,
                "~{memory},~{dirflag},~{fpsr},~{flags}",
                side_effect=True,
            )
            b.call(iasm, [])
            return False

        def r_op(op: InsnOperand, dbits: int = 64) -> ir.Value:
            return self._read_operand(
                b, module, ins, op, reg_slots, stack_arr, stack_base_regs,
                ext_helpers, global_data_symbols, image, ref_globals_map, dbits,
            )

        def w_op(op: InsnOperand, val: ir.Value) -> None:
            self._write_operand(
                b, module, ins, op, val, reg_slots, stack_arr, stack_base_regs,
                ext_helpers, global_data_symbols, ref_globals_map, image=image,
            )

        # Control flow: returns
        if m in ("ret", "retq", "retn"):
            rsp_v = self._read_reg(b, reg_slots, "rsp")
            self._write_reg(b, reg_slots, "rsp", b.add(rsp_v, ir.Constant(self.i64, 8)))
            b.ret(self._read_reg(b, reg_slots, "rax"))
            return True

        # Unconditional jump (intra-function branch or external tailcall)
        if m == "jmp":
            if ops and ops[0].kind == "imm":
                if ops[0].imm in llvm_blocks:
                    b.branch(llvm_blocks[ops[0].imm])
                    return True
                if blk.tailcall_target is not None:
                    res = self._emit_direct_call(
                        b, module, blk.tailcall_target, reg_slots,
                        ext_helpers, symbol_lookup, callee_arg_counts, callee_param_widths,
                        r_op_fn=r_op, stack_arg_base=0x28,
                    )
                    b.ret(res)
                    return True
            b.ret(self._read_reg(b, reg_slots, "rax"))
            return True

        # Conditional jumps
        if m in ("jrcxz", "jecxz"):
            reg_c = "rcx" if m == "jrcxz" else "ecx"
            cv = self._read_reg(b, reg_slots, reg_c)
            cond = b.icmp_unsigned("==", cv, ir.Constant(cv.type, 0))
            t_blk = llvm_blocks.get(blk.branch_target or -1)
            f_blk = llvm_blocks.get(blk.fallthrough or -1)
            if t_blk and f_blk:
                b.cbranch(cond, t_blk, f_blk)
                return True

        if m.startswith("j") and len(m) <= 4 and ops and ops[0].kind == "imm":
            cc = m[1:]
            cond = self._eval_cc(b, flag_slots, cc)
            t_blk = llvm_blocks.get(ops[0].imm)
            f_blk = llvm_blocks.get(blk.fallthrough or -1)
            if t_blk and f_blk:
                b.cbranch(cond, t_blk, f_blk)
                return True

        # Function call
        if m == "call":
            if ops and ops[0].kind == "imm":
                res = self._emit_direct_call(
                    b, module, ops[0].imm, reg_slots,
                    ext_helpers, symbol_lookup, callee_arg_counts, callee_param_widths,
                    r_op_fn=r_op, stack_arg_base=0x20,
                )
            else:
                arg_vals = [self._read_reg(b, reg_slots, r) for r in ("rcx", "rdx", "r8", "r9")]
                callee_ty = ir.FunctionType(self.i64, [self.i64] * 4)
                tgt = r_op(ops[0], 64)
                fn_ptr = b.inttoptr(self._match_width(b, tgt, 64), callee_ty.as_pointer())
                res = b.call(fn_ptr, arg_vals, name="icall_res")
            self._write_reg(b, reg_slots, "rax", res)
            return False

        # Stack push / pop
        if m == "push" and len(ops) == 1:
            val = r_op(ops[0], 64)
            val64 = self._match_width(b, val, 64, signed=(ops[0].kind == "imm"))
            new_rsp = b.sub(self._read_reg(b, reg_slots, "rsp"), ir.Constant(self.i64, 8))
            self._write_reg(b, reg_slots, "rsp", new_rsp)
            w_op(InsnOperand(kind="mem", size=8, base="rsp"), val64)
            return False

        if m == "pop" and len(ops) == 1:
            val64 = r_op(InsnOperand(kind="mem", size=8, base="rsp"), 64)
            new_rsp = b.add(self._read_reg(b, reg_slots, "rsp"), ir.Constant(self.i64, 8))
            self._write_reg(b, reg_slots, "rsp", new_rsp)
            w_op(ops[0], val64)
            return False

        if m == "leave":
            rbp_v = self._read_reg(b, reg_slots, "rbp")
            self._write_reg(b, reg_slots, "rsp", rbp_v)
            saved_rbp = r_op(InsnOperand(kind="mem", size=8, base="rsp"), 64)
            self._write_reg(b, reg_slots, "rsp", b.add(rbp_v, ir.Constant(self.i64, 8)))
            self._write_reg(b, reg_slots, "rbp", saved_rbp)
            return False

        # Sign-extension helpers
        if m in ("cdqe", "cltq"):
            eax = self._read_reg(b, reg_slots, "eax")
            self._write_reg(b, reg_slots, "rax", b.sext(eax, self.i64))
            return False
        if m == "cdq":
            eax = self._read_reg(b, reg_slots, "eax")
            self._write_reg(b, reg_slots, "edx", b.ashr(eax, ir.Constant(self.i32, 31)))
            return False
        if m == "cqo":
            rax = self._read_reg(b, reg_slots, "rax")
            self._write_reg(b, reg_slots, "rdx", b.ashr(rax, ir.Constant(self.i64, 63)))
            return False

        # Data movement
        if m in ("mov", "movabs") and len(ops) == 2:
            dst_bits = (ops[0].size * 8) if ops[0].size else 64
            src = r_op(ops[1], dst_bits)
            src = self._match_width(b, src, dst_bits, signed=(ops[1].kind == "imm"))
            if (
                ops[0].kind == "reg"
                and ops[0].reg not in ("rsp", "esp", "rbp", "ebp")
                and ops[1].kind == "reg"
                and ops[1].reg in stack_base_regs
                and dst_bits == 64
            ):
                stk_ptr = b.gep(stack_arr, [ir.Constant(self.i64, 0), src])
                src = b.ptrtoint(stk_ptr, self.i64)
            w_op(ops[0], src)
            return False

        if m == "movzx" and len(ops) == 2:
            dst_bits = (ops[0].size * 8) if ops[0].size else 64
            w_op(ops[0], self._match_width(b, r_op(ops[1]), dst_bits, signed=False))
            return False

        if m in ("movsx", "movsxd") and len(ops) == 2:
            dst_bits = (ops[0].size * 8) if ops[0].size else 64
            w_op(ops[0], self._match_width(b, r_op(ops[1]), dst_bits, signed=True))
            return False

        if m == "lea" and len(ops) == 2 and ops[1].kind == "mem":
            dst_bits = (ops[0].size * 8) if ops[0].size else 64
            ea = self._compute_ea(
                b, module, ins, ops[1], reg_slots, ext_helpers,
                global_data_symbols, ref_globals_map, is_lea=True, image=image,
            )
            if (
                has_escaped_stack_lea
                and ops[0].kind == "reg"
                and ops[0].reg not in ("rsp", "esp", "rbp", "ebp")
                and ops[1].base in stack_base_regs
                and ops[1].segment is None
            ):
                stk_ptr = b.gep(stack_arr, [ir.Constant(self.i64, 0), ea])
                ea = b.ptrtoint(stk_ptr, self.i64)
            w_op(ops[0], self._match_width(b, ea, dst_bits))
            return False

        if m == "xchg" and len(ops) == 2:
            bits = (ops[0].size * 8) if ops[0].size else 64
            v0 = self._match_width(b, r_op(ops[0], bits), bits)
            v1 = self._match_width(b, r_op(ops[1], bits), bits)
            w_op(ops[0], v1)
            w_op(ops[1], v0)
            return False

        if m == "xadd" and len(ops) == 2:
            bits = (ops[0].size * 8) if ops[0].size else 64
            v0 = self._match_width(b, r_op(ops[0], bits), bits)
            v1 = self._match_width(b, r_op(ops[1], bits), bits)
            res = b.add(v0, v1)
            self._update_flags_arith(b, flag_slots, v0, v1, res, is_sub=False)
            w_op(ops[1], v0)
            w_op(ops[0], res)
            return False

        if m == "cmpxchg" and len(ops) == 2:
            bits = (ops[0].size * 8) if ops[0].size else 64
            acc_reg = {8: "al", 16: "ax", 32: "eax", 64: "rax"}[bits]
            acc_v = self._read_reg(b, reg_slots, acc_reg)
            dst_v = self._match_width(b, r_op(ops[0], bits), bits)
            src_v = self._match_width(b, r_op(ops[1], bits), bits)
            diff = b.sub(acc_v, dst_v)
            self._update_flags_arith(b, flag_slots, acc_v, dst_v, diff, is_sub=True)
            eq = b.icmp_unsigned("==", acc_v, dst_v)
            w_op(ops[0], b.select(eq, src_v, dst_v))
            self._write_reg(b, reg_slots, acc_reg, b.select(eq, acc_v, dst_v))
            return False

        # Conditional move & setcc
        if m.startswith("cmov") and len(ops) == 2:
            cc = m[4:]
            bits = (ops[0].size * 8) if ops[0].size else 64
            cond = self._eval_cc(b, flag_slots, cc)
            cur = self._match_width(b, r_op(ops[0], bits), bits)
            alt = self._match_width(b, r_op(ops[1], bits), bits)
            w_op(ops[0], b.select(cond, alt, cur))
            return False

        if m.startswith("set") and len(ops) == 1:
            cc = m[3:]
            cond = self._eval_cc(b, flag_slots, cc)
            w_op(ops[0], b.zext(cond, self.i8))
            return False

        # Bit scan & population count: bsf, tzcnt, bsr, lzcnt, popcnt
        if m in ("bsf", "tzcnt", "bsr", "lzcnt", "popcnt") and len(ops) == 2:
            bits = (ops[0].size * 8) if ops[0].size else 64
            ty = self._int_ty(bits)
            src = self._match_width(b, r_op(ops[1], bits), bits)
            if m in ("bsf", "tzcnt"):
                iname = f"llvm.cttz.i{bits}"
                cttz = ext_helpers.get(iname)
                if cttz is None:
                    cttz = ir.Function(module, ir.FunctionType(ty, [ty, self.i1]), name=iname)
                    ext_helpers[iname] = cttz
                res = b.call(cttz, [src, ir.Constant(self.i1, 0)])
                b.store(b.icmp_unsigned("==", src, ir.Constant(ty, 0)), flag_slots["zf"])
            elif m in ("bsr", "lzcnt"):
                iname = f"llvm.ctlz.i{bits}"
                ctlz = ext_helpers.get(iname)
                if ctlz is None:
                    ctlz = ir.Function(module, ir.FunctionType(ty, [ty, self.i1]), name=iname)
                    ext_helpers[iname] = ctlz
                lz = b.call(ctlz, [src, ir.Constant(self.i1, 0)])
                res = b.sub(ir.Constant(ty, bits - 1), lz) if m == "bsr" else lz
                b.store(b.icmp_unsigned("==", src, ir.Constant(ty, 0)), flag_slots["zf"])
            else:
                ctpop = module.declare_intrinsic("llvm.ctpop", [ty])
                res = b.call(ctpop, [src])
            w_op(ops[0], res)
            return False

        # Comparisons
        if m == "cmp" and len(ops) == 2:
            bits = (ops[0].size * 8) if ops[0].size else 64
            lhs = self._match_width(b, r_op(ops[0], bits), bits)
            rhs = self._match_width(b, r_op(ops[1], bits), bits, signed=(ops[1].kind == "imm"))
            res = b.sub(lhs, rhs)
            self._update_flags_arith(b, flag_slots, lhs, rhs, res, is_sub=True)
            return False

        if m == "test" and len(ops) == 2:
            bits = (ops[0].size * 8) if ops[0].size else 64
            lhs = self._match_width(b, r_op(ops[0], bits), bits)
            rhs = self._match_width(b, r_op(ops[1], bits), bits, signed=(ops[1].kind == "imm"))
            res = b.and_(lhs, rhs)
            self._update_flags_logic(b, flag_slots, res)
            return False

        # Unary ALU: inc, dec, neg, not, bswap
        if m in ("inc", "dec", "neg", "not", "bswap") and len(ops) == 1:
            bits = (ops[0].size * 8) if ops[0].size else 64
            v = self._match_width(b, r_op(ops[0], bits), bits)
            one = ir.Constant(self._int_ty(bits), 1)
            zero = ir.Constant(self._int_ty(bits), 0)
            if m == "inc":
                old_cf = b.load(flag_slots["cf"])
                res = b.add(v, one)
                self._update_flags_arith(b, flag_slots, v, one, res, is_sub=False)
                b.store(old_cf, flag_slots["cf"])
            elif m == "dec":
                old_cf = b.load(flag_slots["cf"])
                res = b.sub(v, one)
                self._update_flags_arith(b, flag_slots, v, one, res, is_sub=True)
                b.store(old_cf, flag_slots["cf"])
            elif m == "neg":
                res = b.sub(zero, v)
                self._update_flags_arith(b, flag_slots, zero, v, res, is_sub=True)
            elif m == "not":
                res = b.not_(v)
            else:
                bswap_fn = module.declare_intrinsic("llvm.bswap", [self._int_ty(bits)])
                res = b.call(bswap_fn, [v])
            w_op(ops[0], res)
            return False

        # Widening 1-operand mul / imul, and div / idiv
        if m in ("mul", "imul") and len(ops) == 1:
            bits = (ops[0].size * 8) if ops[0].size else 64
            acc_reg = {8: "al", 16: "ax", 32: "eax", 64: "rax"}[bits]
            hi_reg = {8: "ah", 16: "dx", 32: "edx", 64: "rdx"}[bits]
            lhs = self._read_reg(b, reg_slots, acc_reg)
            rhs = self._match_width(b, r_op(ops[0], bits), bits)
            wide_ty = self._int_ty(bits * 2)
            if m == "mul":
                prod = b.mul(b.zext(lhs, wide_ty), b.zext(rhs, wide_ty))
            else:
                prod = b.mul(b.sext(lhs, wide_ty), b.sext(rhs, wide_ty))
            lo = b.trunc(prod, self._int_ty(bits))
            hi = b.trunc(b.lshr(prod, ir.Constant(wide_ty, bits)), self._int_ty(bits))
            self._write_reg(b, reg_slots, acc_reg, lo)
            self._write_reg(b, reg_slots, hi_reg, hi)
            return False

        if m in ("div", "idiv") and len(ops) == 1:
            bits = (ops[0].size * 8) if ops[0].size else 64
            lo_reg = {8: "al", 16: "ax", 32: "eax", 64: "rax"}[bits]
            hi_reg = {8: "ah", 16: "dx", 32: "edx", 64: "rdx"}[bits]
            wide_ty = self._int_ty(bits * 2)
            lo = b.zext(self._read_reg(b, reg_slots, lo_reg), wide_ty)
            hi = b.shl(b.zext(self._read_reg(b, reg_slots, hi_reg), wide_ty), ir.Constant(wide_ty, bits))
            dividend = b.or_(hi, lo)
            divisor_narrow = self._match_width(b, r_op(ops[0], bits), bits)
            if m == "div":
                d_wide = b.zext(divisor_narrow, wide_ty)
                quot = b.trunc(b.udiv(dividend, d_wide), self._int_ty(bits))
                rem = b.trunc(b.urem(dividend, d_wide), self._int_ty(bits))
            else:
                d_wide = b.sext(divisor_narrow, wide_ty)
                quot = b.trunc(b.sdiv(dividend, d_wide), self._int_ty(bits))
                rem = b.trunc(b.srem(dividend, d_wide), self._int_ty(bits))
            self._write_reg(b, reg_slots, lo_reg, quot)
            self._write_reg(b, reg_slots, hi_reg, rem)
            return False

        # 2- and 3-operand imul
        if m == "imul" and len(ops) in (2, 3):
            bits = (ops[0].size * 8) if ops[0].size else 64
            src1_op = ops[0] if len(ops) == 2 else ops[1]
            src2_op = ops[1] if len(ops) == 2 else ops[2]
            lhs = self._match_width(b, r_op(src1_op, bits), bits)
            rhs = self._match_width(b, r_op(src2_op, bits), bits, signed=(src2_op.kind == "imm"))
            w_op(ops[0], b.mul(lhs, rhs))
            return False

        # Binary ALU & shifts
        if (
            m in ("add", "sub", "adc", "sbb", "and", "or", "xor", "shl", "sal", "shr", "sar", "rol", "ror")
            and len(ops) == 2
        ):
            bits = (ops[0].size * 8) if ops[0].size else 64
            if (
                m == "xor"
                and ops[0].kind == "reg"
                and ops[1].kind == "reg"
                and ops[0].reg == ops[1].reg
            ):
                zero = ir.Constant(self._int_ty(bits), 0)
                w_op(ops[0], zero)
                self._update_flags_logic(b, flag_slots, zero)
                return False

            lhs = self._match_width(b, r_op(ops[0], bits), bits)
            rhs = self._match_width(b, r_op(ops[1], bits), bits, signed=(ops[1].kind == "imm"))

            if m == "add":
                res = b.add(lhs, rhs)
                self._update_flags_arith(b, flag_slots, lhs, rhs, res, is_sub=False)
            elif m == "adc":
                cf_ext = b.zext(b.load(flag_slots["cf"]), self._int_ty(bits))
                res = b.add(b.add(lhs, rhs), cf_ext)
                self._update_flags_arith(b, flag_slots, lhs, rhs, res, is_sub=False)
            elif m == "sub":
                res = b.sub(lhs, rhs)
                self._update_flags_arith(b, flag_slots, lhs, rhs, res, is_sub=True)
            elif m == "sbb":
                cf_ext = b.zext(b.load(flag_slots["cf"]), self._int_ty(bits))
                res = b.sub(b.sub(lhs, rhs), cf_ext)
                self._update_flags_arith(b, flag_slots, lhs, rhs, res, is_sub=True)
            elif m == "and":
                res = b.and_(lhs, rhs)
                self._update_flags_logic(b, flag_slots, res)
            elif m == "or":
                res = b.or_(lhs, rhs)
                self._update_flags_logic(b, flag_slots, res)
            elif m == "xor":
                res = b.xor(lhs, rhs)
                self._update_flags_logic(b, flag_slots, res)
            else:
                shift_mask = 63 if bits == 64 else 31
                amt = b.and_(rhs, ir.Constant(self._int_ty(bits), shift_mask))
                if m in ("shl", "sal"):
                    res = b.shl(lhs, amt)
                elif m == "shr":
                    res = b.lshr(lhs, amt)
                elif m == "sar":
                    res = b.ashr(lhs, amt)
                elif m == "rol":
                    fshl = module.declare_intrinsic("llvm.fshl", [self._int_ty(bits)])
                    res = b.call(fshl, [lhs, lhs, amt])
                else:
                    fshr = module.declare_intrinsic("llvm.fshr", [self._int_ty(bits)])
                    res = b.call(fshr, [lhs, lhs, amt])
                self._update_flags_logic(b, flag_slots, res)

            w_op(ops[0], res)
            return False

        # Bit test instructions: bt, bts, btr, btc (including x86-64 memory bit-string indexing!)
        if m in ("bt", "bts", "btr", "btc") and len(ops) == 2:
            bits = (ops[0].size * 8) if ops[0].size else 64
            if bits not in (16, 32, 64):
                bits = 32
            ty = self._int_ty(bits)
            rhs_raw = r_op(ops[1], bits)

            if ops[0].kind == "mem" and ops[1].kind == "reg":
                # x86-64 architectural rule: BT [mem], reg indexes across arbitrary memory words!
                rhs_s64 = self._match_width(b, rhs_raw, 64, signed=True)
                lg2 = {16: 4, 32: 5, 64: 6}[bits]
                word_idx = b.ashr(rhs_s64, ir.Constant(self.i64, lg2))
                byte_off = b.mul(word_idx, ir.Constant(self.i64, bits // 8))
                base_ea = self._compute_ea(
                    b, module, ins, ops[0], reg_slots, ext_helpers,
                    global_data_symbols, ref_globals_map,
                )
                eff_ea = b.add(base_ea, byte_off)
                if ops[0].base in stack_base_regs and ops[0].segment is None:
                    raw_ptr = b.gep(stack_arr, [ir.Constant(self.i64, 0), eff_ea])
                    ptr = b.bitcast(raw_ptr, ty.as_pointer())
                else:
                    ptr = b.inttoptr(eff_ea, ty.as_pointer())
                lhs = b.load(ptr, name="bt_word")
                bit_idx = b.and_(self._match_width(b, rhs_s64, bits), ir.Constant(ty, bits - 1))
                bit_val = b.trunc(b.lshr(lhs, bit_idx), self.i1)
                b.store(bit_val, flag_slots["cf"])
                if m in ("bts", "btr", "btc"):
                    mask = b.shl(ir.Constant(ty, 1), bit_idx)
                    if m == "bts":
                        new_w = b.or_(lhs, mask)
                    elif m == "btr":
                        new_w = b.and_(lhs, b.not_(mask))
                    else:
                        new_w = b.xor(lhs, mask)
                    b.store(new_w, ptr)
                return False

            lhs = self._match_width(b, r_op(ops[0], bits), bits)
            rhs = self._match_width(b, rhs_raw, bits)
            bit_idx = b.and_(rhs, ir.Constant(ty, bits - 1))
            bit_val = b.trunc(b.lshr(lhs, bit_idx), self.i1)
            b.store(bit_val, flag_slots["cf"])
            if m in ("bts", "btr", "btc"):
                mask = b.shl(ir.Constant(ty, 1), bit_idx)
                if m == "bts":
                    w_op(ops[0], b.or_(lhs, mask))
                elif m == "btr":
                    w_op(ops[0], b.and_(lhs, b.not_(mask)))
                else:
                    w_op(ops[0], b.xor(lhs, mask))
            return False

        return False
