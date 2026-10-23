/*
 * Reconstructed Windows NT Kernel Subsystem: File System Runtime Library, MCBs, Locks & Oplocks (FsRtl)
 * Source Binary : ntoskrnl.exe (10.0.16299.15 x64, RSDS 83DB4240-4EFD-4AB6-AFB6-FA864B700CB3)
 * Functions     : 24 Z3-verified / assembly-preserved routines
 */
#include "../../include/nt_reconstructed_types.h"

/*
 * Function : FsRtlInitializeOplock @ 0x14058ca50 (3 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.06 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) FsRtlInitializeOplock(void) {
    return;
}

/*
 * Function : FsRtlSyncVolumes @ 0x14057aa20 (3 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.67 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) FsRtlSyncVolumes(void) {
    return (uint64_t)(0ULL);
}

/*
 * Function : FsRtlAcknowledgeEcp @ 0x1405bbb80 (5 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.21 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) FsRtlAcknowledgeEcp(uint32_t* arg1) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0xffffffffffffffe8ULL)))));
    (*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0xffffffffffffffe8ULL)))) = (uint32_t)(((uint32_t)(ssa_mem_ld) | (uint32_t)(8ULL)));
    return;
}

/*
 * Function : FsRtlPrepareToReuseEcp @ 0x1405dcde0 (5 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.13 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) FsRtlPrepareToReuseEcp(uint32_t* arg1) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0xffffffffffffffe8ULL)))));
    (*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0xffffffffffffffe8ULL)))) = (uint32_t)(((uint32_t)(ssa_mem_ld) & (uint32_t)(0xfffffffffffffff7ULL)));
    return;
}

/*
 * Function : FsRtlResetBaseMcb @ 0x140115ff0 (5 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.87 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) FsRtlResetBaseMcb(uint32_t* arg1) {
    (*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(4ULL)))) = (uint32_t)(0ULL);
    return;
}

/*
 * Function : FsRtlAreVolumeStartupApplicationsComplete @ 0x140579130 (7 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.04 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint8_t __attribute__((ms_abi)) FsRtlAreVolumeStartupApplicationsComplete(void) {
    uint8_t __attribute__((unused)) ssa_mem_ld = (uint8_t)((*(uint8_t*)(uintptr_t)(((uint64_t)(uintptr_t)&FsRtlpVolumeStartupApplicationsComplete))));
    return (uint8_t)(((uint64_t)(uint8_t)(ssa_mem_ld)));
}

/*
 * Function : FsRtlRegisterFltMgrCalls @ 0x1405de980 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.17 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) FsRtlRegisterFltMgrCalls(uint64_t arg1) {
    (*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)&FltMgrCallbacks))) = (uint64_t)(((uint64_t)arg1));
    return;
}

/*
 * Function : FsRtlRegisterMupCalls @ 0x1405de970 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.36 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) FsRtlRegisterMupCalls(uint64_t arg1) {
    (*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)&pFsRtlpMupCalls))) = (uint64_t)(((uint64_t)arg1));
    return;
}

/*
 * Function : FsRtlIsEcpAcknowledged @ 0x14055d030 (9 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.91 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) FsRtlIsEcpAcknowledged(uint32_t* arg1) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0xffffffffffffffe8ULL)))));
    return (uint32_t)(((uint64_t)(uint32_t)(((uint32_t)(((uint32_t)(ssa_mem_ld) >> (uint32_t)(3ULL))) & (uint32_t)(0x1fffff01ULL)))));
}

/*
 * Function : FsRtlIsEcpFromUserMode @ 0x14055b7e0 (9 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.02 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) FsRtlIsEcpFromUserMode(uint32_t* arg1) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0xffffffffffffffe8ULL)))));
    return (uint32_t)(((uint64_t)(uint32_t)(((uint32_t)(((uint32_t)(ssa_mem_ld) >> (uint32_t)(4ULL))) & (uint32_t)(0xfffff01ULL)))));
}

/*
 * Function : FsRtlLookupReservedPerFileContext @ 0x140007490 (14 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (6.77 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) FsRtlLookupReservedPerFileContext(uint64_t* arg1) {
    uint64_t __attribute__((unused)) ssa_common_ret_op = 0;
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))));
    if (((uint64_t)(ssa_mem_ld) == (uint64_t)(0ULL))) {
        ssa_common_ret_op = (uint64_t)(0ULL);
    } else {
        uint64_t __attribute__((unused)) ssa_mem_ld_1 = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld) + (uint64_t)(0x18ULL)))));
        ssa_common_ret_op = (uint64_t)(ssa_mem_ld_1);
    }
    return (uint64_t)(ssa_common_ret_op);
}

/*
 * Function : FsRtlMdlReadCompleteDev @ 0x14010aa30 (16 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.77 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint8_t __attribute__((ms_abi)) FsRtlMdlReadCompleteDev(void) {
    uint64_t __attribute__((unused)) ssa_call_res = CcMdlReadComplete2(0ULL, 0ULL, 0ULL, 0ULL);
    return (uint8_t)(((uint64_t)(((uint64_t)(ssa_call_res) & (uint64_t)(0xffffffffffffff00ULL))) | (uint64_t)(1ULL)));
}

/*
 * Function : FsRtlInitializeFileLock @ 0x14013d4e0 (20 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (6.28 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) FsRtlInitializeFileLock(struct _RECON_FSRTLINITIALIZEFILELOCK_RCX* arg1, uint64_t arg2, uint64_t arg3) {
    arg1->field_0x0 = (uint64_t)(((uint64_t)arg2));
    arg1->field_0x18 = (uint64_t)(0ULL);
    arg1->field_0x10 = (uint8_t)(0ULL);
    arg1->field_0x8 = (uint64_t)(((uint64_t)arg3));
    arg1->field_0x58 = (uint32_t)(0ULL);
    return (uint64_t)(0ULL);
}

/*
 * Function : FsRtlAddMcbEntry @ 0x1401e0530 (22 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.96 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) FsRtlAddMcbEntry(uint64_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4) {
    uint64_t __attribute__((unused)) ssa_call_res = FsRtlAddLargeMcbEntry(((uint64_t)arg1), ((uint64_t)(((uint64_t)arg2)) & (uint64_t)(0xffffffffULL)), ((uint64_t)(((uint64_t)arg3)) & (uint64_t)(0xffffffffULL)), ((uint64_t)(((uint64_t)arg4)) & (uint64_t)(0xffffffffULL)));
    return (uint64_t)(ssa_call_res);
}

/*
 * Function : FsRtlGetEcpListFromIrp @ 0x1404f2cf0 (23 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (13.89 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
NTSTATUS __attribute__((ms_abi)) FsRtlGetEcpListFromIrp(PIRP arg1, uint64_t* arg2) {
    uint64_t __attribute__((unused)) ssa_common_ret_op = 0;
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)(arg1->Flags);
    if (((uint32_t)(((uint32_t)(ssa_mem_ld) & (uint32_t)(0x80ULL))) == (uint32_t)(0ULL))) {
        ssa_common_ret_op = (uint64_t)(0xc000000dULL);
    } else {
        uint64_t __attribute__((unused)) ssa_mem_ld_1 = (uint64_t)(uintptr_t)(arg1->UserBuffer);
        (*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg2))) = (uint64_t)(ssa_mem_ld_1);
        ssa_common_ret_op = (uint64_t)(0ULL);
    }
    return (NTSTATUS)(ssa_common_ret_op);
}

/*
 * Function : FsRtlAreThereWaitingFileLocks @ 0x1401e0c40 (24 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (6.49 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
BOOLEAN __attribute__((ms_abi)) FsRtlAreThereWaitingFileLocks(PKDPC arg1) {
    uint64_t __attribute__((unused)) ssa_reg_rax_0 = 0;
    if (((uint64_t)(((uint64_t)arg1)) == (uint64_t)(0ULL))) {
        ssa_reg_rax_0 = (uint64_t)(0ULL);
        goto L_bb_1401e0c57;
    }
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)(uintptr_t)(arg1->DeferredRoutine);
    if (((uint64_t)(ssa_mem_ld) == (uint64_t)(0ULL))) {
        ssa_reg_rax_0 = (uint64_t)(0ULL);
        goto L_bb_1401e0c57;
    }
    uint64_t __attribute__((unused)) ssa_mem_ld_1 = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld) + (uint64_t)(0x30ULL)))));
    ssa_reg_rax_0 = (uint64_t)(((uint64_t)(uint8_t)(((uint64_t)(ssa_mem_ld_1) != (uint64_t)(0ULL)))));
L_bb_1401e0c57:
    return (BOOLEAN)(ssa_reg_rax_0);
}

/*
 * Function : FsRtlRemoveReservedPerFileContext @ 0x1401e2410 (26 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (14.88 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) FsRtlRemoveReservedPerFileContext(uint64_t* arg1) {
    uint64_t __attribute__((unused)) ssa_common_ret_op = 0;
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))));
    if (((uint64_t)(ssa_mem_ld) == (uint64_t)(0ULL))) {
        ssa_common_ret_op = (uint64_t)(0ULL);
        goto L_common_ret;
    }
    goto L_bb_1401e241b;
L_common_ret:
    return (uint64_t)(ssa_common_ret_op);
L_bb_1401e241b:
    uint64_t __attribute__((unused)) ssa_mem_ld_1 = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld) + (uint64_t)(0x18ULL)))));
    if (((uint64_t)(ssa_mem_ld_1) == (uint64_t)(0ULL))) {
        ssa_common_ret_op = (uint64_t)(0ULL);
        goto L_common_ret;
    }
    (*(uint64_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld) + (uint64_t)(0x18ULL)))) = (uint64_t)(0ULL);
    ssa_common_ret_op = (uint64_t)(ssa_mem_ld_1);
    goto L_common_ret;
}

/*
 * Function : FsRtlFastUnlockAll @ 0x1400f6270 (27 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.40 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) FsRtlFastUnlockAll(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4) {
    uint64_t __attribute__((unused)) ssa_call_res = FsRtlPrivateFastUnlockAll(((uint64_t)arg1), ((uint64_t)arg2), ((uint64_t)arg3), 0ULL);
    return (uint64_t)(ssa_call_res);
}

/*
 * Function : FsRtlAllocateResource @ 0x1406a67a0 (31 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (11.51 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) FsRtlAllocateResource(void) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)&FsRtlPagingIoResourceSelector))));
    (*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)&FsRtlPagingIoResourceSelector))) = (uint32_t)(((uint32_t)(ssa_mem_ld) + (uint32_t)(1ULL)));
    uint64_t __attribute__((unused)) ssa_mem_ld_1 = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)&FsRtlPagingIoResources))));
    return (uint64_t)(((uint64_t)(ssa_mem_ld_1) + (uint64_t)(((uint64_t)(uint32_t)(((uint32_t)(((uint32_t)(ssa_mem_ld) & (uint32_t)(15ULL))) * (uint32_t)(0x68ULL)))))));
}

/*
 * Function : FsRtlIsDaxVolume @ 0x1401e04a0 (33 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (16.90 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) FsRtlIsDaxVolume(uint64_t* arg1) {
    uint64_t __attribute__((unused)) ssa_common_ret_op = 0;
    uint64_t __attribute__((unused)) ssa_reg_rax_0 = 0;
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x10ULL)))));
    if (((uint64_t)(ssa_mem_ld) == (uint64_t)(0ULL))) {
        ssa_reg_rax_0 = (uint64_t)(0ULL);
        goto L_bb_1401e04be;
    }
    uint64_t __attribute__((unused)) ssa_mem_ld_1 = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld) + (uint64_t)(8ULL)))));
    if (((uint64_t)(ssa_mem_ld_1) == (uint64_t)(0ULL))) {
        ssa_reg_rax_0 = (uint64_t)(0ULL);
        goto L_bb_1401e04be;
    }
    uint32_t __attribute__((unused)) ssa_mem_ld_2 = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld_1) + (uint64_t)(0x30ULL)))));
    uint64_t __attribute__((unused)) ssa__56 = (uint64_t)(((uint64_t)(uint32_t)(ssa_mem_ld_2)));
    if (((uint32_t)(((uint32_t)(ssa_mem_ld_2) & (uint32_t)(0x10000000ULL))) == (uint32_t)(0ULL))) {
        ssa_reg_rax_0 = (uint64_t)(ssa__56);
        goto L_bb_1401e04be;
    }
    goto L_bb_1401e04bb;
L_common_ret:
    return (uint64_t)(ssa_common_ret_op);
L_bb_1401e04bb:
    ssa_common_ret_op = (uint64_t)(((uint64_t)(((uint64_t)(ssa__56) & (uint64_t)(0xffffff00ULL))) | (uint64_t)(1ULL)));
    goto L_common_ret;
L_bb_1401e04be:
    ssa_common_ret_op = (uint64_t)(((uint64_t)(ssa_reg_rax_0) & (uint64_t)(0xffffff00ULL)));
    goto L_common_ret;
}

/*
 * Function : FsRtlSetEcpListIntoIrp @ 0x1405527d0 (33 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (12.54 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
NTSTATUS __attribute__((ms_abi)) FsRtlSetEcpListIntoIrp(PIRP arg1, uint64_t arg2) {
    uint64_t __attribute__((unused)) ssa_common_ret_op = 0;
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)(arg1->Flags);
    if (((uint32_t)(((uint32_t)(ssa_mem_ld) & (uint32_t)(0x80ULL))) == (uint32_t)(0ULL))) {
        ssa_common_ret_op = (uint64_t)(0xc00000efULL);
        goto L_common_ret;
    }
    uint64_t __attribute__((unused)) ssa_mem_ld_1 = (uint64_t)(uintptr_t)(arg1->UserBuffer);
    if (((uint64_t)(ssa_mem_ld_1) == (uint64_t)(0ULL))) {
        goto L_bb_1405527de;
    }
    ssa_common_ret_op = (uint64_t)(0xc00000f0ULL);
L_common_ret:
    return (NTSTATUS)(ssa_common_ret_op);
L_bb_1405527de:
    arg1->UserBuffer = (void*)(uintptr_t)(((uint64_t)arg2));
    ssa_common_ret_op = (uint64_t)(0ULL);
    goto L_common_ret;
}

/*
 * Function : FsRtlIsTotalDeviceFailure @ 0x1400d6380 (34 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (21.96 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint8_t __attribute__((ms_abi)) FsRtlIsTotalDeviceFailure(uint32_t arg1) {
    uint32_t __attribute__((unused)) ssa_r_ecx = (uint32_t)(((uint32_t)(uint64_t)(((uint64_t)arg1))));
    return (uint8_t)(((uint64_t)(uint8_t)(((((uint32_t)(ssa_r_ecx) != (uint32_t)(0xffffffffc0000470ULL))) && (((((uint32_t)(ssa_r_ecx) != (uint32_t)(0xffffffffc000009cULL))) && (((((int32_t)(ssa_r_ecx) < (int32_t)(0ULL))) && (((uint32_t)(ssa_r_ecx) != (uint32_t)(0xffffffffc000003fULL)))))))))));
}

/*
 * Function : FsRtlAreThereCurrentOrInProgressFileLocks @ 0x1401e0c10 (35 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (21.82 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) FsRtlAreThereCurrentOrInProgressFileLocks(PHHIVE arg1) {
    uint8_t __attribute__((unused)) ssa_reg_rdx_0_off0 = 0;
    uint64_t __attribute__((unused)) ssa_reg_rax_1 = 0;
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)(uintptr_t)(arg1->Allocate);
    if (((uint64_t)(ssa_mem_ld) == (uint64_t)(0ULL))) {
        goto L_bb_1401e0c27;
    }
    uint64_t __attribute__((unused)) ssa_mem_ld_1 = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld) + (uint64_t)(0x20ULL)))));
    if (((uint64_t)(ssa_mem_ld_1) == (uint64_t)(0ULL))) {
        goto L_bb_1401e0c21;
    }
    ssa_reg_rdx_0_off0 = (uint8_t)(1ULL);
    ssa_reg_rax_1 = (uint64_t)(ssa_mem_ld);
    goto L_bb_1401e0c30;
L_bb_1401e0c21:
    uint64_t __attribute__((unused)) ssa_mem_ld_2 = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld) + (uint64_t)(0x28ULL)))));
    if (((uint64_t)(ssa_mem_ld_2) == (uint64_t)(0ULL))) {
        goto L_bb_1401e0c27;
    }
    ssa_reg_rdx_0_off0 = (uint8_t)(1ULL);
    ssa_reg_rax_1 = (uint64_t)(ssa_mem_ld);
    goto L_bb_1401e0c30;
L_bb_1401e0c27:
    uint32_t __attribute__((unused)) ssa_mem_ld_3 = (uint32_t)(arg1->DirtyCount);
    ssa_reg_rdx_0_off0 = (uint8_t)(((uint8_t)(uint8_t)(((uint32_t)(ssa_mem_ld_3) != (uint32_t)(0ULL)))));
    ssa_reg_rax_1 = (uint64_t)(((uint64_t)(uint32_t)(ssa_mem_ld_3)));
L_bb_1401e0c30:
    return (uint64_t)(((uint64_t)(((uint64_t)(ssa_reg_rax_1) & (uint64_t)(0xffffffffffffff00ULL))) | (uint64_t)(((uint64_t)(uint8_t)(ssa_reg_rdx_0_off0)))));
}

/*
 * Function : FsRtlInitializeExtraCreateParameterList @ 0x1405d9190 (35 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (6.54 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
NTSTATUS __attribute__((ms_abi)) FsRtlInitializeExtraCreateParameterList(PEXT_SET_PARAMETERS_V0 arg1) {
    uint64_t __attribute__((unused)) ssa_common_ret_op = 0;
    if (((uint64_t)(((uint64_t)arg1)) == (uint64_t)(0ULL))) {
        ssa_common_ret_op = (uint64_t)(0xc000000dULL);
    } else {
        uint64_t __attribute__((unused)) ssa__36 = (uint64_t)(((uint64_t)(((uint64_t)arg1)) + (uint64_t)(8ULL)));
        (*(uint64_t*)((uint8_t*)(uintptr_t)(arg1) + 0x10)) = (uint64_t)(ssa__36);
        arg1->NoWakeTolerance = (int64_t)(ssa__36);
        arg1->Reserved = (uint32_t)(0ULL);
        arg1->Version = (uint32_t)(0x4c706345ULL);
        ssa_common_ret_op = (uint64_t)(0ULL);
    }
    return (NTSTATUS)(ssa_common_ret_op);
}
