"""
Constraint-Based Dataflow Type Inference Engine for Windows x64 (`ms_abi`) Binaries.

Combines:
  1. Backward CFG liveness analysis to determine exact live-in parameter registers
     (`rcx`, `rdx`, `r8`, `r9`) without false positives from scratch clobbers.
  2. Lattice constraint propagation from x86-64 opcodes:
       - Signedness (`sar`, `idiv`, `movsx`, `movsxd`, `jl`/`jg`/`jle`/`jge`)
       - Unsignedness (`shr`, `div`, `jb`/`ja`/`jbe`/`jae`)
       - Pointer dereference & stride widths (`[reg + disp]`, `[reg + idx*scale]`)
       - NT Kernel return idioms (`NTSTATUS` 0xC000xxxx / 0x00000000, `BOOLEAN` `setcc al`)
  3. PDB procedure prototypes & `StructuralMapper` struct pointer refinements.
"""

from __future__ import annotations

from dataclasses import dataclass

from ntrev.frontend.asm_detector import CANONICAL_GPR64
from ntrev.lifter.x86_to_llvm import SUBREG_MAP
from ntrev.models import FunctionCFG, InferredFuncSignature, PDBDatabase

WIN64_ARGS = ["rcx", "rdx", "r8", "r9"]

SIGNED_MNEMONICS = {
    "sar", "idiv", "movsx", "movsxd", "cdqe", "cltq", "cdq", "cqo",
    "jl", "jnge", "jge", "jnl", "jle", "jng", "jg", "jnle",
    "cmovl", "cmovge", "cmovle", "cmovg",
    "setl", "setge", "setle", "setg",
}

UNSIGNED_MNEMONICS = {
    "shr", "div",
    "jb", "jc", "jnae", "jae", "jnb", "jnc", "jbe", "jna", "ja", "jnbe",
    "cmovb", "cmovae", "cmovbe", "cmova",
    "setb", "setae", "setbe", "seta",
}


@dataclass
class RegConstraint:
    is_pointer: bool = False
    elem_bytes: int = 0
    is_signed: bool = False
    is_unsigned: bool = False
    max_width_bits: int = 0


class TypeInferenceEngine:
    """Infers C function signatures and register types from CFG + PDB + struct constraints."""

    def infer_signature(
        self,
        cfg: FunctionCFG,
        pdb: PDBDatabase | None = None,
        inferred_param_structs: dict[str, str] | None = None,
    ) -> InferredFuncSignature:
        struct_types = inferred_param_structs or {}

        # Check authoritative PDB prototype first
        pdb_sym = None
        if pdb is not None:
            pdb_sym = pdb.symbols_by_name.get(cfg.name)
            if pdb_sym is None:
                for sym in pdb.symbols_by_rva.values():
                    if sym.name == cfg.name:
                        pdb_sym = sym
                        break

        live_in_regs = self._compute_live_in_gprs(cfg)
        constraints = self._collect_constraints(cfg)
        ret_type, is_void = self._infer_return_type(cfg, constraints)

        if pdb_sym and pdb_sym.return_type:
            ret_type = pdb_sym.return_type
            is_void = ret_type == "void"

        # Determine parameter count under Windows x64 RCX, RDX, R8, R9 convention
        highest_arg_idx = -1
        for idx, reg in enumerate(WIN64_ARGS):
            if reg in live_in_regs or reg in struct_types:
                highest_arg_idx = idx
        if pdb_sym and pdb_sym.param_types:
            highest_arg_idx = max(highest_arg_idx, len(pdb_sym.param_types) - 1)

        params: list[tuple[str, str]] = []
        reg_types: dict[str, str] = {}

        for idx in range(highest_arg_idx + 1):
            reg = WIN64_ARGS[idx]
            pname = f"arg{idx + 1}"
            if pdb_sym and idx < len(pdb_sym.param_types):
                p_name_pdb, p_type_pdb = pdb_sym.param_types[idx]
                pname = p_name_pdb
                ptype = struct_types.get(reg, p_type_pdb)
            elif reg in struct_types:
                ptype = struct_types[reg]
            else:
                ptype = self._resolve_reg_c_type(constraints.get(reg, RegConstraint()))

            params.append((pname, ptype))
            reg_types[reg] = ptype

        return InferredFuncSignature(
            name=cfg.name,
            return_type=ret_type,
            params=params,
            is_void_return=is_void,
            reg_types=reg_types,
        )

    def _compute_live_in_gprs(self, cfg: FunctionCFG) -> set[str]:
        """Standard backward dataflow liveness over CFG basic blocks."""
        if not cfg.blocks:
            return set()

        use_map: dict[int, set[str]] = {}
        def_map: dict[int, set[str]] = {}

        for b_addr, blk in cfg.blocks.items():
            b_use: set[str] = set()
            b_def: set[str] = set()
            for ins in blk.instructions:
                # Zeroing idiom `xor eax, eax` defines RAX without reading old value
                is_zero_xor = (
                    ins.mnemonic == "xor"
                    and len(ins.operands) == 2
                    and ins.operands[0].kind == "reg"
                    and ins.operands[1].kind == "reg"
                    and ins.operands[0].reg == ins.operands[1].reg
                )
                if not is_zero_xor:
                    reads = self._insn_gpr_reads(ins)
                    for r in reads:
                        if r not in b_def:
                            b_use.add(r)

                writes = self._insn_gpr_writes(ins)
                for w in writes:
                    b_def.add(w)

            use_map[b_addr] = b_use
            def_map[b_addr] = b_def

        live_in: dict[int, set[str]] = {b: set() for b in cfg.blocks}
        changed = True
        while changed:
            changed = False
            for b_addr, blk in cfg.blocks.items():
                live_out: set[str] = set()
                for succ in blk.successors:
                    if succ in live_in:
                        live_out |= live_in[succ]
                new_in = use_map[b_addr] | (live_out - def_map[b_addr])
                if new_in != live_in[b_addr]:
                    live_in[b_addr] = new_in
                    changed = True

        return live_in.get(cfg.entry_address, set())

    def _insn_gpr_reads(self, ins) -> set[str]:
        reads: set[str] = set()
        for r in ins.regs_read:
            canon = CANONICAL_GPR64.get(r)
            if canon:
                reads.add(canon)
        for idx, op in enumerate(ins.operands):
            if op.kind == "mem":
                for mr in (op.base, op.index):
                    canon = CANONICAL_GPR64.get(mr or "")
                    if canon:
                        reads.add(canon)
            elif op.kind == "reg" and op.reg:
                canon = CANONICAL_GPR64.get(op.reg)
                if not canon:
                    continue
                if idx > 0 or ins.mnemonic in (
                    "add", "sub", "adc", "sbb", "and", "or", "xor",
                    "cmp", "test", "imul", "shl", "sal", "shr", "sar",
                    "inc", "dec", "neg", "not", "push", "cmove", "cmovne",
                    "cmovl", "cmovg", "cmovle", "cmovge", "cmovb", "cmova",
                ):
                    reads.add(canon)
        if ins.mnemonic in ("mul", "imul", "div", "idiv") and len(ins.operands) == 1:
            reads.add("rax")
            if ins.mnemonic in ("div", "idiv"):
                reads.add("rdx")
        return reads

    def _insn_gpr_writes(self, ins) -> set[str]:
        writes: set[str] = set()
        for r in ins.regs_write:
            canon = CANONICAL_GPR64.get(r)
            if canon:
                writes.add(canon)
        if ins.operands and ins.operands[0].kind == "reg" and ins.operands[0].reg:
            if ins.mnemonic not in ("cmp", "test", "push"):
                canon = CANONICAL_GPR64.get(ins.operands[0].reg)
                if canon:
                    writes.add(canon)
        if ins.mnemonic in ("mul", "imul", "div", "idiv") and len(ins.operands) == 1:
            writes.add("rax")
            writes.add("rdx")
        if ins.mnemonic == "call":
            writes.update({"rax", "rcx", "rdx", "r8", "r9", "r10", "r11"})
        return writes

    def _collect_constraints(self, cfg: FunctionCFG) -> dict[str, RegConstraint]:
        cmap: dict[str, RegConstraint] = {}
        last_cmp_regs: list[str] = []

        for blk in cfg.sorted_blocks():
            for ins in blk.instructions:
                m = ins.mnemonic
                for op in ins.operands:
                    if op.kind == "reg" and op.reg and op.reg in SUBREG_MAP:
                        parent, _, bits = SUBREG_MAP[op.reg]
                        rc = cmap.setdefault(parent, RegConstraint())
                        # Ignore zero-idiom `xor eax, eax` when inferring width
                        is_zero_xor = (
                            m == "xor"
                            and len(ins.operands) == 2
                            and ins.operands[0].reg == ins.operands[1].reg
                        )
                        if not is_zero_xor:
                            rc.max_width_bits = max(rc.max_width_bits, bits)
                    elif op.kind == "mem" and op.base and op.segment is None:
                        canon = CANONICAL_GPR64.get(op.base)
                        if canon and canon not in ("rsp", "rbp"):
                            rc = cmap.setdefault(canon, RegConstraint())
                            rc.is_pointer = True
                            if op.size in (1, 2, 4, 8) and rc.elem_bytes == 0:
                                rc.elem_bytes = op.size

                if m in ("cmp", "test"):
                    last_cmp_regs = [
                        CANONICAL_GPR64[op.reg]
                        for op in ins.operands
                        if op.kind == "reg" and op.reg in CANONICAL_GPR64
                    ]
                elif m in SIGNED_MNEMONICS:
                    targets = (
                        last_cmp_regs
                        if m.startswith(("j", "set", "cmov"))
                        else [
                            CANONICAL_GPR64[op.reg]
                            for op in ins.operands
                            if op.kind == "reg" and op.reg in CANONICAL_GPR64
                        ]
                    )
                    for r in targets:
                        cmap.setdefault(r, RegConstraint()).is_signed = True
                elif m in UNSIGNED_MNEMONICS:
                    targets = (
                        last_cmp_regs
                        if m.startswith(("j", "set", "cmov"))
                        else [
                            CANONICAL_GPR64[op.reg]
                            for op in ins.operands
                            if op.kind == "reg" and op.reg in CANONICAL_GPR64
                        ]
                    )
                    for r in targets:
                        cmap.setdefault(r, RegConstraint()).is_unsigned = True

        return cmap

    def _infer_return_type(
        self, cfg: FunctionCFG, constraints: dict[str, RegConstraint]
    ) -> tuple[str, bool]:
        writes_rax = False
        sets_al_bool = False
        ntstatus_const = False
        rax_max_write_bits = 0

        for blk in cfg.sorted_blocks():
            for ins in blk.instructions:
                if "rax" in self._insn_gpr_writes(ins):
                    writes_rax = True
                    if ins.operands and ins.operands[0].kind == "reg" and ins.operands[0].reg in SUBREG_MAP:
                        _, _, wbits = SUBREG_MAP[ins.operands[0].reg]
                        is_zero = (
                            ins.mnemonic == "xor"
                            and len(ins.operands) == 2
                            and ins.operands[0].reg == ins.operands[1].reg
                        )
                        if not is_zero:
                            rax_max_write_bits = max(rax_max_write_bits, wbits)
                    if ins.mnemonic.startswith("set") and ins.operands[0].reg == "al":
                        sets_al_bool = True
                    if (
                        ins.mnemonic == "mov"
                        and len(ins.operands) == 2
                        and ins.operands[1].kind == "imm"
                    ):
                        imm32 = ins.operands[1].imm & 0xFFFFFFFF
                        if 0xC0000000 <= imm32 <= 0xCFFFFFFF or 0x80000000 <= imm32 <= 0x8000FFFF:
                            ntstatus_const = True

        if not writes_rax:
            return ("void", True)
        if ntstatus_const:
            return ("NTSTATUS", False)
        if sets_al_bool and rax_max_write_bits <= 8:
            return ("BOOLEAN", False)

        rc = constraints.get("rax", RegConstraint())
        width = rax_max_write_bits or rc.max_width_bits or 64
        signed = rc.is_signed and not rc.is_unsigned
        if width <= 8:
            return ("int8_t" if signed else "uint8_t", False)
        if width <= 16:
            return ("int16_t" if signed else "uint16_t", False)
        if width <= 32:
            return ("int32_t" if signed else "uint32_t", False)
        return ("int64_t" if signed else "uint64_t", False)

    @staticmethod
    def _resolve_reg_c_type(rc: RegConstraint) -> str:
        if rc.is_pointer:
            elem_map = {1: "uint8_t*", 2: "uint16_t*", 4: "uint32_t*", 8: "uint64_t*"}
            if rc.is_signed and not rc.is_unsigned:
                elem_map = {1: "int8_t*", 2: "int16_t*", 4: "int32_t*", 8: "int64_t*"}
            return elem_map.get(rc.elem_bytes, "void*")

        signed = rc.is_signed and not rc.is_unsigned
        w = rc.max_width_bits or 64
        if w <= 8:
            return "int8_t" if signed else "uint8_t"
        if w <= 16:
            return "int16_t" if signed else "uint16_t"
        if w <= 32:
            return "int32_t" if signed else "uint32_t"
        return "int64_t" if signed else "uint64_t"
