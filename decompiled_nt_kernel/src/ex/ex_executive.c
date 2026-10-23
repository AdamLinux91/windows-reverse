/*
 * Reconstructed Windows NT Kernel Subsystem: Executive SLists, Rundown, PushLocks & Resources (Ex/Exp/Exf)
 * Source Binary : ntoskrnl.exe (10.0.16299.15 x64, RSDS 83DB4240-4EFD-4AB6-AFB6-FA864B700CB3)
 * Functions     : 36 Z3-verified / assembly-preserved routines
 */
#include "../../include/nt_reconstructed_types.h"

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] ExpInterlockedPushEntrySList @ 0x140169410
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Hand-coded x86-64 kernel assembly instruction 'lock cmpxchg16b' at 0x140169435
 *   Live-In Registers : rcx, rbx, rdx
 *   Clobbered State   : rbx, r10, rcx, r11, rax, rdx, r8, memory, cc
 */
struct _SINGLE_LIST_ENTRY* __attribute__((naked, ms_abi)) ExpInterlockedPushEntrySList(union _SLIST_HEADER* ListHead, struct _SINGLE_LIST_ENTRY* ListEntry) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "prefetchw byte ptr [rcx]\n\t"
        "push rbx\n\t"
        "mov r10, rcx\n\t"
        "mov rcx, rdx\n\t"
        "mov r11, rdx\n\t"
        "mov rax, qword ptr [r10]\n\t"
        "mov rdx, qword ptr [r10 + 8]\n\t"
        ".L_ExpInterlockedPushEntrySList_140169424:\n\t"
        "mov r8, rdx\n\t"
        "and r8b, 0xf0\n\t"
        "mov qword ptr [r11], r8\n\t"
        "lea rbx, [rax + 0x10001]\n\t"
        "lock cmpxchg16b xmmword ptr [r10]\n\t"
        "jne .L_ExpInterlockedPushEntrySList_140169424\n\t"
        ".L_ExpInterlockedPushEntrySList_14016943c:\n\t"
        "mov rax, r8\n\t"
        "pop rbx\n\t"
        "ret\n\t"
        ".att_syntax prefix"
    );
}

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] ExpInterlockedPopEntrySList @ 0x1401693d0
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Hand-coded x86-64 kernel assembly instruction 'lock cmpxchg16b' at 0x1401693f0
 *   Live-In Registers : rcx, rbx
 *   Clobbered State   : rbx, r10, rax, rdx, r8, rcx, memory, cc
 */
struct _SINGLE_LIST_ENTRY* __attribute__((naked, ms_abi)) ExpInterlockedPopEntrySList(union _SLIST_HEADER* ListHead) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "prefetchw byte ptr [rcx]\n\t"
        "push rbx\n\t"
        "mov r10, rcx\n\t"
        ".L_ExpInterlockedPopEntrySList_1401693d7:\n\t"
        "mov rax, qword ptr [r10]\n\t"
        "mov rdx, qword ptr [r10 + 8]\n\t"
        "mov r8, rdx\n\t"
        "and r8, 0xfffffffffffffff0\n\t"
        "je .L_ExpInterlockedPopEntrySList_1401693fc\n\t"
        ".L_ExpInterlockedPopEntrySList_1401693e7:\n\t"
        "mov rcx, qword ptr [r8]\n\t"
        "mov rbx, rax\n\t"
        "dec bx\n\t"
        "lock cmpxchg16b xmmword ptr [r10]\n\t"
        "jne .L_ExpInterlockedPopEntrySList_1401693d7\n\t"
        ".L_ExpInterlockedPopEntrySList_1401693f7:\n\t"
        "mov rax, r8\n\t"
        "pop rbx\n\t"
        "ret\n\t"
        ".L_ExpInterlockedPopEntrySList_1401693fc:\n\t"
        "lock or qword ptr [r10], 0\n\t"
        "jmp .L_ExpInterlockedPopEntrySList_1401693f7\n\t"
        ".att_syntax prefix"
    );
}

/*
 * Function : ExQueryDepthSList @ 0x1400eb9f0 (4 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.25 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint16_t __attribute__((ms_abi)) ExQueryDepthSList(union _SLIST_HEADER* SListHead) {
    uint16_t __attribute__((unused)) ssa_mem_ld = (uint16_t)((*(uint16_t*)((uint8_t*)(uintptr_t)(SListHead))));
    return (uint16_t)(((uint64_t)(uint16_t)(ssa_mem_ld)));
}

/*
 * Function : ExInitializeRundownProtection @ 0x140038820 (5 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.62 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) ExInitializeRundownProtection(struct _EX_RUNDOWN_REF* RunRef) {
    RunRef->Count = (uint64_t)(0ULL);
    return;
}

/*
 * Function : ExReInitializeRundownProtection @ 0x140141d80 (6 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.31 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) ExReInitializeRundownProtection(struct _EX_RUNDOWN_REF* RunRef) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)(RunRef->Count);
    RunRef->Count = (uint64_t)(0ULL);
    return;
}

/*
 * Function : ExCleanupRundownProtectionCacheAware @ 0x140116c20 (3 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.28 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) ExCleanupRundownProtectionCacheAware(void) {
    return;
}

/*
 * Function : ExSizeOfAutoExpandPushLock @ 0x14027c5d0 (6 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (2.86 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) ExSizeOfAutoExpandPushLock(void) {
    return (uint32_t)(0x10ULL);
}

/*
 * Function : ExGetFirmwareType @ 0x14027aee0 (7 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.44 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) ExGetFirmwareType(void) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)&__nt_data_140356610))));
    return (uint32_t)(((uint64_t)(uint32_t)(ssa_mem_ld)));
}

/*
 * Function : ExRundownCompleted @ 0x14004bdc0 (9 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.21 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) ExRundownCompleted(uint64_t* arg1) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))));
    (*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))) = (uint64_t)(1ULL);
    return (uint32_t)(ssa_mem_ld);
}

/*
 * Function : ExIsManufacturingModeEnabled @ 0x140464280 (10 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.68 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) ExIsManufacturingModeEnabled(void) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)&ExpManufacturingInformation))));
    return (uint32_t)(((uint64_t)(uint32_t)(((uint32_t)(ssa_mem_ld) & (uint32_t)(1ULL)))));
}

/*
 * Function : ExRealTimeIsUniversal @ 0x1401418b0 (11 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.66 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
BOOLEAN __attribute__((ms_abi)) ExRealTimeIsUniversal(void) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)&ExpRealTimeIsUniversal))));
    return (BOOLEAN)(((uint64_t)(uint8_t)(((uint32_t)(ssa_mem_ld) != (uint32_t)(0ULL)))));
}

/*
 * Function : ExIsSoftBoot @ 0x14011aa70 (12 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.79 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint8_t __attribute__((ms_abi)) ExIsSoftBoot(void) {
    uint8_t __attribute__((unused)) ssa_mem_ld = (uint8_t)((*(uint8_t*)(uintptr_t)(((uint64_t)(uintptr_t)&__nt_data_140356618))));
    return (uint8_t)(((uint64_t)(uint8_t)(((uint8_t)(((uint8_t)(ssa_mem_ld) >> (uint8_t)(2ULL))) & (uint8_t)(1ULL)))));
}

/*
 * Function : ExConvertPushLockExclusiveToShared @ 0x14027ae80 (14 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (23.50 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) ExConvertPushLockExclusiveToShared(uint64_t* arg1) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))));
    (*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))) = (uint64_t)(((((uint64_t)(ssa_mem_ld) == (uint64_t)(1ULL))) ? (0x11ULL) : (ssa_mem_ld)));
    return (uint32_t)(ssa_mem_ld);
}

/*
 * Function : ExSystemTimeToLocalTime @ 0x140116cd0 (14 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (6.01 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) ExSystemTimeToLocalTime(uint64_t* arg1, uint64_t* arg2) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))));
    uint64_t __attribute__((unused)) ssa_mem_ld_1 = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)&ExpTimeZoneBias))));
    uint64_t __attribute__((unused)) ssa__28 = (uint64_t)(((uint64_t)(ssa_mem_ld) - (uint64_t)(ssa_mem_ld_1)));
    (*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg2))) = (uint64_t)(ssa__28);
    return (uint64_t)(ssa__28);
}

/*
 * Function : ExGetPreviousMode @ 0x1400f0be0 (17 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.25 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) ExGetPreviousMode(void) {
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    uint8_t __attribute__((unused)) ssa_mem_ld = (uint8_t)(ssa_gs_val->PreviousMode);
    return (uint64_t)(((uint64_t)(uint8_t)(ssa_mem_ld)));
}

/*
 * Function : ExLocalTimeToSystemTime @ 0x140129390 (17 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.75 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) ExLocalTimeToSystemTime(uint64_t* arg1, uint64_t* arg2) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))));
    uint64_t __attribute__((unused)) ssa_mem_ld_1 = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)&ExpTimeZoneBias))));
    (*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg2))) = (uint64_t)(((uint64_t)(ssa_mem_ld_1) + (uint64_t)(ssa_mem_ld)));
    return;
}

/*
 * Function : ExSystemExceptionFilter @ 0x14045a1d0 (21 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.52 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
BOOLEAN __attribute__((ms_abi)) ExSystemExceptionFilter(void) {
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    uint8_t __attribute__((unused)) ssa_mem_ld = (uint8_t)(ssa_gs_val->PreviousMode);
    return (BOOLEAN)(((uint64_t)(uint8_t)(((uint8_t)(ssa_mem_ld) != (uint8_t)(0ULL)))));
}

/*
 * Function : ExInitializeAutoExpandPushLock @ 0x1400fb850 (23 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (6.66 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) ExInitializeAutoExpandPushLock(struct _ANON_TAG_1411* arg1, uint8_t arg2) {
    (*(uint64_t*)((uint8_t*)(uintptr_t)(arg1) + 0x8)) = (uint64_t)(0ULL);
    arg1->SecurityContext = (struct _IO_SECURITY_CONTEXT*)(uintptr_t)(0ULL);
    if (((uint64_t)(((uint64_t)(((uint64_t)arg2)) & (uint64_t)(1ULL))) == (uint64_t)(0ULL))) {
        uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)(arg1->Options);
        arg1->Options = (uint32_t)(((uint32_t)(ssa_mem_ld) | (uint32_t)(4ULL)));
    }
    return (uint64_t)(0ULL);
}

/*
 * Function : ExQueryTimerResolution @ 0x14027b490 (26 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.83 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) ExQueryTimerResolution(uint32_t* arg1, uint32_t* arg2, uint32_t* arg3) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)&KeMaximumIncrement))));
    (*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))) = (uint32_t)(ssa_mem_ld);
    uint32_t __attribute__((unused)) ssa_mem_ld_1 = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)&KeMinimumIncrement))));
    (*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg2))) = (uint32_t)(ssa_mem_ld_1);
    uint32_t __attribute__((unused)) ssa_mem_ld_2 = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)&KeTimeIncrement))));
    (*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg3))) = (uint32_t)(ssa_mem_ld_2);
    return (uint32_t)(((uint64_t)(uint32_t)(ssa_mem_ld_2)));
}

/*
 * Function : ExAllocateTimer @ 0x14011ad60 (30 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (25.40 ms, 33 SMT assertions)
 * Verified : x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) ExAllocateTimer(uint32_t arg1, uint32_t arg2, uint32_t* arg3) {
    uint64_t __attribute__((unused)) ssa_common_ret_op = 0;
    if (((uint64_t)(((uint64_t)(((uint64_t)(uintptr_t)arg3)) & (uint64_t)(2ULL))) == (uint64_t)(0ULL))) {
        uint64_t __attribute__((unused)) ssa_call_res = ExAllocateTimerInternal2(((uint64_t)arg1), ((uint64_t)arg2), 0ULL, ((uint64_t)(((uint64_t)(uintptr_t)arg3)) & (uint64_t)(0xfffffffdULL)));
        ssa_common_ret_op = (uint64_t)(ssa_call_res);
    } else {
        uint64_t __attribute__((unused)) ssa_call_res_1 = KeBugCheckEx(0xc7ULL, 9ULL, 0ULL, ((uint64_t)(((uint64_t)(uintptr_t)arg3)) & (uint64_t)(0xffffffffULL)));
        ssa_common_ret_op = (uint64_t)(ssa_call_res_1);
    }
    return (uint64_t)(ssa_common_ret_op);
}

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] ExpInterlockedFlushSList @ 0x140169450
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Hand-coded x86-64 kernel assembly instruction 'lock cmpxchg16b' at 0x140169466
 *   Live-In Registers : rcx, rbx
 *   Clobbered State   : rbx, r10, rax, rdx, rcx, memory, cc
 */
uint64_t __attribute__((naked, ms_abi)) ExpInterlockedFlushSList(PLIST_ENTRY arg1) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "prefetchw byte ptr [rcx]\n\t"
        "push rbx\n\t"
        "mov r10, rcx\n\t"
        "mov rax, qword ptr [rcx]\n\t"
        "mov rdx, qword ptr [rcx + 8]\n\t"
        ".L_ExpInterlockedFlushSList_14016945e:\n\t"
        "xor ecx, ecx\n\t"
        "mov rbx, rax\n\t"
        "xor bx, bx\n\t"
        "lock cmpxchg16b xmmword ptr [r10]\n\t"
        "jne .L_ExpInterlockedFlushSList_14016945e\n\t"
        ".L_ExpInterlockedFlushSList_14016946d:\n\t"
        "mov rax, rdx\n\t"
        "and al, 0xf0\n\t"
        "pop rbx\n\t"
        "ret\n\t"
        ".att_syntax prefix"
    );
}

/*
 * Function : ExTryConvertPushLockSharedToExclusiveEx @ 0x14012ead0 (37 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (51.12 ms, 33 SMT assertions)
 * Verified : x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) ExTryConvertPushLockSharedToExclusiveEx(uint64_t* arg1, uint32_t* arg2) {
    uint64_t __attribute__((unused)) ssa_common_ret_op = 0;
    if (((uint64_t)(((uint64_t)(((uint64_t)(uintptr_t)arg2)) & (uint64_t)(0xfffffffcULL))) == (uint64_t)(0ULL))) {
        uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))));
        (*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))) = (uint64_t)(((((uint64_t)(ssa_mem_ld) == (uint64_t)(0x11ULL))) ? (1ULL) : (ssa_mem_ld)));
        ssa_common_ret_op = (uint64_t)(((uint64_t)(((uint64_t)(ssa_mem_ld) & (uint64_t)(0xffffffffffffff00ULL))) | (uint64_t)(((uint64_t)(uint8_t)(((uint64_t)(ssa_mem_ld) == (uint64_t)(0x11ULL)))))));
    } else {
        uint64_t __attribute__((unused)) ssa_call_res = KeBugCheckEx(0x152ULL, ((uint64_t)(((uint64_t)(uintptr_t)arg2)) & (uint64_t)(0xffffffffULL)), ((uint64_t)(uintptr_t)arg1), 0ULL);
        ssa_common_ret_op = (uint64_t)(ssa_call_res);
    }
    return (uint32_t)(ssa_common_ret_op);
}

/*
 * Function : ExTryQueueWorkItem @ 0x14003cdc0 (37 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.50 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) ExTryQueueWorkItem(uint64_t arg1, uint64_t arg2) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)&PspSystemPartition))));
    uint64_t __attribute__((unused)) ssa_mem_ld_1 = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(ssa_mem_ld) + (uint64_t)(0x10ULL)))));
    uint64_t __attribute__((unused)) ssa_call_res = ExpTryQueueWorkItem(ssa_mem_ld_1, ((uint64_t)arg1), ((uint64_t)(((uint64_t)arg2)) & (uint64_t)(0xffffffffULL)), 0ULL);
    return (uint64_t)(ssa_call_res);
}

extern void __nt_asm_ext_14027d9fc(void);
extern unsigned char __nt_asm_sym_1403f9146[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] ExAcquireSpinLockExclusive @ 0x1400b4dc0
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Privileged CPU/CSR instruction 'mov rsi, cr8' at 0x1400b4dcd
 *   - Privileged CPU/CSR instruction 'mov cr8, rax' at 0x1400b4dd6
 *   - Privileged/control-register routine (2/18 insns) preserved as native x86-64 assembly
 *   Live-In Registers : rsi, rdi, rcx
 *   Clobbered State   : rdi, rsi, rax, rdx, memory, cc
 */
uint32_t __attribute__((naked, ms_abi)) ExAcquireSpinLockExclusive(uint64_t arg1) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov qword ptr [rsp + 0x10], rsi\n\t"
        "push rdi\n\t"
        "sub rsp, 0x20\n\t"
        "mov rdi, rcx\n\t"
        "mov rsi, cr8\n\t"
        "mov eax, 2\n\t"
        "mov cr8, rax\n\t"
        "test byte ptr [rip + __nt_asm_sym_1403f9146], 0x21\n\t"
        "jne .L_ExAcquireSpinLockExclusive_1401ae776\n\t"
        ".L_ExAcquireSpinLockExclusive_1400b4e0a:\n\t"
        "movzx eax, sil\n\t"
        "mov rsi, qword ptr [rsp + 0x38]\n\t"
        "add rsp, 0x20\n\t"
        "pop rdi\n\t"
        "ret\n\t"
        ".L_ExAcquireSpinLockExclusive_1401ae776:\n\t"
        "movzx edx, sil\n\t"
        "call __nt_asm_ext_14027d9fc\n\t"
        "nop\n\t"
        "jmp .L_ExAcquireSpinLockExclusive_1400b4e0a\n\t"
        ".att_syntax prefix"
    );
}

extern void __nt_asm_ext_14027db74(void);
extern unsigned char __nt_asm_sym_1403f9146[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] ExReleaseSpinLockExclusive @ 0x1400f8400
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Privileged CPU/CSR instruction 'mov cr8, rax' at 0x1400f841f
 *   - Privileged/control-register routine (1/15 insns) preserved as native x86-64 assembly
 *   Live-In Registers : rbx, rdx, rcx
 *   Clobbered State   : rbx, rax, rdx, memory, cc
 */
uint32_t __attribute__((naked, ms_abi)) ExReleaseSpinLockExclusive(uint32_t* arg1, uint64_t arg2) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "push rbx\n\t"
        "sub rsp, 0x20\n\t"
        "test byte ptr [rip + __nt_asm_sym_1403f9146], 1\n\t"
        "movzx ebx, dl\n\t"
        "jne .L_ExReleaseSpinLockExclusive_1401c2446\n\t"
        ".L_ExReleaseSpinLockExclusive_1400f8416:\n\t"
        "mov dword ptr [rcx], 0\n\t"
        ".L_ExReleaseSpinLockExclusive_1400f841c:\n\t"
        "movzx eax, bl\n\t"
        "mov cr8, rax\n\t"
        "add rsp, 0x20\n\t"
        "pop rbx\n\t"
        "ret\n\t"
        ".L_ExReleaseSpinLockExclusive_1401c2446:\n\t"
        "mov rdx, qword ptr [rsp + 0x28]\n\t"
        "call __nt_asm_ext_14027db74\n\t"
        "nop\n\t"
        "jmp .L_ExReleaseSpinLockExclusive_1400f841c\n\t"
        ".att_syntax prefix"
    );
}

extern void __nt_asm_ext_140032590(void);
extern void __nt_asm_ext_1400c0e60(void);
extern void __nt_asm_ext_1400c0930(void);
extern void __nt_asm_ext_1400622a0(void);
extern void __nt_asm_ext_140032820(void);
extern void __nt_asm_ext_1401637d0(void);
extern void __nt_asm_ext_140127404(void);
extern void __nt_asm_ext_1401fe10c(void);
extern unsigned char __nt_asm_sym_1403f9146[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] ExReleaseResourceAndLeaveCriticalRegion @ 0x140032410
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Privileged CPU/CSR instruction 'cli' at 0x140032466
 *   - Privileged CPU/CSR instruction 'cli' at 0x140032500
 *   - Privileged CPU/CSR instruction 'mov cr8, rax' at 0x140032506
 *   - Privileged CPU/CSR instruction 'mov rcx, cr8' at 0x14003250f
 *   - Privileged/control-register routine (4/125 insns) preserved as native x86-64 assembly
 *   Live-In Registers : rbx, rdi, rcx, rsi
 *   Clobbered State   : rdi, rax, rbx, rdx, rsi, rcx, r8, r9, memory, cc
 */
uint64_t __attribute__((naked, ms_abi)) ExReleaseResourceAndLeaveCriticalRegion(struct _RECON_EXRELEASERESOURCEANDLEAVECRITICALREGION_RCX* arg1, struct _RECON_EXRELEASERESOURCEANDLEAVECRITICALREGION_RDX* arg2) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov qword ptr [rsp + 0x18], rbx\n\t"
        "push rdi\n\t"
        "sub rsp, 0x50\n\t"
        "movzx eax, word ptr [rcx + 0x1a]\n\t"
        "mov rbx, rcx\n\t"
        "movzx edx, al\n\t"
        "and dl, 0x41\n\t"
        "cmp dl, 1\n\t"
        "je .L_ExReleaseResourceAndLeaveCriticalRegion_140032565\n\t"
        ".L_ExReleaseResourceAndLeaveCriticalRegion_140032430:\n\t"
        "and ax, 1\n\t"
        "jne .L_ExReleaseResourceAndLeaveCriticalRegion_14003250f\n\t"
        ".L_ExReleaseResourceAndLeaveCriticalRegion_14003243a:\n\t"
        "mov qword ptr [rsp + 0x60], rsi\n\t"
        "test ax, ax\n\t"
        "jne .L_ExReleaseResourceAndLeaveCriticalRegion_140187601\n\t"
        ".L_ExReleaseResourceAndLeaveCriticalRegion_140032448:\n\t"
        "mov rsi, qword ptr gs:[0x188]\n\t"
        "lea rcx, [rbx + 0x60]\n\t"
        "xor edi, edi\n\t"
        "mov qword ptr [rsp + 0x38], rcx\n\t"
        "mov qword ptr [rsp + 0x30], rdi\n\t"
        "mov byte ptr [rsp + 0x68], 2\n\t"
        "cli\n\t"
        "movzx eax, byte ptr [rip + __nt_asm_sym_1403f9146]\n\t"
        "test al, 0x21\n\t"
        "jne .L_ExReleaseResourceAndLeaveCriticalRegion_14018760f\n\t"
        ".L_ExReleaseResourceAndLeaveCriticalRegion_140032476:\n\t"
        "lea rdx, [rsp + 0x30]\n\t"
        "xor al, al\n\t"
        "xchg qword ptr [rcx], rdx\n\t"
        "test rdx, rdx\n\t"
        "jne .L_ExReleaseResourceAndLeaveCriticalRegion_1400324ed\n\t"
        ".L_ExReleaseResourceAndLeaveCriticalRegion_140032485:\n\t"
        "test al, al\n\t"
        "jne .L_ExReleaseResourceAndLeaveCriticalRegion_140032500\n\t"
        ".L_ExReleaseResourceAndLeaveCriticalRegion_140032489:\n\t"
        "mov r8, qword ptr gs:[0x188]\n\t"
        "mov rcx, rbx\n\t"
        "call __nt_asm_ext_140032590\n\t"
        "test al, al\n\t"
        "jne .L_ExReleaseResourceAndLeaveCriticalRegion_140187627\n\t"
        ".L_ExReleaseResourceAndLeaveCriticalRegion_1400324a2:\n\t"
        "movzx eax, byte ptr [rbx + 0x1a]\n\t"
        "lea r8, [rsp + 0x30]\n\t"
        "mov rdx, rsi\n\t"
        "test al, al\n\t"
        "jns .L_ExReleaseResourceAndLeaveCriticalRegion_1400324e6\n\t"
        ".L_ExReleaseResourceAndLeaveCriticalRegion_1400324b2:\n\t"
        "call __nt_asm_ext_1400c0e60\n\t"
        ".L_ExReleaseResourceAndLeaveCriticalRegion_1400324b7:\n\t"
        "mov rcx, qword ptr gs:[0x188]\n\t"
        "add word ptr [rcx + 0x1e4], 1\n\t"
        "jne .L_ExReleaseResourceAndLeaveCriticalRegion_1400324d6\n\t"
        ".L_ExReleaseResourceAndLeaveCriticalRegion_1400324ca:\n\t"
        "lea rax, [rcx + 0x98]\n\t"
        "cmp qword ptr [rax], rax\n\t"
        "jne .L_ExReleaseResourceAndLeaveCriticalRegion_14003254d\n\t"
        ".L_ExReleaseResourceAndLeaveCriticalRegion_1400324d6:\n\t"
        "mov rsi, qword ptr [rsp + 0x60]\n\t"
        "mov rbx, qword ptr [rsp + 0x70]\n\t"
        "add rsp, 0x50\n\t"
        "pop rdi\n\t"
        "ret\n\t"
        ".L_ExReleaseResourceAndLeaveCriticalRegion_1400324e6:\n\t"
        "call __nt_asm_ext_1400c0930\n\t"
        "jmp .L_ExReleaseResourceAndLeaveCriticalRegion_1400324b7\n\t"
        ".L_ExReleaseResourceAndLeaveCriticalRegion_1400324ed:\n\t"
        "lea r8, [rsp + 0x68]\n\t"
        "lea rcx, [rsp + 0x30]\n\t"
        "call __nt_asm_ext_1400622a0\n\t"
        "mov al, 1\n\t"
        "jmp .L_ExReleaseResourceAndLeaveCriticalRegion_140032485\n\t"
        ".L_ExReleaseResourceAndLeaveCriticalRegion_140032500:\n\t"
        "cli\n\t"
        "movzx eax, byte ptr [rsp + 0x68]\n\t"
        "mov cr8, rax\n\t"
        "jmp .L_ExReleaseResourceAndLeaveCriticalRegion_140032489\n\t"
        ".L_ExReleaseResourceAndLeaveCriticalRegion_14003250f:\n\t"
        "mov rcx, cr8\n\t"
        "mov rdx, qword ptr gs:[0x188]\n\t"
        "cmp cl, 2\n\t"
        "ja .L_ExReleaseResourceAndLeaveCriticalRegion_1401875ca\n\t"
        ".L_ExReleaseResourceAndLeaveCriticalRegion_140032525:\n\t"
        "cmp cl, 1\n\t"
        "jae .L_ExReleaseResourceAndLeaveCriticalRegion_14003243a\n\t"
        ".L_ExReleaseResourceAndLeaveCriticalRegion_14003252e:\n\t"
        "test dword ptr [rdx + 0x74], 0x400\n\t"
        "jne .L_ExReleaseResourceAndLeaveCriticalRegion_14003243a\n\t"
        ".L_ExReleaseResourceAndLeaveCriticalRegion_14003253b:\n\t"
        "cmp dword ptr [rdx + 0x1e4], 0\n\t"
        "jne .L_ExReleaseResourceAndLeaveCriticalRegion_14003243a\n\t"
        ".L_ExReleaseResourceAndLeaveCriticalRegion_140032548:\n\t"
        "jmp .L_ExReleaseResourceAndLeaveCriticalRegion_1401875e6\n\t"
        ".L_ExReleaseResourceAndLeaveCriticalRegion_14003254d:\n\t"
        "cmp word ptr [rcx + 0x1e6], 0\n\t"
        "jne .L_ExReleaseResourceAndLeaveCriticalRegion_1400324d6\n\t"
        ".L_ExReleaseResourceAndLeaveCriticalRegion_14003255b:\n\t"
        "call __nt_asm_ext_140032820\n\t"
        "jmp .L_ExReleaseResourceAndLeaveCriticalRegion_1400324d6\n\t"
        ".L_ExReleaseResourceAndLeaveCriticalRegion_140032565:\n\t"
        "xor edi, edi\n\t"
        "xor r9d, r9d\n\t"
        "mov r8, rbx\n\t"
        "mov qword ptr [rsp + 0x20], rdi\n\t"
        "mov ecx, 0x1c6\n\t"
        "lea edx, [rdi + 0xf]\n\t"
        "call __nt_asm_ext_1401637d0\n\t"
        "int3\n\t"
        ".L_ExReleaseResourceAndLeaveCriticalRegion_1401875ca:\n\t"
        "xor edi, edi\n\t"
        "movzx r8d, cl\n\t"
        "xor edx, edx\n\t"
        "mov qword ptr [rsp + 0x20], rdi\n\t"
        "mov ecx, 0x1c6\n\t"
        "lea r9d, [rdi + 2]\n\t"
        "call __nt_asm_ext_1401637d0\n\t"
        "int3\n\t"
        ".L_ExReleaseResourceAndLeaveCriticalRegion_1401875e6:\n\t"
        "xor edi, edi\n\t"
        "xor r9d, r9d\n\t"
        "xor r8d, r8d\n\t"
        "mov qword ptr [rsp + 0x20], rdi\n\t"
        "mov ecx, 0x1c6\n\t"
        "lea edx, [rdi + 7]\n\t"
        "call __nt_asm_ext_1401637d0\n\t"
        "int3\n\t"
        ".L_ExReleaseResourceAndLeaveCriticalRegion_140187601:\n\t"
        "mov rcx, rbx\n\t"
        "call __nt_asm_ext_140127404\n\t"
        "nop\n\t"
        "jmp .L_ExReleaseResourceAndLeaveCriticalRegion_1400324b7\n\t"
        ".L_ExReleaseResourceAndLeaveCriticalRegion_14018760f:\n\t"
        "mov rdx, rcx\n\t"
        "lea r8, [rsp + 0x68]\n\t"
        "lea rcx, [rsp + 0x30]\n\t"
        "call __nt_asm_ext_1401fe10c\n\t"
        "nop\n\t"
        "jmp .L_ExReleaseResourceAndLeaveCriticalRegion_140032485\n\t"
        ".L_ExReleaseResourceAndLeaveCriticalRegion_140187627:\n\t"
        "movzx eax, sil\n\t"
        "and al, 3\n\t"
        "cmp al, 3\n\t"
        "je .L_ExReleaseResourceAndLeaveCriticalRegion_1400324a2\n\t"
        ".L_ExReleaseResourceAndLeaveCriticalRegion_140187635:\n\t"
        "cmp rsi, r8\n\t"
        "je .L_ExReleaseResourceAndLeaveCriticalRegion_1400324a2\n\t"
        ".L_ExReleaseResourceAndLeaveCriticalRegion_14018763e:\n\t"
        "mov r9, rsi\n\t"
        "mov qword ptr [rsp + 0x20], rdi\n\t"
        "mov rdx, rbx\n\t"
        "mov ecx, 0x16e\n\t"
        "call __nt_asm_ext_1401637d0\n\t"
        "int3\n\t"
        ".att_syntax prefix"
    );
}

/*
 * Function : ExGetExclusiveWaiterCount @ 0x14027d400 (45 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (6.68 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) ExGetExclusiveWaiterCount(struct _RECON_EXGETEXCLUSIVEWAITERCOUNT_RCX* arg1) {
    uint64_t __attribute__((unused)) ssa_reg_rcx_0 = 0;
    uint8_t __attribute__((unused)) ssa_mem_ld = (uint8_t)(arg1->field_0x1a);
    if (((uint8_t)(((uint8_t)(ssa_mem_ld) & (uint8_t)(1ULL))) == (uint8_t)(0ULL))) {
        ssa_reg_rcx_0 = (uint64_t)(((uint64_t)(uintptr_t)arg1));
    } else {
        uint64_t __attribute__((unused)) ssa_call_res = KeBugCheckEx(0x1c6ULL, 14ULL, ((uint64_t)(uintptr_t)arg1), 0ULL);
        ssa_reg_rcx_0 = (uint64_t)(0x1c6ULL);
    }
    uint32_t __attribute__((unused)) ssa_mem_ld_2 = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(ssa_reg_rcx_0) + (uint64_t)(0x4cULL)))));
    return (uint32_t)(((uint64_t)(uint32_t)(ssa_mem_ld_2)));
}

/*
 * Function : ExGetSharedWaiterCount @ 0x14027d440 (45 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (6.28 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) ExGetSharedWaiterCount(struct _RECON_EXGETSHAREDWAITERCOUNT_RCX* arg1) {
    uint64_t __attribute__((unused)) ssa_reg_rcx_0 = 0;
    uint8_t __attribute__((unused)) ssa_mem_ld = (uint8_t)(arg1->field_0x1a);
    if (((uint8_t)(((uint8_t)(ssa_mem_ld) & (uint8_t)(1ULL))) == (uint8_t)(0ULL))) {
        ssa_reg_rcx_0 = (uint64_t)(((uint64_t)(uintptr_t)arg1));
    } else {
        uint64_t __attribute__((unused)) ssa_call_res = KeBugCheckEx(0x1c6ULL, 14ULL, ((uint64_t)(uintptr_t)arg1), 0ULL);
        ssa_reg_rcx_0 = (uint64_t)(0x1c6ULL);
    }
    uint32_t __attribute__((unused)) ssa_mem_ld_2 = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(ssa_reg_rcx_0) + (uint64_t)(0x48ULL)))));
    return (uint32_t)(((uint64_t)(uint32_t)(ssa_mem_ld_2)));
}

extern void __nt_asm_ext_14027dba0(void);
extern unsigned char __nt_asm_sym_1403f9146[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] ExReleaseSpinLockShared @ 0x1400f34e0
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Privileged CPU/CSR instruction 'mov cr8, rax' at 0x1400f3503
 *   - Privileged/control-register routine (1/16 insns) preserved as native x86-64 assembly
 *   Live-In Registers : rbx, rdx, rcx
 *   Clobbered State   : rbx, rax, rdx, memory, cc
 */
uint32_t __attribute__((naked, ms_abi)) ExReleaseSpinLockShared(uint32_t* arg1, uint64_t arg2) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "push rbx\n\t"
        "sub rsp, 0x20\n\t"
        "test byte ptr [rip + __nt_asm_sym_1403f9146], 1\n\t"
        "movzx ebx, dl\n\t"
        "jne .L_ExReleaseSpinLockShared_14017918a\n\t"
        ".L_ExReleaseSpinLockShared_1400f34f6:\n\t"
        "lock and dword ptr [rcx], 0xbfffffff\n\t"
        "lock dec dword ptr [rcx]\n\t"
        ".L_ExReleaseSpinLockShared_1400f3500:\n\t"
        "movzx eax, bl\n\t"
        "mov cr8, rax\n\t"
        "add rsp, 0x20\n\t"
        "pop rbx\n\t"
        "ret\n\t"
        ".L_ExReleaseSpinLockShared_14017918a:\n\t"
        "mov rdx, qword ptr [rsp + 0x28]\n\t"
        "call __nt_asm_ext_14027dba0\n\t"
        "nop\n\t"
        "jmp .L_ExReleaseSpinLockShared_1400f3500\n\t"
        ".att_syntax prefix"
    );
}

/*
 * Function : ExGetCurrentProcessorCounts @ 0x1400fca00 (47 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (8.01 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) ExGetCurrentProcessorCounts(uint32_t* arg1, uint32_t* arg2, uint32_t* arg3) {
    struct _KPRCB* __attribute__((unused)) ssa_gs_val = KeGetCurrentPrcb();
    struct _KTHREAD* __attribute__((unused)) ssa_mem_ld = (struct _KTHREAD*)(uintptr_t)(ssa_gs_val->IdleThread);
    uint32_t __attribute__((unused)) ssa_mem_ld_1 = (uint32_t)(ssa_mem_ld->KernelTime);
    (*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))) = (uint32_t)(ssa_mem_ld_1);
    uint32_t __attribute__((unused)) ssa_mem_ld_2 = (uint32_t)(ssa_gs_val->UserTime);
    uint32_t __attribute__((unused)) ssa_mem_ld_3 = (uint32_t)(ssa_gs_val->KernelTime);
    (*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg2))) = (uint32_t)(((uint32_t)(ssa_mem_ld_3) + (uint32_t)(ssa_mem_ld_2)));
    uint32_t __attribute__((unused)) ssa_mem_ld_4 = (uint32_t)(ssa_gs_val->Number);
    (*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg3))) = (uint32_t)(ssa_mem_ld_4);
    return (uint64_t)(((uint64_t)(uint32_t)(ssa_mem_ld_4)));
}

/*
 * Function : ExEnterCriticalRegionAndAcquireSharedWaitForExclusive @ 0x14027d370 (48 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (6.40 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) ExEnterCriticalRegionAndAcquireSharedWaitForExclusive(void) {
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    uint16_t __attribute__((unused)) ssa_mem_ld = (uint16_t)(ssa_gs_val->KernelApcDisable);
    ssa_gs_val->KernelApcDisable = (int16_t)(((uint16_t)(ssa_mem_ld) + (uint16_t)(0xffffffffffffffffULL)));
    uint64_t __attribute__((unused)) ssa_call_res = ExAcquireSharedWaitForExclusive(0ULL, 1ULL, 0ULL, 0ULL);
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val_1 = KeGetCurrentThread();
    uint64_t __attribute__((unused)) ssa_mem_ld_1 = (uint64_t)(uintptr_t)(ssa_gs_val_1->Win32Thread);
    return (uint64_t)(ssa_mem_ld_1);
}

extern void __nt_asm_ext_1400c4010(void);
extern void __nt_asm_ext_1401637d0(void);
extern void __nt_asm_ext_140126eb8(void);
extern void __nt_asm_ext_140126f10(void);
extern void __nt_asm_ext_1402a6460(void);

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] ExEnterCriticalRegionAndAcquireResourceShared @ 0x1400c5470
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Privileged CPU/CSR instruction 'mov rdx, cr8' at 0x1400c54d5
 *   - Privileged/control-register routine (1/90 insns) preserved as native x86-64 assembly
 *   Live-In Registers : rbx, rcx, rdi
 *   Clobbered State   : rbx, rax, rdx, rcx, rdi, r8, r9, memory, cc
 */
uint64_t __attribute__((naked, ms_abi)) ExEnterCriticalRegionAndAcquireResourceShared(uint16_t* arg1, uint32_t* arg2, struct _RECON_EXENTERCRITICALREGIONANDACQUIRERESOURCESHARED_R8* arg3, struct _RECON_EXENTERCRITICALREGIONANDACQUIRERESOURCESHARED_R9* arg4) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "push rbx\n\t"
        "sub rsp, 0x30\n\t"
        "mov rax, qword ptr gs:[0x188]\n\t"
        "mov rbx, rcx\n\t"
        "dec word ptr [rax + 0x1e4]\n\t"
        "movzx eax, word ptr [rcx + 0x1a]\n\t"
        "movzx edx, al\n\t"
        "and dl, 0x41\n\t"
        "cmp dl, 1\n\t"
        "je .L_ExEnterCriticalRegionAndAcquireResourceShared_1400c5520\n\t"
        ".L_ExEnterCriticalRegionAndAcquireResourceShared_1400c549c:\n\t"
        "and ax, 1\n\t"
        "jne .L_ExEnterCriticalRegionAndAcquireResourceShared_1400c54d5\n\t"
        ".L_ExEnterCriticalRegionAndAcquireResourceShared_1400c54a2:\n\t"
        "mov qword ptr [rsp + 0x40], rdi\n\t"
        "test ax, ax\n\t"
        "jne .L_ExEnterCriticalRegionAndAcquireResourceShared_1401b32d6\n\t"
        ".L_ExEnterCriticalRegionAndAcquireResourceShared_1400c54b0:\n\t"
        "mov dl, 1\n\t"
        "mov rcx, rbx\n\t"
        "call __nt_asm_ext_1400c4010\n\t"
        ".L_ExEnterCriticalRegionAndAcquireResourceShared_1400c54ba:\n\t"
        "mov rax, qword ptr gs:[0x188]\n\t"
        "mov rdi, qword ptr [rsp + 0x40]\n\t"
        "mov rax, qword ptr [rax + 0x1c8]\n\t"
        "add rsp, 0x30\n\t"
        "pop rbx\n\t"
        "ret\n\t"
        ".L_ExEnterCriticalRegionAndAcquireResourceShared_1400c54d5:\n\t"
        "mov rdx, cr8\n\t"
        "mov r8, qword ptr gs:[0x188]\n\t"
        "cmp dl, 1\n\t"
        "ja .L_ExEnterCriticalRegionAndAcquireResourceShared_1401b324a\n\t"
        ".L_ExEnterCriticalRegionAndAcquireResourceShared_1400c54eb:\n\t"
        "cmp dl, 2\n\t"
        "jae .L_ExEnterCriticalRegionAndAcquireResourceShared_1401b3268\n\t"
        ".L_ExEnterCriticalRegionAndAcquireResourceShared_1400c54f4:\n\t"
        "test byte ptr [r8 + 0xc0], 2\n\t"
        "jne .L_ExEnterCriticalRegionAndAcquireResourceShared_1401b329a\n\t"
        ".L_ExEnterCriticalRegionAndAcquireResourceShared_1400c5502:\n\t"
        "cmp dl, 1\n\t"
        "jae .L_ExEnterCriticalRegionAndAcquireResourceShared_1400c54a2\n\t"
        ".L_ExEnterCriticalRegionAndAcquireResourceShared_1400c5507:\n\t"
        "test dword ptr [r8 + 0x74], 0x400\n\t"
        "jne .L_ExEnterCriticalRegionAndAcquireResourceShared_1400c54a2\n\t"
        ".L_ExEnterCriticalRegionAndAcquireResourceShared_1400c5511:\n\t"
        "cmp dword ptr [r8 + 0x1e4], 0\n\t"
        "jne .L_ExEnterCriticalRegionAndAcquireResourceShared_1400c54a2\n\t"
        ".L_ExEnterCriticalRegionAndAcquireResourceShared_1400c551b:\n\t"
        "jmp .L_ExEnterCriticalRegionAndAcquireResourceShared_1401b32b8\n\t"
        ".L_ExEnterCriticalRegionAndAcquireResourceShared_1400c5520:\n\t"
        "xor r9d, r9d\n\t"
        "mov qword ptr [rsp + 0x20], 0\n\t"
        "mov r8, rbx\n\t"
        "mov ecx, 0x1c6\n\t"
        "lea edx, [r9 + 0xf]\n\t"
        "call __nt_asm_ext_1401637d0\n\t"
        "int3\n\t"
        ".L_ExEnterCriticalRegionAndAcquireResourceShared_1401b324a:\n\t"
        "movzx r8d, dl\n\t"
        "mov ecx, 0x1c6\n\t"
        "xor edx, edx\n\t"
        "mov qword ptr [rsp + 0x20], 0\n\t"
        "lea r9d, [rdx + 1]\n\t"
        "call __nt_asm_ext_1401637d0\n\t"
        "int3\n\t"
        ".L_ExEnterCriticalRegionAndAcquireResourceShared_1401b3268:\n\t"
        "mov ecx, dword ptr gs:[0x2fec]\n\t"
        "test ecx, 0x10001\n\t"
        "je .L_ExEnterCriticalRegionAndAcquireResourceShared_1400c54f4\n\t"
        ".L_ExEnterCriticalRegionAndAcquireResourceShared_1401b327c:\n\t"
        "xor r9d, r9d\n\t"
        "mov qword ptr [rsp + 0x20], 0\n\t"
        "xor r8d, r8d\n\t"
        "mov ecx, 0x1c6\n\t"
        "lea edx, [r9 + 5]\n\t"
        "call __nt_asm_ext_1401637d0\n\t"
        "int3\n\t"
        ".L_ExEnterCriticalRegionAndAcquireResourceShared_1401b329a:\n\t"
        "xor r9d, r9d\n\t"
        "mov qword ptr [rsp + 0x20], 0\n\t"
        "xor r8d, r8d\n\t"
        "mov ecx, 0x1c6\n\t"
        "lea edx, [r9 + 6]\n\t"
        "call __nt_asm_ext_1401637d0\n\t"
        "int3\n\t"
        ".L_ExEnterCriticalRegionAndAcquireResourceShared_1401b32b8:\n\t"
        "xor r9d, r9d\n\t"
        "mov qword ptr [rsp + 0x20], 0\n\t"
        "xor r8d, r8d\n\t"
        "mov ecx, 0x1c6\n\t"
        "lea edx, [r9 + 7]\n\t"
        "call __nt_asm_ext_1401637d0\n\t"
        "int3\n\t"
        ".L_ExEnterCriticalRegionAndAcquireResourceShared_1401b32d6:\n\t"
        "call __nt_asm_ext_140126eb8\n\t"
        "mov r8b, 1\n\t"
        "mov rdx, rax\n\t"
        "mov rcx, rbx\n\t"
        "mov rdi, rax\n\t"
        "call __nt_asm_ext_140126f10\n\t"
        "test al, al\n\t"
        "jne .L_ExEnterCriticalRegionAndAcquireResourceShared_1400c54ba\n\t"
        ".L_ExEnterCriticalRegionAndAcquireResourceShared_1401b32f4:\n\t"
        "xor edx, edx\n\t"
        "mov rcx, rdi\n\t"
        "call __nt_asm_ext_1402a6460\n\t"
        "nop\n\t"
        "jmp .L_ExEnterCriticalRegionAndAcquireResourceShared_1400c54ba\n\t"
        ".att_syntax prefix"
    );
}

extern void __nt_asm_ext_140127404(void);
extern void __nt_asm_ext_1401637d0(void);

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] ExReleaseResourceLite @ 0x1400c07b0
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Privileged CPU/CSR instruction 'mov rcx, cr8' at 0x1400c08a3
 *   - Privileged/control-register routine (1/54 insns) preserved as native x86-64 assembly
 *   Live-In Registers : rbx, rdi, rcx
 *   Clobbered State   : rdi, rax, rbx, rdx, rcx, r9, r8, memory, cc
 */
uint32_t __attribute__((naked, ms_abi)) ExReleaseResourceLite(uint16_t* arg1, struct _RECON_EXRELEASERESOURCELITE_RDX* arg2) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov qword ptr [rsp + 0x18], rbx\n\t"
        "push rdi\n\t"
        "sub rsp, 0x50\n\t"
        "movzx eax, word ptr [rcx + 0x1a]\n\t"
        "mov rbx, rcx\n\t"
        "movzx edx, al\n\t"
        "and dl, 0x41\n\t"
        "cmp dl, 1\n\t"
        "je .L_ExReleaseResourceLite_1400c08f4\n\t"
        ".L_ExReleaseResourceLite_1400c07d0:\n\t"
        "and ax, 1\n\t"
        "jne .L_ExReleaseResourceLite_1400c08a3\n\t"
        ".L_ExReleaseResourceLite_1400c07da:\n\t"
        "test ax, ax\n\t"
        "jne .L_ExReleaseResourceLite_1400c08e1\n\t"
        ".L_ExReleaseResourceLite_1400c08a3:\n\t"
        "mov rcx, cr8\n\t"
        "mov rdx, qword ptr gs:[0x188]\n\t"
        "cmp cl, 2\n\t"
        "ja .L_ExReleaseResourceLite_1401b1294\n\t"
        ".L_ExReleaseResourceLite_1400c08b9:\n\t"
        "cmp cl, 1\n\t"
        "jae .L_ExReleaseResourceLite_1400c07da\n\t"
        ".L_ExReleaseResourceLite_1400c08c2:\n\t"
        "test dword ptr [rdx + 0x74], 0x400\n\t"
        "jne .L_ExReleaseResourceLite_1400c07da\n\t"
        ".L_ExReleaseResourceLite_1400c08cf:\n\t"
        "cmp dword ptr [rdx + 0x1e4], 0\n\t"
        "jne .L_ExReleaseResourceLite_1400c07da\n\t"
        ".L_ExReleaseResourceLite_1400c08dc:\n\t"
        "jmp .L_ExReleaseResourceLite_1401b12b0\n\t"
        ".L_ExReleaseResourceLite_1400c08e1:\n\t"
        "mov rcx, rbx\n\t"
        "call __nt_asm_ext_140127404\n\t"
        "mov rbx, qword ptr [rsp + 0x70]\n\t"
        "add rsp, 0x50\n\t"
        "pop rdi\n\t"
        "ret\n\t"
        ".L_ExReleaseResourceLite_1400c08f4:\n\t"
        "xor edi, edi\n\t"
        "xor r9d, r9d\n\t"
        "mov r8, rbx\n\t"
        "mov qword ptr [rsp + 0x20], rdi\n\t"
        "mov ecx, 0x1c6\n\t"
        "lea edx, [rdi + 0xf]\n\t"
        "call __nt_asm_ext_1401637d0\n\t"
        "int3\n\t"
        ".L_ExReleaseResourceLite_1401b1294:\n\t"
        "xor edi, edi\n\t"
        "movzx r8d, cl\n\t"
        "xor edx, edx\n\t"
        "mov qword ptr [rsp + 0x20], rdi\n\t"
        "mov ecx, 0x1c6\n\t"
        "lea r9d, [rdi + 2]\n\t"
        "call __nt_asm_ext_1401637d0\n\t"
        "int3\n\t"
        ".L_ExReleaseResourceLite_1401b12b0:\n\t"
        "xor edi, edi\n\t"
        "xor r9d, r9d\n\t"
        "xor r8d, r8d\n\t"
        "mov qword ptr [rsp + 0x20], rdi\n\t"
        "mov ecx, 0x1c6\n\t"
        "lea edx, [rdi + 7]\n\t"
        "call __nt_asm_ext_1401637d0\n\t"
        "int3\n\t"
        ".att_syntax prefix"
    );
}

extern void __nt_asm_ext_1401637d0(void);
extern void __nt_asm_ext_140127ba8(void);

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] ExIsFastResourceHeldExclusive @ 0x14012ea30
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Privileged CPU/CSR instruction 'mov rcx, cr8' at 0x14012ea43
 *   - Privileged CPU/CSR instruction 'mov r10, cr8' at 0x1401d2085
 *   - Privileged CPU/CSR instruction 'mov cr8, r9' at 0x1401d2089
 *   - Privileged CPU/CSR instruction 'mov cr8, rax' at 0x1401d20a8
 *   - Privileged/control-register routine (4/42 insns) preserved as native x86-64 assembly
 *   Live-In Registers : rcx
 *   Clobbered State   : rax, rdx, rcx, r9, r8, r10, memory, cc
 */
uint32_t __attribute__((naked, ms_abi)) ExIsFastResourceHeldExclusive(uint16_t* arg1) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "sub rsp, 0x38\n\t"
        "movzx eax, word ptr [rcx + 0x1a]\n\t"
        "mov rdx, rcx\n\t"
        "test al, 1\n\t"
        "je .L_ExIsFastResourceHeldExclusive_1401d2052\n\t"
        ".L_ExIsFastResourceHeldExclusive_14012ea43:\n\t"
        "mov rcx, cr8\n\t"
        "mov r9d, 2\n\t"
        "cmp cl, r9b\n\t"
        "ja .L_ExIsFastResourceHeldExclusive_1401d206d\n\t"
        ".L_ExIsFastResourceHeldExclusive_14012ea56:\n\t"
        "test al, al\n\t"
        "js .L_ExIsFastResourceHeldExclusive_1401d2085\n\t"
        ".L_ExIsFastResourceHeldExclusive_14012ea5e:\n\t"
        "xor eax, eax\n\t"
        ".L_ExIsFastResourceHeldExclusive_14012ea60:\n\t"
        "add rsp, 0x38\n\t"
        "ret\n\t"
        ".L_ExIsFastResourceHeldExclusive_1401d2052:\n\t"
        "xor eax, eax\n\t"
        "mov r8, rdx\n\t"
        "xor r9d, r9d\n\t"
        "mov qword ptr [rsp + 0x20], rax\n\t"
        "mov ecx, 0x1c6\n\t"
        "lea edx, [rax + 3]\n\t"
        "call __nt_asm_ext_1401637d0\n\t"
        "int3\n\t"
        ".L_ExIsFastResourceHeldExclusive_1401d206d:\n\t"
        "movzx r8d, cl\n\t"
        "xor eax, eax\n\t"
        "mov ecx, 0x1c6\n\t"
        "mov qword ptr [rsp + 0x20], rax\n\t"
        "xor edx, edx\n\t"
        "call __nt_asm_ext_1401637d0\n\t"
        "int3\n\t"
        ".L_ExIsFastResourceHeldExclusive_1401d2085:\n\t"
        "mov r10, cr8\n\t"
        "mov cr8, r9\n\t"
        "mov rcx, qword ptr gs:[0x188]\n\t"
        "xor r9d, r9d\n\t"
        "xor r8d, r8d\n\t"
        "call __nt_asm_ext_140127ba8\n\t"
        "mov r9, rax\n\t"
        "movzx eax, r10b\n\t"
        "mov cr8, rax\n\t"
        "xor eax, eax\n\t"
        "test r9, r9\n\t"
        "setne al\n\t"
        "jmp .L_ExIsFastResourceHeldExclusive_14012ea60\n\t"
        ".att_syntax prefix"
    );
}

extern void __nt_asm_ext_140170180(void);
extern void __nt_asm_ext_1401637d0(void);

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] ExInitializeFastOwnerEntry @ 0x14012c570
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Privileged CPU/CSR instruction 'mov rax, cr8' at 0x14012c579
 *   - Privileged/control-register routine (1/21 insns) preserved as native x86-64 assembly
 *   Live-In Registers : rbx, rcx
 *   Clobbered State   : rbx, rax, r9, rdx, r8, rcx, memory, cc
 */
uint64_t __attribute__((naked, ms_abi)) ExInitializeFastOwnerEntry(uint64_t arg1) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "push rbx\n\t"
        "sub rsp, 0x30\n\t"
        "mov rbx, rcx\n\t"
        "mov rax, cr8\n\t"
        "mov r9d, 2\n\t"
        "cmp al, r9b\n\t"
        "ja .L_ExInitializeFastOwnerEntry_1401d1146\n\t"
        ".L_ExInitializeFastOwnerEntry_14012c58c:\n\t"
        "xor edx, edx\n\t"
        "lea r8d, [r9 + 0x46]\n\t"
        "call __nt_asm_ext_140170180\n\t"
        "mov rax, qword ptr gs:[0x188]\n\t"
        "mov qword ptr [rbx + 0x20], rax\n\t"
        "add rsp, 0x30\n\t"
        "pop rbx\n\t"
        "ret\n\t"
        ".L_ExInitializeFastOwnerEntry_1401d1146:\n\t"
        "and qword ptr [rsp + 0x20], 0\n\t"
        "xor edx, edx\n\t"
        "movzx r8d, al\n\t"
        "mov ecx, 0x1c6\n\t"
        "call __nt_asm_ext_1401637d0\n\t"
        "int3\n\t"
        ".att_syntax prefix"
    );
}

extern void __nt_asm_ext_1401266b0(void);
extern void __nt_asm_ext_1401637d0(void);

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] ExAcquireResourceExclusiveLite @ 0x1400c3c60
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Privileged CPU/CSR instruction 'mov rdx, cr8' at 0x1400c3e67
 *   - Privileged/control-register routine (1/82 insns) preserved as native x86-64 assembly
 *   Live-In Registers : rbx, r14, rdx, rcx
 *   Clobbered State   : rbx, r14, rcx, r8, rax, rdx, r9, memory, cc
 */
uint32_t __attribute__((naked, ms_abi)) ExAcquireResourceExclusiveLite(uint16_t* arg1, uint64_t arg2, uint64_t arg3, struct _RECON_EXACQUIRERESOURCEEXCLUSIVELITE_R9* arg4) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "push rbx\n\t"
        "push r14\n\t"
        "sub rsp, 0x78\n\t"
        "test dl, dl\n\t"
        "mov rbx, rcx\n\t"
        "movzx ecx, word ptr [rcx + 0x1a]\n\t"
        "movzx r14d, dl\n\t"
        "sete r8b\n\t"
        "movzx eax, cl\n\t"
        "inc r8b\n\t"
        "and al, 0x41\n\t"
        "cmp al, 1\n\t"
        "je .L_ExAcquireResourceExclusiveLite_1400c3eed\n\t"
        ".L_ExAcquireResourceExclusiveLite_1400c3c89:\n\t"
        "and cx, 1\n\t"
        "jne .L_ExAcquireResourceExclusiveLite_1400c3e67\n\t"
        ".L_ExAcquireResourceExclusiveLite_1400c3c93:\n\t"
        "test cx, cx\n\t"
        "jne .L_ExAcquireResourceExclusiveLite_1400c3ed9\n\t"
        ".L_ExAcquireResourceExclusiveLite_1400c3e67:\n\t"
        "mov rdx, cr8\n\t"
        "mov r9, qword ptr gs:[0x188]\n\t"
        "cmp dl, r8b\n\t"
        "ja .L_ExAcquireResourceExclusiveLite_1401b2a06\n\t"
        ".L_ExAcquireResourceExclusiveLite_1400c3e7d:\n\t"
        "cmp dl, 2\n\t"
        "jae .L_ExAcquireResourceExclusiveLite_1401b2a22\n\t"
        ".L_ExAcquireResourceExclusiveLite_1400c3e86:\n\t"
        "test byte ptr [r9 + 0xc0], 2\n\t"
        "jne .L_ExAcquireResourceExclusiveLite_1401b2a50\n\t"
        ".L_ExAcquireResourceExclusiveLite_1400c3e94:\n\t"
        "cmp dl, 1\n\t"
        "jae .L_ExAcquireResourceExclusiveLite_1400c3c93\n\t"
        ".L_ExAcquireResourceExclusiveLite_1400c3e9d:\n\t"
        "test dword ptr [r9 + 0x74], 0x400\n\t"
        "jne .L_ExAcquireResourceExclusiveLite_1400c3c93\n\t"
        ".L_ExAcquireResourceExclusiveLite_1400c3eab:\n\t"
        "cmp dword ptr [r9 + 0x1e4], 0\n\t"
        "jne .L_ExAcquireResourceExclusiveLite_1400c3c93\n\t"
        ".L_ExAcquireResourceExclusiveLite_1400c3eb9:\n\t"
        "jmp .L_ExAcquireResourceExclusiveLite_1401b2a6b\n\t"
        ".L_ExAcquireResourceExclusiveLite_1400c3ed9:\n\t"
        "movzx edx, r14b\n\t"
        "mov rcx, rbx\n\t"
        "call __nt_asm_ext_1401266b0\n\t"
        "add rsp, 0x78\n\t"
        "pop r14\n\t"
        "pop rbx\n\t"
        "ret\n\t"
        ".L_ExAcquireResourceExclusiveLite_1400c3eed:\n\t"
        "xor eax, eax\n\t"
        "xor r9d, r9d\n\t"
        "mov r8, rbx\n\t"
        "mov qword ptr [rsp + 0x20], rax\n\t"
        "mov ecx, 0x1c6\n\t"
        "lea edx, [rax + 0xf]\n\t"
        "call __nt_asm_ext_1401637d0\n\t"
        "int3\n\t"
        ".L_ExAcquireResourceExclusiveLite_1401b2a06:\n\t"
        "movzx r9d, r8b\n\t"
        "xor eax, eax\n\t"
        "movzx r8d, dl\n\t"
        "mov ecx, 0x1c6\n\t"
        "xor edx, edx\n\t"
        "mov qword ptr [rsp + 0x20], rax\n\t"
        "call __nt_asm_ext_1401637d0\n\t"
        "int3\n\t"
        ".L_ExAcquireResourceExclusiveLite_1401b2a22:\n\t"
        "mov eax, dword ptr gs:[0x2fec]\n\t"
        "test eax, 0x10001\n\t"
        "je .L_ExAcquireResourceExclusiveLite_1400c3e86\n\t"
        ".L_ExAcquireResourceExclusiveLite_1401b2a35:\n\t"
        "xor eax, eax\n\t"
        "xor r9d, r9d\n\t"
        "xor r8d, r8d\n\t"
        "mov qword ptr [rsp + 0x20], rax\n\t"
        "mov ecx, 0x1c6\n\t"
        "lea edx, [rax + 5]\n\t"
        "call __nt_asm_ext_1401637d0\n\t"
        "int3\n\t"
        ".L_ExAcquireResourceExclusiveLite_1401b2a50:\n\t"
        "xor eax, eax\n\t"
        "xor r9d, r9d\n\t"
        "xor r8d, r8d\n\t"
        "mov qword ptr [rsp + 0x20], rax\n\t"
        "mov ecx, 0x1c6\n\t"
        "lea edx, [rax + 6]\n\t"
        "call __nt_asm_ext_1401637d0\n\t"
        "int3\n\t"
        ".L_ExAcquireResourceExclusiveLite_1401b2a6b:\n\t"
        "xor eax, eax\n\t"
        "xor r9d, r9d\n\t"
        "xor r8d, r8d\n\t"
        "mov qword ptr [rsp + 0x20], rax\n\t"
        "mov ecx, 0x1c6\n\t"
        "lea edx, [rax + 7]\n\t"
        "call __nt_asm_ext_1401637d0\n\t"
        "int3\n\t"
        ".att_syntax prefix"
    );
}
