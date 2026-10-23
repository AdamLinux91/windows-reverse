"""Tests for compiler de-optimization: 5-branch loop collapse, linear reroll, and magic division."""

from __future__ import annotations

from conftest import assemble_function_cfg
from ntrev.analysis import CompilerDeoptimizer, TypeInferenceEngine, Z3EquivalenceVerifier


def test_five_branch_duffs_fallthrough_loop_collapsed_and_z3_verified() -> None:
    """
    Test Duff's-device fallthrough unrolling: 5 branches cascading through `.L5 -> .L4 -> ... -> .L1`,
    verifying our path-projection algorithm collapses it into 1 clean `for` loop proven by Z3!
    """
    cfg = assemble_function_cfg(
        """
        xor eax, eax
        test edx, edx
        je .L_ret
        cmp edx, 1
        je .L1
        cmp edx, 2
        je .L2
        cmp edx, 3
        je .L3
        cmp edx, 4
        je .L4
    .L5:
        mov r8d, dword ptr [rcx + 16]
        add rax, r8
    .L4:
        mov r8d, dword ptr [rcx + 12]
        add rax, r8
    .L3:
        mov r8d, dword ptr [rcx + 8]
        add rax, r8
    .L2:
        mov r8d, dword ptr [rcx + 4]
        add rax, r8
    .L1:
        mov r8d, dword ptr [rcx]
        add rax, r8
    .L_ret:
        ret
        """,
        func_name="KiDuffCascadeSum",
    )
    sig = TypeInferenceEngine().infer_signature(cfg)
    loops, _ = CompilerDeoptimizer().analyze_function(cfg, sig)

    assert len(loops) == 1
    rloop = loops[0]
    assert rloop.kind == "multi_branch_dispatch"
    assert rloop.max_unroll_factor == 5
    assert "for (uint64_t i = 0; i < bound; ++i)" in rloop.c_loop_code

    proof = Z3EquivalenceVerifier().verify_function(
        cfg=cfg,
        llvm_ir="",
        rerolled_loops=loops,
        param_names=[p for p, _ in sig.params],
    )
    assert proof.status == "VERIFIED_EQUIVALENT"
    assert "loop_reroll_equivalence:multi_branch_dispatch" in proof.verified_stages


def test_linear_strided_xor_unroll_collapsed_and_z3_verified() -> None:
    cfg = assemble_function_cfg(
        """
        mov rax, qword ptr [rcx]
        xor rax, qword ptr [rcx + 8]
        xor rax, qword ptr [rcx + 16]
        xor rax, qword ptr [rcx + 24]
        ret
        """,
        func_name="RtlXorChecksum4",
    )
    sig = TypeInferenceEngine().infer_signature(cfg)
    loops, _ = CompilerDeoptimizer().analyze_function(cfg, sig)

    assert len(loops) == 1
    assert loops[0].kind == "linear_strided"
    assert loops[0].max_unroll_factor == 4
    assert "^=" in loops[0].c_loop_code

    proof = Z3EquivalenceVerifier().verify_function(
        cfg=cfg,
        llvm_ir="",
        rerolled_loops=loops,
        param_names=[p for p, _ in sig.params],
    )
    assert proof.status == "VERIFIED_EQUIVALENT"


def test_magic_multiplier_division_and_65bit_overflow_trick_recovery() -> None:
    # 32-bit unsigned division by 7 using Hacker's Delight 33-bit add-shr trick (`gcc -O2` output for `u32 / 7`)
    cfg_div7 = assemble_function_cfg(
        """
        mov eax, ecx
        mov edx, 0x24924925
        mul edx
        mov eax, ecx
        sub eax, edx
        shr eax, 1
        add eax, edx
        shr eax, 2
        ret
        """,
        func_name="RtlDivU32By7",
    )
    sig = TypeInferenceEngine().infer_signature(cfg_div7)
    _, divs = CompilerDeoptimizer().analyze_function(cfg_div7, sig)

    assert len(divs) == 1
    assert divs[0].divisor == 7
    assert divs[0].bit_width == 32
    assert "/ (uint32_t)7" in divs[0].c_expr

    ok, cex, _ = Z3EquivalenceVerifier().verify_magic_division(cfg_div7, divs[0], return_bits=32)
    assert ok is True, f"Unexpected counterexample: {cex}"
