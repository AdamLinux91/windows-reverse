/*
 * Reconstructed Windows NT Kernel Subsystem: I/O Manager, IRP Stack, WorkItems & Plug-and-Play (Io/Iop/Pi/Pnp)
 * Source Binary : ntoskrnl.exe (10.0.16299.15 x64, RSDS 83DB4240-4EFD-4AB6-AFB6-FA864B700CB3)
 * Functions     : 34 Z3-verified / assembly-preserved routines
 */
#include "../../include/nt_reconstructed_types.h"

/*
 * Function : IoGetAttachedDevice @ 0x140055bd0 (25 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (13353.90 ms, 33 SMT assertions)
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
 * Z3 Proof : VERIFIED_EQUIVALENT (16.35 ms, 33 SMT assertions)
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
 * Z3 Proof : VERIFIED_EQUIVALENT (4.30 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
struct _EPROCESS* __attribute__((ms_abi)) IoGetCurrentProcess(void) {
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    struct _KPROCESS* __attribute__((unused)) ssa_mem_ld = (struct _KPROCESS*)(uintptr_t)(ssa_gs_val->ApcState.Process);
    return (struct _EPROCESS*)(uintptr_t)(((uint64_t)(uintptr_t)ssa_mem_ld));
}

/*
 * Function : IoThreadToProcess @ 0x140050520 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.55 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
struct _EPROCESS* __attribute__((ms_abi)) IoThreadToProcess(struct _ETHREAD* Thread) {
    struct _KPROCESS* __attribute__((unused)) ssa_mem_ld = (struct _KPROCESS*)(uintptr_t)(Thread->Tcb.Process);
    return (struct _EPROCESS*)(uintptr_t)(((uint64_t)(uintptr_t)ssa_mem_ld));
}

/*
 * Function : IoIs32bitProcess @ 0x140022e20 (167 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (45.66 ms, 33 SMT assertions)
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
    uint64_t __attribute__((unused)) ssa_mem_ld_7 = (uint64_t)((*(uint64_t*)((uint8_t*)(uintptr_t)(ssa_mem_ld_6) + 0x428)));
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
 * Z3 Proof : VERIFIED_EQUIVALENT (10.87 ms, 38 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence, magic_mod_equivalence:mod_13
 * De-Opt   : Strength-reduced magic 0x4ec4ec4f -> '% 13'
 */
ULONG __attribute__((ms_abi)) PiUEventHashStringIntoBucket(const uint16_t* SourceString) {
    uint8_t __attribute__((aligned(16))) ssa_stack_frame[1024];
    (*(uint32_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 784ULL))) = (uint32_t)(0ULL);
    RtlInitUnicodeString((struct _UNICODE_STRING*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 744ULL)), (const uint16_t*)(uintptr_t)(((uint64_t)(uintptr_t)SourceString)));
    uint64_t __attribute__((unused)) ssa_call_res_1 = (uint64_t)RtlHashUnicodeString((const struct _UNICODE_STRING*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 744ULL)), (BOOLEAN)(1ULL), (ULONG)(0ULL), (ULONG*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 784ULL)));
    uint32_t __attribute__((unused)) ssa_mem_ld_1 = (uint32_t)((*(uint32_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 784ULL))));
    return (ULONG)(((uint64_t)(uint32_t)(((uint32_t)(((uint32_t)(((uint32_t)(uint64_t)(((uint64_t)(((uint64_t)(((uint64_t)(uint32_t)(ssa_mem_ld_1))) * (uint64_t)(0x4ec4ec4fULL))) >> (uint64_t)(0x22ULL))))) * (uint32_t)(0xfffffffffffffff3ULL))) + (uint32_t)(ssa_mem_ld_1)))));
}

/*
 * Function : IoIsFileOriginRemote @ 0x140112f80 (6 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.27 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint8_t __attribute__((ms_abi)) IoIsFileOriginRemote(uint8_t* arg1) {
    uint8_t __attribute__((unused)) ssa_mem_ld = (uint8_t)((*(uint8_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x53ULL)))));
    return (uint8_t)(((uint64_t)(uint8_t)(((uint8_t)(ssa_mem_ld) & (uint8_t)(1ULL)))));
}

/*
 * Function : IoSizeofGenericIrpExtension @ 0x1401f46e0 (6 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.40 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) IoSizeofGenericIrpExtension(void) {
    return (uint32_t)(4ULL);
}

/*
 * Function : IoSizeofWorkItem @ 0x14010de30 (6 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.03 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) IoSizeofWorkItem(void) {
    return (uint32_t)(0x58ULL);
}

/*
 * Function : IoGetConfigurationInformation @ 0x1405ac970 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.22 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) IoGetConfigurationInformation(void) {
    return (uint64_t)(((uint64_t)(uintptr_t)&__nt_rodata_1407ea710));
}

/*
 * Function : IoGetDeviceToVerify @ 0x1401f2900 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.13 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) IoGetDeviceToVerify(uint64_t* arg1) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x688ULL)))));
    return (uint64_t)(ssa_mem_ld);
}

/*
 * Function : IoGetFileObjectGenericMapping @ 0x140555d60 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.37 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) IoGetFileObjectGenericMapping(void) {
    return (uint64_t)(((uint64_t)(uintptr_t)&__nt_rodata_1407835d0));
}

/*
 * Function : IoSetDeviceToVerify @ 0x140012480 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.99 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) IoSetDeviceToVerify(uint64_t* arg1, uint64_t arg2) {
    (*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x688ULL)))) = (uint64_t)(((uint64_t)arg2));
    return;
}

/*
 * Function : IoClearIrpExtraCreateParameter @ 0x1401f2740 (13 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.43 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) IoClearIrpExtraCreateParameter(PIRP arg1) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)(arg1->Flags);
    if (((uint32_t)(((uint32_t)(ssa_mem_ld) & (uint32_t)(0x80ULL))) == (uint32_t)(0ULL))) {
    } else {
        arg1->UserBuffer = (void*)(uintptr_t)(0ULL);
    }
    return (uint32_t)(((uint64_t)(uint32_t)(ssa_mem_ld)));
}

/*
 * Function : IoGetInitialStack @ 0x1406ae750 (14 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.51 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) IoGetInitialStack(void) {
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)(uintptr_t)(ssa_gs_val->InitialStack);
    return (uint64_t)(ssa_mem_ld);
}

/*
 * Function : IoClearActivityIdThread @ 0x140024020 (17 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.62 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) IoClearActivityIdThread(uint64_t arg1) {
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    (*(uint64_t*)((uint8_t*)(uintptr_t)(ssa_gs_val) + 0x758)) = (uint64_t)(((uint64_t)arg1));
    return (uint64_t)(((uint64_t)(uintptr_t)ssa_gs_val));
}

/*
 * Function : IoGetActivityIdThread @ 0x140024530 (17 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.42 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) IoGetActivityIdThread(void) {
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)((uint8_t*)(uintptr_t)(ssa_gs_val) + 0x758)));
    return (uint64_t)(ssa_mem_ld);
}

/*
 * Function : IoGetTopLevelIrp @ 0x1400aba00 (17 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.50 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) IoGetTopLevelIrp(void) {
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)((uint8_t*)(uintptr_t)(ssa_gs_val) + 0x680)));
    return (uint64_t)(ssa_mem_ld);
}

/*
 * Function : IoSetTopLevelIrp @ 0x1400ab9e0 (17 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.55 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) IoSetTopLevelIrp(uint64_t arg1) {
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    (*(uint64_t*)((uint8_t*)(uintptr_t)(ssa_gs_val) + 0x680)) = (uint64_t)(((uint64_t)arg1));
    return (uint64_t)(((uint64_t)(uintptr_t)ssa_gs_val));
}

/*
 * Function : IoIsWdmVersionAvailable @ 0x1406b65c0 (19 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.44 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint8_t __attribute__((ms_abi)) IoIsWdmVersionAvailable(uint8_t arg1, uint8_t arg2) {
    uint64_t __attribute__((unused)) ssa_reg_rax_1 = 0;
    uint8_t __attribute__((unused)) ssa_r_cl = (uint8_t)(((uint8_t)(uint64_t)(((uint64_t)arg1))));
    if (((uint8_t)(ssa_r_cl) < (uint8_t)(6ULL))) {
        goto L_bb_1406b65d0;
    }
    if (((((uint8_t)(ssa_r_cl) == (uint8_t)(6ULL))) ? (((uint8_t)(((uint8_t)(uint64_t)(((uint64_t)arg2)))) == (uint8_t)(0ULL))) : (0ULL))) {
        goto L_bb_1406b65d0;
    }
    ssa_reg_rax_1 = (uint64_t)(0ULL);
    goto L_bb_1406b65d2;
L_bb_1406b65d0:
    ssa_reg_rax_1 = (uint64_t)(1ULL);
L_bb_1406b65d2:
    return (uint8_t)(ssa_reg_rax_1);
}

/*
 * Function : IoInitializeMiniCompletionPacket @ 0x1406af2e0 (20 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (6.01 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) IoInitializeMiniCompletionPacket(PIO_MINI_COMPLETION_PACKET_USER arg1, uint64_t arg2, uint64_t arg3) {
    arg1->PacketType = (uint32_t)(4ULL);
    arg1->MiniPacketCallback = (void**)(uintptr_t)(((uint64_t)arg2));
    arg1->Context = (void*)(uintptr_t)(((uint64_t)arg3));
    arg1->Allocated = (uint8_t)(0ULL);
    return;
}

/*
 * Function : IoPropagateIrpExtension @ 0x1400eb6f0 (20 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.79 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) IoPropagateIrpExtension(uint64_t arg1, uint64_t arg2, uint32_t arg3) {
    uint64_t __attribute__((unused)) ssa_call_res = IoPropagateIrpExtensionEx(((uint64_t)arg1), ((uint64_t)arg2), 0ULL, ((uint64_t)(((uint64_t)arg3)) & (uint64_t)(0xffffffffULL)));
    return (uint64_t)(ssa_call_res);
}

/*
 * Function : IoSetHardErrorOrVerifyDevice @ 0x1401f2f70 (20 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.26 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) IoSetHardErrorOrVerifyDevice(uint64_t* arg1, uint64_t arg2) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x98ULL)))));
    if (((uint64_t)(ssa_mem_ld) == (uint64_t)(0ULL))) {
    } else {
        (*(uint64_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld) + (uint64_t)(0x688ULL)))) = (uint64_t)(((uint64_t)arg2));
    }
    return (uint64_t)(ssa_mem_ld);
}

/*
 * Function : IoSetActivityIdThread @ 0x140024040 (24 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.29 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) IoSetActivityIdThread(uint64_t arg1) {
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)((uint8_t*)(uintptr_t)(ssa_gs_val) + 0x758)));
    (*(uint64_t*)((uint8_t*)(uintptr_t)(ssa_gs_val) + 0x758)) = (uint64_t)(((uint64_t)arg1));
    return (uint64_t)(ssa_mem_ld);
}

/*
 * Function : IoQueueWorkItemEx @ 0x14003d080 (33 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.73 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) IoQueueWorkItemEx(uint64_t arg1, uint32_t arg2, uint64_t arg3, uint64_t arg4) {
    uint64_t __attribute__((unused)) ssa_call_res = IopQueueWorkItemProlog(((uint64_t)arg1), ((uint64_t)arg2), ((uint64_t)arg4), ((uint64_t)arg4));
    uint64_t __attribute__((unused)) ssa_call_res_1 = ExQueueWorkItemFromIo(ssa_call_res, ((uint64_t)(((uint64_t)arg3)) & (uint64_t)(0xffffffffULL)), ((uint64_t)arg4), ((uint64_t)arg4));
    return (uint64_t)(ssa_call_res_1);
}

/*
 * Function : IoGetPagingIoPriority @ 0x1400fc7f0 (34 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (7.93 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) IoGetPagingIoPriority(uint32_t* arg1) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x10ULL)))));
    return (uint32_t)(((((uint32_t)(((uint32_t)(ssa_mem_ld) & (uint32_t)(2ULL))) == (uint32_t)(0ULL))) ? (0ULL) : (((((uint32_t)(((uint32_t)(ssa_mem_ld) & (uint32_t)(0xe0000ULL))) == (uint32_t)(0xa0000ULL))) ? (2ULL) : (1ULL)))));
}

/*
 * Function : IoSetIoPriorityHint @ 0x1400f7950 (36 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (10.91 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
NTSTATUS __attribute__((ms_abi)) IoSetIoPriorityHint(uint32_t* arg1, uint32_t arg2) {
    uint64_t __attribute__((unused)) ssa_common_ret_op = 0;
    uint32_t __attribute__((unused)) ssa_r_edx = (uint32_t)(((uint32_t)(uint64_t)(((uint64_t)arg2))));
    if (((uint32_t)(ssa_r_edx) > (uint32_t)(4ULL))) {
        ssa_common_ret_op = (uint64_t)(0xc000000dULL);
    } else {
        uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x10ULL)))));
        (*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x10ULL)))) = (uint32_t)(((uint32_t)(((uint32_t)(ssa_mem_ld) & (uint32_t)(0xfffffffffff1ffffULL))) | (uint32_t)(((uint32_t)(((uint32_t)(ssa_r_edx) << (uint32_t)(0x11ULL))) + (uint32_t)(0x20000ULL)))));
        ssa_common_ret_op = (uint64_t)(0ULL);
    }
    return (NTSTATUS)(ssa_common_ret_op);
}

/*
 * Function : IoQueryFileInformation @ 0x14057c760 (37 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.91 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) IoQueryFileInformation(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4) {
    uint64_t __attribute__((unused)) ssa_call_res = IopQueryXxxInformation(((uint64_t)arg1), ((uint64_t)arg2), ((uint64_t)arg3), 0ULL);
    return (uint64_t)(ssa_call_res);
}

/*
 * Function : IoQueryVolumeInformation @ 0x14057c730 (37 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.93 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) IoQueryVolumeInformation(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4) {
    uint64_t __attribute__((unused)) ssa_call_res = IopQueryXxxInformation(((uint64_t)arg1), ((uint64_t)arg2), ((uint64_t)arg3), 0ULL);
    return (uint64_t)(ssa_call_res);
}

/*
 * Function : IoQueueWorkItem @ 0x14003cd90 (37 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.55 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) IoQueueWorkItem(uint32_t* arg1, uint32_t arg2, uint64_t arg3, uint64_t arg4) {
    (*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x40ULL)))) = (uint32_t)(0ULL);
    uint64_t __attribute__((unused)) ssa_call_res = IopQueueWorkItemProlog(((uint64_t)(uintptr_t)arg1), ((uint64_t)arg2), ((uint64_t)arg4), ((uint64_t)arg4));
    uint64_t __attribute__((unused)) ssa_call_res_1 = ExQueueWorkItemFromIo(ssa_call_res, ((uint64_t)(((uint64_t)arg3)) & (uint64_t)(0xffffffffULL)), ((uint64_t)arg4), ((uint64_t)arg4));
    return (uint64_t)(ssa_call_res_1);
}

extern void __nt_asm_ext_1401fe1d8(void);
extern unsigned char __nt_asm_sym_1403f9146[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] IoReleaseCancelSpinLock @ 0x1400f4430
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Privileged CPU/CSR instruction 'mov cr8, rax' at 0x1400f4480
 *   - Privileged/control-register routine (1/19 insns) preserved as native x86-64 assembly
 *   Live-In Registers : rbx, rdi, rcx
 *   Clobbered State   : rdi, rbx, rax, rdx, rcx, memory, cc
 */
uint32_t __attribute__((naked, ms_abi)) IoReleaseCancelSpinLock(uint64_t arg1) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov qword ptr [rsp + 0x10], rbx\n\t"
        "push rdi\n\t"
        "sub rsp, 0x20\n\t"
        "mov rbx, qword ptr gs:[0x28]\n\t"
        "movzx edi, cl\n\t"
        "add rbx, 0x70\n\t"
        "test byte ptr [rip + __nt_asm_sym_1403f9146], 1\n\t"
        "jne .L_IoReleaseCancelSpinLock_1401c11f2\n\t"
        ".L_IoReleaseCancelSpinLock_1400f447c:\n\t"
        "movzx eax, dil\n\t"
        "mov cr8, rax\n\t"
        "mov rbx, qword ptr [rsp + 0x38]\n\t"
        "add rsp, 0x20\n\t"
        "pop rdi\n\t"
        "ret\n\t"
        ".L_IoReleaseCancelSpinLock_1401c11f2:\n\t"
        "mov rdx, qword ptr [rsp + 0x28]\n\t"
        "mov rcx, rbx\n\t"
        "call __nt_asm_ext_1401fe1d8\n\t"
        "nop\n\t"
        "jmp .L_IoReleaseCancelSpinLock_1400f447c\n\t"
        ".att_syntax prefix"
    );
}

/*
 * Function : IoGetActivityIdIrp @ 0x1400e8870 (47 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (14.42 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
NTSTATUS __attribute__((ms_abi)) IoGetActivityIdIrp(struct _RECON_IOGETACTIVITYIDIRP_RCX* arg1, void* arg2) {
    uint64_t __attribute__((unused)) ssa_common_ret_op = 0;
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)(arg1->field_0xc8);
    uint8_t __attribute__((unused)) ssa_mem_ld_1 = (uint8_t)(arg1->field_0x47);
    if (((((int8_t)(ssa_mem_ld_1) < (int8_t)(0ULL))) ? (1U) : (((uint64_t)(ssa_mem_ld) == (uint64_t)(0ULL))))) {
        ssa_common_ret_op = (uint64_t)(0xc0000225ULL);
    } else {
        uint8_t __attribute__((unused)) ssa_mem_ld_2 = (uint8_t)((*(uint8_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld) + (uint64_t)(2ULL)))));
        ssa_common_ret_op = (uint64_t)(((((uint8_t)(((uint8_t)(ssa_mem_ld_2) & (uint8_t)(1ULL))) == (uint8_t)(0ULL))) ? (0xc0000225ULL) : (0ULL)));
    }
    return (NTSTATUS)(ssa_common_ret_op);
}

/*
 * Function : IoGetIoAttributionHandle @ 0x1400f1250 (47 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (26.34 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
NTSTATUS __attribute__((ms_abi)) IoGetIoAttributionHandle(struct _RECON_IOGETIOATTRIBUTIONHANDLE_RCX* arg1, uint64_t* arg2) {
    uint64_t __attribute__((unused)) ssa_common_ret_op = 0;
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)(arg1->field_0xc8);
    uint8_t __attribute__((unused)) ssa_mem_ld_1 = (uint8_t)(arg1->field_0x47);
    if (((((int8_t)(ssa_mem_ld_1) < (int8_t)(0ULL))) ? (1U) : (((uint64_t)(ssa_mem_ld) == (uint64_t)(0ULL))))) {
        ssa_common_ret_op = (uint64_t)(0xc0000225ULL);
        goto L_common_ret;
    }
    uint8_t __attribute__((unused)) ssa_mem_ld_2 = (uint8_t)((*(uint8_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld) + (uint64_t)(2ULL)))));
    if (((uint8_t)(((uint8_t)(ssa_mem_ld_2) & (uint8_t)(0x40ULL))) == (uint8_t)(0ULL))) {
        ssa_common_ret_op = (uint64_t)(0xc0000225ULL);
        goto L_common_ret;
    }
    goto L_bb_1400f126e;
L_common_ret:
    return (NTSTATUS)(ssa_common_ret_op);
L_bb_1400f126e:
    uint64_t __attribute__((unused)) ssa_mem_ld_4 = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld) + (uint64_t)(0x10ULL)))));
    (*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg2))) = (uint64_t)(ssa_mem_ld_4);
    ssa_common_ret_op = (uint64_t)(0ULL);
    goto L_common_ret;
}

/*
 * Function : IoSizeOfIrpEx @ 0x1401f3040 (48 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (29.74 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
int32_t __attribute__((ms_abi)) IoSizeOfIrpEx(uint32_t* arg1, int8_t arg2) {
    uint8_t __attribute__((unused)) ssa_reg_rdx_0_off0 = 0;
    uint8_t __attribute__((unused)) ssa_extract_t = (uint8_t)(((uint8_t)(uint64_t)(((uint64_t)arg2))));
    switch (((uint64_t)(uintptr_t)arg1)) {
        case 0LL:
            ssa_reg_rdx_0_off0 = (uint8_t)(ssa_extract_t);
            goto L_bb_1401f3056;
        case -1LL:
            goto L_bb_1401f3054;
        default:
            goto L_bb_1401f304b;
    }
L_bb_1401f304b:
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x30ULL)))));
    if (((uint32_t)(((uint32_t)(ssa_mem_ld) & (uint32_t)(0x8000000ULL))) == (uint32_t)(0ULL))) {
        ssa_reg_rdx_0_off0 = (uint8_t)(ssa_extract_t);
        goto L_bb_1401f3056;
    }
L_bb_1401f3054:
    ssa_reg_rdx_0_off0 = (uint8_t)(((uint8_t)(ssa_extract_t) + (uint8_t)(1ULL)));
L_bb_1401f3056:
    return (int32_t)(((uint64_t)(uint32_t)(((uint32_t)(((uint32_t)(((uint32_t)(int32_t)(int8_t)(ssa_reg_rdx_0_off0))) & (uint32_t)(0xffffffffffff0000ULL))) | (uint32_t)(((uint32_t)(uint16_t)(((uint16_t)(((uint16_t)(((uint16_t)(int16_t)(int8_t)(ssa_reg_rdx_0_off0))) * (uint16_t)(0x48ULL))) + (uint16_t)(0xd0ULL)))))))));
}
