/*
 * Reconstructed Windows NT Kernel Subsystem: Kernel System Service Dispatchers & Native API (Zw/Nt)
 * Source Binary : ntoskrnl.exe (10.0.16299.15 x64, RSDS 83DB4240-4EFD-4AB6-AFB6-FA864B700CB3)
 * Functions     : 48 Z3-verified / assembly-preserved routines
 */
#include "../../include/nt_reconstructed_types.h"

extern unsigned char __nt_asm_sym_1403f5530[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] NtCommitComplete @ 0x140142360
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b31ca]' at 0x140142360
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) NtCommitComplete(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5530]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5538[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] NtCommitEnlistment @ 0x140142370
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b31c2]' at 0x140142370
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) NtCommitEnlistment(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5538]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5540[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] NtCommitTransaction @ 0x140142380
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b31ba]' at 0x140142380
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) NtCommitTransaction(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5540]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5548[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] NtCreateEnlistment @ 0x140142390
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b31b2]' at 0x140142390
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) NtCreateEnlistment(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5548]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f55d8[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] NtCreateResourceManager @ 0x1401423a0
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b3232]' at 0x1401423a0
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) NtCreateResourceManager(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f55d8]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f55e0[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] NtCreateTransaction @ 0x1401423b0
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b322a]' at 0x1401423b0
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) NtCreateTransaction(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f55e0]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f55e8[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] NtCreateTransactionManager @ 0x1401423c0
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b3222]' at 0x1401423c0
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) NtCreateTransactionManager(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f55e8]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f55f0[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] NtEnumerateTransactionObject @ 0x1401423d0
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b321a]' at 0x1401423d0
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) NtEnumerateTransactionObject(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f55f0]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f55f8[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] NtFreezeTransactions @ 0x1401423e0
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b3212]' at 0x1401423e0
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) NtFreezeTransactions(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f55f8]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5600[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] NtGetNotificationResourceManager @ 0x1401423f0
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b320a]' at 0x1401423f0
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) NtGetNotificationResourceManager(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5600]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5618[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] NtOpenEnlistment @ 0x140142400
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b3212]' at 0x140142400
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) NtOpenEnlistment(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5618]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5620[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] NtOpenResourceManager @ 0x140142410
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b320a]' at 0x140142410
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) NtOpenResourceManager(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5620]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5628[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] NtOpenTransaction @ 0x140142420
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b3202]' at 0x140142420
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) NtOpenTransaction(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5628]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5630[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] NtOpenTransactionManager @ 0x140142430
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b31fa]' at 0x140142430
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) NtOpenTransactionManager(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5630]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5698[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] NtPrePrepareComplete @ 0x140142440
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b3252]' at 0x140142440
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) NtPrePrepareComplete(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5698]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5690[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] NtPrePrepareEnlistment @ 0x140142450
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b323a]' at 0x140142450
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) NtPrePrepareEnlistment(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5690]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5688[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] NtPrepareComplete @ 0x140142460
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b3222]' at 0x140142460
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) NtPrepareComplete(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5688]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5680[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] NtPrepareEnlistment @ 0x140142470
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b320a]' at 0x140142470
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) NtPrepareEnlistment(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5680]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5678[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] NtPropagationComplete @ 0x140142480
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b31f2]' at 0x140142480
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) NtPropagationComplete(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5678]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5670[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] NtPropagationFailed @ 0x140142490
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b31da]' at 0x140142490
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) NtPropagationFailed(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5670]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5668[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] NtQueryInformationEnlistment @ 0x1401424a0
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b31c2]' at 0x1401424a0
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) NtQueryInformationEnlistment(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5668]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5660[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] NtQueryInformationResourceManager @ 0x1401424b0
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b31aa]' at 0x1401424b0
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) NtQueryInformationResourceManager(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5660]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5658[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] NtQueryInformationTransaction @ 0x1401424c0
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b3192]' at 0x1401424c0
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) NtQueryInformationTransaction(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5658]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5650[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] NtQueryInformationTransactionManager @ 0x1401424d0
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b317a]' at 0x1401424d0
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) NtQueryInformationTransactionManager(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5650]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5648[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] NtReadOnlyEnlistment @ 0x1401424e0
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b3162]' at 0x1401424e0
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) NtReadOnlyEnlistment(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5648]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5518[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] NtRecoverEnlistment @ 0x1401424f0
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b3022]' at 0x1401424f0
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) NtRecoverEnlistment(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5518]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5510[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] NtRecoverResourceManager @ 0x140142500
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b300a]' at 0x140142500
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) NtRecoverResourceManager(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5510]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5508[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] NtRecoverTransactionManager @ 0x140142510
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b2ff2]' at 0x140142510
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) NtRecoverTransactionManager(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5508]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5500[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] NtRollbackComplete @ 0x140142520
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b2fda]' at 0x140142520
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) NtRollbackComplete(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5500]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f54f8[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] NtRollbackEnlistment @ 0x140142530
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b2fc2]' at 0x140142530
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) NtRollbackEnlistment(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f54f8]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f5640[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] NtRollbackTransaction @ 0x140142540
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b30fa]' at 0x140142540
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) NtRollbackTransaction(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f5640]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f54e8[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] NtSetInformationEnlistment @ 0x140142550
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b2f92]' at 0x140142550
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) NtSetInformationEnlistment(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f54e8]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f54e0[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] NtSetInformationResourceManager @ 0x140142560
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b2f7a]' at 0x140142560
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) NtSetInformationResourceManager(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f54e0]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f54d8[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] NtSetInformationTransaction @ 0x140142570
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b2f62]' at 0x140142570
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) NtSetInformationTransaction(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f54d8]\n\t"
        ".att_syntax prefix"
    );
}

extern unsigned char __nt_asm_sym_1403f54d0[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] NtThawTransactions @ 0x140142580
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Indirect dispatch / IAT tail-jump 'jmp qword ptr [rip + 0x2b2f4a]' at 0x140142580
 *   - Privileged/control-register routine (1/1 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) NtThawTransactions(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "jmp qword ptr [rip + __nt_asm_sym_1403f54d0]\n\t"
        ".att_syntax prefix"
    );
}

/*
 * Function : NtVdmControl @ 0x14058c9c0 (6 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (2.93 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
NTSTATUS __attribute__((ms_abi)) NtVdmControl(void) {
    return (NTSTATUS)(0xc0000002ULL);
}

/*
 * Function : NtOpenProcessToken @ 0x14051ca10 (20 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.48 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) NtOpenProcessToken(uint64_t arg1, uint64_t arg2, uint64_t arg3) {
    uint64_t __attribute__((unused)) ssa_call_res = NtOpenProcessTokenEx(((uint64_t)arg1), ((uint64_t)arg2), 0ULL, ((uint64_t)arg3));
    return (uint64_t)(ssa_call_res);
}

/*
 * Function : NtOpenThreadToken @ 0x14051c270 (22 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.04 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) NtOpenThreadToken(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4) {
    uint64_t __attribute__((unused)) ssa_call_res = NtOpenThreadTokenEx(((uint64_t)arg1), ((uint64_t)arg2), ((uint64_t)arg3), 0ULL);
    return (uint64_t)(ssa_call_res);
}

extern void __nt_asm_ext_14016e900(void);
extern unsigned char __nt_asm_sym_140166a60[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] ZwClose @ 0x140160000
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Privileged CPU/CSR instruction 'cli' at 0x140160003
 *   - Hand-coded x86-64 kernel assembly instruction 'pushfq' at 0x140160009
 *   Clobbered State   : rax, memory, cc
 */
uint64_t __attribute__((naked, ms_abi)) ZwClose(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov rax, rsp\n\t"
        "cli\n\t"
        "sub rsp, 0x10\n\t"
        "push rax\n\t"
        "pushfq\n\t"
        "push 0x10\n\t"
        "lea rax, [rip + __nt_asm_sym_140166a60]\n\t"
        "push rax\n\t"
        "mov eax, 0xf\n\t"
        "jmp __nt_asm_ext_14016e900\n\t"
        "ret\n\t"
        ".att_syntax prefix"
    );
}

extern void __nt_asm_ext_14016e900(void);
extern unsigned char __nt_asm_sym_140166a60[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] ZwOpenKey @ 0x140160060
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Privileged CPU/CSR instruction 'cli' at 0x140160063
 *   - Hand-coded x86-64 kernel assembly instruction 'pushfq' at 0x140160069
 *   Clobbered State   : rax, memory, cc
 */
uint64_t __attribute__((naked, ms_abi)) ZwOpenKey(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov rax, rsp\n\t"
        "cli\n\t"
        "sub rsp, 0x10\n\t"
        "push rax\n\t"
        "pushfq\n\t"
        "push 0x10\n\t"
        "lea rax, [rip + __nt_asm_sym_140166a60]\n\t"
        "push rax\n\t"
        "mov eax, 0x12\n\t"
        "jmp __nt_asm_ext_14016e900\n\t"
        "ret\n\t"
        ".att_syntax prefix"
    );
}

extern void __nt_asm_ext_14016e900(void);
extern unsigned char __nt_asm_sym_140166a60[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] ZwQueryValueKey @ 0x140160100
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Privileged CPU/CSR instruction 'cli' at 0x140160103
 *   - Hand-coded x86-64 kernel assembly instruction 'pushfq' at 0x140160109
 *   Clobbered State   : rax, memory, cc
 */
uint64_t __attribute__((naked, ms_abi)) ZwQueryValueKey(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov rax, rsp\n\t"
        "cli\n\t"
        "sub rsp, 0x10\n\t"
        "push rax\n\t"
        "pushfq\n\t"
        "push 0x10\n\t"
        "lea rax, [rip + __nt_asm_sym_140166a60]\n\t"
        "push rax\n\t"
        "mov eax, 0x17\n\t"
        "jmp __nt_asm_ext_14016e900\n\t"
        "ret\n\t"
        ".att_syntax prefix"
    );
}

extern void __nt_asm_ext_14016e900(void);
extern unsigned char __nt_asm_sym_140166a60[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] ZwCreateFile @ 0x1401608c0
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Privileged CPU/CSR instruction 'cli' at 0x1401608c3
 *   - Hand-coded x86-64 kernel assembly instruction 'pushfq' at 0x1401608c9
 *   Clobbered State   : rax, memory, cc
 */
uint64_t __attribute__((naked, ms_abi)) ZwCreateFile(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov rax, rsp\n\t"
        "cli\n\t"
        "sub rsp, 0x10\n\t"
        "push rax\n\t"
        "pushfq\n\t"
        "push 0x10\n\t"
        "lea rax, [rip + __nt_asm_sym_140166a60]\n\t"
        "push rax\n\t"
        "mov eax, 0x55\n\t"
        "jmp __nt_asm_ext_14016e900\n\t"
        "ret\n\t"
        ".att_syntax prefix"
    );
}

extern void __nt_asm_ext_14016e900(void);
extern unsigned char __nt_asm_sym_140166a60[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] ZwReadFile @ 0x14015fee0
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Privileged CPU/CSR instruction 'cli' at 0x14015fee3
 *   - Hand-coded x86-64 kernel assembly instruction 'pushfq' at 0x14015fee9
 *   Clobbered State   : rax, memory, cc
 */
uint64_t __attribute__((naked, ms_abi)) ZwReadFile(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov rax, rsp\n\t"
        "cli\n\t"
        "sub rsp, 0x10\n\t"
        "push rax\n\t"
        "pushfq\n\t"
        "push 0x10\n\t"
        "lea rax, [rip + __nt_asm_sym_140166a60]\n\t"
        "push rax\n\t"
        "mov eax, 6\n\t"
        "jmp __nt_asm_ext_14016e900\n\t"
        "ret\n\t"
        ".att_syntax prefix"
    );
}

extern void __nt_asm_ext_14016e900(void);
extern unsigned char __nt_asm_sym_140166a60[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] ZwWriteFile @ 0x14015ff20
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Privileged CPU/CSR instruction 'cli' at 0x14015ff23
 *   - Hand-coded x86-64 kernel assembly instruction 'pushfq' at 0x14015ff29
 *   Clobbered State   : rax, memory, cc
 */
uint64_t __attribute__((naked, ms_abi)) ZwWriteFile(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov rax, rsp\n\t"
        "cli\n\t"
        "sub rsp, 0x10\n\t"
        "push rax\n\t"
        "pushfq\n\t"
        "push 0x10\n\t"
        "lea rax, [rip + __nt_asm_sym_140166a60]\n\t"
        "push rax\n\t"
        "mov eax, 8\n\t"
        "jmp __nt_asm_ext_14016e900\n\t"
        "ret\n\t"
        ".att_syntax prefix"
    );
}

extern void __nt_asm_ext_14016e900(void);
extern unsigned char __nt_asm_sym_140166a60[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] ZwAllocateVirtualMemory @ 0x140160120
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Privileged CPU/CSR instruction 'cli' at 0x140160123
 *   - Hand-coded x86-64 kernel assembly instruction 'pushfq' at 0x140160129
 *   Clobbered State   : rax, memory, cc
 */
uint64_t __attribute__((naked, ms_abi)) ZwAllocateVirtualMemory(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov rax, rsp\n\t"
        "cli\n\t"
        "sub rsp, 0x10\n\t"
        "push rax\n\t"
        "pushfq\n\t"
        "push 0x10\n\t"
        "lea rax, [rip + __nt_asm_sym_140166a60]\n\t"
        "push rax\n\t"
        "mov eax, 0x18\n\t"
        "jmp __nt_asm_ext_14016e900\n\t"
        "ret\n\t"
        ".att_syntax prefix"
    );
}

extern void __nt_asm_ext_14016e900(void);
extern unsigned char __nt_asm_sym_140166a60[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] ZwFreeVirtualMemory @ 0x1401601e0
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Privileged CPU/CSR instruction 'cli' at 0x1401601e3
 *   - Hand-coded x86-64 kernel assembly instruction 'pushfq' at 0x1401601e9
 *   Clobbered State   : rax, memory, cc
 */
uint64_t __attribute__((naked, ms_abi)) ZwFreeVirtualMemory(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov rax, rsp\n\t"
        "cli\n\t"
        "sub rsp, 0x10\n\t"
        "push rax\n\t"
        "pushfq\n\t"
        "push 0x10\n\t"
        "lea rax, [rip + __nt_asm_sym_140166a60]\n\t"
        "push rax\n\t"
        "mov eax, 0x1e\n\t"
        "jmp __nt_asm_ext_14016e900\n\t"
        "ret\n\t"
        ".att_syntax prefix"
    );
}

extern void __nt_asm_ext_14016e900(void);
extern unsigned char __nt_asm_sym_140166a60[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] ZwQuerySystemInformation @ 0x1401604e0
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Privileged CPU/CSR instruction 'cli' at 0x1401604e3
 *   - Hand-coded x86-64 kernel assembly instruction 'pushfq' at 0x1401604e9
 *   Clobbered State   : rax, memory, cc
 */
uint64_t __attribute__((naked, ms_abi)) ZwQuerySystemInformation(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov rax, rsp\n\t"
        "cli\n\t"
        "sub rsp, 0x10\n\t"
        "push rax\n\t"
        "pushfq\n\t"
        "push 0x10\n\t"
        "lea rax, [rip + __nt_asm_sym_140166a60]\n\t"
        "push rax\n\t"
        "mov eax, 0x36\n\t"
        "jmp __nt_asm_ext_14016e900\n\t"
        "ret\n\t"
        ".att_syntax prefix"
    );
}

extern void __nt_asm_ext_14016e900(void);
extern unsigned char __nt_asm_sym_140166a60[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] ZwWaitForSingleObject @ 0x14015fea0
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Privileged CPU/CSR instruction 'cli' at 0x14015fea3
 *   - Hand-coded x86-64 kernel assembly instruction 'pushfq' at 0x14015fea9
 *   Clobbered State   : rax, memory, cc
 */
uint64_t __attribute__((naked, ms_abi)) ZwWaitForSingleObject(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov rax, rsp\n\t"
        "cli\n\t"
        "sub rsp, 0x10\n\t"
        "push rax\n\t"
        "pushfq\n\t"
        "push 0x10\n\t"
        "lea rax, [rip + __nt_asm_sym_140166a60]\n\t"
        "push rax\n\t"
        "mov eax, 4\n\t"
        "jmp __nt_asm_ext_14016e900\n\t"
        "ret\n\t"
        ".att_syntax prefix"
    );
}
