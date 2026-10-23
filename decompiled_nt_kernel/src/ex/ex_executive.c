/*
 * Reconstructed Windows NT Kernel Subsystem: Executive Lock-Free SLists & Rundown Protection (Ex/Exp)
 * Source Binary : ntoskrnl.exe (10.0.16299.15 x64, RSDS 83DB4240-4EFD-4AB6-AFB6-FA864B700CB3)
 * Functions     : 5 Z3-verified / assembly-preserved routines
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
 * Z3 Proof : VERIFIED_EQUIVALENT (3.02 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint16_t __attribute__((ms_abi)) ExQueryDepthSList(union _SLIST_HEADER* SListHead) {
    uint16_t __attribute__((unused)) ssa_mem_ld = (uint16_t)((*(uint16_t*)(uintptr_t)(((uint64_t)(uintptr_t)SListHead))));
    return (uint16_t)(((uint64_t)(uint16_t)(ssa_mem_ld)));
}

/*
 * Function : ExInitializeRundownProtection @ 0x140038820 (5 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.97 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) ExInitializeRundownProtection(struct _EX_RUNDOWN_REF* RunRef) {
    RunRef->Count = (uint64_t)(0ULL);
    return;
}

/*
 * Function : ExReInitializeRundownProtection @ 0x140141d80 (6 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.30 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) ExReInitializeRundownProtection(struct _EX_RUNDOWN_REF* RunRef) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)(RunRef->Count);
    RunRef->Count = (uint64_t)(0ULL);
    return;
}
