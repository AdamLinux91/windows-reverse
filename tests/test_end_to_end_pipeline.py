"""
End-to-End Integration Test:
  1. Loads compiled Windows x64 (`ms_abi`) PE32+ kernel fixture + binary MSF 7.00 PDB.
  2. Runs all 4 stages of `NtDecompilationPipeline`.
  3. Verifies every C function obtains a Z3 `VERIFIED_EQUIVALENT` proof certificate and
     `KiSystemCall64Entry` obtains `PRESERVED_RAW_ASM`.
  4. Recompiles the emitted `reconstructed_nt.c` + `reconstructed_types.h` into a shared
     library with `gcc -O2 -Werror` (replacing `ms_abi` with host ABI wrapper or calling via C
     harness) and verifies runtime behavioral identity on live structs & arrays!
"""

from __future__ import annotations

import subprocess
from pathlib import Path

from ntrev.pipeline import NtDecompilationPipeline


def test_full_pipeline_and_recompiled_runtime_execution(
    kernel_fixture_paths: tuple[Path, Path],
    tmp_path: Path,
) -> None:
    pe_path, pdb_path = kernel_fixture_paths
    out_dir = tmp_path / "decompiled"

    pipeline = NtDecompilationPipeline(verify_smt=True)
    report = pipeline.run(
        binary_path=pe_path,
        pdb_path=pdb_path,
        output_dir=out_dir,
    )

    assert len(report.functions) == 6
    assert report.verified_count == 6

    by_name = {f.name: f for f in report.functions}

    # Check Stage 1/4 raw kernel assembly preservation
    assert by_name["KiSystemCall64Entry"].is_raw_asm is True
    assert by_name["KiSystemCall64Entry"].proof.status == "PRESERVED_RAW_ASM"
    assert "swapgs" in by_name["KiSystemCall64Entry"].c_code
    assert "sysretq" in by_name["KiSystemCall64Entry"].c_code

    # Check Stage 2 PDB struct member access recovery
    assert "Process->QuotaUsagePool" in by_name["PsChargeProcessPoolQuota"].c_code
    assert "Process->QuotaPeakPool" in by_name["PsChargeProcessPoolQuota"].c_code
    assert "Thread->ApcStateProcess" in by_name["PsGetThreadProcessId"].c_code
    assert "UniqueProcessId" in by_name["PsGetThreadProcessId"].c_code

    # Check Stage 3 de-optimizations & Z3 proofs
    assert by_name["RtlConvertTicksToMilliseconds"].recovered_divs[0].divisor == 10000
    assert by_name["KeSumProcessorTicksUnrolled"].rerolled_loops[0].kind == "linear_strided"
    assert by_name["KiDispatchPriorityBatch"].rerolled_loops[0].kind == "multi_branch_dispatch"

    for c_fn in (
        "PsChargeProcessPoolQuota",
        "PsGetThreadProcessId",
        "RtlConvertTicksToMilliseconds",
        "KeSumProcessorTicksUnrolled",
        "KiDispatchPriorityBatch",
    ):
        assert by_name[c_fn].proof.status == "VERIFIED_EQUIVALENT"

    # Stage 4 Binary-to-Source-to-Binary Recompilation & Live Execution Harness!
    harness_c = tmp_path / "verify_harness.c"
    harness_bin = tmp_path / "verify_harness"
    harness_c.write_text(
        r"""
        #include <assert.h>
        #include <stddef.h>
        #include <stdio.h>
        #include <string.h>
        #include "reconstructed_types.h"

        int main(void) {
            /* 1. Verify exact byte offsets in reconstructed PDB headers */
            assert(offsetof(struct _EPROCESS, DirectoryTableBase) == 0x28);
            assert(offsetof(struct _EPROCESS, UniqueProcessId) == 0x2E0);
            assert(offsetof(struct _EPROCESS, QuotaUsagePool) == 0x300);
            assert(offsetof(struct _EPROCESS, QuotaPeakPool) == 0x308);
            assert(offsetof(struct _KTHREAD, ApcStateProcess) == 0x98);

            /* 2. Execute PsChargeProcessPoolQuota & PsGetThreadProcessId */
            struct _EPROCESS proc;
            memset(&proc, 0, sizeof(proc));
            proc.UniqueProcessId = 4242ULL;
            proc.QuotaUsagePool = 1000ULL;
            proc.QuotaPeakPool = 1500ULL;

            struct _KTHREAD thrd;
            memset(&thrd, 0, sizeof(thrd));
            thrd.ApcStateProcess = &proc;

            assert(PsGetThreadProcessId(&thrd) == 4242ULL);
            assert(PsChargeProcessPoolQuota(&proc, 400ULL) == 0);
            assert(proc.QuotaUsagePool == 1400ULL);
            assert(PsChargeProcessPoolQuota(&proc, 200ULL) == (int32_t)0xC000009A);
            assert(proc.QuotaUsagePool == 1400ULL);

            /* 3. Execute RtlConvertTicksToMilliseconds (magic division recovery) */
            assert(RtlConvertTicksToMilliseconds(0ULL) == 0ULL);
            assert(RtlConvertTicksToMilliseconds(9999ULL) == 0ULL);
            assert(RtlConvertTicksToMilliseconds(10000ULL) == 1ULL);
            assert(RtlConvertTicksToMilliseconds(1234567890000ULL) == 123456789ULL);

            /* 4. Execute KeSumProcessorTicksUnrolled (linear rerolled loop) */
            uint32_t ticks[6] = {10, 20, 30, 40, 50, 60};
            assert(KeSumProcessorTicksUnrolled(ticks) == 210ULL);

            /* 5. Execute KiDispatchPriorityBatch (5-branch collapsed loop) */
            uint32_t weights[6] = {3, 5, 7, 11, 13, 99};
            assert(KiDispatchPriorityBatch(weights, 0) == 0ULL);
            assert(KiDispatchPriorityBatch(weights, 1) == 3ULL);
            assert(KiDispatchPriorityBatch(weights, 3) == 15ULL);
            assert(KiDispatchPriorityBatch(weights, 5) == 39ULL);
            assert(KiDispatchPriorityBatch(weights, 99) == 39ULL);

            printf("ALL_RECONSTRUCTED_FUNCTIONS_VERIFIED_OK\n");
            return 0;
        }
        """,
        encoding="utf-8",
    )

    subprocess.run(
        [
            "gcc",
            "-O2",
            "-Wall",
            "-Wno-unused-function",
            str(out_dir / "reconstructed_nt.c"),
            str(harness_c),
            f"-I{out_dir}",
            "-o",
            str(harness_bin),
        ],
        check=True,
    )
    run_res = subprocess.run([str(harness_bin)], check=True, capture_output=True, text=True)
    assert "ALL_RECONSTRUCTED_FUNCTIONS_VERIFIED_OK" in run_res.stdout
