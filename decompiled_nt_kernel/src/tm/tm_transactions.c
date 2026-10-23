/*
 * Reconstructed Windows NT Kernel Subsystem: Kernel Transaction Manager & Enlistments (Tm/Tmp)
 * Source Binary : ntoskrnl.exe (10.0.16299.15 x64, RSDS 83DB4240-4EFD-4AB6-AFB6-FA864B700CB3)
 * Functions     : 35 Z3-verified / assembly-preserved routines
 */
#include "../../include/nt_reconstructed_types.h"

extern unsigned char __nt_asm_sym_1403f54c8[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] TmCancelPropagationRequest @ 0x140142590
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b2f32]' at 0x140142590
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) TmCancelPropagationRequest(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f54c8]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f54c0[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] TmCommitComplete @ 0x1401425a0
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b2f1a]' at 0x1401425a0
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) TmCommitComplete(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f54c0]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f54b8[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] TmCommitEnlistment @ 0x1401425b0
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b2f02]' at 0x1401425b0
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) TmCommitEnlistment(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f54b8]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f54b0[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] TmCommitTransaction @ 0x1401425c0
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b2eea]' at 0x1401425c0
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) TmCommitTransaction(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f54b0]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f54a8[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] TmCreateEnlistment @ 0x1401425d0
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b2ed2]' at 0x1401425d0
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) TmCreateEnlistment(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f54a8]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f54a0[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] TmCurrentTransaction @ 0x1401425e0
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b2eba]' at 0x1401425e0
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) TmCurrentTransaction(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f54a0]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5498[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] TmDereferenceEnlistmentKey @ 0x1401425f0
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b2ea2]' at 0x1401425f0
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) TmDereferenceEnlistmentKey(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5498]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5490[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] TmEnableCallbacks @ 0x140142600
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b2e8a]' at 0x140142600
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) TmEnableCallbacks(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5490]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5488[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] TmEndPropagationRequest @ 0x140142610
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b2e72]' at 0x140142610
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) TmEndPropagationRequest(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5488]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5480[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] TmFreezeTransactions @ 0x140142620
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b2e5a]' at 0x140142620
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) TmFreezeTransactions(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5480]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5478[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] TmGetTransactionId @ 0x140142630
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b2e42]' at 0x140142630
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) TmGetTransactionId(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5478]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5528[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] TmInitSystem @ 0x140142350
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b31d2]' at 0x140142350
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) TmInitSystem(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5528]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5520[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] TmInitSystemPhase2 @ 0x140142340
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b31da]' at 0x140142340
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) TmInitSystemPhase2(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5520]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5470[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] TmInitializeTransactionManager @ 0x140142640
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b2e2a]' at 0x140142640
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) TmInitializeTransactionManager(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5470]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5468[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] TmIsKTMCommitCoordinator @ 0x140142650
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b2e12]' at 0x140142650
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) TmIsKTMCommitCoordinator(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5468]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5460[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] TmIsTransactionActive @ 0x140142660
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b2dfa]' at 0x140142660
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) TmIsTransactionActive(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5460]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5458[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] TmPrePrepareComplete @ 0x140142670
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b2de2]' at 0x140142670
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) TmPrePrepareComplete(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5458]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5450[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] TmPrePrepareEnlistment @ 0x140142680
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b2dca]' at 0x140142680
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) TmPrePrepareEnlistment(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5450]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5448[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] TmPrepareComplete @ 0x140142690
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b2db2]' at 0x140142690
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) TmPrepareComplete(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5448]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5440[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] TmPrepareEnlistment @ 0x1401426a0
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b2d9a]' at 0x1401426a0
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) TmPrepareEnlistment(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5440]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f54f0[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] TmPropagationComplete @ 0x1401426b0
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b2e3a]' at 0x1401426b0
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) TmPropagationComplete(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f54f0]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5550[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] TmPropagationFailed @ 0x1401426c0
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b2e8a]' at 0x1401426c0
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) TmPropagationFailed(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5550]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5558[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] TmReadOnlyEnlistment @ 0x1401426d0
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b2e82]' at 0x1401426d0
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) TmReadOnlyEnlistment(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5558]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5560[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] TmRecoverEnlistment @ 0x1401426e0
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b2e7a]' at 0x1401426e0
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) TmRecoverEnlistment(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5560]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5568[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] TmRecoverResourceManager @ 0x1401426f0
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b2e72]' at 0x1401426f0
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) TmRecoverResourceManager(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5568]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5570[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] TmRecoverTransactionManager @ 0x140142700
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b2e6a]' at 0x140142700
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) TmRecoverTransactionManager(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5570]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5578[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] TmReferenceEnlistmentKey @ 0x140142710
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b2e62]' at 0x140142710
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) TmReferenceEnlistmentKey(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5578]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5580[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] TmRenameTransactionManager @ 0x140142720
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b2e5a]' at 0x140142720
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) TmRenameTransactionManager(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5580]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5588[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] TmRequestOutcomeEnlistment @ 0x140142730
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b2e52]' at 0x140142730
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) TmRequestOutcomeEnlistment(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5588]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5590[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] TmRollbackComplete @ 0x140142740
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b2e4a]' at 0x140142740
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) TmRollbackComplete(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5590]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5598[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] TmRollbackEnlistment @ 0x140142750
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b2e42]' at 0x140142750
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) TmRollbackEnlistment(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5598]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f55a0[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] TmRollbackTransaction @ 0x140142760
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b2e3a]' at 0x140142760
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) TmRollbackTransaction(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f55a0]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f55a8[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] TmSetCurrentTransaction @ 0x140142770
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b2e32]' at 0x140142770
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) TmSetCurrentTransaction(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f55a8]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f55b0[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] TmSinglePhaseReject @ 0x140142780
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b2e2a]' at 0x140142780
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) TmSinglePhaseReject(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f55b0]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f55b8[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] TmThawTransactions @ 0x140142790
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b2e22]' at 0x140142790
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) TmThawTransactions(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f55b8]\n\t"
        ".att_syntax prefix"
    );
}
