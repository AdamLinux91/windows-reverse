/*
 * Reconstructed Windows NT Kernel Subsystem: Security Reference Monitor & Kernel Debugger (Se/Kd)
 * Source Binary : ntoskrnl.exe (10.0.16299.15 x64, RSDS 83DB4240-4EFD-4AB6-AFB6-FA864B700CB3)
 * Functions     : 4 Z3-verified / assembly-preserved routines
 */
#include "../../include/nt_reconstructed_types.h"

/*
 * Function : SeTokenIsRestricted @ 0x140016910 (12 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.05 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
BOOLEAN __attribute__((ms_abi)) SeTokenIsRestricted(struct _TOKEN* Token) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)(Token->TokenFlags);
    return (BOOLEAN)(((uint64_t)(uint32_t)(((uint32_t)(((uint32_t)(ssa_mem_ld) >> (uint32_t)(4ULL))) & (uint32_t)(0xfffff01ULL)))));
}

/*
 * Function : SeTokenIsAdmin @ 0x140458290 (119 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (48.86 ms, 33 SMT assertions)
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
 * Z3 Proof : VERIFIED_EQUIVALENT (4.47 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
NTSTATUS __attribute__((ms_abi)) SeQueryAuthenticationIdToken(struct _TOKEN* Token, struct _LUID* AuthenticationId) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)Token)) + (uint64_t)(0x18ULL)))));
    (*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)AuthenticationId))) = (uint64_t)(ssa_mem_ld);
    return (NTSTATUS)(0ULL);
}

/*
 * Function : KdRefreshDebuggerNotPresent @ 0x140141ef0 (36 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (2468.96 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
BOOLEAN __attribute__((ms_abi)) KdRefreshDebuggerNotPresent(void) {
    uint64_t __attribute__((unused)) ssa_reg_rax_0 = 0;
    uint64_t __attribute__((unused)) ssa_reg_rax_1 = 0;
    uint8_t __attribute__((aligned(16))) ssa_stack_frame[4096];
    (*(uint64_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 3080ULL))) = (uint64_t)(0ULL);
    (*(uint64_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 3064ULL))) = (uint64_t)(0ULL);
    uint8_t __attribute__((unused)) ssa_mem_ld = (uint8_t)((*(uint8_t*)(uintptr_t)(((uint64_t)(uintptr_t)&KdPitchDebugger))));
    if (((uint8_t)(ssa_mem_ld) == (uint8_t)(0ULL))) {
        goto L_bb_1401d8d30;
    }
    ssa_reg_rax_0 = (uint64_t)(1ULL);
L_bb_140141f09:
    return (BOOLEAN)(ssa_reg_rax_0);
L_bb_1401d8d30:
    (*(uint64_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 3056ULL))) = (uint64_t)(((uint64_t)(uintptr_t)&__nt_rodata_1401715d0));
    ssa_reg_rax_1 = (uint64_t)(0xffffffffffffffffULL);
L_bb_1401d8d40:
    uint64_t __attribute__((unused)) ssa__112 = (uint64_t)(((uint64_t)(ssa_reg_rax_1) + (uint64_t)(1ULL)));
    uint8_t __attribute__((unused)) ssa_mem_ld_3 = (uint8_t)((*(uint8_t*)(uintptr_t)(((uint64_t)(ssa__112) + (uint64_t)(((uint64_t)(uintptr_t)&__nt_rodata_1401715d0))))));
    if (((uint8_t)(ssa_mem_ld_3) == (uint8_t)(0ULL))) {
        goto L_bb_1401d8d49;
    }
    ssa_reg_rax_1 = (uint64_t)(ssa__112);
    goto L_bb_1401d8d40;
L_bb_1401d8d49:
    (*(uint16_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 3048ULL))) = (uint16_t)(((uint16_t)(uint64_t)(ssa__112)));
    uint64_t __attribute__((unused)) ssa_call_res = (uint64_t)KdEnterDebugger((void*)(uintptr_t)(0ULL));
    uint64_t __attribute__((unused)) ssa_call_res_1 = (uint64_t)KdpPrintString((struct _STRING*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 3048ULL)));
    uint8_t __attribute__((unused)) ssa_mem_ld_4 = (uint8_t)((*(uint8_t*)(uintptr_t)(((uint64_t)(uintptr_t)&KdDebuggerNotPresent))));
    KdExitDebugger((BOOLEAN)(((uint64_t)(ssa_call_res) & (uint64_t)(0xffULL))));
    ssa_reg_rax_0 = (uint64_t)(((uint64_t)(((uint64_t)(0ULL) & (uint64_t)(0xffffffffffffff00ULL))) | (uint64_t)(((uint64_t)(uint8_t)(ssa_mem_ld_4)))));
    goto L_bb_140141f09;
}
