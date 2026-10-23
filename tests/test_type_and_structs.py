"""Tests for Type Inference, KPCR gs:[0x188], CONTAINING_RECORD, and struct synthesis."""

from __future__ import annotations

from conftest import assemble_function_cfg
from ntrev.analysis import TypeInferenceEngine
from ntrev.symbols import StructuralMapper


def test_kpcr_gs_current_thread_and_chained_eprocess_resolution() -> None:
    cfg = assemble_function_cfg(
        """
        mov rax, qword ptr gs:[0x188]
        mov rax, qword ptr [rax + 0x98]
        mov rax, qword ptr [rax + 0x2e0]
        ret
        """,
        func_name="PsGetCurrentProcessIdFromPcr",
    )
    mapper = StructuralMapper()
    refs, new_structs, _ = mapper.map_function_structures(cfg)

    assert not new_structs
    exprs = [r.c_expr for r in refs]
    assert "KeGetCurrentThread()" in exprs
    assert "(KeGetCurrentThread())->ApcStateProcess" in exprs or "KeGetCurrentThread()->ApcStateProcess" in exprs
    assert any("UniqueProcessId" in e for e in exprs)


def test_containing_record_negative_offset_list_entry_resolution() -> None:
    # _EPROCESS.ActiveProcessLinks is at +0x2E8 -> CONTAINING_RECORD subtracts 0x2E8!
    cfg = assemble_function_cfg(
        """
        lea rax, [rcx - 0x2e8]
        mov rax, qword ptr [rax + 0x2e0]
        ret
        """,
        func_name="PsEntryToProcessId",
    )
    mapper = StructuralMapper()
    refs, _, _ = mapper.map_function_structures(cfg)

    assert len(refs) == 2
    assert refs[0].is_containing_record is True
    assert "CONTAINING_RECORD(arg1, struct _EPROCESS, ActiveProcessLinks)" in refs[0].c_expr
    assert "UniqueProcessId" in refs[1].c_expr


def test_synthetic_struct_reconstruction_for_stripped_binary() -> None:
    cfg = assemble_function_cfg(
        """
        mov eax, dword ptr [rcx + 0x00]
        add eax, dword ptr [rcx + 0x04]
        mov qword ptr [rcx + 0x10], rax
        ret
        """,
        func_name="StrippedUpdateCtx",
    )
    mapper = StructuralMapper(pdb=None)
    refs, new_structs, param_structs = mapper.map_function_structures(cfg)

    assert len(new_structs) == 1
    recon = new_structs[0]
    assert recon.source == "reconstructed"
    assert recon.size == 0x18
    assert [f.offset for f in recon.fields] == [0x00, 0x04, 0x10]
    assert "rcx" in param_structs


def test_ntstatus_and_signedness_type_inference() -> None:
    cfg = assemble_function_cfg(
        """
        cmp ecx, edx
        jl .L_err
        xor eax, eax
        ret
    .L_err:
        mov eax, 0xc000000d
        ret
        """,
        func_name="NtValidateBounds",
    )
    sig = TypeInferenceEngine().infer_signature(cfg)
    assert sig.return_type == "NTSTATUS"
    assert len(sig.params) == 2
    # `jl` implies signed 32-bit comparison on `ecx` and `edx`!
    assert sig.params[0][1] == "int32_t"
    assert sig.params[1][1] == "int32_t"
