"""Tests for non-C NT kernel assembly detection & inline assembly preservation."""

from __future__ import annotations

from conftest import assemble_function_cfg
from ntrev.frontend import AsmBlockDetector
from ntrev.lifter import X86ToLLVMLifter


def test_whole_function_syscall_stub_detected_and_preserved() -> None:
    cfg = assemble_function_cfg(
        """
        swapgs
        mov qword ptr gs:[0x10], rsp
        mov rsp, qword ptr gs:[0x1a8]
        rdmsr
        swapgs
        sysretq
        """,
        func_name="KiSystemCall64Shadow",
    )
    detector = AsmBlockDetector()
    cls = detector.classify_function(cfg)

    assert cls.is_raw_asm is True
    assert any("swapgs" in r for r in cls.reasons)
    assert any("sysretq" in r for r in cls.reasons)

    rendered = detector.format_raw_asm_function(cfg)
    assert "__attribute__((naked)) void KiSystemCall64Shadow(void)" in rendered
    assert "swapgs" in rendered
    assert "sysretq" in rendered


def test_hybrid_c_function_with_inline_invlpg_splices_inline_asm() -> None:
    cfg = assemble_function_cfg(
        """
        test rcx, rcx
        je .L_invalid
        invlpg [rcx]
        xor eax, eax
        ret
    .L_invalid:
        mov eax, 0xc000000d
        ret
        """,
        func_name="MmFlushSingleTlbPage",
    )
    detector = AsmBlockDetector()
    cls = detector.classify_function(cfg)

    # Surrounded by standard ms_abi C control flow + return -> hybrid inline asm, NOT naked whole-func
    assert cls.is_raw_asm is False
    assert cls.has_inline_asm_blocks is True

    lifter = X86ToLLVMLifter()
    lifted = lifter.lift_function(cfg, param_count=1)
    assert lifted.verified is True
    assert "asm sideeffect" in lifted.optimized_ir
    assert "invlpg" in lifted.optimized_ir


def test_pure_c_function_is_not_flagged_as_asm() -> None:
    cfg = assemble_function_cfg(
        """
        lea rax, [rcx + rdx*4 + 16]
        ret
        """,
        func_name="RtlComputeOffset",
    )
    cls = AsmBlockDetector().classify_function(cfg)
    assert cls.is_raw_asm is False
    assert cls.has_inline_asm_blocks is False
