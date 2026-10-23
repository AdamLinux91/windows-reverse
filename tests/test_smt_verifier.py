"""Tests for Z3 SMT formal semantic equivalence proofs and counterexample generation."""

from __future__ import annotations

from conftest import assemble_function_cfg
from ntrev.analysis import Z3EquivalenceVerifier
from ntrev.models import RecoveredDivMod


def test_z3_proves_strength_reduced_lea_equals_imul_and_branchless_abs() -> None:
    # Pair 1: `imul rax, rcx, 9` vs `lea rax, [rcx + rcx*8]`
    cfg_imul = assemble_function_cfg("imul rax, rcx, 9\nret", func_name="mul9_imul")
    cfg_lea = assemble_function_cfg("lea rax, [rcx + rcx*8]\nret", func_name="mul9_lea")

    verifier = Z3EquivalenceVerifier()
    proof = verifier.prove_two_x86_cfgs_equivalent(cfg_imul, cfg_lea, return_bits=64)
    assert proof.status == "VERIFIED_EQUIVALENT"
    assert proof.counterexample is None

    # Pair 2: Branching 64-bit `abs(rcx)` vs branchless `cqo; xor rax, rdx; sub rax, rdx`
    cfg_branch_abs = assemble_function_cfg(
        """
        mov rax, rcx
        test rax, rax
        jge .L_pos
        neg rax
    .L_pos:
        ret
        """,
        func_name="abs_branch",
    )
    cfg_branchless_abs = assemble_function_cfg(
        """
        mov rax, rcx
        cqo
        xor rax, rdx
        sub rax, rdx
        ret
        """,
        func_name="abs_branchless",
    )
    proof_abs = verifier.prove_two_x86_cfgs_equivalent(cfg_branch_abs, cfg_branchless_abs)
    assert proof_abs.status == "VERIFIED_EQUIVALENT"


def test_z3_catches_subtle_signed_vs_unsigned_bug_with_counterexample() -> None:
    # `ja` (unsigned >) vs `jg` (signed >) — identical on positive numbers, diverges on negative!
    cfg_unsigned = assemble_function_cfg(
        """
        xor eax, eax
        cmp ecx, edx
        seta al
        ret
        """,
        func_name="gt_unsigned",
    )
    cfg_signed = assemble_function_cfg(
        """
        xor eax, eax
        cmp ecx, edx
        setg al
        ret
        """,
        func_name="gt_signed",
    )
    proof = Z3EquivalenceVerifier().prove_two_x86_cfgs_equivalent(
        cfg_unsigned, cfg_signed, return_bits=8
    )
    assert proof.status == "COUNTEREXAMPLE_FOUND"
    assert proof.counterexample is not None
    assert proof.counterexample["x86_output"] != proof.counterexample["reconstructed_output"]


def test_z3_qf_lia_rejects_corrupted_magic_divisor_constant() -> None:
    cfg_div = assemble_function_cfg(
        """
        movabs rax, 0x346dc5d63886594b
        mul rcx
        mov rax, rdx
        shr rax, 11
        ret
        """,
        func_name="TicksDiv10000",
    )
    # Intentionally corrupt divisor 10000 -> 10001 and verify Z3 catches it immediately!
    wrong_div = RecoveredDivMod(
        insn_addrs=[cfg_div.address],
        dividend_reg="rcx",
        dividend_c_expr="Ticks",
        divisor=10001,
        is_signed=False,
        is_modulo=False,
        bit_width=64,
        magic_constant=0x346DC5D63886594B,
        shift_amount=11,
        c_expr="(Ticks / 10001)",
    )
    ok, cex, _ = Z3EquivalenceVerifier().verify_magic_division(cfg_div, wrong_div)
    assert ok is False
    assert cex is not None
    assert cex["x86_output"] != cex["reconstructed_output"]
