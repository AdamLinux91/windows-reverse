"""Core data models shared across the 4-stage decompilation & equivalence pipeline."""

from __future__ import annotations

from dataclasses import asdict, dataclass, field
from typing import Any


@dataclass
class InsnOperand:
    """Normalized x86-64 instruction operand."""

    kind: str  # "reg" | "mem" | "imm"
    size: int = 8  # Size in bytes (1, 2, 4, 8, 16)
    reg: str | None = None
    imm: int = 0
    base: str | None = None
    index: str | None = None
    scale: int = 1
    disp: int = 0
    segment: str | None = None


@dataclass
class DisasmInsn:
    """Single disassembled machine instruction with semantic operand metadata."""

    address: int
    size: int
    raw_bytes: bytes
    mnemonic: str
    op_str: str
    operands: list[InsnOperand] = field(default_factory=list)
    regs_read: list[str] = field(default_factory=list)
    regs_write: list[str] = field(default_factory=list)
    groups: list[str] = field(default_factory=list)
    is_privileged: bool = False

    @property
    def end_address(self) -> int:
        return self.address + self.size

    @property
    def text(self) -> str:
        return f"{self.mnemonic} {self.op_str}".strip()


@dataclass
class BasicBlock:
    """Control-Flow Graph basic block."""

    address: int
    instructions: list[DisasmInsn] = field(default_factory=list)
    successors: list[int] = field(default_factory=list)
    predecessors: list[int] = field(default_factory=list)
    fallthrough: int | None = None
    branch_target: int | None = None
    is_raw_asm_block: bool = False
    raw_asm_reason: str | None = None

    @property
    def end_address(self) -> int:
        if not self.instructions:
            return self.address
        return self.instructions[-1].end_address


@dataclass
class AsmBlockClassification:
    """Result of classifying whether a function or block is non-C kernel assembly."""

    is_raw_asm: bool = False
    has_inline_asm_blocks: bool = False
    reasons: list[str] = field(default_factory=list)
    privileged_instructions: list[str] = field(default_factory=list)
    inline_block_addrs: list[int] = field(default_factory=list)
    inputs: list[str] = field(default_factory=list)
    outputs: list[str] = field(default_factory=list)
    clobbers: list[str] = field(default_factory=list)


@dataclass
class FunctionCFG:
    """Disassembled function Control-Flow Graph."""

    name: str
    address: int
    size: int
    entry_address: int
    blocks: dict[int, BasicBlock] = field(default_factory=dict)
    has_pdata_unwind: bool = False
    section_name: str = ".text"
    asm_classification: AsmBlockClassification = field(default_factory=AsmBlockClassification)

    def sorted_blocks(self) -> list[BasicBlock]:
        return [self.blocks[k] for k in sorted(self.blocks.keys())]

    def all_instructions(self) -> list[DisasmInsn]:
        insns: list[DisasmInsn] = []
        for blk in self.sorted_blocks():
            insns.extend(blk.instructions)
        return insns


@dataclass
class PESectionInfo:
    """Loaded PE32+ or ELF64 section descriptor."""

    name: str
    virtual_address: int  # RVA
    virtual_size: int
    raw_data: bytes
    is_executable: bool
    is_writable: bool


@dataclass
class RuntimeFunctionEntry:
    """Windows x64 .pdata RUNTIME_FUNCTION entry (SEH unwind bounds)."""

    begin_rva: int
    end_rva: int
    unwind_info_rva: int
    prolog_size: int = 0
    unwind_flags: int = 0


@dataclass
class CodeViewRSDS:
    """PE Debug Directory RSDS CodeView PDB70 signature."""

    guid: str
    age: int
    pdb_filename: str

    @property
    def symbol_server_url(self) -> str:
        from pathlib import PureWindowsPath

        leaf = PureWindowsPath(self.pdb_filename).name
        clean_guid = self.guid.replace("-", "").upper()
        return f"https://msdl.microsoft.com/download/symbols/{leaf}/{clean_guid}{self.age:X}/{leaf}"


@dataclass
class BinaryImage:
    """Parsed Windows PE32+ (ntoskrnl.exe / driver) or x86-64 object image."""

    path: str
    format: str  # "PE32+" | "ELF64"
    image_base: int
    entry_point_rva: int
    machine: str
    sections: list[PESectionInfo] = field(default_factory=list)
    exports: dict[int, str] = field(default_factory=dict)  # RVA -> name
    imports: dict[int, str] = field(default_factory=dict)  # IAT VA -> module!func
    pdata_functions: list[RuntimeFunctionEntry] = field(default_factory=list)
    rsds: CodeViewRSDS | None = None

    def read_va(self, va: int, length: int) -> bytes | None:
        rva = va - self.image_base
        for sec in self.sections:
            if sec.virtual_address <= rva < sec.virtual_address + len(sec.raw_data):
                off = rva - sec.virtual_address
                return sec.raw_data[off : off + length]
        return None


@dataclass
class StructField:
    """Member field of a PDB or reconstructed C structure."""

    name: str
    offset: int
    size: int
    c_type: str
    is_pointer: bool = False
    pointee_type: str | None = None
    array_len: int | None = None
    bit_offset: int | None = None
    bit_size: int | None = None


@dataclass
class StructDef:
    """Complete C struct/union layout recovered from PDB, Ghidra, or access analysis."""

    name: str
    size: int
    fields: list[StructField] = field(default_factory=list)
    typedef_name: str | None = None
    pointer_typedef: str | None = None
    is_union: bool = False
    source: str = "pdb"  # "pdb" | "nt_builtin" | "reconstructed" | "ghidra"

    def field_at_offset(self, offset: int) -> StructField | None:
        for f in self.fields:
            if f.offset == offset:
                return f
        # Check containment inside sub-field
        for f in sorted(self.fields, key=lambda x: x.offset):
            if f.offset <= offset < f.offset + max(f.size, 1):
                return f
        return None


@dataclass
class PDBSymbol:
    """Public or procedure symbol from PDB DBI/Publics or Ghidra export."""

    name: str
    rva: int
    size: int = 0
    section: int = 1
    is_function: bool = True
    return_type: str | None = None
    param_types: list[tuple[str, str]] = field(default_factory=list)


@dataclass
class PDBDatabase:
    """Parsed PDB v7.00 symbol & structural type database."""

    pdb_path: str
    guid: str = "00000000-0000-0000-0000-000000000000"
    age: int = 1
    structs: dict[str, StructDef] = field(default_factory=dict)
    symbols_by_rva: dict[int, PDBSymbol] = field(default_factory=dict)
    symbols_by_name: dict[str, PDBSymbol] = field(default_factory=dict)


@dataclass
class FieldAccessRef:
    """Mapped memory operand resolved to a typed C struct field expression."""

    insn_addr: int
    base_reg: str
    disp: int
    access_size: int
    struct_name: str
    field_name: str
    field_c_type: str
    c_expr: str
    is_write: bool = False
    is_kpcr_gs: bool = False
    is_containing_record: bool = False


@dataclass
class InferredFuncSignature:
    """Recovered C function prototype under Windows x64 (__fastcall / ms_abi) convention."""

    name: str
    return_type: str = "uint64_t"
    params: list[tuple[str, str]] = field(default_factory=list)  # (param_name, c_type)
    calling_convention: str = "__attribute__((ms_abi))"
    reg_types: dict[str, str] = field(default_factory=dict)
    is_void_return: bool = False

    def format_prototype(self, include_abi: bool = True) -> str:
        abi = f"{self.calling_convention} " if include_abi and self.calling_convention else ""
        if not self.params:
            param_str = "void"
        else:
            param_str = ", ".join(f"{ctype} {pname}" for pname, ctype in self.params)
        return f"{self.return_type} {abi}{self.name}({param_str})"


@dataclass
class RerolledLoop:
    """Collapsed loop recovered from linear unrolling or multi-branch compiler unrolling."""

    kind: str  # "linear_strided" | "multi_branch_dispatch"
    header_addr: int
    trip_count_expr: str
    max_unroll_factor: int
    induction_var: str
    base_expr: str
    elem_c_type: str
    stride_bytes: int
    op_kind: str  # "add" | "xor" | "or" | "and" | "store_const" | "copy"
    accumulator_init: str
    body_c_code: str
    c_loop_code: str
    covered_addrs: list[int] = field(default_factory=list)
    replaces_entire_function: bool = False
    description: str = ""


@dataclass
class RecoveredDivMod:
    """Recovered integer division/modulo from compiler magic-multiplier strength reduction."""

    insn_addrs: list[int]
    dividend_reg: str
    dividend_c_expr: str
    divisor: int
    is_signed: bool
    is_modulo: bool
    bit_width: int
    magic_constant: int
    shift_amount: int
    c_expr: str


@dataclass
class EquivalenceProof:
    """Z3 Theorem Prover formal semantic equivalence verification certificate."""

    function_name: str
    status: str  # "VERIFIED_EQUIVALENT" | "COUNTEREXAMPLE_FOUND" | "PRESERVED_RAW_ASM"
    solver: str = "Z3"
    bit_width: int = 64
    num_assertions: int = 0
    solve_time_ms: float = 0.0
    verified_stages: list[str] = field(default_factory=list)
    counterexample: dict[str, str] | None = None
    notes: str = ""

    def to_dict(self) -> dict[str, Any]:
        return asdict(self)


@dataclass
class DecompiledFunction:
    """Complete decompiled & Z3-verified output artifact for a single binary function."""

    name: str
    address: int
    size: int
    signature: InferredFuncSignature
    is_raw_asm: bool
    has_inline_asm: bool
    raw_llvm_ir: str
    optimized_llvm_ir: str
    c_code: str
    field_accesses: list[FieldAccessRef] = field(default_factory=list)
    rerolled_loops: list[RerolledLoop] = field(default_factory=list)
    recovered_divs: list[RecoveredDivMod] = field(default_factory=list)
    proof: EquivalenceProof = field(
        default_factory=lambda: EquivalenceProof(function_name="", status="UNVERIFIED")
    )


@dataclass
class DecompilationReport:
    """Full binary decompilation deliverable: header, source, LLVM IR, and Z3 proof manifest."""

    binary_path: str
    pdb_path: str | None
    image_base: int
    functions: list[DecompiledFunction] = field(default_factory=list)
    reconstructed_structs: list[StructDef] = field(default_factory=list)
    header_code: str = ""
    source_code: str = ""
    combined_llvm_ir: str = ""

    @property
    def verified_count(self) -> int:
        return sum(
            1
            for f in self.functions
            if f.proof.status in ("VERIFIED_EQUIVALENT", "PRESERVED_RAW_ASM")
        )
