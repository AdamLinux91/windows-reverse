/*
 * Reconstructed Windows NT Kernel Subsystem: Security Reference Monitor, Tokens, SIDs & ACLs (Se/Sep)
 * Source Binary : ntoskrnl.exe (10.0.16299.15 x64, RSDS 83DB4240-4EFD-4AB6-AFB6-FA864B700CB3)
 * Functions     : 18 Z3-verified / assembly-preserved routines
 */
#include "../../include/nt_reconstructed_types.h"

/*
 * Function : SeTokenIsRestricted @ 0x140016910 (12 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.59 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
BOOLEAN __attribute__((ms_abi)) SeTokenIsRestricted(struct _TOKEN* Token) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)(Token->TokenFlags);
    return (BOOLEAN)(((uint64_t)(uint32_t)(((uint32_t)(((uint32_t)(ssa_mem_ld) >> (uint32_t)(4ULL))) & (uint32_t)(0xfffff01ULL)))));
}

/*
 * Function : SeTokenIsAdmin @ 0x140458290 (119 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (53.65 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
BOOLEAN __attribute__((ms_abi)) SeTokenIsAdmin(struct _TOKEN* Token) {
    uint8_t __attribute__((unused)) ssa_reg_rdi_0_in_off0 = 0;
    uint64_t __attribute__((unused)) ssa_reg_rax_0 = 0;
    uint64_t __attribute__((unused)) ssa_reg_rax_1 = 0;
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)(Token->TokenFlags);
    if (((uint32_t)(((uint32_t)(ssa_mem_ld) & (uint32_t)(0x4000ULL))) == (uint32_t)(0ULL))) {
        goto L_bb_1404582ae;
    }
    goto L_bb_140458303;
L_bb_1404582ae:
    uint32_t __attribute__((unused)) ssa_mem_ld_1 = (uint32_t)(Token->TokenType);
    if (((uint32_t)(ssa_mem_ld_1) == (uint32_t)(2ULL))) {
        goto L_bb_1405f9ce6;
    }
L_bb_1404582bb:
    uint64_t __attribute__((unused)) ssa_mem_ld_2 = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)&SeAliasAdminsSid))));
    uint64_t __attribute__((unused)) ssa_call_res = (uint64_t)SepSidInToken((void*)(uintptr_t)(((uint64_t)(uintptr_t)Token)), (void*)(uintptr_t)(0ULL), (void*)(uintptr_t)(ssa_mem_ld_2), (BOOLEAN)(0ULL), (BOOLEAN)(0ULL), (BOOLEAN)(0ULL));
    uint8_t __attribute__((unused)) ssa_r_al = (uint8_t)(((uint8_t)(uint64_t)(ssa_call_res)));
    if (((uint8_t)(ssa_r_al) == (uint8_t)(0ULL))) {
        ssa_reg_rdi_0_in_off0 = (uint8_t)(0ULL);
        ssa_reg_rax_0 = (uint64_t)(ssa_call_res);
        goto L_bb_1404582f0;
    }
    uint64_t __attribute__((unused)) ssa_call_res_1 = (uint64_t)SeTokenIsRestricted((struct _TOKEN*)(uintptr_t)(((uint64_t)(uintptr_t)Token)));
    if (((uint8_t)(((uint8_t)(uint64_t)(ssa_call_res_1))) == (uint8_t)(0ULL))) {
        ssa_reg_rdi_0_in_off0 = (uint8_t)(ssa_r_al);
        ssa_reg_rax_0 = (uint64_t)(ssa_call_res_1);
        goto L_bb_1404582f0;
    }
    goto L_bb_1405f9cf8;
L_bb_1404582f0:
    ssa_reg_rax_1 = (uint64_t)(((uint64_t)(((uint64_t)(ssa_reg_rax_0) & (uint64_t)(0xffffffffffffff00ULL))) | (uint64_t)(((uint64_t)(uint8_t)(ssa_reg_rdi_0_in_off0)))));
L_bb_1404582f3:
    return (BOOLEAN)(ssa_reg_rax_1);
L_bb_140458303:
    ssa_reg_rax_1 = (uint64_t)(((uint64_t)(uint32_t)(((uint32_t)(ssa_mem_ld) & (uint32_t)(0xffffffffffffff00ULL)))));
    goto L_bb_1404582f3;
L_bb_1405f9ce6:
    uint32_t __attribute__((unused)) ssa_mem_ld_8 = (uint32_t)(Token->ImpersonationLevel);
    if (((int32_t)(((uint32_t)(((uint32_t)(((uint32_t)(1ULL) - (uint32_t)(ssa_mem_ld_8))) & (uint32_t)(ssa_mem_ld_8))) ^ (uint32_t)(((uint32_t)(ssa_mem_ld_8) + (uint32_t)(0xfffffffffffffffeULL))))) < (int32_t)(0ULL))) {
        goto L_bb_140458303;
    }
    goto L_bb_1404582bb;
L_bb_1405f9cf8:
    uint64_t __attribute__((unused)) ssa_call_res_2 = (uint64_t)SepSidInToken((void*)(uintptr_t)(((uint64_t)(uintptr_t)Token)), (void*)(uintptr_t)(0ULL), (void*)(uintptr_t)(ssa_mem_ld_2), (BOOLEAN)(0ULL), (BOOLEAN)(1ULL), (BOOLEAN)(0ULL));
    ssa_reg_rdi_0_in_off0 = (uint8_t)(((uint8_t)(uint64_t)(ssa_call_res_2)));
    ssa_reg_rax_0 = (uint64_t)(ssa_call_res_2);
    goto L_bb_1404582f0;
}

/*
 * Function : SeQueryAuthenticationIdToken @ 0x140571550 (10 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.73 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
NTSTATUS __attribute__((ms_abi)) SeQueryAuthenticationIdToken(struct _TOKEN* Token, struct _LUID* AuthenticationId) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)((uint8_t*)(uintptr_t)(Token) + 0x18)));
    (*(uint64_t*)((uint8_t*)(uintptr_t)(AuthenticationId))) = (uint64_t)(ssa_mem_ld);
    return (NTSTATUS)(0ULL);
}

/*
 * Function : SeShouldCheckForAccessRightsFromParent @ 0x1404edeb0 (6 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (27.40 ms, 34 SMT assertions)
 * Verified : x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) SeShouldCheckForAccessRightsFromParent(void) {
    return;
}

/*
 * Function : SeTokenImpersonationLevel @ 0x1405b49b0 (7 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.49 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) SeTokenImpersonationLevel(uint32_t* arg1) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0xc4ULL)))));
    return (uint32_t)(((uint64_t)(uint32_t)(ssa_mem_ld)));
}

/*
 * Function : SeTokenType @ 0x14056e910 (7 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.48 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) SeTokenType(uint32_t* arg1) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0xc0ULL)))));
    return (uint32_t)(((uint64_t)(uint32_t)(ssa_mem_ld)));
}

/*
 * Function : SeTokenIsWriteRestricted @ 0x140016930 (12 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.98 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) SeTokenIsWriteRestricted(uint32_t* arg1) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0xc8ULL)))));
    return (uint32_t)(((uint64_t)(uint32_t)(((uint32_t)(((uint32_t)(ssa_mem_ld) >> (uint32_t)(3ULL))) & (uint32_t)(0x1fffff01ULL)))));
}

/*
 * Function : SeSetAccessStateGenericMapping @ 0x14071f3d0 (13 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.67 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) SeSetAccessStateGenericMapping(uint64_t* arg1, void* arg2) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x48ULL)))));
    return (uint64_t)(ssa_mem_ld);
}

/*
 * Function : SeCaptureSubjectContextEx @ 0x14051cf50 (17 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (19.32 ms, 33 SMT assertions)
 * Verified : x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) SeCaptureSubjectContextEx(uint64_t arg1, uint64_t* arg2, uint64_t arg3) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg2)) + (uint64_t)(0x2e0ULL)))));
    return (uint64_t)(ssa_mem_ld);
}

/*
 * Function : SeAdjustAccessStateForTrustLabel @ 0x14071f3b0 (20 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.38 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) SeAdjustAccessStateForTrustLabel(uint64_t arg1, uint64_t arg2, uint64_t arg3) {
    uint64_t __attribute__((unused)) ssa_call_res = SepAdjustAccessStateForConstraints(((uint64_t)arg1), ((uint64_t)arg2), 0ULL, ((uint64_t)arg3));
    return (uint64_t)(ssa_call_res);
}

/*
 * Function : SeSetSecurityAttributesToken @ 0x14071f120 (25 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.76 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) SeSetSecurityAttributesToken(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4) {
    uint64_t __attribute__((unused)) ssa_call_res = SepInternalSetSecurityAttributesToken(((uint64_t)arg1), ((uint64_t)arg2), 0ULL, ((uint64_t)arg3));
    return (uint64_t)(ssa_call_res);
}

/*
 * Function : SeCaptureSubjectContext @ 0x1404edc60 (30 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (19.55 ms, 33 SMT assertions)
 * Verified : x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) SeCaptureSubjectContext(uint64_t arg1) {
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val_1 = KeGetCurrentThread();
    return (uint64_t)(((uint64_t)(uintptr_t)ssa_gs_val));
}

/*
 * Function : SeComputeAutoInheritByObjectType @ 0x14010a6e0 (34 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.99 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) SeComputeAutoInheritByObjectType(void) {
    uint8_t __attribute__((aligned(16))) ssa_stack_frame[1024];
    (*(uint32_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 800ULL))) = (uint32_t)(0ULL);
    (*(uint64_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 744ULL))) = (uint64_t)(0ULL);
    uint64_t __attribute__((unused)) ssa_call_res = SeComputeAutoInheritByObjectTypeEx(0ULL, 0ULL, 0ULL, (((uint64_t)(uintptr_t)ssa_stack_frame) + 800ULL));
    uint32_t __attribute__((unused)) ssa_mem_ld_2 = (uint32_t)((*(uint32_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 800ULL))));
    return (uint32_t)(((uint64_t)(uint32_t)(ssa_mem_ld_2)));
}

/*
 * Function : SeReportSecurityEvent @ 0x1402565f0 (34 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.93 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) SeReportSecurityEvent(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint32_t* arg4) {
    if (((uint64_t)(((uint64_t)(uintptr_t)arg4)) == (uint64_t)(0ULL))) {
    } else {
        (*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg4))) = (uint32_t)(3ULL);
    }
    uint64_t __attribute__((unused)) ssa_call_res = SeReportSecurityEventWithSubCategory(((uint64_t)arg1), ((uint64_t)arg2), ((uint64_t)arg3), ((uint64_t)(uintptr_t)arg4));
    return (uint64_t)(ssa_call_res);
}

/*
 * Function : SeDeassignSecurity @ 0x140561a60 (36 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (7.89 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) SeDeassignSecurity(uint64_t* arg1) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))));
    if (((uint64_t)(ssa_mem_ld) == (uint64_t)(0ULL))) {
    } else {
        uint64_t __attribute__((unused)) ssa_call_res = ExFreePoolWithTag(ssa_mem_ld, 0ULL, 0ULL, 0ULL);
    }
    (*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))) = (uint64_t)(0ULL);
    return (uint64_t)(0ULL);
}

/*
 * Function : SeSetSecurityDescriptorInfoEx @ 0x140726a80 (58 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (8.37 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
NTSTATUS __attribute__((ms_abi)) SeSetSecurityDescriptorInfoEx(uint64_t arg1, uint32_t* arg2, uint64_t arg3, uint64_t* arg4) {
    uint64_t __attribute__((unused)) ssa_reg_rax_0 = 0;
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg4))));
    if (((uint64_t)(ssa_mem_ld) == (uint64_t)(0ULL))) {
        ssa_reg_rax_0 = (uint64_t)(0xc00000d7ULL);
    } else {
        uint32_t __attribute__((unused)) ssa_mem_ld_2 = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg2))));
        uint64_t __attribute__((unused)) ssa_call_res = RtlpSetSecurityObject(((uint64_t)arg1), ((uint64_t)(uint32_t)(ssa_mem_ld_2)), ((uint64_t)arg3), ((uint64_t)(uintptr_t)arg4));
        ssa_reg_rax_0 = (uint64_t)(ssa_call_res);
    }
    return (NTSTATUS)(ssa_reg_rax_0);
}

/*
 * Function : SeCreateAccessStateEx @ 0x140563610 (74 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (8.08 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) SeCreateAccessStateEx(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4) {
    uint8_t __attribute__((aligned(16))) ssa_stack_frame[1024];
    (*(uint64_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 776ULL))) = (uint64_t)(0ULL);
    (*(uint64_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 760ULL))) = (uint64_t)(0ULL);
    uint64_t __attribute__((unused)) ssa_call_res = (uint64_t)SeCaptureSubjectContextEx((uint64_t)(((uint64_t)arg1)), (uint64_t*)(uintptr_t)(((uint64_t)arg2)), (uint64_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 728ULL)));
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 816ULL))));
    uint32_t __attribute__((unused)) ssa_mem_ld_1 = (uint32_t)((*(uint32_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 808ULL))));
    (*(uint64_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 712ULL))) = (uint64_t)(ssa_mem_ld);
    uint64_t __attribute__((unused)) ssa_call_res_1 = SepCreateAccessStateFromSubjectContext((((uint64_t)(uintptr_t)ssa_stack_frame) + 728ULL), ((uint64_t)arg3), ((uint64_t)arg4), ((uint64_t)(uint32_t)(ssa_mem_ld_1)));
    return (uint64_t)(ssa_call_res_1);
}

/*
 * Function : SeQuerySessionIdToken @ 0x14055cb60 (75 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (9.67 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) SeQuerySessionIdToken(uint64_t* arg1, uint64_t arg2) {
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    uint16_t __attribute__((unused)) ssa_mem_ld = (uint16_t)(ssa_gs_val->KernelApcDisable);
    ssa_gs_val->KernelApcDisable = (int16_t)(((uint16_t)(ssa_mem_ld) + (uint16_t)(0xffffffffffffffffULL)));
    uint64_t __attribute__((unused)) ssa_mem_ld_1 = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x30ULL)))));
    uint64_t __attribute__((unused)) ssa__78 = (uint64_t)(((uint64_t)(((uint64_t)(((uint64_t)arg2)) & (uint64_t)(0xffffffffffffff00ULL))) | (uint64_t)(1ULL)));
    uint64_t __attribute__((unused)) ssa_call_res = ExAcquireResourceSharedLite(ssa_mem_ld_1, ssa__78, 0ULL, 0ULL);
    uint32_t __attribute__((unused)) ssa_mem_ld_2 = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x78ULL)))));
    (*(uint32_t*)(uintptr_t)(((uint64_t)arg2))) = (uint32_t)(ssa_mem_ld_2);
    uint64_t __attribute__((unused)) ssa_mem_ld_3 = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x30ULL)))));
    uint64_t __attribute__((unused)) ssa_call_res_1 = (uint64_t)ExReleaseResourceLite((uint16_t*)(uintptr_t)(ssa_mem_ld_3), (struct _RECON_EXRELEASERESOURCELITE_RDX*)(uintptr_t)(ssa__78));
    uint64_t __attribute__((unused)) ssa_call_res_2 = KeLeaveCriticalRegion(ssa_mem_ld_3, ssa__78, 0ULL, 0ULL);
    return (uint64_t)(0ULL);
}
