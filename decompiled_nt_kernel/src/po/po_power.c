/*
 * Reconstructed Windows NT Kernel Subsystem: Power Manager, Device Power IRPs & Idle States (Po/Pop)
 * Source Binary : ntoskrnl.exe (10.0.16299.15 x64, RSDS 83DB4240-4EFD-4AB6-AFB6-FA864B700CB3)
 * Functions     : 17 Z3-verified / assembly-preserved routines
 */
#include "../../include/nt_reconstructed_types.h"

/*
 * Function : PoEndDeviceBusy @ 0x140234e50 (5 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.20 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) PoEndDeviceBusy(uint32_t* arg1) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(8ULL)))));
    (*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(8ULL)))) = (uint32_t)(((uint32_t)(ssa_mem_ld) + (uint32_t)(0xffffffffffffffffULL)));
    return;
}

/*
 * Function : PoSetDeviceBusyEx @ 0x140137f20 (5 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.34 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) PoSetDeviceBusyEx(uint32_t* arg1) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(4ULL)))));
    (*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(4ULL)))) = (uint32_t)(((uint32_t)(ssa_mem_ld) + (uint32_t)(1ULL)));
    return;
}

/*
 * Function : PoEnergyEstimationEnabled @ 0x140038840 (7 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.37 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint8_t __attribute__((ms_abi)) PoEnergyEstimationEnabled(void) {
    uint8_t __attribute__((unused)) ssa_mem_ld = (uint8_t)((*(uint8_t*)(uintptr_t)(((uint64_t)(uintptr_t)&PopEnergyEstimationEnabled))));
    return (uint8_t)(((uint64_t)(uint8_t)(ssa_mem_ld)));
}

/*
 * Function : PoSetFixedWakeSource @ 0x140427b60 (7 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.98 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) PoSetFixedWakeSource(uint32_t arg1) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)&PopFixedWakeSourceMask))));
    (*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)&PopFixedWakeSourceMask))) = (uint32_t)(((uint32_t)(ssa_mem_ld) | (uint32_t)(((uint32_t)(uint64_t)(((uint64_t)arg1))))));
    return;
}

/*
 * Function : PoIsRunningStopWatch @ 0x140045d30 (9 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.11 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
BOOLEAN __attribute__((ms_abi)) PoIsRunningStopWatch(uint64_t* arg1) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x18ULL)))));
    return (BOOLEAN)(((uint64_t)(uint8_t)(((uint64_t)(ssa_mem_ld) != (uint64_t)(0ULL)))));
}

/*
 * Function : PoStartDeviceBusy @ 0x140234e60 (9 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.24 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) PoStartDeviceBusy(PRTL_DYNAMIC_HASH_TABLE arg1) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)(arg1->Shift);
    arg1->Shift = (uint32_t)(((uint32_t)(ssa_mem_ld) + (uint32_t)(1ULL)));
    uint32_t __attribute__((unused)) ssa_mem_ld_1 = (uint32_t)(arg1->TableSize);
    arg1->TableSize = (uint32_t)(((uint32_t)(ssa_mem_ld_1) + (uint32_t)(1ULL)));
    return;
}

/*
 * Function : PopSetShutdownMarker @ 0x1406ec260 (11 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.43 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) PopSetShutdownMarker(void) {
    (*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)&PopBsdShutdownInProgress))) = (uint32_t)(1ULL);
    return;
}

/*
 * Function : PopIsUserInitiatedMonitorRequestReason @ 0x140234da8 (21 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (21.27 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) PopIsUserInitiatedMonitorRequestReason(uint32_t arg1) {
    uint32_t __attribute__((unused)) ssa_r_ecx = (uint32_t)(((uint32_t)(uint64_t)(((uint64_t)arg1))));
    return (uint32_t)(((((((uint32_t)(ssa_r_ecx) < (uint32_t)(0x1fULL))) ? (((((uint32_t)(0x4000c802ULL) >> (uint32_t)(ssa_r_ecx))) & 1U)) : (0ULL))) ? (0x4000c801ULL) : (0ULL)));
}

/*
 * Function : PopExecutionRequiredCallback @ 0x140582b80 (24 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.24 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) PopExecutionRequiredCallback(uint64_t arg1, uint64_t arg2) {
    uint64_t __attribute__((unused)) ssa_call_res = PopHandleConvergedPowerRequestUpdate(((uint64_t)arg2), 3ULL, 0ULL, 0ULL);
    return (uint64_t)(0ULL);
}

/*
 * Function : PopIntSteerSetTimeUnparkTrigger @ 0x140141f20 (24 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.95 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
NTSTATUS __attribute__((ms_abi)) PopIntSteerSetTimeUnparkTrigger(uint64_t arg1, uint32_t* arg2, uint32_t arg3) {
    uint64_t __attribute__((unused)) ssa_common_ret_op = 0;
    if (((uint32_t)(((uint32_t)(uint64_t)(((uint64_t)arg3)))) == (uint32_t)(4ULL))) {
        uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg2))));
        (*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)&PpmIntSteerTriggerMax))) = (uint64_t)(((uint64_t)(uint32_t)(ssa_mem_ld)));
        ssa_common_ret_op = (uint64_t)(0ULL);
    }
    else {
        ssa_common_ret_op = (uint64_t)(0xc000000dULL);
    }
    return (NTSTATUS)(ssa_common_ret_op);
}

/*
 * Function : PoFxIssueComponentPerfStateChange @ 0x140234fc0 (35 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.70 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) PoFxIssueComponentPerfStateChange(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4) {
    uint64_t __attribute__((unused)) ssa_call_res = PoFxIssueComponentPerfStateChangeMultiple(((uint64_t)arg1), ((uint64_t)arg2), ((uint64_t)arg3), 1ULL);
    return (uint64_t)(ssa_call_res);
}

/*
 * Function : PoGetThermalRequestSupport @ 0x1406ed550 (35 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (6.78 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
BOOLEAN __attribute__((ms_abi)) PoGetThermalRequestSupport(uint64_t* arg1, uint32_t arg2, PLOG_HANDLE_CONTEXT arg3) {
    uint64_t __attribute__((unused)) ssa__sink = 0;
    uint64_t __attribute__((unused)) ssa_reg_rax_0 = 0;
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x20ULL)))));
    switch (((uint32_t)(uint64_t)(((uint64_t)arg2)))) {
        case 0LL:
            goto L_bb_1406ed568;
        case 1LL:
            ssa__sink = (uint64_t)(0x80ULL);
            goto L_bb_1406ed56f;
        default:
            ssa_reg_rax_0 = (uint64_t)(0ULL);
            goto L_bb_1406ed572;
    }
L_bb_1406ed568:
    ssa__sink = (uint64_t)(0x88ULL);
L_bb_1406ed56f:
    uint64_t __attribute__((unused)) ssa_flg_zf_0_in = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld) + (uint64_t)(ssa__sink)))));
    ssa_reg_rax_0 = (uint64_t)(((uint64_t)(uint8_t)(((uint64_t)(ssa_flg_zf_0_in) != (uint64_t)(0ULL)))));
L_bb_1406ed572:
    return (BOOLEAN)(ssa_reg_rax_0);
}

/*
 * Function : PopAcquireTransitionLock @ 0x1406f78bc (37 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.54 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) PopAcquireTransitionLock(void) {
    uint64_t __attribute__((unused)) ssa_call_res = KeWaitForSingleObject(((uint64_t)(uintptr_t)&PopTransitionLock), 7ULL, 0ULL, 0ULL);
    return (uint64_t)(ssa_call_res);
}

/*
 * Function : PoGetSystemWake @ 0x140141150 (39 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (6.13 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
int32_t __attribute__((ms_abi)) PoGetSystemWake(uint8_t* arg1, uint64_t arg2, struct _RECON_POGETSYSTEMWAKE_R8* arg3) {
    uint64_t __attribute__((unused)) ssa_reg_rax_0 = 0;
    uint8_t __attribute__((unused)) ssa_mem_ld = (uint8_t)((*(uint8_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x42ULL)))));
    uint64_t __attribute__((unused)) ssa__48 = (uint64_t)(((uint64_t)(int64_t)(int32_t)(((uint32_t)(((uint32_t)(int32_t)(int8_t)(ssa_mem_ld))) + (uint32_t)(0xffffffffffffffffULL)))));
    uint64_t __attribute__((unused)) ssa__54 = (uint64_t)(((uint64_t)(ssa__48) & (uint64_t)(0xffffffffffffff00ULL)));
    uint64_t __attribute__((unused)) ssa_mem_ld_1 = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x110ULL))) + (uint64_t)(((uint64_t)(ssa__48) * (uint64_t)(0x48ULL)))))));
    uint8_t __attribute__((unused)) ssa_mem_ld_2 = (uint8_t)((*(uint8_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld_1) + (uint64_t)(0xb8ULL)))));
    if (((uint8_t)(ssa_mem_ld_2) == (uint8_t)(0ULL))) {
        uint8_t __attribute__((unused)) ssa_mem_ld_3 = (uint8_t)((*(uint8_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld_1) + (uint64_t)(0xf0ULL)))));
        ssa_reg_rax_0 = (uint64_t)(((uint64_t)(ssa__54) | (uint64_t)(((uint64_t)(uint8_t)(ssa_mem_ld_3)))));
    }
    else {
        ssa_reg_rax_0 = (uint64_t)(ssa__54);
    }
    return (int32_t)(ssa_reg_rax_0);
}

/*
 * Function : PoGetProcessorIdleAccounting @ 0x140231f10 (42 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (8.05 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) PoGetProcessorIdleAccounting(uint64_t* arg1) {
    uint64_t __attribute__((unused)) ssa__81_sink = 0;
    uint64_t __attribute__((unused)) ssa_common_ret_op_ph = 0;
    uint64_t __attribute__((unused)) ssa_common_ret_op = 0;
    struct _KPRCB* __attribute__((unused)) ssa_gs_val = KeGetCurrentPrcb();
    struct _PROC_IDLE_ACCOUNTING* __attribute__((unused)) ssa_mem_ld = (struct _PROC_IDLE_ACCOUNTING*)(uintptr_t)(ssa_gs_val->PowerState.IdleAccounting);
    if (((uint64_t)(((uint64_t)(uintptr_t)ssa_mem_ld)) == (uint64_t)(0ULL))) {
        goto L_bb_140231f25;
    }
    goto L_bb_140231f2e;
L_bb_140231f25:
    if (((uint64_t)(((uint64_t)(uintptr_t)arg1)) == (uint64_t)(0ULL))) {
        ssa_common_ret_op = (uint64_t)(((uint64_t)(uintptr_t)ssa_gs_val));
        goto L_common_ret;
    }
    ssa__81_sink = (uint64_t)(0ULL);
    ssa_common_ret_op_ph = (uint64_t)(((uint64_t)(uintptr_t)ssa_gs_val));
L_common_ret_sink_split:
    (*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))) = (uint64_t)(ssa__81_sink);
    ssa_common_ret_op = (uint64_t)(ssa_common_ret_op_ph);
L_common_ret:
    return (uint64_t)(ssa_common_ret_op);
L_bb_140231f2e:
    if (((uint64_t)(((uint64_t)(uintptr_t)arg1)) == (uint64_t)(0ULL))) {
        ssa_common_ret_op = (uint64_t)(((uint64_t)(uintptr_t)ssa_gs_val));
        goto L_common_ret;
    }
    uint32_t __attribute__((unused)) ssa_mem_ld_2 = (uint32_t)(ssa_mem_ld->TotalTransitions);
    uint64_t __attribute__((unused)) ssa__81 = (uint64_t)(((uint64_t)(uint32_t)(ssa_mem_ld_2)));
    ssa__81_sink = (uint64_t)(ssa__81);
    ssa_common_ret_op_ph = (uint64_t)(ssa__81);
    goto L_common_ret_sink_split;
}

/*
 * Function : PopGetTransitionsToOnCount @ 0x14023f1c0 (44 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.98 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) PopGetTransitionsToOnCount(void) {
    uint64_t __attribute__((unused)) ssa_call_res = PopAcquireRwLockExclusive(((uint64_t)(uintptr_t)&PopTelemetryOsState), 0ULL, 0ULL, 0ULL);
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)&__nt_data_14035cd88))));
    uint64_t __attribute__((unused)) ssa_call_res_1 = PopReleaseRwLock(((uint64_t)(uintptr_t)&PopTelemetryOsState), 0ULL, 0ULL, 0ULL);
    return (uint32_t)(((uint64_t)(uint32_t)(ssa_mem_ld)));
}

/*
 * Function : PopDispatchCallback @ 0x1405db930 (48 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (22.10 ms, 33 SMT assertions)
 * Verified : x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) PopDispatchCallback(uint64_t arg1) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)&ExCbPowerState))));
    uint64_t __attribute__((unused)) ssa_call_res = ExNotifyCallback(ssa_mem_ld, ((uint64_t)(((uint64_t)arg1)) & (uint64_t)(0xffffffffULL)), 0ULL, 0ULL);
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    return (uint64_t)(((uint64_t)(uintptr_t)ssa_gs_val));
}
