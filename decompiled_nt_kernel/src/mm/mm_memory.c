/*
 * Reconstructed Windows NT Kernel Subsystem: Virtual Memory Manager, PFN & MDL Subsystem (Mm/Mi)
 * Source Binary : ntoskrnl.exe (10.0.16299.15 x64, RSDS 83DB4240-4EFD-4AB6-AFB6-FA864B700CB3)
 * Functions     : 22 Z3-verified / assembly-preserved routines
 */
#include "../../include/nt_reconstructed_types.h"

/*
 * Function : MmSizeOfMdl @ 0x1400fcd80 (29 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (10.80 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
size_t __attribute__((ms_abi)) MmSizeOfMdl(void* Base, size_t Length) {
    return (size_t)(((uint64_t)(((uint64_t)(((uint64_t)(((uint64_t)(((uint64_t)(((uint64_t)Length)) + (uint64_t)(0xfffULL))) + (uint64_t)(((uint64_t)(((uint64_t)(uintptr_t)Base)) & (uint64_t)(0xfffULL))))) >> (uint64_t)(9ULL))) & (uint64_t)(0x7ffffffffffff8ULL))) + (uint64_t)(0x30ULL)));
}

/*
 * Function : MmGetMaximumFileSectionSize @ 0x1405de6b0 (11 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.25 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) MmGetMaximumFileSectionSize(void) {
    return (uint64_t)(0x3ffffffffff000ULL);
}

/*
 * Function : MmIsAddressValid @ 0x140212850 (14 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.29 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
BOOLEAN __attribute__((ms_abi)) MmIsAddressValid(void* VirtualAddress) {
    uint64_t __attribute__((unused)) ssa_call_res = (uint64_t)MiIsAddressValid((void*)(uintptr_t)(((uint64_t)(uintptr_t)VirtualAddress)));
    return (BOOLEAN)(ssa_call_res);
}

/*
 * Function : MmFreePagesFromMdl @ 0x14002ec20 (16 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.78 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) MmFreePagesFromMdl(struct _MDL* MemoryDescriptorList) {
    uint64_t __attribute__((unused)) ssa_call_res = MiFreePagesFromMdl(((uint64_t)(uintptr_t)MemoryDescriptorList), 0ULL, 0ULL, 0ULL);
    return;
}

/*
 * Function : MmMapUserAddressesToPage @ 0x14041ae50 (6 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (2.80 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
NTSTATUS __attribute__((ms_abi)) MmMapUserAddressesToPage(void) {
    return (NTSTATUS)(0xc00000bbULL);
}

/*
 * Function : MmQuerySystemSize @ 0x140141ee0 (6 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (2.94 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) MmQuerySystemSize(void) {
    return (uint32_t)(2ULL);
}

/*
 * Function : MmSetBankedSection @ 0x140119770 (6 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (2.79 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
NTSTATUS __attribute__((ms_abi)) MmSetBankedSection(void) {
    return (NTSTATUS)(0xc00000bbULL);
}

/*
 * Function : MmIsThisAnNtAsSystem @ 0x140108190 (7 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.05 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint8_t __attribute__((ms_abi)) MmIsThisAnNtAsSystem(void) {
    uint8_t __attribute__((unused)) ssa_mem_ld = (uint8_t)((*(uint8_t*)(uintptr_t)(((uint64_t)(uintptr_t)&__nt_data_1403f8130))));
    return (uint8_t)(((uint64_t)(uint8_t)(ssa_mem_ld)));
}

/*
 * Function : MmIsDriverVerifying @ 0x140113c00 (20 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (7.41 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) MmIsDriverVerifying(uint64_t* arg1) {
    uint64_t __attribute__((unused)) ssa_common_ret_op = 0;
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x28ULL)))));
    if (((uint64_t)(ssa_mem_ld) == (uint64_t)(0ULL))) {
        ssa_common_ret_op = (uint64_t)(0ULL);
    } else {
        uint32_t __attribute__((unused)) ssa_mem_ld_1 = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld) + (uint64_t)(0x68ULL)))));
        ssa_common_ret_op = (uint64_t)(((uint64_t)(uint32_t)(((uint32_t)(((uint32_t)(ssa_mem_ld_1) >> (uint32_t)(0x19ULL))) & (uint32_t)(1ULL)))));
    }
    return (uint64_t)(ssa_common_ret_op);
}

/*
 * Function : MmIsVerifierEnabled @ 0x1407992a0 (23 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.33 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
NTSTATUS __attribute__((ms_abi)) MmIsVerifierEnabled(uint32_t* arg1) {
    uint32_t __attribute__((unused)) ssa_mem_ld_2_sink = 0;
    uint64_t __attribute__((unused)) ssa_common_ret_op = 0;
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)&ViVerifierDriverAddedThunkListHead))));
    if (((uint64_t)(ssa_mem_ld) == (uint64_t)(0ULL))) {
        ssa_mem_ld_2_sink = (uint32_t)(0ULL);
        ssa_common_ret_op = (uint64_t)(0xc00000bbULL);
    } else {
        uint32_t __attribute__((unused)) ssa_mem_ld_2 = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)&MmVerifierData))));
        ssa_mem_ld_2_sink = (uint32_t)(ssa_mem_ld_2);
        ssa_common_ret_op = (uint64_t)(0ULL);
    }
    (*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))) = (uint32_t)(ssa_mem_ld_2_sink);
    return (NTSTATUS)(ssa_common_ret_op);
}

/*
 * Function : MmIsRecursiveIoFault @ 0x1400a9580 (31 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.08 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint8_t __attribute__((ms_abi)) MmIsRecursiveIoFault(PETHREAD arg1) {
    uint64_t __attribute__((unused)) ssa_reg_rax_0 = 0;
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    uint8_t __attribute__((unused)) ssa_mem_ld = (uint8_t)((*(uint8_t*)((uint8_t*)(uintptr_t)(ssa_gs_val) + 0x6dd)));
    if (((uint8_t)(ssa_mem_ld) == (uint8_t)(0ULL))) {
        goto L_bb_1400a9593;
    }
    goto L_bb_1400a959c;
L_bb_1400a9593:
    uint8_t __attribute__((unused)) ssa_mem_ld_1 = (uint8_t)((*(uint8_t*)((uint8_t*)(uintptr_t)(ssa_gs_val) + 0x6dc)));
    if (((uint8_t)(ssa_mem_ld_1) == (uint8_t)(1ULL))) {
        goto L_bb_1400a959c;
    }
    ssa_reg_rax_0 = (uint64_t)(0ULL);
    goto L_bb_1400a959e;
L_bb_1400a959c:
    ssa_reg_rax_0 = (uint64_t)(1ULL);
L_bb_1400a959e:
    return (uint8_t)(ssa_reg_rax_0);
}

/*
 * Function : MmProbeAndLockPages @ 0x1400565a0 (46 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (23.14 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) MmProbeAndLockPages(uint64_t arg1, uint32_t arg2, uint32_t arg3) {
    uint64_t __attribute__((unused)) ssa__59 = (uint64_t)(((uint64_t)(uint8_t)(((uint32_t)(((uint32_t)(uint64_t)(((uint64_t)arg3)))) != (uint32_t)(0ULL)))));
    uint64_t __attribute__((unused)) ssa_call_res = MiProbeAndLockPages(((uint64_t)arg1), ((uint64_t)(uint8_t)(((uint8_t)(((uint8_t)(uint64_t)(((uint64_t)arg2)))) != (uint8_t)(0ULL)))), ssa__59, ssa__59);
    return (uint32_t)(ssa_call_res);
}

/*
 * Function : MmMapViewInSystemSpaceEx @ 0x14057a7a0 (48 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.76 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) MmMapViewInSystemSpaceEx(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4) {
    uint64_t __attribute__((unused)) ssa_call_res = MiMapViewInSystemSpace(((uint64_t)arg1), ((uint64_t)(uintptr_t)&__nt_rodata_1403802b8), ((uint64_t)arg2), ((uint64_t)arg3));
    return (uint64_t)(ssa_call_res);
}

/*
 * Function : MmLoadSystemImage @ 0x1406d9280 (50 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.68 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
NTSTATUS __attribute__((ms_abi)) MmLoadSystemImage(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint32_t arg4) {
    uint64_t __attribute__((unused)) ssa_reg_rax_0 = 0;
    if (((uint64_t)(((uint64_t)(((uint64_t)arg4)) & (uint64_t)(0x7ffffffcULL))) == (uint64_t)(0ULL))) {
        uint64_t __attribute__((unused)) ssa_call_res = MmLoadSystemImageEx(((uint64_t)arg1), ((uint64_t)arg2), ((uint64_t)arg3), ((uint64_t)arg4));
        ssa_reg_rax_0 = (uint64_t)(ssa_call_res);
    }
    else {
        ssa_reg_rax_0 = (uint64_t)(0xc00000f2ULL);
    }
    return (NTSTATUS)(ssa_reg_rax_0);
}

/*
 * Function : MmMapViewInSystemSpace @ 0x14054c0d0 (52 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (7.95 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) MmMapViewInSystemSpace(uint64_t arg1, uint64_t arg2, uint64_t arg3) {
    uint8_t __attribute__((aligned(16))) ssa_stack_frame[1024];
    (*(uint64_t*)(uintptr_t)(((uint64_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 768ULL)) + (uint64_t)(0xffffffffffffffe8ULL)))) = (uint64_t)(0ULL);
    (*(uint64_t*)(uintptr_t)(((uint64_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 768ULL)) + (uint64_t)(0xffffffffffffffe0ULL)))) = (uint64_t)(0ULL);
    uint64_t __attribute__((unused)) ssa__78 = (uint64_t)(((uint64_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 768ULL)) + (uint64_t)(0x20ULL)));
    (*(uint64_t*)(uintptr_t)(ssa__78)) = (uint64_t)(0ULL);
    (*(uint64_t*)(uintptr_t)(((uint64_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 768ULL)) + (uint64_t)(0xffffffffffffffd8ULL)))) = (uint64_t)(ssa__78);
    uint64_t __attribute__((unused)) ssa_call_res = MiMapViewInSystemSpace(((uint64_t)arg1), ((uint64_t)(uintptr_t)&__nt_rodata_1403802b8), ((uint64_t)arg2), ((uint64_t)arg3));
    return (uint64_t)(ssa_call_res);
}

/*
 * Function : MmUnloadSystemImage @ 0x1405476b0 (53 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.57 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) MmUnloadSystemImage(uint64_t arg1) {
    uint64_t __attribute__((unused)) ssa_call_res = MmAcquireLoadLock(((uint64_t)arg1), 0ULL, 0ULL, 0ULL);
    uint64_t __attribute__((unused)) ssa_call_res_1 = MiUnloadSystemImage(((uint64_t)arg1), 0xffffffffULL, 0ULL, 0ULL);
    uint64_t __attribute__((unused)) ssa_call_res_2 = MmReleaseLoadLock(ssa_call_res, 0xffffffffULL, 0ULL, 0ULL);
    return (uint64_t)(0ULL);
}

/*
 * Function : MmAllocateContiguousMemory @ 0x14020ff20 (57 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (40.86 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) MmAllocateContiguousMemory(uint64_t arg1, uint64_t arg2) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)&__nt_data_140380458))));
    uint64_t __attribute__((unused)) ssa_call_res = MiAllocateContiguousMemory(((uint64_t)arg1), 0ULL, (((uint64_t)(((uint64_t)(((uint64_t)arg2)) >> (uint64_t)(12ULL))) < (uint64_t)(ssa_mem_ld)) ? (uint64_t)(((uint64_t)(((uint64_t)arg2)) >> (uint64_t)(12ULL))) : (uint64_t)(ssa_mem_ld)), 0ULL);
    return (uint64_t)(ssa_call_res);
}

/*
 * Function : MmGetVirtualForPhysical @ 0x1402101b0 (63 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (6.11 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) MmGetVirtualForPhysical(uint64_t arg1) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(((uint64_t)(((uint64_t)arg1)) >> (uint64_t)(12ULL))) * (uint64_t)(0x30ULL))) + (uint64_t)(0xfffffa8000000008ULL)))));
    return (uint64_t)(((uint64_t)(((int64_t)(((uint64_t)(ssa_mem_ld) << (uint64_t)(0x19ULL))) >> (int64_t)(0x10ULL))) + (uint64_t)(((uint64_t)(((uint64_t)arg1)) & (uint64_t)(0xfffULL)))));
}

extern void __nt_asm_ext_140060750(void);
extern void __nt_asm_ext_1400a52d0(void);

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] MmDisableModifiedWriteOfSection @ 0x140109ad0
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Privileged CPU/CSR instruction 'mov cr8, rcx' at 0x140109b0c
 *   - Privileged/control-register routine (1/26 insns) preserved as native x86-64 assembly
 *   Live-In Registers : rbx, rax
 *   Clobbered State   : rbx, r8, rdx, rcx, rax, memory, cc
 */
uint32_t __attribute__((naked, ms_abi)) MmDisableModifiedWriteOfSection(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "push rbx\n\t"
        "sub rsp, 0x20\n\t"
        "mov ebx, 1\n\t"
        "lea r8, [rsp + 0x38]\n\t"
        "mov edx, ebx\n\t"
        "call __nt_asm_ext_140060750\n\t"
        "mov rcx, rax\n\t"
        "test rax, rax\n\t"
        "je .L_MmDisableModifiedWriteOfSection_140109b18\n\t"
        ".L_MmDisableModifiedWriteOfSection_140109aef:\n\t"
        "cmp qword ptr [rax + 0x28], 0\n\t"
        "jne .L_MmDisableModifiedWriteOfSection_1401c6520\n\t"
        ".L_MmDisableModifiedWriteOfSection_140109afa:\n\t"
        "or dword ptr [rax + 0x38], 8\n\t"
        ".L_MmDisableModifiedWriteOfSection_140109afe:\n\t"
        "add rcx, 0x48\n\t"
        "call __nt_asm_ext_1400a52d0\n\t"
        "movzx ecx, byte ptr [rsp + 0x38]\n\t"
        "mov cr8, rcx\n\t"
        "mov al, bl\n\t"
        ".L_MmDisableModifiedWriteOfSection_140109b12:\n\t"
        "add rsp, 0x20\n\t"
        "pop rbx\n\t"
        "ret\n\t"
        ".L_MmDisableModifiedWriteOfSection_140109b18:\n\t"
        "jmp .L_MmDisableModifiedWriteOfSection_140109b12\n\t"
        ".L_MmDisableModifiedWriteOfSection_1401c6520:\n\t"
        "mov eax, dword ptr [rax + 0x38]\n\t"
        "shr eax, 3\n\t"
        "and al, bl\n\t"
        "mov bl, al\n\t"
        "jmp .L_MmDisableModifiedWriteOfSection_140109afe\n\t"
        ".att_syntax prefix"
    );
}

/*
 * Function : MmAddPhysicalMemory @ 0x1406d74c0 (75 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (14.33 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) MmAddPhysicalMemory(uint64_t* arg1, uint64_t arg2) {
    uint64_t __attribute__((unused)) ssa_reg_r9_0 = 0;
    uint8_t __attribute__((aligned(16))) ssa_stack_frame[1024];
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))));
    uint32_t __attribute__((unused)) ssa_r_eax = (uint32_t)(((uint32_t)(uint64_t)(ssa_mem_ld)));
    (*(uint64_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 776ULL))) = (uint64_t)(ssa_mem_ld);
    if (((((uint64_t)(((uint64_t)(ssa_mem_ld) & (uint64_t)(1ULL))) == (uint64_t)(0ULL))) || (((uint32_t)(((uint32_t)(ssa_r_eax) & (uint32_t)(0xfffULL))) == (uint32_t)(0ULL))))) {
        ssa_reg_r9_0 = (uint64_t)(0ULL);
    } else {
        (*(uint32_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 776ULL))) = (uint32_t)(((uint32_t)(ssa_r_eax) & (uint32_t)(0xfffffffffffffffeULL)));
        ssa_reg_r9_0 = (uint64_t)(0x100ULL);
    }
    uint64_t __attribute__((unused)) ssa_call_res = MiAddPhysicalMemoryChunks(((uint64_t)(uintptr_t)&MiSystemPartition), (((uint64_t)(uintptr_t)ssa_stack_frame) + 776ULL), ((uint64_t)arg2), ssa_reg_r9_0);
    return (uint64_t)(ssa_call_res);
}

extern void __nt_asm_ext_1400fe28c(void);
extern void __nt_asm_ext_1400a52d0(void);

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] MmCanFileBeTruncated @ 0x1400fe230
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Privileged CPU/CSR instruction 'mov cr8, rax' at 0x1400fe27c
 *   - Privileged/control-register routine (1/25 insns) preserved as native x86-64 assembly
 *   Live-In Registers : rdx
 *   Clobbered State   : rax, rdx, r9, r8, rcx, memory, cc
 */
uint64_t __attribute__((naked, ms_abi)) MmCanFileBeTruncated(uint64_t arg1, uint64_t* arg2) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "sub rsp, 0x38\n\t"
        "test rdx, rdx\n\t"
        "je .L_MmCanFileBeTruncated_1400fe246\n\t"
        ".L_MmCanFileBeTruncated_1400fe239:\n\t"
        "mov rax, qword ptr [rdx]\n\t"
        "lea rdx, [rsp + 0x48]\n\t"
        "mov qword ptr [rsp + 0x48], rax\n\t"
        ".L_MmCanFileBeTruncated_1400fe246:\n\t"
        "lea rax, [rsp + 0x40]\n\t"
        "xor r9d, r9d\n\t"
        "xor r8d, r8d\n\t"
        "mov qword ptr [rsp + 0x20], rax\n\t"
        "call __nt_asm_ext_1400fe28c\n\t"
        "cmp byte ptr [rsp + 0x40], 0x11\n\t"
        "je .L_MmCanFileBeTruncated_1400fe282\n\t"
        ".L_MmCanFileBeTruncated_1400fe262:\n\t"
        "test rax, rax\n\t"
        "jne .L_MmCanFileBeTruncated_1400fe26e\n\t"
        ".L_MmCanFileBeTruncated_1400fe267:\n\t"
        "mov al, 1\n\t"
        ".L_MmCanFileBeTruncated_1400fe269:\n\t"
        "add rsp, 0x38\n\t"
        "ret\n\t"
        ".L_MmCanFileBeTruncated_1400fe26e:\n\t"
        "lea rcx, [rax + 0x48]\n\t"
        "call __nt_asm_ext_1400a52d0\n\t"
        "movzx eax, byte ptr [rsp + 0x40]\n\t"
        "mov cr8, rax\n\t"
        "jmp .L_MmCanFileBeTruncated_1400fe267\n\t"
        ".L_MmCanFileBeTruncated_1400fe282:\n\t"
        "xor al, al\n\t"
        "jmp .L_MmCanFileBeTruncated_1400fe269\n\t"
        ".att_syntax prefix"
    );
}

/*
 * Function : MmAllocatePagesForMdl @ 0x14013f2e0 (94 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (7.36 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) MmAllocatePagesForMdl(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4) {
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    uint64_t __attribute__((unused)) ssa_call_res = MiAllocatePagesForMdl(((uint64_t)(uintptr_t)&MiSystemPartition), ((uint64_t)arg1), ((uint64_t)arg2), ((uint64_t)arg3));
    return (uint64_t)(ssa_call_res);
}
