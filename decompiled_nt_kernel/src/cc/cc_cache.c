/*
 * Reconstructed Windows NT Kernel Subsystem: Common Cache Manager, BCBs & Dirty Page Tracking (Cc)
 * Source Binary : ntoskrnl.exe (10.0.16299.15 x64, RSDS 83DB4240-4EFD-4AB6-AFB6-FA864B700CB3)
 * Functions     : 16 Z3-verified / assembly-preserved routines
 */
#include "../../include/nt_reconstructed_types.h"

/*
 * Function : CcTestControl @ 0x1401420c0 (6 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (2.91 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
NTSTATUS __attribute__((ms_abi)) CcTestControl(void) {
    return (NTSTATUS)(0xc0000002ULL);
}

/*
 * Function : CcSetReadAheadGranularity @ 0x140107c40 (10 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.59 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) CcSetReadAheadGranularity(uint64_t* arg1, uint32_t arg2) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x30ULL)))));
    (*(uint32_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld) + (uint64_t)(4ULL)))) = (uint32_t)(((uint32_t)(((uint32_t)(uint64_t)(((uint64_t)arg2)))) + (uint32_t)(0xffffffffffffffffULL)));
    return (uint64_t)(ssa_mem_ld);
}

/*
 * Function : CcMapAndRead @ 0x1400c7a50 (12 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (28.86 ms, 33 SMT assertions)
 * Verified : x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) CcMapAndRead(uint64_t arg1, uint64_t arg2, uint8_t arg3) {
    uint8_t __attribute__((aligned(16))) ssa_stack_frame[1024];
    (*(uint8_t*)(uintptr_t)(((uint64_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 768ULL)) + (uint64_t)(0x18ULL)))) = (uint8_t)(((uint8_t)(uint64_t)(((uint64_t)arg3))));
    return (uint64_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 768ULL));
}

/*
 * Function : CcGetFileObjectFromBcb @ 0x1401d9900 (16 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.09 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) CcGetFileObjectFromBcb(uint64_t* arg1) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0xb0ULL)))));
    uint64_t __attribute__((unused)) ssa_mem_ld_1 = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld) + (uint64_t)(0x60ULL)))));
    return (uint64_t)(((uint64_t)(ssa_mem_ld_1) & (uint64_t)(0xfffffffffffffff0ULL)));
}

/*
 * Function : CcNotifyWriteBehindHelper @ 0x1401d9bf0 (16 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.87 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint8_t __attribute__((ms_abi)) CcNotifyWriteBehindHelper(void) {
    uint64_t __attribute__((unused)) ssa_call_res = CcNotifyWriteBehindInternal(0ULL, 0ULL, 0ULL, 0ULL);
    return (uint8_t)(((uint64_t)(((uint64_t)(ssa_call_res) & (uint64_t)(0xffffffffffffff00ULL))) | (uint64_t)(1ULL)));
}

/*
 * Function : CcGetCachedDirtyPageCountForFile @ 0x1401da5a0 (21 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.36 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) CcGetCachedDirtyPageCountForFile(PFILE_OBJECT arg1) {
    uint64_t __attribute__((unused)) ssa_reg_rax_0 = 0;
    if (((uint64_t)(((uint64_t)arg1)) == (uint64_t)(0ULL))) {
        ssa_reg_rax_0 = (uint64_t)(0xffffffffULL);
        goto L_bb_1401da5b4;
    }
    struct _DEVICE_OBJECT* __attribute__((unused)) ssa_mem_ld = (struct _DEVICE_OBJECT*)(uintptr_t)(arg1->DeviceObject);
    if (((uint64_t)(((uint64_t)(uintptr_t)ssa_mem_ld)) == (uint64_t)(0ULL))) {
        ssa_reg_rax_0 = (uint64_t)(0xffffffffULL);
        goto L_bb_1401da5b4;
    }
    uint32_t __attribute__((unused)) ssa_mem_ld_1 = (uint32_t)((*(uint32_t*)((uint8_t*)(uintptr_t)(ssa_mem_ld) + 0x70)));
    ssa_reg_rax_0 = (uint64_t)(((uint64_t)(uint32_t)(ssa_mem_ld_1)));
L_bb_1401da5b4:
    return (uint32_t)(ssa_reg_rax_0);
}

/*
 * Function : CcFlushCache @ 0x14010b060 (27 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.94 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) CcFlushCache(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4) {
    uint64_t __attribute__((unused)) ssa_call_res = CcFlushCachePriv(((uint64_t)arg1), ((uint64_t)arg2), ((uint64_t)arg3), 0ULL);
    return (uint64_t)(ssa_call_res);
}

/*
 * Function : CcFlushCacheToLsn @ 0x140115b40 (32 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.08 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) CcFlushCacheToLsn(uint64_t arg1, uint64_t arg2, uint64_t arg3) {
    uint64_t __attribute__((unused)) ssa_call_res = CcFlushCachePriv(((uint64_t)arg1), 0ULL, 0ULL, ((uint64_t)arg2));
    return (uint64_t)(ssa_call_res);
}

/*
 * Function : CcIncrementVacbActiveCount @ 0x1400b6ac8 (32 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (24.69 ms, 33 SMT assertions)
 * Verified : x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) CcIncrementVacbActiveCount(uint32_t* arg1) {
    uint64_t __attribute__((unused)) ssa_common_ret_op = 0;
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x10ULL)))));
    uint32_t __attribute__((unused)) ssa__42 = (uint32_t)(((uint32_t)(ssa_mem_ld) + (uint32_t)(1ULL)));
    (*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(0x10ULL)))) = (uint32_t)(ssa__42);
    if (((uint16_t)(((uint16_t)(uint32_t)(ssa__42))) == (uint16_t)(0ULL))) {
        uint64_t __attribute__((unused)) ssa_call_res = KeBugCheckEx(0x34ULL, 0x9afULL, 0xffffffffc0000420ULL, 0ULL);
        ssa_common_ret_op = (uint64_t)(ssa_call_res);
    } else {
        ssa_common_ret_op = (uint64_t)(((uint64_t)(uint32_t)(ssa__42)));
    }
    return (uint32_t)(ssa_common_ret_op);
}

/*
 * Function : CcSetDirtyPageThreshold @ 0x1401d9980 (37 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (11.18 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) CcSetDirtyPageThreshold(PKPRCB arg1, uint32_t arg2) {
    uint8_t __attribute__((unused)) ssa__54_pn_in = 0;
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)(arg1->RspBase);
    uint64_t __attribute__((unused)) ssa_mem_ld_1 = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld) + (uint64_t)(8ULL)))));
    if (((uint64_t)(ssa_mem_ld_1) == (uint64_t)(0ULL))) {
        goto L_bb_1401d9994;
    }
    (*(uint32_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld_1) + (uint64_t)(0x108ULL)))) = (uint32_t)(((uint32_t)(uint64_t)(((uint64_t)arg2))));
L_bb_1401d9994:
    struct _KTHREAD* __attribute__((unused)) ssa_mem_ld_2 = (struct _KTHREAD*)(uintptr_t)(arg1->IdleThread);
    uint8_t __attribute__((unused)) ssa_mem_ld_3 = (uint8_t)((*(uint8_t*)((uint8_t*)(uintptr_t)(ssa_mem_ld_2) + 0x4)));
    if (((uint8_t)(((uint8_t)(ssa_mem_ld_3) & (uint8_t)(4ULL))) == (uint8_t)(0ULL))) {
        goto L_bb_1401d999f;
    }
    ssa__54_pn_in = (uint8_t)(ssa_mem_ld_3);
    goto L_bb_1401d99a4;
L_bb_1401d999f:
    uint8_t __attribute__((unused)) ssa__70 = (uint8_t)(((uint8_t)(ssa_mem_ld_3) | (uint8_t)(4ULL)));
    (*(uint8_t*)((uint8_t*)(uintptr_t)(ssa_mem_ld_2) + 0x4)) = (uint8_t)(ssa__70);
    ssa__54_pn_in = (uint8_t)(ssa__70);
L_bb_1401d99a4:
    return (uint64_t)(((uint64_t)(((uint64_t)(ssa_mem_ld) & (uint64_t)(0xffffffffffffff00ULL))) | (uint64_t)(((uint64_t)(uint8_t)(ssa__54_pn_in)))));
}

/*
 * Function : CcCopyReadExceptionFilter @ 0x1401d9ec4 (38 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (7.23 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) CcCopyReadExceptionFilter(uint64_t* arg1, uint32_t* arg2) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))));
    uint32_t __attribute__((unused)) ssa_mem_ld_1 = (uint32_t)((*(uint32_t*)(uintptr_t)(ssa_mem_ld)));
    (*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg2))) = (uint32_t)(ssa_mem_ld_1);
    if (((uint32_t)(ssa_mem_ld_1) == (uint32_t)(0xffffffffc0000006ULL))) {
        goto L_bb_1401d9ed6;
    }
    goto L_bb_1401d9ee4;
L_bb_1401d9ed6:
    uint64_t __attribute__((unused)) ssa_mem_ld_2 = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))));
    uint32_t __attribute__((unused)) ssa_mem_ld_3 = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld_2) + (uint64_t)(0x18ULL)))));
    if (((uint32_t)(ssa_mem_ld_3) < (uint32_t)(3ULL))) {
        goto L_bb_1401d9ee4;
    }
    uint32_t __attribute__((unused)) ssa_mem_ld_4 = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld_2) + (uint64_t)(0x30ULL)))));
    (*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg2))) = (uint32_t)(ssa_mem_ld_4);
L_bb_1401d9ee4:
    return (uint64_t)(1ULL);
}

/*
 * Function : CcWaitForCurrentLazyWriterActivity @ 0x140141b10 (38 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.95 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) CcWaitForCurrentLazyWriterActivity(void) {
    uint8_t __attribute__((aligned(16))) ssa_stack_frame[1024];
    (*(uint32_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 776ULL))) = (uint32_t)(0ULL);
    uint64_t __attribute__((unused)) ssa_call_res = CcForEachPartition(((uint64_t)(uintptr_t)&__nt_rodata_14013f350), (((uint64_t)(uintptr_t)ssa_stack_frame) + 776ULL), 0ULL, 0ULL);
    uint32_t __attribute__((unused)) ssa_mem_ld_1 = (uint32_t)((*(uint32_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 776ULL))));
    return (uint32_t)(((uint64_t)(uint32_t)(ssa_mem_ld_1)));
}

/*
 * Function : CcFastCopyWrite @ 0x1401da0a0 (39 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (7.18 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) CcFastCopyWrite(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4) {
    uint8_t __attribute__((aligned(16))) ssa_stack_frame[1024];
    (*(uint64_t*)(uintptr_t)(((uint64_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 768ULL)) + (uint64_t)(0xffffffffffffffe0ULL)))) = (uint64_t)(0ULL);
    uint64_t __attribute__((unused)) ssa__58 = (uint64_t)(((uint64_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 768ULL)) + (uint64_t)(0xffffffffffffffe8ULL)));
    (*(uint64_t*)(uintptr_t)(((uint64_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 768ULL)) + (uint64_t)(0xffffffffffffffd8ULL)))) = (uint64_t)(((uint64_t)arg4));
    (*(uint64_t*)(uintptr_t)(ssa__58)) = (uint64_t)(((uint64_t)(((uint64_t)arg2)) & (uint64_t)(0xffffffffULL)));
    uint64_t __attribute__((unused)) ssa_call_res = CcCopyWriteEx(((uint64_t)arg1), ssa__58, ((uint64_t)arg3), ((uint64_t)(((uint64_t)(((uint64_t)arg4)) & (uint64_t)(0xffffffffffffff00ULL))) | (uint64_t)(1ULL)));
    return (uint32_t)(ssa_call_res);
}

/*
 * Function : CcGetPartition @ 0x14005f070 (39 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (24.74 ms, 33 SMT assertions)
 * Verified : x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) CcGetPartition(PSHARED_CACHE_MAP arg1) {
    uint64_t __attribute__((unused)) ssa_common_ret_op = 0;
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)(uintptr_t)(arg1->Section);
    if (((uint64_t)(ssa_mem_ld) == (uint64_t)(0ULL))) {
        struct _CC_PARTITION* __attribute__((unused)) ssa_mem_ld_1 = (struct _CC_PARTITION*)(uintptr_t)(arg1->Partition);
        ssa_common_ret_op = (uint64_t)(((uint64_t)(uintptr_t)ssa_mem_ld_1));
    } else {
        uint64_t __attribute__((unused)) ssa_mem_ld_2 = (uint64_t)(uintptr_t)(arg1->FileObjectFastRef.Object);
        ssa_common_ret_op = (uint64_t)(((uint64_t)(ssa_mem_ld_2) & (uint64_t)(0xfffffffffffffff0ULL)));
    }
    return (uint64_t)(ssa_common_ret_op);
}

/*
 * Function : CcIsThereDirtyData @ 0x1401da680 (43 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.48 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint8_t __attribute__((ms_abi)) CcIsThereDirtyData(uint64_t arg1) {
    uint8_t __attribute__((aligned(16))) ssa_stack_frame[1024];
    (*(uint64_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 744ULL))) = (uint64_t)(((uint64_t)arg1));
    (*(uint8_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 752ULL))) = (uint8_t)(0ULL);
    uint64_t __attribute__((unused)) ssa_call_res = CcForEachPartition(((uint64_t)(uintptr_t)&__nt_rodata_1401da7a0), (((uint64_t)(uintptr_t)ssa_stack_frame) + 744ULL), 1ULL, 0ULL);
    uint8_t __attribute__((unused)) ssa_mem_ld = (uint8_t)((*(uint8_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 752ULL))));
    return (uint8_t)(((uint64_t)(((uint64_t)(ssa_call_res) & (uint64_t)(0xffffffffffffff00ULL))) | (uint64_t)(((uint64_t)(uint8_t)(ssa_mem_ld)))));
}

extern void __nt_asm_ext_1400bff30(void);
extern void __nt_asm_ext_1400c04f0(void);
extern void __nt_asm_ext_14005f170(void);
extern void __nt_asm_ext_140032134(void);
extern void __nt_asm_ext_1400c45a0(void);
extern void __nt_asm_ext_1400b4e70(void);
extern void __nt_asm_ext_140032380(void);
extern void __nt_asm_ext_1400322dc(void);
extern void __nt_asm_ext_1400bd9c0(void);
extern void __nt_asm_ext_1400b9e20(void);
extern void __nt_asm_ext_140032820(void);
extern void __nt_asm_ext_1400ca260(void);
extern void __nt_asm_ext_1401637d0(void);
extern void __nt_asm_ext_14027d9fc(void);
extern void __nt_asm_ext_140068fa0(void);
extern void __nt_asm_ext_14027db74(void);
extern void __nt_asm_ext_1402728fc(void);
extern void __nt_asm_ext_1400c0420(void);
extern unsigned char __nt_asm_sym_1403f9146[];
extern unsigned char __nt_asm_sym_14037ff80[];
extern unsigned char __nt_asm_sym_1403808b0[];
extern unsigned char __nt_asm_sym_14037fe80[];
extern unsigned char __nt_asm_sym_1403f9144[];
extern unsigned char __nt_asm_sym_140381e40[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] CcSetDirtyPinnedData @ 0x140031be0
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Privileged CPU/CSR instruction 'mov rbp, cr8' at 0x140031c75
 *   - Privileged CPU/CSR instruction 'mov cr8, rcx' at 0x140031c79
 *   - Privileged CPU/CSR instruction 'mov cr8, rax' at 0x140031cf2
 *   - Privileged CPU/CSR instruction 'mov rbx, cr8' at 0x140031e4f
 *   - Privileged CPU/CSR instruction 'mov cr8, rax' at 0x140031e58
 *   - Privileged CPU/CSR instruction 'mov cr8, rax' at 0x140031f03
 *   - Privileged CPU/CSR instruction 'mov rax, cr8' at 0x140031f76
 *   - Privileged CPU/CSR instruction 'mov cr8, rcx' at 0x140031f7f
 *   - Privileged CPU/CSR instruction 'mov cr8, rax' at 0x140031fba
 *   - Privileged CPU/CSR instruction 'mov rax, cr8' at 0x1400320f4
 *   - Privileged/control-register routine (10/354 insns) preserved as native x86-64 assembly
 *   Live-In Registers : rdi, r12, r15, rcx, rdx, rbx, rsi, r13, r14, rbp
 *   Clobbered State   : rdi, r12, r15, rax, r8, rcx, rdx, rsi, r14, rbx, r13, rbp, r9, memory, cc
 */
uint64_t __attribute__((naked, ms_abi)) CcSetDirtyPinnedData(struct _RECON_CCSETDIRTYPINNEDDATA_RCX* arg1, struct _ANON_TAG_147d* arg2) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "push rdi\n\t"
        "push r12\n\t"
        "push r15\n\t"
        "sub rsp, 0x80\n\t"
        "movzx eax, word ptr [rcx]\n\t"
        "xor r8d, r8d\n\t"
        "mov rdi, rcx\n\t"
        "mov r15, rdx\n\t"
        "mov ecx, 0x2fd\n\t"
        "mov r12d, r8d\n\t"
        "lea edx, [rcx - 3]\n\t"
        "cmp ax, cx\n\t"
        "jne .L_CcSetDirtyPinnedData_140032112\n\t"
        ".L_CcSetDirtyPinnedData_140031c0d:\n\t"
        "mov qword ptr [rsp + 0xa8], rbx\n\t"
        "mov qword ptr [rsp + 0x70], rsi\n\t"
        "mov esi, 1\n\t"
        "mov qword ptr [rsp + 0x68], r13\n\t"
        "mov qword ptr [rsp + 0x60], r14\n\t"
        "lea r14, [rsp + 0x30]\n\t"
        "mov qword ptr [rsp + 0x30], rdi\n\t"
        "mov qword ptr [rsp + 0x38], r8\n\t"
        "cmp ax, dx\n\t"
        "je .L_CcSetDirtyPinnedData_140032120\n\t"
        ".L_CcSetDirtyPinnedData_140031c41:\n\t"
        "mov rax, qword ptr [r14]\n\t"
        "mov ecx, 2\n\t"
        "mov qword ptr [rsp + 0x78], rbp\n\t"
        "mov rbx, qword ptr [rax + 0xb0]\n\t"
        "mov r13, qword ptr [rbx + 0x210]\n\t"
        "cmp qword ptr [rbx + 0xa8], r8\n\t"
        "je .L_CcSetDirtyPinnedData_140031d07\n\t"
        ".L_CcSetDirtyPinnedData_140031c69:\n\t"
        "mov rax, qword ptr [rbx + 0x60]\n\t"
        "and rax, 0xfffffffffffffff0\n\t"
        "mov rsi, qword ptr [rax + 0x28]\n\t"
        "mov rbp, cr8\n\t"
        "mov cr8, rcx\n\t"
        "test byte ptr [rip + __nt_asm_sym_1403f9146], 0x21\n\t"
        "jne .L_CcSetDirtyPinnedData_1401873ec\n\t"
        ".L_CcSetDirtyPinnedData_140031c8a:\n\t"
        "mov dword ptr [rsp + 0xa0], r8d\n\t"
        "lock bts dword ptr [rip + __nt_asm_sym_14037ff80], 0x1f\n\t"
        "jb .L_CcSetDirtyPinnedData_140032015\n\t"
        ".L_CcSetDirtyPinnedData_140031ca1:\n\t"
        "mov edx, dword ptr [rip + __nt_asm_sym_14037ff80]\n\t"
        "mov ecx, edx\n\t"
        "btr ecx, 0x1e\n\t"
        "cmp ecx, 0x80000000\n\t"
        "jne .L_CcSetDirtyPinnedData_1401873fe\n\t"
        ".L_CcSetDirtyPinnedData_140031cb9:\n\t"
        "mov rax, qword ptr [rsi]\n\t"
        "test rax, rax\n\t"
        "je .L_CcSetDirtyPinnedData_140187440\n\t"
        ".L_CcSetDirtyPinnedData_140031cc5:\n\t"
        "movzx edx, word ptr [rax + 0x3c]\n\t"
        "mov rax, qword ptr [rip + __nt_asm_sym_1403808b0]\n\t"
        "and edx, 0x3ff\n\t"
        "mov rsi, qword ptr [rax + rdx*8]\n\t"
        ".L_CcSetDirtyPinnedData_140031cda:\n\t"
        "test byte ptr [rip + __nt_asm_sym_1403f9146], 1\n\t"
        "jne .L_CcSetDirtyPinnedData_14018744c\n\t"
        ".L_CcSetDirtyPinnedData_140031ce7:\n\t"
        "mov dword ptr [rip + __nt_asm_sym_14037ff80], r8d\n\t"
        ".L_CcSetDirtyPinnedData_140031cee:\n\t"
        "movzx eax, bpl\n\t"
        "mov cr8, rax\n\t"
        "mov rax, qword ptr [rsi + 0xa8]\n\t"
        "cmp r13, qword ptr [rax + 8]\n\t"
        "jne .L_CcSetDirtyPinnedData_140187468\n\t"
        ".L_CcSetDirtyPinnedData_140031d07:\n\t"
        "test dword ptr [rbx + 0x98], 0x1000000\n\t"
        "je .L_CcSetDirtyPinnedData_140031d28\n\t"
        ".L_CcSetDirtyPinnedData_140031d13:\n\t"
        "mov rax, qword ptr [rbx + 0xf0]\n\t"
        "cmp qword ptr [rax + 0x88], -1\n\t"
        "je .L_CcSetDirtyPinnedData_1400320a9\n\t"
        ".L_CcSetDirtyPinnedData_140031d28:\n\t"
        "test dword ptr [rbx + 0x98], 0x200\n\t"
        "je .L_CcSetDirtyPinnedData_140032091\n\t"
        ".L_CcSetDirtyPinnedData_140031d38:\n\t"
        "mov rax, qword ptr [r14]\n\t"
        "test rax, rax\n\t"
        "je .L_CcSetDirtyPinnedData_140031f1b\n\t"
        ".L_CcSetDirtyPinnedData_140031d44:\n\t"
        "movabs r12, 0xffff800000000000\n\t"
        ".L_CcSetDirtyPinnedData_140031d4e:\n\t"
        "mov qword ptr [rsp + 0x30], rax\n\t"
        "lea rdx, [rip + __nt_asm_sym_14037fe80]\n\t"
        "lea r14, [r14 + 8]\n\t"
        "test al, 1\n\t"
        "jne .L_CcSetDirtyPinnedData_1401874d8\n\t"
        ".L_CcSetDirtyPinnedData_140031d66:\n\t"
        "mov rbp, qword ptr [rax + 0xb0]\n\t"
        "mov dword ptr [rsp + 0xb0], r8d\n\t"
        "mov rbx, qword ptr gs:[0x188]\n\t"
        "lea rdi, [rbp + 0x118]\n\t"
        "dec word ptr [rbx + 0x1e6]\n\t"
        "inc byte ptr [rbx + 0x31a]\n\t"
        "cmp byte ptr [rbx + 0x31a], 1\n\t"
        "jne .L_CcSetDirtyPinnedData_1400320f4\n\t"
        ".L_CcSetDirtyPinnedData_140031d9f:\n\t"
        "cmp byte ptr [rbx + 0x318], 0\n\t"
        "je .L_CcSetDirtyPinnedData_1400320c9\n\t"
        ".L_CcSetDirtyPinnedData_140031dac:\n\t"
        "movzx ecx, byte ptr [rbx + 0x318]\n\t"
        "lea rsi, [rbx + 0x320]\n\t"
        "bsf eax, ecx\n\t"
        "btr ecx, eax\n\t"
        "mov dword ptr [rsp + 0xb8], eax\n\t"
        "mov byte ptr [rbx + 0x318], cl\n\t"
        "lea rcx, [rax + rax*2]\n\t"
        "shl rcx, 5\n\t"
        "add rsi, rcx\n\t"
        "je .L_CcSetDirtyPinnedData_1400320e9\n\t"
        ".L_CcSetDirtyPinnedData_140031dde:\n\t"
        "cmp rdi, r12\n\t"
        "jb .L_CcSetDirtyPinnedData_140031e02\n\t"
        ".L_CcSetDirtyPinnedData_140031de3:\n\t"
        "mov rax, rdi\n\t"
        "shr rax, 0x27\n\t"
        "and eax, 0x1ff\n\t"
        "sub eax, 0x100\n\t"
        "cmp byte ptr [rax + rdx + 0x1910], 1\n\t"
        "je .L_CcSetDirtyPinnedData_1401874ab\n\t"
        ".L_CcSetDirtyPinnedData_140031e02:\n\t"
        "or eax, 0xffffffff\n\t"
        ".L_CcSetDirtyPinnedData_140031e05:\n\t"
        "mov dword ptr [rsi + 0x28], eax\n\t"
        "mov rax, rdi\n\t"
        "movabs rcx, 0x7ffffffffffffffc\n\t"
        "and rax, rcx\n\t"
        "mov qword ptr [rsi + 0x20], rax\n\t"
        ".L_CcSetDirtyPinnedData_140031e1c:\n\t"
        "dec byte ptr [rbx + 0x31a]\n\t"
        "lea r8, [rsp + 0xb0]\n\t"
        "mov rdx, rdi\n\t"
        "mov rcx, rbx\n\t"
        "call __nt_asm_ext_1400bff30\n\t"
        "add word ptr [rbx + 0x1e6], 1\n\t"
        "jne .L_CcSetDirtyPinnedData_140031e4f\n\t"
        ".L_CcSetDirtyPinnedData_140031e3f:\n\t"
        "add rbx, 0x98\n\t"
        "cmp qword ptr [rbx], rbx\n\t"
        "jne .L_CcSetDirtyPinnedData_140032087\n\t"
        ".L_CcSetDirtyPinnedData_140031e4f:\n\t"
        "mov rbx, cr8\n\t"
        "mov eax, 1\n\t"
        "mov cr8, rax\n\t"
        "lock btr dword ptr [rdi], 0\n\t"
        "jae .L_CcSetDirtyPinnedData_14003205c\n\t"
        ".L_CcSetDirtyPinnedData_140031e67:\n\t"
        "test rsi, rsi\n\t"
        "je .L_CcSetDirtyPinnedData_140031e70\n\t"
        ".L_CcSetDirtyPinnedData_140031e6c:\n\t"
        "or byte ptr [rsi + 0x1a], 1\n\t"
        ".L_CcSetDirtyPinnedData_140031e70:\n\t"
        "mov rax, qword ptr gs:[0x188]\n\t"
        "mov qword ptr [rdi + 8], rax\n\t"
        "movzx eax, bl\n\t"
        "mov rbx, qword ptr [rsp + 0x30]\n\t"
        "mov dword ptr [rdi + 0x30], eax\n\t"
        "cmp byte ptr [rbx + 2], 0\n\t"
        "je .L_CcSetDirtyPinnedData_140031f44\n\t"
        ".L_CcSetDirtyPinnedData_140031e92:\n\t"
        "mov esi, 1\n\t"
        ".L_CcSetDirtyPinnedData_140031e97:\n\t"
        "test r15, r15\n\t"
        "je .L_CcSetDirtyPinnedData_140031eda\n\t"
        ".L_CcSetDirtyPinnedData_140031e9c:\n\t"
        "mov rax, qword ptr [rbx + 0x28]\n\t"
        "test rax, rax\n\t"
        "je .L_CcSetDirtyPinnedData_14003206c\n\t"
        ".L_CcSetDirtyPinnedData_140031ea9:\n\t"
        "cmp qword ptr [r15], rax\n\t"
        "jl .L_CcSetDirtyPinnedData_14003206c\n\t"
        ".L_CcSetDirtyPinnedData_140031eb2:\n\t"
        "mov rax, qword ptr [rbx + 0x30]\n\t"
        "test rax, rax\n\t"
        "je .L_CcSetDirtyPinnedData_140031ec0\n\t"
        ".L_CcSetDirtyPinnedData_140031ebb:\n\t"
        "cmp qword ptr [r15], rax\n\t"
        "jle .L_CcSetDirtyPinnedData_140031ec7\n\t"
        ".L_CcSetDirtyPinnedData_140031ec0:\n\t"
        "mov rax, qword ptr [r15]\n\t"
        "mov qword ptr [rbx + 0x30], rax\n\t"
        ".L_CcSetDirtyPinnedData_140031ec7:\n\t"
        "mov rax, qword ptr [r15]\n\t"
        "cmp rax, qword ptr [rbp + 0x100]\n\t"
        "jle .L_CcSetDirtyPinnedData_140031eda\n\t"
        ".L_CcSetDirtyPinnedData_140031ed3:\n\t"
        "mov qword ptr [rbp + 0x100], rax\n\t"
        ".L_CcSetDirtyPinnedData_140031eda:\n\t"
        "mov rax, qword ptr [rbx + 0x20]\n\t"
        "cmp rax, qword ptr [rbp + 0x30]\n\t"
        "jg .L_CcSetDirtyPinnedData_1401874c7\n\t"
        ".L_CcSetDirtyPinnedData_140031ee8:\n\t"
        "movzx ebx, byte ptr [rdi + 0x30]\n\t"
        "xor eax, eax\n\t"
        "mov qword ptr [rdi + 8], 0\n\t"
        "lock cmpxchg dword ptr [rdi], esi\n\t"
        "jne .L_CcSetDirtyPinnedData_140032078\n\t"
        ".L_CcSetDirtyPinnedData_140031f00:\n\t"
        "movzx eax, bl\n\t"
        "mov cr8, rax\n\t"
        "mov rcx, rdi\n\t"
        "call __nt_asm_ext_1400c04f0\n\t"
        "mov rax, qword ptr [r14]\n\t"
        "test rax, rax\n\t"
        "jne .L_CcSetDirtyPinnedData_1401874d0\n\t"
        ".L_CcSetDirtyPinnedData_140031f1b:\n\t"
        "mov rbp, qword ptr [rsp + 0x78]\n\t"
        "mov rsi, qword ptr [rsp + 0x70]\n\t"
        "mov r13, qword ptr [rsp + 0x68]\n\t"
        "mov rbx, qword ptr [rsp + 0xa8]\n\t"
        "mov r14, qword ptr [rsp + 0x60]\n\t"
        "add rsp, 0x80\n\t"
        "pop r15\n\t"
        "pop r12\n\t"
        "pop rdi\n\t"
        "ret\n\t"
        ".L_CcSetDirtyPinnedData_140031f44:\n\t"
        "mov esi, dword ptr [rbx + 4]\n\t"
        "shr esi, 0xc\n\t"
        "mov byte ptr [rbx + 2], 1\n\t"
        "test r15, r15\n\t"
        "je .L_CcSetDirtyPinnedData_140031f61\n\t"
        ".L_CcSetDirtyPinnedData_140031f53:\n\t"
        "mov rax, qword ptr [r15]\n\t"
        "mov qword ptr [rbx + 0x28], rax\n\t"
        "mov rax, qword ptr [r15]\n\t"
        "mov qword ptr [rbx + 0x30], rax\n\t"
        ".L_CcSetDirtyPinnedData_140031f61:\n\t"
        "lea rdx, [r13 + 0x80]\n\t"
        "mov qword ptr [rsp + 0x40], 0\n\t"
        "mov qword ptr [rsp + 0x48], rdx\n\t"
        "mov rax, cr8\n\t"
        "mov ecx, 2\n\t"
        "mov cr8, rcx\n\t"
        "lea rcx, [rsp + 0x40]\n\t"
        "mov byte ptr [rsp + 0x50], al\n\t"
        "call __nt_asm_ext_14005f170\n\t"
        "cmp dword ptr [rbp + 0x70], 0\n\t"
        "je .L_CcSetDirtyPinnedData_140032034\n\t"
        ".L_CcSetDirtyPinnedData_140031f9b:\n\t"
        "mov r9d, esi\n\t"
        "xor r8d, r8d\n\t"
        "xor edx, edx\n\t"
        "mov rcx, rbp\n\t"
        "call __nt_asm_ext_140032134\n\t"
        "lea rcx, [rsp + 0x40]\n\t"
        "call __nt_asm_ext_1400c45a0\n\t"
        "movzx eax, byte ptr [rsp + 0x50]\n\t"
        "mov cr8, rax\n\t"
        "test dword ptr [rbp + 0x98], 0x10000000\n\t"
        "je .L_CcSetDirtyPinnedData_140031e92\n\t"
        ".L_CcSetDirtyPinnedData_140031fce:\n\t"
        "mov rax, qword ptr gs:[0x188]\n\t"
        "shl esi, 0xc\n\t"
        "mov r8d, esi\n\t"
        "mov rcx, qword ptr [rax + 0x220]\n\t"
        "mov rdx, qword ptr [rcx + 0x708]\n\t"
        "test rdx, rdx\n\t"
        "je .L_CcSetDirtyPinnedData_1401874bd\n\t"
        ".L_CcSetDirtyPinnedData_140031ff4:\n\t"
        "test esi, esi\n\t"
        "je .L_CcSetDirtyPinnedData_140031ffe\n\t"
        ".L_CcSetDirtyPinnedData_140031ff8:\n\t"
        "lock xadd qword ptr [rdx + 8], r8\n\t"
        ".L_CcSetDirtyPinnedData_140031ffe:\n\t"
        "mov esi, 1\n\t"
        "mov eax, esi\n\t"
        "lock xadd qword ptr [rdx + 0x18], rax\n\t"
        "mov rbx, qword ptr [rsp + 0x30]\n\t"
        "jmp .L_CcSetDirtyPinnedData_140031e97\n\t"
        ".L_CcSetDirtyPinnedData_140032015:\n\t"
        "movzx edx, bpl\n\t"
        "lea rcx, [rip + __nt_asm_sym_14037ff80]\n\t"
        "call __nt_asm_ext_1400b4e70\n\t"
        "mov dword ptr [rsp + 0xa0], eax\n\t"
        "xor r8d, r8d\n\t"
        "jmp .L_CcSetDirtyPinnedData_140031ca1\n\t"
        ".L_CcSetDirtyPinnedData_140032034:\n\t"
        "mov eax, dword ptr [rbp + 0x98]\n\t"
        "test al, 2\n\t"
        "jne .L_CcSetDirtyPinnedData_140031f9b\n\t"
        ".L_CcSetDirtyPinnedData_140032042:\n\t"
        "xor r8d, r8d\n\t"
        "xor edx, edx\n\t"
        "mov rcx, r13\n\t"
        "call __nt_asm_ext_140032380\n\t"
        "mov rcx, rbp\n\t"
        "call __nt_asm_ext_1400322dc\n\t"
        "jmp .L_CcSetDirtyPinnedData_140031f9b\n\t"
        ".L_CcSetDirtyPinnedData_14003205c:\n\t"
        "mov rdx, rsi\n\t"
        "mov rcx, rdi\n\t"
        "call __nt_asm_ext_1400bd9c0\n\t"
        "jmp .L_CcSetDirtyPinnedData_140031e67\n\t"
        ".L_CcSetDirtyPinnedData_14003206c:\n\t"
        "mov rax, qword ptr [r15]\n\t"
        "mov qword ptr [rbx + 0x28], rax\n\t"
        "jmp .L_CcSetDirtyPinnedData_140031eb2\n\t"
        ".L_CcSetDirtyPinnedData_140032078:\n\t"
        "mov edx, eax\n\t"
        "mov rcx, rdi\n\t"
        "call __nt_asm_ext_1400b9e20\n\t"
        "jmp .L_CcSetDirtyPinnedData_140031f00\n\t"
        ".L_CcSetDirtyPinnedData_140032087:\n\t"
        "call __nt_asm_ext_140032820\n\t"
        "jmp .L_CcSetDirtyPinnedData_140031e4f\n\t"
        ".L_CcSetDirtyPinnedData_140032091:\n\t"
        "mov r8d, dword ptr [rdi + 4]\n\t"
        "lea rdx, [rdi + 8]\n\t"
        "xor r9d, r9d\n\t"
        "mov rcx, rbx\n\t"
        "call __nt_asm_ext_1400ca260\n\t"
        "jmp .L_CcSetDirtyPinnedData_140031f1b\n\t"
        ".L_CcSetDirtyPinnedData_1400320a9:\n\t"
        "movabs rdx, 0xfffff78000000320\n\t"
        "mov rdx, qword ptr [rdx]\n\t"
        "mov rax, qword ptr [rbx + 0xf0]\n\t"
        "mov qword ptr [rax + 0x88], rdx\n\t"
        "jmp .L_CcSetDirtyPinnedData_140031d28\n\t"
        ".L_CcSetDirtyPinnedData_1400320c9:\n\t"
        "cmp byte ptr [rbx + 0x58e], 0\n\t"
        "jne .L_CcSetDirtyPinnedData_140187486\n\t"
        ".L_CcSetDirtyPinnedData_1400320d6:\n\t"
        "test dword ptr [rip + __nt_asm_sym_1403f9144], 0x200\n\t"
        "mov rsi, r8\n\t"
        "jne .L_CcSetDirtyPinnedData_14018749a\n\t"
        ".L_CcSetDirtyPinnedData_1400320e9:\n\t"
        "lock bts dword ptr [rbx + 0x78], 0x10\n\t"
        "jmp .L_CcSetDirtyPinnedData_140031e1c\n\t"
        ".L_CcSetDirtyPinnedData_1400320f4:\n\t"
        "mov rax, cr8\n\t"
        "mov qword ptr [rsp + 0x20], r8\n\t"
        "mov rdx, rbx\n\t"
        "mov r8, rdi\n\t"
        "movzx r9d, al\n\t"
        "mov ecx, 0x192\n\t"
        "call __nt_asm_ext_1401637d0\n\t"
        "int3\n\t"
        ".L_CcSetDirtyPinnedData_140032112:\n\t"
        "cmp ax, dx\n\t"
        "je .L_CcSetDirtyPinnedData_140031c0d\n\t"
        ".L_CcSetDirtyPinnedData_14003211b:\n\t"
        "jmp .L_CcSetDirtyPinnedData_1401873ce\n\t"
        ".L_CcSetDirtyPinnedData_140032120:\n\t"
        "lea r14, [rdi + 0x10]\n\t"
        "mov r12d, esi\n\t"
        "jmp .L_CcSetDirtyPinnedData_140031c41\n\t"
        ".L_CcSetDirtyPinnedData_1401873ce:\n\t"
        "xor r9d, r9d\n\t"
        "mov qword ptr [rsp + 0x20], r8\n\t"
        "mov edx, 0xe3d\n\t"
        "mov r8, 0xffffffffc0000420\n\t"
        "lea ecx, [r9 + 0x34]\n\t"
        "call __nt_asm_ext_1401637d0\n\t"
        "int3\n\t"
        ".L_CcSetDirtyPinnedData_1401873ec:\n\t"
        "movzx edx, bpl\n\t"
        "lea rcx, [rip + __nt_asm_sym_14037ff80]\n\t"
        "call __nt_asm_ext_14027d9fc\n\t"
        "jmp .L_CcSetDirtyPinnedData_140187438\n\t"
        ".L_CcSetDirtyPinnedData_1401873fe:\n\t"
        "bt edx, 0x1e\n\t"
        "jb .L_CcSetDirtyPinnedData_140187418\n\t"
        ".L_CcSetDirtyPinnedData_140187404:\n\t"
        "mov ecx, edx\n\t"
        "mov eax, edx\n\t"
        "bts ecx, 0x1e\n\t"
        "lock cmpxchg dword ptr [rip + __nt_asm_sym_14037ff80], ecx\n\t"
        "mov edx, eax\n\t"
        "jne .L_CcSetDirtyPinnedData_14018742b\n\t"
        ".L_CcSetDirtyPinnedData_140187418:\n\t"
        "lea rcx, [rsp + 0xa0]\n\t"
        "call __nt_asm_ext_140068fa0\n\t"
        "mov edx, dword ptr [rip + __nt_asm_sym_14037ff80]\n\t"
        ".L_CcSetDirtyPinnedData_14018742b:\n\t"
        "mov eax, edx\n\t"
        "btr eax, 0x1e\n\t"
        "cmp eax, 0x80000000\n\t"
        "jne .L_CcSetDirtyPinnedData_1401873fe\n\t"
        ".L_CcSetDirtyPinnedData_140187438:\n\t"
        "xor r8d, r8d\n\t"
        "jmp .L_CcSetDirtyPinnedData_140031cb9\n\t"
        ".L_CcSetDirtyPinnedData_140187440:\n\t"
        "lea rsi, [rip + __nt_asm_sym_140381e40]\n\t"
        "jmp .L_CcSetDirtyPinnedData_140031cda\n\t"
        ".L_CcSetDirtyPinnedData_14018744c:\n\t"
        "mov rdx, qword ptr [rsp + 0x98]\n\t"
        "lea rcx, [rip + __nt_asm_sym_14037ff80]\n\t"
        "call __nt_asm_ext_14027db74\n\t"
        "xor r8d, r8d\n\t"
        "jmp .L_CcSetDirtyPinnedData_140031cee\n\t"
        ".L_CcSetDirtyPinnedData_140187468:\n\t"
        "xor r9d, r9d\n\t"
        "mov qword ptr [rsp + 0x20], r8\n\t"
        "mov edx, 0x1293\n\t"
        "mov r8, 0xffffffffc0000420\n\t"
        "lea ecx, [r9 + 0x34]\n\t"
        "call __nt_asm_ext_1401637d0\n\t"
        "int3\n\t"
        ".L_CcSetDirtyPinnedData_140187486:\n\t"
        "mov eax, r8d\n\t"
        "xchg byte ptr [rbx + 0x58e], al\n\t"
        "or byte ptr [rbx + 0x318], al\n\t"
        "jmp .L_CcSetDirtyPinnedData_140031dac\n\t"
        ".L_CcSetDirtyPinnedData_14018749a:\n\t"
        "mov rdx, rdi\n\t"
        "mov rcx, rbx\n\t"
        "call __nt_asm_ext_1402728fc\n\t"
        "nop\n\t"
        "jmp .L_CcSetDirtyPinnedData_1400320e9\n\t"
        ".L_CcSetDirtyPinnedData_1401874ab:\n\t"
        "mov rcx, qword ptr [rbx + 0xb8]\n\t"
        "call __nt_asm_ext_1400c0420\n\t"
        "nop\n\t"
        "jmp .L_CcSetDirtyPinnedData_140031e05\n\t"
        ".L_CcSetDirtyPinnedData_1401874bd:\n\t"
        "mov rbx, qword ptr [rsp + 0x30]\n\t"
        "jmp .L_CcSetDirtyPinnedData_140031e92\n\t"
        ".L_CcSetDirtyPinnedData_1401874c7:\n\t"
        "mov qword ptr [rbp + 0x30], rax\n\t"
        "jmp .L_CcSetDirtyPinnedData_140031ee8\n\t"
        ".L_CcSetDirtyPinnedData_1401874d0:\n\t"
        "xor r8d, r8d\n\t"
        "jmp .L_CcSetDirtyPinnedData_140031d4e\n\t"
        ".L_CcSetDirtyPinnedData_1401874d8:\n\t"
        "xor r9d, r9d\n\t"
        "mov qword ptr [rsp + 0x20], r8\n\t"
        "mov edx, 0xe9d\n\t"
        "mov r8, 0xffffffffc0000420\n\t"
        "lea ecx, [r9 + 0x34]\n\t"
        "call __nt_asm_ext_1401637d0\n\t"
        "int3\n\t"
        ".att_syntax prefix"
    );
}
