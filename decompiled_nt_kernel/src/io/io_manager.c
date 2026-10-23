/*
 * Reconstructed Windows NT Kernel Subsystem: I/O Manager, IRP Inspection & Plug-and-Play (Io/Pi)
 * Source Binary : ntoskrnl.exe (10.0.16299.15 x64, RSDS 83DB4240-4EFD-4AB6-AFB6-FA864B700CB3)
 * Functions     : 6 Z3-verified / assembly-preserved routines
 */
#include "../../include/nt_reconstructed_types.h"

/*
 * Function : IoGetAttachedDevice @ 0x140055bd0 (25 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (11054.08 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
struct _DEVICE_OBJECT* __attribute__((ms_abi)) IoGetAttachedDevice(struct _DEVICE_OBJECT* DeviceObject) {
    uint64_t __attribute__((unused)) ssa_reg_rax_0 = 0;
    uint64_t __attribute__((unused)) ssa_reg_rcx_0 = 0;
    struct _DEVICE_OBJECT* __attribute__((unused)) ssa_mem_ld = (struct _DEVICE_OBJECT*)(uintptr_t)(DeviceObject->AttachedDevice);
    if (((uint64_t)(((uint64_t)(uintptr_t)ssa_mem_ld)) == (uint64_t)(0ULL))) {
        ssa_reg_rcx_0 = (uint64_t)(((uint64_t)(uintptr_t)DeviceObject));
        goto L_bb_140055be5;
    }
    ssa_reg_rax_0 = (uint64_t)(((uint64_t)(uintptr_t)ssa_mem_ld));
L_bb_140055bd9:
    uint64_t __attribute__((unused)) ssa_mem_ld_1 = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(ssa_reg_rax_0) + (uint64_t)(0x18ULL)))));
    if (((uint64_t)(ssa_mem_ld_1) == (uint64_t)(0ULL))) {
        ssa_reg_rcx_0 = (uint64_t)(ssa_reg_rax_0);
        goto L_bb_140055be5;
    }
    ssa_reg_rax_0 = (uint64_t)(ssa_mem_ld_1);
    goto L_bb_140055bd9;
L_bb_140055be5:
    return (struct _DEVICE_OBJECT*)(uintptr_t)(ssa_reg_rcx_0);
}

/*
 * Function : IoGetIoPriorityHint @ 0x14005a000 (47 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (16.07 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) IoGetIoPriorityHint(struct _IRP* Irp) {
    uint32_t __attribute__((unused)) ssa_reg_rax_0_shrunk = 0;
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)(Irp->Flags);
    uint32_t __attribute__((unused)) ssa__39 = (uint32_t)(((uint32_t)(((uint32_t)(ssa_mem_ld) >> (uint32_t)(0x11ULL))) & (uint32_t)(7ULL)));
    if (((uint32_t)(ssa__39) == (uint32_t)(0ULL))) {
        goto L_bb_14005a00b;
    }
    goto L_bb_14005a011;
L_bb_14005a00b:
    ssa_reg_rax_0_shrunk = (uint32_t)(2ULL);
L_bb_14005a010:
    return (uint32_t)(((uint64_t)(uint32_t)(ssa_reg_rax_0_shrunk)));
L_bb_14005a011:
    uint32_t __attribute__((unused)) ssa__61 = (uint32_t)(((uint32_t)(ssa__39) + (uint32_t)(0xffffffffffffffffULL)));
    if (((uint32_t)(ssa__39) > (uint32_t)(2ULL))) {
        ssa_reg_rax_0_shrunk = (uint32_t)(ssa__61);
        goto L_bb_14005a010;
    }
    struct _ETHREAD* __attribute__((unused)) ssa_mem_ld_1 = (struct _ETHREAD*)(uintptr_t)(Irp->Tail.Overlay.Thread);
    if (((uint64_t)(((uint64_t)(uintptr_t)ssa_mem_ld_1)) == (uint64_t)(0ULL))) {
        ssa_reg_rax_0_shrunk = (uint32_t)(ssa__61);
        goto L_bb_14005a010;
    }
    uint32_t __attribute__((unused)) ssa_mem_ld_2 = (uint32_t)(ssa_mem_ld_1->IoBoostCount);
    if (((uint32_t)(ssa_mem_ld_2) == (uint32_t)(0ULL))) {
        ssa_reg_rax_0_shrunk = (uint32_t)(ssa__61);
        goto L_bb_14005a010;
    }
    goto L_bb_14005a00b;
}

/*
 * Function : IoGetCurrentProcess @ 0x1400e9990 (17 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.17 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
struct _EPROCESS* __attribute__((ms_abi)) IoGetCurrentProcess(void) {
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    struct _KPROCESS* __attribute__((unused)) ssa_mem_ld = (struct _KPROCESS*)(uintptr_t)(ssa_gs_val->ApcState.Process);
    return (struct _EPROCESS*)(uintptr_t)(((uint64_t)(uintptr_t)ssa_mem_ld));
}

/*
 * Function : IoThreadToProcess @ 0x140050520 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.47 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
struct _EPROCESS* __attribute__((ms_abi)) IoThreadToProcess(struct _ETHREAD* Thread) {
    struct _KPROCESS* __attribute__((unused)) ssa_mem_ld = (struct _KPROCESS*)(uintptr_t)(Thread->Tcb.Process);
    return (struct _EPROCESS*)(uintptr_t)(((uint64_t)(uintptr_t)ssa_mem_ld));
}

/*
 * Function : IoIs32bitProcess @ 0x140022e20 (167 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (45.08 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
BOOLEAN __attribute__((ms_abi)) IoIs32bitProcess(struct _IRP* Irp) {
    uint64_t __attribute__((unused)) ssa_reg_rax_1 = 0;
    uint64_t __attribute__((unused)) ssa_common_ret_op = 0;
    uint64_t __attribute__((unused)) ssa_reg_rax_0 = 0;
    if (((uint64_t)(((uint64_t)(uintptr_t)Irp)) == (uint64_t)(0ULL))) {
        goto L_bb_140022e57;
    }
    uint8_t __attribute__((unused)) ssa_mem_ld = (uint8_t)(Irp->RequestorMode);
    if (((uint8_t)(ssa_mem_ld) == (uint8_t)(1ULL))) {
        goto L_bb_140022e2b;
    }
    ssa_reg_rax_0 = (uint64_t)(0ULL);
    goto L_bb_140022e54;
L_bb_140022e2b:
    uint32_t __attribute__((unused)) ssa_mem_ld_1 = (uint32_t)(Irp->Flags);
    struct _ETHREAD* __attribute__((unused)) ssa_mem_ld_2 = (struct _ETHREAD*)(uintptr_t)(Irp->Tail.Overlay.Thread);
    if (((uint32_t)(((uint32_t)(ssa_mem_ld_1) & (uint32_t)(0x2000ULL))) == (uint32_t)(0ULL))) {
        goto L_bb_140022e9a;
    }
    uint64_t __attribute__((unused)) ssa_mem_ld_3 = (uint64_t)(uintptr_t)(Irp->Overlay.AsynchronousParameters.UserApcRoutine);
    ssa_reg_rax_1 = (uint64_t)(((uint64_t)(ssa_mem_ld_3) & (uint64_t)(0xfffffffffffffff9ULL)));
L_bb_140022e43:
    if (((uint64_t)(ssa_reg_rax_1) == (uint64_t)(0ULL))) {
        ssa_reg_rax_0 = (uint64_t)(0ULL);
        goto L_bb_140022e54;
    }
    uint64_t __attribute__((unused)) ssa_mem_ld_4 = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(ssa_reg_rax_1) + (uint64_t)(0x428ULL)))));
    if (((uint64_t)(ssa_mem_ld_4) == (uint64_t)(0ULL))) {
        ssa_reg_rax_0 = (uint64_t)(ssa_reg_rax_1);
        goto L_bb_140022e54;
    }
    goto L_bb_140022eb4;
L_common_ret:
    return (BOOLEAN)(ssa_common_ret_op);
L_bb_140022e54:
    ssa_common_ret_op = (uint64_t)(((uint64_t)(ssa_reg_rax_0) & (uint64_t)(0xffffffffffffff00ULL)));
    goto L_common_ret;
L_bb_140022e57:
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    uint8_t __attribute__((unused)) ssa_mem_ld_5 = (uint8_t)(ssa_gs_val->PreviousMode);
    if (((uint8_t)(ssa_mem_ld_5) == (uint8_t)(1ULL))) {
        goto L_bb_140022e69;
    }
    ssa_reg_rax_0 = (uint64_t)(((uint64_t)(uintptr_t)ssa_gs_val));
    goto L_bb_140022e54;
L_bb_140022e69:
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val_1 = KeGetCurrentThread();
    struct _KPROCESS* __attribute__((unused)) ssa_mem_ld_6 = (struct _KPROCESS*)(uintptr_t)(ssa_gs_val_1->ApcState.Process);
    uint64_t __attribute__((unused)) ssa_mem_ld_7 = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)ssa_mem_ld_6)) + (uint64_t)(0x428ULL)))));
    if (((uint64_t)(ssa_mem_ld_7) == (uint64_t)(0ULL))) {
        ssa_reg_rax_0 = (uint64_t)(0ULL);
        goto L_bb_140022e54;
    }
    uint16_t __attribute__((unused)) ssa_mem_ld_8 = (uint16_t)((*(uint16_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld_7) + (uint64_t)(8ULL)))));
    switch (ssa_mem_ld_8) {
        case 332LL:
            ssa_common_ret_op = (uint64_t)(0x101ULL);
            ssa_common_ret_op = (uint64_t)(0x101ULL);
            goto L_common_ret;
        case 452LL:
            ssa_common_ret_op = (uint64_t)(0x101ULL);
            ssa_common_ret_op = (uint64_t)(0x101ULL);
            goto L_common_ret;
        default:
            ssa_reg_rax_0 = (uint64_t)(0x1c4ULL);
            goto L_bb_140022e54;
    }
L_bb_140022e9a:
    if (((uint64_t)(((uint64_t)(uintptr_t)ssa_mem_ld_2)) == (uint64_t)(0ULL))) {
        ssa_reg_rax_0 = (uint64_t)(((uint64_t)(uint32_t)(ssa_mem_ld_1)));
        goto L_bb_140022e54;
    }
    uint8_t __attribute__((unused)) ssa_mem_ld_9 = (uint8_t)(Irp->ApcEnvironment);
    switch (ssa_mem_ld_9) {
        case 0LL:
            goto L_bb_140022eab;
        case 1LL:
            goto L_bb_14017f74a;
        default:
            ssa_reg_rax_0 = (uint64_t)(0ULL);
            goto L_bb_140022e54;
    }
L_bb_140022eab:
    struct _KPROCESS* __attribute__((unused)) ssa_mem_ld_10 = (struct _KPROCESS*)(uintptr_t)(ssa_mem_ld_2->Tcb.Process);
    ssa_reg_rax_1 = (uint64_t)(((uint64_t)(uintptr_t)ssa_mem_ld_10));
    goto L_bb_140022e43;
L_bb_140022eb4:
    uint16_t __attribute__((unused)) ssa_mem_ld_11 = (uint16_t)((*(uint16_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld_4) + (uint64_t)(8ULL)))));
    switch (ssa_mem_ld_11) {
        case 332LL:
            ssa_common_ret_op = (uint64_t)(0x101ULL);
            ssa_common_ret_op = (uint64_t)(0x101ULL);
            goto L_common_ret;
        case 452LL:
            ssa_common_ret_op = (uint64_t)(0x101ULL);
            ssa_common_ret_op = (uint64_t)(0x101ULL);
            goto L_common_ret;
        default:
            ssa_reg_rax_0 = (uint64_t)(0x1c4ULL);
            goto L_bb_140022e54;
    }
L_bb_14017f74a:
    struct _KPROCESS* __attribute__((unused)) ssa_mem_ld_12 = (struct _KPROCESS*)(uintptr_t)(ssa_mem_ld_2->Tcb.ApcState.Process);
    ssa_reg_rax_1 = (uint64_t)(((uint64_t)(uintptr_t)ssa_mem_ld_12));
    goto L_bb_140022e43;
}

/*
 * Function : PiUEventHashStringIntoBucket @ 0x14002468c (72 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (9.02 ms, 38 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence, magic_mod_equivalence:mod_13
 * De-Opt   : Strength-reduced magic 0x4ec4ec4f -> '% 13'
 */
ULONG __attribute__((ms_abi)) PiUEventHashStringIntoBucket(const uint16_t* SourceString) {
    uint8_t __attribute__((aligned(16))) ssa_stack_frame[4096];
    (*(uint32_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 3088ULL))) = (uint32_t)(0ULL);
    RtlInitUnicodeString((struct _UNICODE_STRING*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 3048ULL)), (const uint16_t*)(uintptr_t)(((uint64_t)(uintptr_t)SourceString)));
    uint64_t __attribute__((unused)) ssa_call_res_1 = (uint64_t)RtlHashUnicodeString((const struct _UNICODE_STRING*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 3048ULL)), (BOOLEAN)(1ULL), (ULONG)(0ULL), (ULONG*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 3088ULL)));
    uint32_t __attribute__((unused)) ssa_mem_ld_1 = (uint32_t)((*(uint32_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 3088ULL))));
    return (ULONG)(((uint64_t)(uint32_t)(((uint32_t)(((uint32_t)(((uint32_t)(uint64_t)(((uint64_t)(((uint64_t)(((uint64_t)(uint32_t)(ssa_mem_ld_1))) * (uint64_t)(0x4ec4ec4fULL))) >> (uint64_t)(0x22ULL))))) * (uint32_t)(0xfffffffffffffff3ULL))) + (uint32_t)(ssa_mem_ld_1)))));
}
