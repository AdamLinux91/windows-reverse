/*
 * Reconstructed Windows NT Kernel Subsystem: Configuration Manager & Kernel Registry Transactions (Cm/Cmp)
 * Source Binary : ntoskrnl.exe (10.0.16299.15 x64, RSDS 83DB4240-4EFD-4AB6-AFB6-FA864B700CB3)
 * Functions     : 21 Z3-verified / assembly-preserved routines
 */
#include "../../include/nt_reconstructed_types.h"

/*
 * Function : CmpTryAcquireIXLockExclusive @ 0x1404b0bfc (7 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.57 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
BOOLEAN __attribute__((ms_abi)) CmpTryAcquireIXLockExclusive(uint32_t* arg1) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))));
    return (BOOLEAN)(((uint64_t)(uint8_t)(((uint32_t)(ssa_mem_ld) == (uint32_t)(0ULL)))));
}

/*
 * Function : CmpCommitDiscardReplacePre @ 0x14068ef80 (10 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.85 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
BOOLEAN __attribute__((ms_abi)) CmpCommitDiscardReplacePre(uint32_t* arg1) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x20ULL)))));
    return (BOOLEAN)(((uint64_t)(uint8_t)(((uint32_t)(ssa_mem_ld) == (uint32_t)(0xffffffffffffffffULL)))));
}

/*
 * Function : CmpGetEffectiveKcbSemantics @ 0x14068f100 (15 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.32 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) CmpGetEffectiveKcbSemantics(PCM_KEY_CONTROL_BLOCK arg1) {
    uint64_t __attribute__((unused)) ssa_common_ret_op = 0;
    uint16_t __attribute__((unused)) ssa_mem_ld = (uint16_t)(arg1->LayerHeight);
    if (((uint16_t)(ssa_mem_ld) == (uint16_t)(0ULL))) {
        ssa_common_ret_op = (uint64_t)(0ULL);
    } else {
        uint8_t __attribute__((unused)) ssa_mem_ld_1 = (uint8_t)(arg1->LayerSemantics);
        ssa_common_ret_op = (uint64_t)(((uint64_t)(uint8_t)(ssa_mem_ld_1)));
    }
    return (uint32_t)(ssa_common_ret_op);
}

/*
 * Function : CmpSetGlobalQuotaAllowed @ 0x140583888 (15 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.13 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) CmpSetGlobalQuotaAllowed(void) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)&CmpGlobalQuota))));
    (*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)&CmpGlobalQuotaAllowed))) = (uint64_t)(ssa_mem_ld);
    return (uint64_t)(ssa_mem_ld);
}

/*
 * Function : CmpTransSilentIgnore @ 0x1400bef00 (15 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.49 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint8_t __attribute__((ms_abi)) CmpTransSilentIgnore(void) {
    uint8_t __attribute__((unused)) ssa_mem_ld = (uint8_t)((*(uint8_t*)(uintptr_t)(((uint64_t)(uintptr_t)&CmpMiniNTBoot))));
    return (uint8_t)(((uint64_t)(uint8_t)(((uint8_t)(ssa_mem_ld) != (uint8_t)(0ULL)))));
}

/*
 * Function : CmGetCallbackVersion @ 0x140680510 (20 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.19 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) CmGetCallbackVersion(uint32_t* arg1, uint32_t* arg2) {
    if (((uint64_t)(((uint64_t)(uintptr_t)arg1)) == (uint64_t)(0ULL))) {
        goto L_bb_14068051c;
    }
    (*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))) = (uint32_t)(1ULL);
L_bb_14068051c:
    if (((uint64_t)(((uint64_t)(uintptr_t)arg2)) == (uint64_t)(0ULL))) {
        goto L_bb_140680523;
    }
    (*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg2))) = (uint32_t)(1ULL);
L_bb_140680523:
    return (uint32_t)(1ULL);
}

/*
 * Function : CmGetBoundTransaction @ 0x14057e9b0 (21 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (6.57 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) CmGetBoundTransaction(uint64_t arg1, uint64_t* arg2) {
    uint64_t __attribute__((unused)) ssa_common_ret_op = 0;
    if (((uint64_t)(((uint64_t)arg1)) == (uint64_t)(0ULL))) {
        goto L_bb_14057e9c2;
    }
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg2)) + (uint64_t)(0x38ULL)))));
    if (((uint64_t)(((uint64_t)(ssa_mem_ld) & (uint64_t)(1ULL))) == (uint64_t)(0ULL))) {
        ssa_common_ret_op = (uint64_t)(ssa_mem_ld);
        goto L_common_ret;
    }
    goto L_bb_14057e9c2;
L_common_ret:
    return (uint64_t)(ssa_common_ret_op);
L_bb_14057e9c2:
    ssa_common_ret_op = (uint64_t)(0ULL);
    goto L_common_ret;
}

/*
 * Function : CmpFlushNotifiesPreCallback @ 0x14068f090 (21 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.59 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) CmpFlushNotifiesPreCallback(uint32_t* arg1, uint8_t* arg2) {
    uint64_t __attribute__((unused)) ssa_common_ret_op = 0;
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x20ULL)))));
    if (((uint32_t)(ssa_mem_ld) == (uint32_t)(0xffffffffffffffffULL))) {
        goto L_bb_14068f09f;
    }
    uint8_t __attribute__((unused)) ssa_mem_ld_1 = (uint8_t)((*(uint8_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg2)) + (uint64_t)(4ULL)))));
    if (((uint8_t)(ssa_mem_ld_1) == (uint8_t)(0ULL))) {
        goto L_bb_14068f09f;
    }
    ssa_common_ret_op = (uint64_t)(0ULL);
L_common_ret:
    return (uint32_t)(ssa_common_ret_op);
L_bb_14068f09f:
    ssa_common_ret_op = (uint64_t)(1ULL);
    goto L_common_ret;
}

/*
 * Function : CmpValidateGlobalFlushControlFlags @ 0x1405de780 (21 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (12.05 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) CmpValidateGlobalFlushControlFlags(void) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)&CmpGlobalFlushControlFlags))));
    if (((uint32_t)(ssa_mem_ld) > (uint32_t)(1ULL))) {
        (*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)&CmpGlobalFlushControlFlags))) = (uint32_t)(0ULL);
    }
    return;
}

/*
 * Function : CmpValueEnumStackGetCurrentValueHive @ 0x1401ddd08 (21 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.44 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) CmpValueEnumStackGetCurrentValueHive(uint16_t* arg1) {
    uint16_t __attribute__((unused)) ssa_mem_ld = (uint16_t)((*(uint16_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(6ULL)))));
    uint64_t __attribute__((unused)) ssa_call_res = (uint64_t)CmpSortedValueEnumStackGetEntryAtLayerHeight((uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1)), (int32_t*)(uintptr_t)(((uint64_t)(uint16_t)(ssa_mem_ld))));
    uint64_t __attribute__((unused)) ssa_mem_ld_1 = (uint64_t)((*(uint64_t*)(uintptr_t)(ssa_call_res)));
    return (uint64_t)(ssa_mem_ld_1);
}

/*
 * Function : CmpComputeKcbConvKey @ 0x1406868d0 (29 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (7.53 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) CmpComputeKcbConvKey(PMM_SESSION_SPACE arg1) {
    uint32_t __attribute__((unused)) ssa_common_ret_op_in = 0;
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)(uintptr_t)(arg1->PagedPoolEnd);
    if (((uint64_t)(ssa_mem_ld) == (uint64_t)(0ULL))) {
        uint64_t __attribute__((unused)) ssa_mem_ld_4 = (uint64_t)(uintptr_t)(arg1->SessionObject);
        uint32_t __attribute__((unused)) ssa_mem_ld_5 = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld_4) + (uint64_t)(8ULL)))));
        ssa_common_ret_op_in = (uint32_t)(ssa_mem_ld_5);
    } else {
        uint32_t __attribute__((unused)) ssa_mem_ld_1 = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld) + (uint64_t)(8ULL)))));
        uint64_t __attribute__((unused)) ssa_mem_ld_2 = (uint64_t)(uintptr_t)(arg1->SessionObject);
        uint32_t __attribute__((unused)) ssa_mem_ld_3 = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld_2) + (uint64_t)(8ULL)))));
        ssa_common_ret_op_in = (uint32_t)(((uint32_t)(ssa_mem_ld_3) + (uint32_t)(((uint32_t)(ssa_mem_ld_1) * (uint32_t)(0x25ULL)))));
    }
    return (uint64_t)(((uint64_t)(uint32_t)(ssa_common_ret_op_in)));
}

/*
 * Function : CmpInitializeKcbStack @ 0x1404e7c30 (33 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.90 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) CmpInitializeKcbStack(uint64_t arg1) {
    uint64_t __attribute__((unused)) ssa_call_res = memset(((uint64_t)arg1), 0ULL, 0x20ULL, 0ULL);
    (*(uint16_t*)(uintptr_t)(((uint64_t)(((uint64_t)arg1)) + (uint64_t)(2ULL)))) = (uint16_t)(0xffffffffffffffffULL);
    return (uint32_t)(0xffffffffULL);
}

/*
 * Function : CmpGetComponentNameAtIndex @ 0x140568a5c (38 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (93.17 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) CmpGetComponentNameAtIndex(uint64_t* arg1, uint32_t* arg2) {
    uint64_t __attribute__((unused)) ssa_common_ret_op = 0;
    if (((uint64_t)(((uint64_t)(((uint64_t)(uintptr_t)arg2)) & (uint64_t)(0xfffffff8ULL))) == (uint64_t)(0ULL))) {
        ssa_common_ret_op = (uint64_t)(((uint64_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x20ULL))) + (uint64_t)(((uint64_t)(((uint64_t)(((uint64_t)(uintptr_t)arg2)) << (uint64_t)(4ULL))) & (uint64_t)(0x70ULL)))));
    } else {
        uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0xa0ULL)))));
        ssa_common_ret_op = (uint64_t)(((uint64_t)(((uint64_t)(((uint64_t)(((uint64_t)(uint32_t)(((uint32_t)(((uint32_t)(uint64_t)(((uint64_t)(uintptr_t)arg2)))) + (uint32_t)(0xfffffffffffffff8ULL))))) << (uint64_t)(4ULL))) + (uint64_t)(0x60ULL))) + (uint64_t)(ssa_mem_ld)));
    }
    return (uint64_t)(ssa_common_ret_op);
}

/*
 * Function : CmpKeyNodeStackGetEntryAtLayerHeight @ 0x14068f1c4 (38 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (80.04 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
int64_t __attribute__((ms_abi)) CmpKeyNodeStackGetEntryAtLayerHeight(uint64_t* arg1, int32_t* arg2) {
    uint64_t __attribute__((unused)) ssa_common_ret_op = 0;
    uint16_t __attribute__((unused)) ssa_r_dx = (uint16_t)(((uint16_t)(uint64_t)(((uint64_t)(uintptr_t)arg2))));
    uint16_t __attribute__((unused)) ssa__25 = (uint16_t)(((uint16_t)(ssa_r_dx) + (uint16_t)(0xfffffffffffffffeULL)));
    if (((((int16_t)(ssa__25) < (int16_t)(0ULL))) != (((((int16_t)(ssa_r_dx) < (int16_t)(0ULL))) && (((int16_t)(((uint16_t)(ssa__25) ^ (uint16_t)(ssa_r_dx))) < (int16_t)(0ULL))))))) {
        ssa_common_ret_op = (uint64_t)(((uint64_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(8ULL))) + (uint64_t)(((uint64_t)(((uint64_t)(int64_t)(int16_t)(ssa_r_dx))) << (uint64_t)(5ULL)))));
    } else {
        uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x48ULL)))));
        ssa_common_ret_op = (uint64_t)(((uint64_t)(ssa_mem_ld) + (uint64_t)(((uint64_t)(((uint64_t)(int64_t)(int16_t)(ssa__25))) << (uint64_t)(5ULL)))));
    }
    return (int64_t)(ssa_common_ret_op);
}

/*
 * Function : CmpSortedValueEnumStackGetEntryAtLayerHeight @ 0x140693710 (38 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (87.60 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
int64_t __attribute__((ms_abi)) CmpSortedValueEnumStackGetEntryAtLayerHeight(uint64_t* arg1, int32_t* arg2) {
    uint64_t __attribute__((unused)) ssa_common_ret_op = 0;
    uint16_t __attribute__((unused)) ssa_r_dx = (uint16_t)(((uint16_t)(uint64_t)(((uint64_t)(uintptr_t)arg2))));
    uint16_t __attribute__((unused)) ssa__25 = (uint16_t)(((uint16_t)(ssa_r_dx) + (uint16_t)(0xfffffffffffffffeULL)));
    if (((((int16_t)(ssa__25) < (int16_t)(0ULL))) != (((((int16_t)(ssa_r_dx) < (int16_t)(0ULL))) && (((int16_t)(((uint16_t)(ssa__25) ^ (uint16_t)(ssa_r_dx))) < (int16_t)(0ULL))))))) {
        ssa_common_ret_op = (uint64_t)(((uint64_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x10ULL))) + (uint64_t)(((uint64_t)(((uint64_t)(int64_t)(int16_t)(ssa_r_dx))) << (uint64_t)(5ULL)))));
    } else {
        uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x50ULL)))));
        ssa_common_ret_op = (uint64_t)(((uint64_t)(ssa_mem_ld) + (uint64_t)(((uint64_t)(((uint64_t)(int64_t)(int16_t)(ssa__25))) << (uint64_t)(5ULL)))));
    }
    return (int64_t)(ssa_common_ret_op);
}

/*
 * Function : CmpSetRespectIoPriorityThread @ 0x1400499a8 (40 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (6.18 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) CmpSetRespectIoPriorityThread(uint32_t* arg1, uint32_t arg2) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x6d4ULL)))));
    (*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x6d4ULL)))) = (uint32_t)(((uint32_t)(((uint32_t)(ssa_mem_ld) & (uint32_t)(0xffffffffffffff7fULL))) | (uint32_t)(((uint32_t)(((uint32_t)(((uint32_t)(uint64_t)(((uint64_t)arg2)))) << (uint32_t)(7ULL))) & (uint32_t)(0x80ULL)))));
    return (uint32_t)(((uint64_t)(uint32_t)(((uint32_t)(((uint32_t)(ssa_mem_ld) >> (uint32_t)(7ULL))) & (uint32_t)(0x1ffff01ULL)))));
}

/*
 * Function : CmpFindValueByName @ 0x1405b0a60 (41 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (6.96 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) CmpFindValueByName(uint64_t arg1, uint64_t arg2) {
    uint8_t __attribute__((aligned(16))) ssa_stack_frame[1024];
    (*(uint64_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 752ULL))) = (uint64_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 784ULL));
    (*(uint64_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 744ULL))) = (uint64_t)(0ULL);
    uint64_t __attribute__((unused)) ssa_call_res = CmpFindNameInList(((uint64_t)arg1), ((uint64_t)(((uint64_t)arg2)) + (uint64_t)(0x24ULL)), 0ULL, 0ULL);
    uint32_t __attribute__((unused)) ssa_mem_ld_1 = (uint32_t)((*(uint32_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 784ULL))));
    return (uint64_t)(((uint64_t)(uint32_t)(ssa_mem_ld_1)));
}

/*
 * Function : CmpInitializeDiscardReplaceContext @ 0x14068f138 (44 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (6.82 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) CmpInitializeDiscardReplaceContext(uint64_t arg1) {
    uint64_t __attribute__((unused)) ssa_call_res = memset(((uint64_t)arg1), 0ULL, 0x20ULL, 0ULL);
    uint64_t __attribute__((unused)) ssa__57 = (uint64_t)(((uint64_t)(((uint64_t)arg1)) + (uint64_t)(0x10ULL)));
    (*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)arg1)) + (uint64_t)(8ULL)))) = (uint32_t)(0xffffffffc0000001ULL);
    (*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)arg1)) + (uint64_t)(0x18ULL)))) = (uint64_t)(ssa__57);
    (*(uint64_t*)(uintptr_t)(ssa__57)) = (uint64_t)(ssa__57);
    return (uint64_t)(ssa__57);
}

/*
 * Function : CmpInitializeFreezeThaw @ 0x1405de1c8 (46 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (7.02 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) CmpInitializeFreezeThaw(void) {
    (*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)&CmpFreezeListLock))) = (uint64_t)(0ULL);
    uint32_t __attribute__((unused)) ssa_mem_ld_1 = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)&CmFreezeThawTimeoutInSeconds))));
    (*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)&__nt_data_14037ee48))) = (uint64_t)(((uint64_t)(uintptr_t)&CmpFreezeThawWaitListHead));
    (*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)&CmpFreezeThawWaitListHead))) = (uint64_t)(((uint64_t)(uintptr_t)&CmpFreezeThawWaitListHead));
    if (((uint32_t)(ssa_mem_ld_1) > (uint32_t)(0x384ULL))) {
        (*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)&CmFreezeThawTimeoutInSeconds))) = (uint32_t)(0x3cULL);
    }
    return (uint64_t)(((uint64_t)(uintptr_t)&CmpFreezeThawWaitListHead));
}

/*
 * Function : CmpSystemQuotaWarningWorker @ 0x140682eb0 (52 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.42 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) CmpSystemQuotaWarningWorker(void) {
    uint8_t __attribute__((aligned(16))) ssa_stack_frame[1024];
    uint64_t __attribute__((unused)) ssa_call_res = ExFreePoolWithTag(0ULL, 0ULL, 0ULL, 0ULL);
    (*(uint64_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 752ULL))) = (uint64_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 784ULL));
    (*(uint32_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 744ULL))) = (uint32_t)(1ULL);
    uint64_t __attribute__((unused)) ssa_call_res_1 = ExRaiseHardError(0xc0000256ULL, 0ULL, 0ULL, 0ULL);
    return (uint64_t)(ssa_call_res_1);
}

/*
 * Function : CmpArmDelayDerefKCBWorker @ 0x140569978 (53 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (6.36 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) CmpArmDelayDerefKCBWorker(void) {
    uint8_t __attribute__((aligned(16))) ssa_stack_frame[1024];
    uint64_t __attribute__((unused)) ssa__48 = (uint64_t)(((uint64_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 768ULL)) + (uint64_t)(0xffffffffffffffe8ULL)));
    (*(uint64_t*)(uintptr_t)(((uint64_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 768ULL)) + (uint64_t)(0xfffffffffffffff0ULL)))) = (uint64_t)(0xffffffffffffffffULL);
    (*(uint64_t*)(uintptr_t)(ssa__48)) = (uint64_t)(0ULL);
    uint64_t __attribute__((unused)) ssa_call_res = KeSetTimer2(((uint64_t)(uintptr_t)&CmpDelayDerefKCBTimer), 0xffffffffff676980ULL, 0ULL, ssa__48);
    return (uint64_t)(ssa_call_res);
}
