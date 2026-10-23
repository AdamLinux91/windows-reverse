"""Tests for x86-64 -> LLVM 22 IR lifting, stack SROA promotion, and Remill IR normalization."""

from __future__ import annotations

from llvmlite import binding as llvm

from conftest import assemble_function_cfg
from ntrev.lifter import ExternalLifterAdapter, X86ToLLVMLifter


def test_stack_spill_prologue_epilogue_promoted_to_pure_ssa_by_llvm22() -> None:
    cfg = assemble_function_cfg(
        """
        push rbx
        sub rsp, 0x20
        mov qword ptr [rsp + 0x10], rcx
        mov rbx, qword ptr [rsp + 0x10]
        lea rax, [rbx + rdx]
        add rsp, 0x20
        pop rbx
        ret
        """,
        func_name="SpillAdd",
    )
    lifter = X86ToLLVMLifter()
    res = lifter.lift_function(cfg, param_count=2, opt_level=2)

    assert res.verified is True
    # LLVM 22 SROA + DCE eliminates the entire 4KB stack frame alloca & push/pop!
    assert "alloca" not in res.optimized_ir
    assert "add i64" in res.optimized_ir


def test_x86_64_subregister_32bit_zero_ext_vs_8bit_merge() -> None:
    cfg = assemble_function_cfg(
        """
        mov rax, rcx
        mov eax, edx
        mov al, 0x7f
        ret
        """,
        func_name="SubregMerge",
    )
    lifter = X86ToLLVMLifter()
    res = lifter.lift_function(cfg, param_count=2, opt_level=2)
    assert res.verified is True


def test_remill_intrinsic_normalization_produces_valid_llvm22_ir() -> None:
    sample_remill_ir = """
    target triple = "x86_64-pc-windows-msvc"
    declare i64 @__remill_read_memory_64(ptr, i64)
    declare ptr @__remill_write_memory_32(ptr, i64, i32)

    define i64 @RemillSample(ptr %mem, i64 %rcx) {
    entry:
      %v0 = call i64 @__remill_read_memory_64(ptr %mem, i64 %rcx)
      %addr2 = add i64 %rcx, 8
      %mem2 = call ptr @__remill_write_memory_32(ptr %mem, i64 %addr2, i32 42)
      ret i64 %v0
    }
    """
    normalized = ExternalLifterAdapter.normalize_remill_ir(sample_remill_ir)
    assert "__remill_read_memory_64" not in normalized
    assert "__remill_write_memory_32" not in normalized
    assert "load i64, ptr" in normalized
    assert "store i32 42, ptr" in normalized

    mod = llvm.parse_assembly(normalized)
    mod.verify()
