/*
 * Reconstructed Windows NT Kernel Subsystem: Kernel Core Dispatcher, IRQL, DPC & Synchronization (Ke/Ki/Kz)
 * Source Binary : ntoskrnl.exe (10.0.16299.15 x64, RSDS 83DB4240-4EFD-4AB6-AFB6-FA864B700CB3)
 * Functions     : 18 Z3-verified / assembly-preserved routines
 */
#include "../../include/nt_reconstructed_types.h"

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] KeGetCurrentIrql @ 0x1400e9b50
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Privileged CPU/CSR instruction 'mov rax, cr8' at 0x1400e9b50
 *   - Privileged/control-register routine (1/2 insns) preserved as native x86-64 assembly
 *   Clobbered State   : rax, memory, cc
 */
KIRQL __attribute__((naked, ms_abi)) KeGetCurrentIrql(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov rax, cr8\n\t"
        "ret\n\t"
        ".att_syntax prefix"
    );
}

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] KzRaiseIrql @ 0x1400e9b30
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Privileged CPU/CSR instruction 'mov rax, cr8' at 0x1400e9b30
 *   - Privileged CPU/CSR instruction 'mov cr8, rcx' at 0x1400e9b37
 *   - Privileged/control-register routine (2/4 insns) preserved as native x86-64 assembly
 *   Live-In Registers : rcx
 *   Clobbered State   : rax, rcx, memory, cc
 */
KIRQL __attribute__((naked, ms_abi)) KzRaiseIrql(KIRQL NewIrql) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov rax, cr8\n\t"
        "movzx ecx, cl\n\t"
        "mov cr8, rcx\n\t"
        "ret\n\t"
        ".att_syntax prefix"
    );
}

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] KzLowerIrql @ 0x1400ed1e0
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Privileged CPU/CSR instruction 'mov cr8, rax' at 0x1400ed1e3
 *   - Privileged/control-register routine (1/3 insns) preserved as native x86-64 assembly
 *   Live-In Registers : rcx
 *   Clobbered State   : rax, memory, cc
 */
void __attribute__((naked, ms_abi)) KzLowerIrql(KIRQL NewIrql) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "movzx eax, cl\n\t"
        "mov cr8, rax\n\t"
        "ret\n\t"
        ".att_syntax prefix"
    );
}

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] KeRaiseIrqlToDpcLevel @ 0x1400e9b10
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Privileged CPU/CSR instruction 'mov rax, cr8' at 0x1400e9b10
 *   - Privileged CPU/CSR instruction 'mov cr8, rcx' at 0x1400e9b19
 *   - Privileged/control-register routine (2/4 insns) preserved as native x86-64 assembly
 *   Clobbered State   : rax, rcx, memory, cc
 */
KIRQL __attribute__((naked, ms_abi)) KeRaiseIrqlToDpcLevel(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov rax, cr8\n\t"
        "mov ecx, 2\n\t"
        "mov cr8, rcx\n\t"
        "ret\n\t"
        ".att_syntax prefix"
    );
}

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] KiFlushCurrentTbWorker @ 0x1400769f0
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Privileged CPU/CSR instruction 'mov rcx, cr4' at 0x1400769f0
 *   - Privileged CPU/CSR instruction 'mov cr4, rax' at 0x1400769ff
 *   - Privileged CPU/CSR instruction 'mov cr4, rcx' at 0x140076a02
 *   - Privileged CPU/CSR instruction 'mov rax, cr3' at 0x140076a06
 *   - Privileged CPU/CSR instruction 'mov cr3, rax' at 0x140076a09
 *   - Privileged/control-register routine (5/11 insns) preserved as native x86-64 assembly
 *   Clobbered State   : rcx, rax, memory, cc
 */
uint64_t __attribute__((naked, ms_abi)) KiFlushCurrentTbWorker(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "mov rcx, cr4\n\t"
        "test cl, cl\n\t"
        "jns .L_KiFlushCurrentTbWorker_140076a06\n\t"
        ".L_KiFlushCurrentTbWorker_1400769f7:\n\t"
        "mov rax, rcx\n\t"
        "btr rax, 7\n\t"
        "mov cr4, rax\n\t"
        "mov cr4, rcx\n\t"
        "ret\n\t"
        ".L_KiFlushCurrentTbWorker_140076a06:\n\t"
        "mov rax, cr3\n\t"
        "mov cr3, rax\n\t"
        "ret\n\t"
        ".att_syntax prefix"
    );
}

__attribute__((weak, naked)) void __nt_asm_ext_140163d20(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_140710720(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_14016fa40(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_140513cd0(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_140166eb0(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_140164530(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_140201a60(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_14016fcc0(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_140163ca0(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_140166a40(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_14016f080(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_140275690(void) { __asm__ __volatile__ ("ret"); }
__attribute__((weak, naked)) void __nt_asm_ext_140275710(void) { __asm__ __volatile__ ("ret"); }

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] KiSystemCall64 @ 0x14016e980
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Non-C kernel entry/exit instruction 'swapgs' at 0x14016e980
 *   - Non-ABI kernel stack pivot 'mov rsp, qword ptr gs:[0x1a8]' at 0x14016e98c
 *   - Privileged CPU/CSR instruction 'stmxcsr dword ptr [rbp - 0x54]' at 0x14016e9e4
 *   - Privileged CPU/CSR instruction 'ldmxcsr dword ptr gs:[0x180]' at 0x14016e9e8
 *   - Privileged CPU/CSR instruction 'sti' at 0x14016ea49
 *   - Privileged CPU/CSR instruction 'rdmsr' at 0x14016ea62
 *   - Privileged CPU/CSR instruction 'sti' at 0x14016ea8e
 *   - Privileged CPU/CSR instruction 'sti' at 0x14016eab5
 *   - Privileged CPU/CSR instruction 'mov rcx, cr8' at 0x14016ec86
 *   - Privileged CPU/CSR instruction 'cli' at 0x14016ec9e
 *   - Privileged CPU/CSR instruction 'mov cr8, rcx' at 0x14016ecf0
 *   - Privileged CPU/CSR instruction 'sti' at 0x14016ecf4
 *   - Privileged CPU/CSR instruction 'cli' at 0x14016ecfa
 *   - Privileged CPU/CSR instruction 'mov cr8, rcx' at 0x14016ed00
 *   - Privileged CPU/CSR instruction 'ldmxcsr dword ptr [rbp - 0x54]' at 0x14016ed96
 *   - Non-ABI kernel stack pivot 'mov rsp, r8' at 0x14016ee21
 *   - Non-C kernel entry/exit instruction 'swapgs' at 0x14016ee24
 *   - Non-C kernel entry/exit instruction 'sysretq' at 0x14016ee27
 *   - Privileged CPU/CSR instruction 'cli' at 0x14016ee42
 *   - Non-ABI kernel stack pivot 'mov rsp, qword ptr [rsp + 0x100]' at 0x14016ee4d
 *   - Privileged CPU/CSR instruction 'sti' at 0x14016ee55
 *   - Privileged CPU/CSR instruction 'mov rcx, cr8' at 0x14016ee60
 *   - Privileged CPU/CSR instruction 'cli' at 0x14016ee78
 *   - Privileged CPU/CSR instruction 'mov cr8, rcx' at 0x14016ee94
 *   - Privileged CPU/CSR instruction 'sti' at 0x14016ee98
 *   - Privileged CPU/CSR instruction 'mov cr8, rcx' at 0x14016eea3
 *   - Privileged CPU/CSR instruction 'cli' at 0x14016eea7
 *   - Privileged CPU/CSR instruction 'ldmxcsr dword ptr [rbp - 0x54]' at 0x14016eeea
 *   - Non-C kernel entry/exit instruction 'swapgs' at 0x14016ef48
 *   - Non-C kernel entry/exit instruction 'iretq' at 0x14016ef4b
 *   - Privileged CPU/CSR instruction 'mov r8, cr8' at 0x14016efbf
 *   - Privileged/control-register routine (31/385 insns) preserved as native x86-64 assembly
 *   Live-In Registers : r11, rcx, r10, rbp, rbx, rdi, rsi, rax, rdx, r8, r9
 *   Clobbered State   : r11, rcx, rbp, rbx, rax, rdx, r8, r9, rdi, r10, rsi, memory, cc
 */
__attribute__((naked)) void KiSystemCall64(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "swapgs\n\t"
        "mov qword ptr gs:[0x10], rsp\n\t"
        "mov rsp, qword ptr gs:[0x1a8]\n\t"
        "push 0x2b\n\t"
        "push qword ptr gs:[0x10]\n\t"
        "push r11\n\t"
        "push 0x33\n\t"
        "push rcx\n\t"
        "mov rcx, r10\n\t"
        "sub rsp, 8\n\t"
        "push rbp\n\t"
        "sub rsp, 0x158\n\t"
        "lea rbp, [rsp + 0x80]\n\t"
        "mov qword ptr [rbp + 0xc0], rbx\n\t"
        "mov qword ptr [rbp + 0xc8], rdi\n\t"
        "mov qword ptr [rbp + 0xd0], rsi\n\t"
        "mov byte ptr [rbp - 0x55], 2\n\t"
        "mov rbx, qword ptr gs:[0x188]\n\t"
        "prefetchw byte ptr [rbx + 0x90]\n\t"
        "stmxcsr dword ptr [rbp - 0x54]\n\t"
        "ldmxcsr dword ptr gs:[0x180]\n\t"
        "cmp byte ptr [rbx + 3], 0\n\t"
        "mov word ptr [rbp + 0x80], 0\n\t"
        "je .L_KiSystemCall64_14016eab5\n\t"
        ".L_KiSystemCall64_14016ea04:\n\t"
        "mov qword ptr [rbp - 0x50], rax\n\t"
        "mov qword ptr [rbp - 0x48], rcx\n\t"
        "mov qword ptr [rbp - 0x40], rdx\n\t"
        "test byte ptr [rbx + 3], 3\n\t"
        "mov qword ptr [rbp - 0x38], r8\n\t"
        "mov qword ptr [rbp - 0x30], r9\n\t"
        "je .L_KiSystemCall64_14016ea23\n\t"
        ".L_KiSystemCall64_14016ea1e:\n\t"
        "call __nt_asm_ext_140163d20\n\t"
        ".L_KiSystemCall64_14016ea23:\n\t"
        "test byte ptr [rbx + 3], 4\n\t"
        "je .L_KiSystemCall64_14016ea57\n\t"
        ".L_KiSystemCall64_14016ea29:\n\t"
        "mov qword ptr [rbp - 0x20], r10\n\t"
        "mov qword ptr [rbp - 0x28], r10\n\t"
        "movaps xmmword ptr [rbp - 0x10], xmm0\n\t"
        "movaps xmmword ptr [rbp], xmm1\n\t"
        "movaps xmmword ptr [rbp + 0x10], xmm2\n\t"
        "movaps xmmword ptr [rbp + 0x20], xmm3\n\t"
        "movaps xmmword ptr [rbp + 0x30], xmm4\n\t"
        "movaps xmmword ptr [rbp + 0x40], xmm5\n\t"
        "sti\n\t"
        "mov rcx, rsp\n\t"
        "call __nt_asm_ext_140710720\n\t"
        "jmp .L_KiSystemCall64_14016ee57\n\t"
        ".L_KiSystemCall64_14016ea57:\n\t"
        "test byte ptr [rbx + 3], 0x80\n\t"
        "je .L_KiSystemCall64_14016ea96\n\t"
        ".L_KiSystemCall64_14016ea5d:\n\t"
        "mov ecx, 0xc0000102\n\t"
        "rdmsr\n\t"
        "shl rdx, 0x20\n\t"
        "or rax, rdx\n\t"
        "cmp qword ptr [rbx + 0xf0], rax\n\t"
        "je .L_KiSystemCall64_14016ea96\n\t"
        ".L_KiSystemCall64_14016ea74:\n\t"
        "mov rdx, qword ptr [rbx + 0x1f0]\n\t"
        "bts dword ptr [rbx + 0x74], 8\n\t"
        "dec word ptr [rbx + 0x1e6]\n\t"
        "mov qword ptr [rdx + 0x80], rax\n\t"
        "sti\n\t"
        "call __nt_asm_ext_14016fa40\n\t"
        "jmp .L_KiSystemCall64_14016eaa1\n\t"
        ".L_KiSystemCall64_14016ea96:\n\t"
        "test byte ptr [rbx + 3], 0x40\n\t"
        "je .L_KiSystemCall64_14016eaa1\n\t"
        ".L_KiSystemCall64_14016ea9c:\n\t"
        "bts dword ptr [rbx + 0x74], 0x10\n\t"
        ".L_KiSystemCall64_14016eaa1:\n\t"
        "mov rax, qword ptr [rbp - 0x50]\n\t"
        "mov rcx, qword ptr [rbp - 0x48]\n\t"
        "mov rdx, qword ptr [rbp - 0x40]\n\t"
        "mov r8, qword ptr [rbp - 0x38]\n\t"
        "mov r9, qword ptr [rbp - 0x30]\n\t"
        ".L_KiSystemCall64_14016eab5:\n\t"
        "sti\n\t"
        "mov qword ptr [rbx + 0x88], rcx\n\t"
        "mov dword ptr [rbx + 0x80], eax\n\t"
        "nop word ptr [rax + rax]\n\t"
        "mov qword ptr [rbx + 0x90], rsp\n\t"
        "mov edi, eax\n\t"
        "shr edi, 7\n\t"
        "and edi, 0x20\n\t"
        "and eax, 0xfff\n\t"
        ".L_KiSystemCall64_14016eae4:\n\t"
        "lea r10, [rip + 0x2a1d95]\n\t"
        "lea r11, [rip + 0x28abce]\n\t"
        "test dword ptr [rbx + 0x78], 0x80\n\t"
        "je .L_KiSystemCall64_14016eb0e\n\t"
        ".L_KiSystemCall64_14016eafb:\n\t"
        "test dword ptr [rbx + 0x78], 0x200000\n\t"
        "je .L_KiSystemCall64_14016eb0b\n\t"
        ".L_KiSystemCall64_14016eb04:\n\t"
        "lea r11, [rip + 0x28abf5]\n\t"
        ".L_KiSystemCall64_14016eb0b:\n\t"
        "mov r10, r11\n\t"
        ".L_KiSystemCall64_14016eb0e:\n\t"
        "cmp eax, dword ptr [rdi + r10 + 0x10]\n\t"
        "jae .L_KiSystemCall64_14016ef4d\n\t"
        ".L_KiSystemCall64_14016eb19:\n\t"
        "mov r10, qword ptr [rdi + r10]\n\t"
        "movsxd r11, dword ptr [r10 + rax*4]\n\t"
        "mov rax, r11\n\t"
        "sar r11, 4\n\t"
        "add r10, r11\n\t"
        "cmp edi, 0x20\n\t"
        "jne .L_KiSystemCall64_14016eb80\n\t"
        ".L_KiSystemCall64_14016eb30:\n\t"
        "mov r11, qword ptr [rbx + 0xf0]\n\t"
        "cmp dword ptr [r11 + 0x1740], 0\n\t"
        "je .L_KiSystemCall64_14016eb80\n\t"
        ".L_KiSystemCall64_14016eb41:\n\t"
        "mov qword ptr [rbp - 0x50], rax\n\t"
        "mov qword ptr [rbp - 0x48], rcx\n\t"
        "mov qword ptr [rbp - 0x40], rdx\n\t"
        "mov rbx, r8\n\t"
        "mov rdi, r9\n\t"
        "mov rsi, r10\n\t"
        "mov ecx, 7\n\t"
        "xor edx, edx\n\t"
        "xor r8, r8\n\t"
        "xor r9, r9\n\t"
        "call __nt_asm_ext_140513cd0\n\t"
        "mov rax, qword ptr [rbp - 0x50]\n\t"
        "mov rcx, qword ptr [rbp - 0x48]\n\t"
        "mov rdx, qword ptr [rbp - 0x40]\n\t"
        "mov r8, rbx\n\t"
        "mov r9, rdi\n\t"
        "mov r10, rsi\n\t"
        "nop dword ptr [rax]\n\t"
        ".L_KiSystemCall64_14016eb80:\n\t"
        "and eax, 0xf\n\t"
        "je .L_KiSystemCall64_14016ec40\n\t"
        ".L_KiSystemCall64_14016eb89:\n\t"
        "shl eax, 3\n\t"
        "lea rsp, [rsp - 0x70]\n\t"
        "lea rdi, [rsp + 0x18]\n\t"
        "mov rsi, qword ptr [rbp + 0x100]\n\t"
        "lea rsi, [rsi + 0x20]\n\t"
        "test byte ptr [rbp + 0xf0], 1\n\t"
        "je .L_KiSystemCall64_14016ebc0\n\t"
        ".L_KiSystemCall64_14016ebaa:\n\t"
        "cmp rsi, qword ptr [rip + 0x15265f]\n\t"
        "cmovae rsi, qword ptr [rip + 0x152657]\n\t"
        "nop dword ptr [rax]\n\t"
        ".L_KiSystemCall64_14016ebc0:\n\t"
        "lea r11, [rip + 0x79]\n\t"
        "sub r11, rax\n\t"
        "jmp r11\n\t"
        "nop dword ptr [rax]\n\t"
        "mov rax, qword ptr [rsi + 0x70]\n\t"
        "mov qword ptr [rdi + 0x70], rax\n\t"
        "mov rax, qword ptr [rsi + 0x68]\n\t"
        "mov qword ptr [rdi + 0x68], rax\n\t"
        "mov rax, qword ptr [rsi + 0x60]\n\t"
        "mov qword ptr [rdi + 0x60], rax\n\t"
        "mov rax, qword ptr [rsi + 0x58]\n\t"
        "mov qword ptr [rdi + 0x58], rax\n\t"
        "mov rax, qword ptr [rsi + 0x50]\n\t"
        "mov qword ptr [rdi + 0x50], rax\n\t"
        "mov rax, qword ptr [rsi + 0x48]\n\t"
        "mov qword ptr [rdi + 0x48], rax\n\t"
        "mov rax, qword ptr [rsi + 0x40]\n\t"
        "mov qword ptr [rdi + 0x40], rax\n\t"
        "mov rax, qword ptr [rsi + 0x38]\n\t"
        "mov qword ptr [rdi + 0x38], rax\n\t"
        "mov rax, qword ptr [rsi + 0x30]\n\t"
        "mov qword ptr [rdi + 0x30], rax\n\t"
        "mov rax, qword ptr [rsi + 0x28]\n\t"
        "mov qword ptr [rdi + 0x28], rax\n\t"
        "mov rax, qword ptr [rsi + 0x20]\n\t"
        "mov qword ptr [rdi + 0x20], rax\n\t"
        "mov rax, qword ptr [rsi + 0x18]\n\t"
        "mov qword ptr [rdi + 0x18], rax\n\t"
        "mov rax, qword ptr [rsi + 0x10]\n\t"
        "mov qword ptr [rdi + 0x10], rax\n\t"
        "mov rax, qword ptr [rsi + 8]\n\t"
        "mov qword ptr [rdi + 8], rax\n\t"
        ".L_KiSystemCall64_14016ec40:\n\t"
        "test dword ptr [rip + 0x28a4fe], 0x40\n\t"
        "jne .L_KiSystemCall64_14016efeb\n\t"
        ".L_KiSystemCall64_14016ec50:\n\t"
        "call r10\n\t"
        ".L_KiSystemCall64_14016ec53:\n\t"
        "inc dword ptr gs:[0x2eb8]\n\t"
        ".L_KiSystemCall64_14016ec5b:\n\t"
        "mov rbx, qword ptr [rbp + 0xc0]\n\t"
        "mov rdi, qword ptr [rbp + 0xc8]\n\t"
        "mov rsi, qword ptr [rbp + 0xd0]\n\t"
        "mov r11, qword ptr gs:[0x188]\n\t"
        "test byte ptr [rbp + 0xf0], 1\n\t"
        "je .L_KiSystemCall64_14016ee2a\n\t"
        ".L_KiSystemCall64_14016ec86:\n\t"
        "mov rcx, cr8\n\t"
        "or cl, byte ptr [r11 + 0x24a]\n\t"
        "or ecx, dword ptr [r11 + 0x1e4]\n\t"
        "jne .L_KiSystemCall64_14016efb7\n\t"
        ".L_KiSystemCall64_14016ec9e:\n\t"
        "cli\n\t"
        "mov rcx, qword ptr gs:[0x188]\n\t"
        "cmp byte ptr [rcx + 0xc2], 0\n\t"
        "je .L_KiSystemCall64_14016ed08\n\t"
        ".L_KiSystemCall64_14016ecb1:\n\t"
        "mov qword ptr [rbp - 0x50], rax\n\t"
        "xor eax, eax\n\t"
        "mov qword ptr [rbp - 0x48], rax\n\t"
        "mov qword ptr [rbp - 0x40], rax\n\t"
        "mov qword ptr [rbp - 0x38], rax\n\t"
        "mov qword ptr [rbp - 0x30], rax\n\t"
        "mov qword ptr [rbp - 0x28], rax\n\t"
        "mov qword ptr [rbp - 0x20], rax\n\t"
        "pxor xmm0, xmm0\n\t"
        "movaps xmmword ptr [rbp - 0x10], xmm0\n\t"
        "movaps xmmword ptr [rbp], xmm0\n\t"
        "movaps xmmword ptr [rbp + 0x10], xmm0\n\t"
        "movaps xmmword ptr [rbp + 0x20], xmm0\n\t"
        "movaps xmmword ptr [rbp + 0x30], xmm0\n\t"
        "movaps xmmword ptr [rbp + 0x40], xmm0\n\t"
        "mov ecx, 1\n\t"
        "mov cr8, rcx\n\t"
        "sti\n\t"
        "call __nt_asm_ext_140166eb0\n\t"
        "cli\n\t"
        "mov ecx, 0\n\t"
        "mov cr8, rcx\n\t"
        "mov rax, qword ptr [rbp - 0x50]\n\t"
        ".L_KiSystemCall64_14016ed08:\n\t"
        "mov rcx, qword ptr gs:[0x188]\n\t"
        "test dword ptr [rcx], 0x8000000\n\t"
        "je .L_KiSystemCall64_14016ed58\n\t"
        ".L_KiSystemCall64_14016ed19:\n\t"
        "mov qword ptr [rbp - 0x50], rax\n\t"
        "xor eax, eax\n\t"
        "mov qword ptr [rbp - 0x48], rax\n\t"
        "mov qword ptr [rbp - 0x40], rax\n\t"
        "mov qword ptr [rbp - 0x38], rax\n\t"
        "mov qword ptr [rbp - 0x30], rax\n\t"
        "mov qword ptr [rbp - 0x28], rax\n\t"
        "mov qword ptr [rbp - 0x20], rax\n\t"
        "pxor xmm0, xmm0\n\t"
        "movaps xmmword ptr [rbp - 0x10], xmm0\n\t"
        "movaps xmmword ptr [rbp], xmm0\n\t"
        "movaps xmmword ptr [rbp + 0x10], xmm0\n\t"
        "movaps xmmword ptr [rbp + 0x20], xmm0\n\t"
        "movaps xmmword ptr [rbp + 0x30], xmm0\n\t"
        "movaps xmmword ptr [rbp + 0x40], xmm0\n\t"
        "call __nt_asm_ext_140164530\n\t"
        ".L_KiSystemCall64_14016ed58:\n\t"
        "mov rcx, qword ptr gs:[0x188]\n\t"
        "test dword ptr [rcx], 0x40010000\n\t"
        "je .L_KiSystemCall64_14016ed96\n\t"
        ".L_KiSystemCall64_14016ed69:\n\t"
        "mov qword ptr [rbp - 0x50], rax\n\t"
        "test byte ptr [rcx + 2], 1\n\t"
        "je .L_KiSystemCall64_14016ed81\n\t"
        ".L_KiSystemCall64_14016ed73:\n\t"
        "call __nt_asm_ext_140201a60\n\t"
        "mov rcx, qword ptr gs:[0x188]\n\t"
        ".L_KiSystemCall64_14016ed81:\n\t"
        "test byte ptr [rcx + 3], 0x40\n\t"
        "je .L_KiSystemCall64_14016ed92\n\t"
        ".L_KiSystemCall64_14016ed87:\n\t"
        "lea rsp, [rbp - 0x80]\n\t"
        "xor ecx, ecx\n\t"
        "call __nt_asm_ext_14016fcc0\n\t"
        ".L_KiSystemCall64_14016ed92:\n\t"
        "mov rax, qword ptr [rbp - 0x50]\n\t"
        ".L_KiSystemCall64_14016ed96:\n\t"
        "ldmxcsr dword ptr [rbp - 0x54]\n\t"
        "xor r10, r10\n\t"
        "cmp word ptr [rbp + 0x80], 0\n\t"
        "je .L_KiSystemCall64_14016ede8\n\t"
        ".L_KiSystemCall64_14016eda7:\n\t"
        "mov qword ptr [rbp - 0x50], rax\n\t"
        "call __nt_asm_ext_140163ca0\n\t"
        "mov rax, qword ptr gs:[0x188]\n\t"
        "mov rax, qword ptr [rax + 0xb8]\n\t"
        "mov rax, qword ptr [rax + 0x2c8]\n\t"
        "or rax, rax\n\t"
        "je .L_KiSystemCall64_14016ede4\n\t"
        ".L_KiSystemCall64_14016edcc:\n\t"
        "cmp word ptr [rbp + 0xf0], 0x33\n\t"
        "jne .L_KiSystemCall64_14016ede4\n\t"
        ".L_KiSystemCall64_14016edd6:\n\t"
        "mov r10, qword ptr [rbp + 0xe8]\n\t"
        "mov qword ptr [rbp + 0xe8], rax\n\t"
        ".L_KiSystemCall64_14016ede4:\n\t"
        "mov rax, qword ptr [rbp - 0x50]\n\t"
        ".L_KiSystemCall64_14016ede8:\n\t"
        "mov r8, qword ptr [rbp + 0x100]\n\t"
        "mov r9, qword ptr [rbp + 0xd8]\n\t"
        "xor edx, edx\n\t"
        "pxor xmm0, xmm0\n\t"
        "pxor xmm1, xmm1\n\t"
        "pxor xmm2, xmm2\n\t"
        "pxor xmm3, xmm3\n\t"
        "pxor xmm4, xmm4\n\t"
        "pxor xmm5, xmm5\n\t"
        "mov rcx, qword ptr [rbp + 0xe8]\n\t"
        "mov r11, qword ptr [rbp + 0xf8]\n\t"
        "mov rbp, r9\n\t"
        "mov rsp, r8\n\t"
        "swapgs\n\t"
        "sysretq\n\t"
        ".L_KiSystemCall64_14016ee2a:\n\t"
        "mov rdx, qword ptr [rbp + 0xb8]\n\t"
        "mov qword ptr [r11 + 0x90], rdx\n\t"
        "mov dl, byte ptr [rbp - 0x58]\n\t"
        "mov byte ptr [r11 + 0x232], dl\n\t"
        "cli\n\t"
        "mov rsp, rbp\n\t"
        "mov rbp, qword ptr [rbp + 0xd8]\n\t"
        "mov rsp, qword ptr [rsp + 0x100]\n\t"
        "sti\n\t"
        "ret\n\t"
        ".L_KiSystemCall64_14016ee57:\n\t"
        "mov r11, qword ptr gs:[0x188]\n\t"
        "mov rcx, cr8\n\t"
        "or cl, byte ptr [r11 + 0x24a]\n\t"
        "or ecx, dword ptr [r11 + 0x1e4]\n\t"
        "jne .L_KiSystemCall64_14016efb7\n\t"
        ".L_KiSystemCall64_14016ee78:\n\t"
        "cli\n\t"
        "mov qword ptr [rbp - 0x50], rax\n\t"
        "mov rcx, qword ptr gs:[0x188]\n\t"
        "cmp byte ptr [rcx + 0xc2], 0\n\t"
        "je .L_KiSystemCall64_14016eea8\n\t"
        ".L_KiSystemCall64_14016ee8f:\n\t"
        "mov ecx, 1\n\t"
        "mov cr8, rcx\n\t"
        "sti\n\t"
        "call __nt_asm_ext_140166eb0\n\t"
        "mov ecx, 0\n\t"
        "mov cr8, rcx\n\t"
        "cli\n\t"
        ".L_KiSystemCall64_14016eea8:\n\t"
        "mov rcx, qword ptr gs:[0x188]\n\t"
        "test dword ptr [rcx], 0x8000000\n\t"
        "je .L_KiSystemCall64_14016eebe\n\t"
        ".L_KiSystemCall64_14016eeb9:\n\t"
        "call __nt_asm_ext_140164530\n\t"
        ".L_KiSystemCall64_14016eebe:\n\t"
        "mov rcx, qword ptr gs:[0x188]\n\t"
        "test byte ptr [rcx + 2], 1\n\t"
        "je .L_KiSystemCall64_14016eedb\n\t"
        ".L_KiSystemCall64_14016eecd:\n\t"
        "call __nt_asm_ext_140201a60\n\t"
        "mov rcx, qword ptr gs:[0x188]\n\t"
        ".L_KiSystemCall64_14016eedb:\n\t"
        "cmp word ptr [rbp + 0x80], 0\n\t"
        "je .L_KiSystemCall64_14016eeea\n\t"
        ".L_KiSystemCall64_14016eee5:\n\t"
        "call __nt_asm_ext_140163ca0\n\t"
        ".L_KiSystemCall64_14016eeea:\n\t"
        "ldmxcsr dword ptr [rbp - 0x54]\n\t"
        "movaps xmm0, xmmword ptr [rbp - 0x10]\n\t"
        "movaps xmm1, xmmword ptr [rbp]\n\t"
        "movaps xmm2, xmmword ptr [rbp + 0x10]\n\t"
        "movaps xmm3, xmmword ptr [rbp + 0x20]\n\t"
        "movaps xmm4, xmmword ptr [rbp + 0x30]\n\t"
        "movaps xmm5, xmmword ptr [rbp + 0x40]\n\t"
        "mov r11, qword ptr [rbp - 0x20]\n\t"
        "mov r10, qword ptr [rbp - 0x28]\n\t"
        "mov r9, qword ptr [rbp - 0x30]\n\t"
        "mov r8, qword ptr [rbp - 0x38]\n\t"
        "mov rdx, qword ptr [rbp - 0x40]\n\t"
        "mov rcx, qword ptr [rbp - 0x48]\n\t"
        "mov rax, qword ptr [rbp - 0x50]\n\t"
        "mov rsi, qword ptr [rbp + 0xd0]\n\t"
        "mov rdi, qword ptr [rbp + 0xc8]\n\t"
        "mov rbx, qword ptr [rbp + 0xc0]\n\t"
        "mov rsp, rbp\n\t"
        "mov rbp, qword ptr [rbp + 0xd8]\n\t"
        "add rsp, 0xe8\n\t"
        "swapgs\n\t"
        "iretq\n\t"
        ".L_KiSystemCall64_14016ef4d:\n\t"
        "cmp edi, 0x20\n\t"
        "jne .L_KiSystemCall64_14016efad\n\t"
        ".L_KiSystemCall64_14016ef52:\n\t"
        "mov dword ptr [rbp - 0x80], eax\n\t"
        "mov qword ptr [rbp - 0x78], rcx\n\t"
        "mov qword ptr [rbp - 0x70], rdx\n\t"
        "mov qword ptr [rbp - 0x68], r8\n\t"
        "mov qword ptr [rbp - 0x60], r9\n\t"
        "call __nt_asm_ext_140166a40\n\t"
        "or eax, eax\n\t"
        "mov eax, dword ptr [rbp - 0x80]\n\t"
        "mov rcx, qword ptr [rbp - 0x78]\n\t"
        "mov rdx, qword ptr [rbp - 0x70]\n\t"
        "mov r8, qword ptr [rbp - 0x68]\n\t"
        "mov r9, qword ptr [rbp - 0x60]\n\t"
        "mov qword ptr [rbx + 0x90], rsp\n\t"
        "je .L_KiSystemCall64_14016eae4\n\t"
        ".L_KiSystemCall64_14016ef8c:\n\t"
        "lea rdi, [rip + 0x28a74d]\n\t"
        "mov esi, dword ptr [rdi + 0x10]\n\t"
        "mov rdi, qword ptr [rdi]\n\t"
        "cmp eax, esi\n\t"
        "jae .L_KiSystemCall64_14016efad\n\t"
        ".L_KiSystemCall64_14016ef9d:\n\t"
        "lea rdi, [rdi + rsi*4]\n\t"
        "movsx eax, byte ptr [rax + rdi]\n\t"
        "or eax, eax\n\t"
        "jle .L_KiSystemCall64_14016ec5b\n\t"
        ".L_KiSystemCall64_14016efad:\n\t"
        "mov eax, 0xc000001c\n\t"
        "jmp .L_KiSystemCall64_14016ec5b\n\t"
        ".L_KiSystemCall64_14016efb7:\n\t"
        "mov ecx, 0x4a\n\t"
        "xor r9d, r9d\n\t"
        "mov r8, cr8\n\t"
        "or r8d, r8d\n\t"
        "jne .L_KiSystemCall64_14016efdc\n\t"
        ".L_KiSystemCall64_14016efc8:\n\t"
        "mov ecx, 1\n\t"
        "movzx r8d, byte ptr [r11 + 0x24a]\n\t"
        "mov r9d, dword ptr [r11 + 0x1e4]\n\t"
        ".L_KiSystemCall64_14016efdc:\n\t"
        "mov rdx, qword ptr [rbp + 0xe8]\n\t"
        "mov r10, rbp\n\t"
        "call __nt_asm_ext_14016f080\n\t"
        ".L_KiSystemCall64_14016efeb:\n\t"
        "sub rsp, 0x50\n\t"
        "mov qword ptr [rsp + 0x20], rcx\n\t"
        "mov qword ptr [rsp + 0x28], rdx\n\t"
        "mov qword ptr [rsp + 0x30], r8\n\t"
        "mov qword ptr [rsp + 0x38], r9\n\t"
        "mov qword ptr [rsp + 0x40], r10\n\t"
        "mov rcx, r10\n\t"
        "call __nt_asm_ext_140275690\n\t"
        "mov rcx, qword ptr [rsp + 0x20]\n\t"
        "mov rdx, qword ptr [rsp + 0x28]\n\t"
        "mov r8, qword ptr [rsp + 0x30]\n\t"
        "mov r9, qword ptr [rsp + 0x38]\n\t"
        "mov r10, qword ptr [rsp + 0x40]\n\t"
        "add rsp, 0x50\n\t"
        "call r10\n\t"
        "mov qword ptr [rbp - 0x50], rax\n\t"
        "mov rcx, rax\n\t"
        "call __nt_asm_ext_140275710\n\t"
        "mov rax, qword ptr [rbp - 0x50]\n\t"
        "jmp .L_KiSystemCall64_14016ec53\n\t"
        "ret\n\t"
        ".att_syntax prefix"
    );
}

/*
 * Function : KeGetCurrentThread @ 0x14015f4e0 (10 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (11.91 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
struct _KTHREAD* __attribute__((ms_abi)) KeGetCurrentThread(void) {
    return (struct _KTHREAD*)(uintptr_t)(__ntrev_readgsqword(0x188));
}

/*
 * Function : KeGetCurrentProcessorNumberEx @ 0x14001f570 (44 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (8.14 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
ULONG __attribute__((ms_abi)) KeGetCurrentProcessorNumberEx(struct _PROCESSOR_NUMBER* ProcNumber) {
    struct _KPRCB* __attribute__((unused)) ssa_gs_val = KeGetCurrentPrcb();
    if (((uint64_t)(((uint64_t)(uintptr_t)ProcNumber)) == (uint64_t)(0ULL))) {
    } else {
        uint8_t __attribute__((unused)) ssa_mem_ld_1 = (uint8_t)(ssa_gs_val->Group);
        ProcNumber->Group = (uint16_t)(((uint16_t)(uint8_t)(ssa_mem_ld_1)));
        uint8_t __attribute__((unused)) ssa_mem_ld_2 = (uint8_t)(ssa_gs_val->GroupIndex);
        ProcNumber->Number = (uint8_t)(ssa_mem_ld_2);
        ProcNumber->Reserved = (uint8_t)(0ULL);
    }
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)(ssa_gs_val->Number);
    return (ULONG)(((uint64_t)(uint32_t)(ssa_mem_ld)));
}

/*
 * Function : KeQueryPriorityThread @ 0x1400339f0 (30 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.83 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
int32_t __attribute__((ms_abi)) KeQueryPriorityThread(struct _KTHREAD* Thread) {
    uint64_t __attribute__((unused)) ssa_common_ret_op = 0;
    struct _KPROCESS* __attribute__((unused)) ssa_mem_ld = (struct _KPROCESS*)(uintptr_t)(Thread->Process);
    if (((uint64_t)(((uint64_t)(uintptr_t)ssa_mem_ld)) == (uint64_t)(((uint64_t)(uintptr_t)&KiInitialProcess)))) {
        ssa_common_ret_op = (uint64_t)(1ULL);
    } else {
        uint8_t __attribute__((unused)) ssa_mem_ld_1 = (uint8_t)(Thread->Priority);
        ssa_common_ret_op = (uint64_t)(((uint64_t)(uint32_t)(((uint32_t)(int32_t)(int8_t)(ssa_mem_ld_1)))));
    }
    return (int32_t)(ssa_common_ret_op);
}

/*
 * Function : KeInitializeDpc @ 0x1400e02a0 (25 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (8.21 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) KeInitializeDpc(struct _KDPC* Dpc, void* DeferredRoutine, void* DeferredContext) {
    Dpc->TargetInfoAsUlong = (uint32_t)(0x113ULL);
    Dpc->DpcData = (void*)(uintptr_t)(0ULL);
    Dpc->ProcessorHistory = (uint64_t)(0ULL);
    Dpc->DeferredRoutine = (void**)(uintptr_t)(((uint64_t)(uintptr_t)DeferredRoutine));
    Dpc->DeferredContext = (void*)(uintptr_t)(((uint64_t)(uintptr_t)DeferredContext));
    return;
}

/*
 * Function : KeSetImportanceDpc @ 0x14010f680 (4 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.72 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) KeSetImportanceDpc(struct _KDPC* Dpc, uint32_t Importance) {
    Dpc->Importance = (uint8_t)(((uint8_t)(uint64_t)(((uint64_t)Importance))));
    return;
}

/*
 * Function : KeSetTargetProcessorDpc @ 0x140200ba0 (104 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4634.70 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) KeSetTargetProcessorDpc(struct _KDPC* Dpc, int8_t Number) {
    uint16_t __attribute__((unused)) ssa_reg_rbx_0_off0 = 0;
    uint64_t __attribute__((unused)) ssa_reg_rax_0 = 0;
    uint8_t __attribute__((aligned(16))) ssa_stack_frame[4096];
    (*(uint64_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 3080ULL))) = (uint64_t)(0ULL);
    (*(uint64_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 3096ULL))) = (uint64_t)(0ULL);
    (*(uint64_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 3064ULL))) = (uint64_t)(0ULL);
    uint8_t __attribute__((unused)) ssa_mem_ld = (uint8_t)((*(uint8_t*)(uintptr_t)(((uint64_t)(uintptr_t)&KeForceGroupAwareness))));
    uint8_t __attribute__((unused)) ssa_r_dl = (uint8_t)(((uint8_t)(uint64_t)(((uint64_t)Number))));
    if (((uint8_t)(ssa_mem_ld) == (uint8_t)(0ULL))) {
        ssa_reg_rbx_0_off0 = (uint16_t)(0ULL);
        goto L_bb_140200bcc;
    }
    uint16_t __attribute__((unused)) ssa_mem_ld_1 = (uint16_t)((*(uint16_t*)(uintptr_t)(((uint64_t)(uintptr_t)&KiActiveGroups))));
    ssa_reg_rbx_0_off0 = (uint16_t)(((uint16_t)(ssa_mem_ld_1) + (uint16_t)(0xffffffffffffffffULL)));
L_bb_140200bcc:
    uint64_t __attribute__((unused)) ssa_call_res = KeQueryActiveProcessorCountEx(((uint64_t)(uint16_t)(ssa_reg_rbx_0_off0)), ((uint64_t)Number), 0ULL, 0ULL);
    if (((uint32_t)(((uint32_t)(int32_t)(int8_t)(ssa_r_dl))) < (uint32_t)(((uint32_t)(uint64_t)(ssa_call_res))))) {
        goto L_bb_140200bdc;
    }
    ssa_reg_rax_0 = (uint64_t)(ssa_call_res);
    goto L_bb_140200bf8;
L_bb_140200bdc:
    (*(uint16_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 3088ULL))) = (uint16_t)(ssa_reg_rbx_0_off0);
    (*(uint8_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 3090ULL))) = (uint8_t)(ssa_r_dl);
    (*(uint8_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 3091ULL))) = (uint8_t)(0ULL);
    uint64_t __attribute__((unused)) ssa_call_res_1 = KeSetTargetProcessorDpcEx(((uint64_t)(uintptr_t)Dpc), (((uint64_t)(uintptr_t)ssa_stack_frame) + 3088ULL), 0ULL, 0ULL);
    ssa_reg_rax_0 = (uint64_t)(ssa_call_res_1);
L_bb_140200bf8:
    return;
}

/*
 * Function : KeCheckForZeroPage @ 0x140166a80 (92 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (2602.79 ms, 49 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence, loop_reroll_equivalence:unrolled_page_loop
 * De-Opt   : Collapsed 16x-unrolled strided loop (4096B = 512 x 8B, op=or) into single counted for-loop
 */
uint64_t __attribute__((ms_abi)) KeCheckForZeroPage(uint64_t* PageBase) {
    uint64_t acc = 0ULL;
    for (size_t i = 0; i < 512ULL; ++i) {
        acc |= ((const uint64_t *)(uintptr_t)PageBase)[i];
    }
    return (uint64_t)(acc);
}

/*
 * Function : KeSetCoalescableTimer @ 0x140037840 (202 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5376.16 ms, 40 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence, magic_div_equivalence:div_10000
 * De-Opt   : Strength-reduced magic 0x346dc5d63886594b -> '/ 10000'
 */
BOOLEAN __attribute__((ms_abi)) KeSetCoalescableTimer(void* Timer, int64_t DueTime, ULONG Period, ULONG TolerableDelay) {
    uint32_t __attribute__((unused)) ssa_reg_r9_1_off0 = 0;
    uint32_t __attribute__((unused)) ssa_reg_r11_1_off0 = 0;
    uint64_t __attribute__((unused)) ssa_reg_rdx_1 = 0;
    uint64_t __attribute__((unused)) ssa_reg_r9_0 = 0;
    uint8_t __attribute__((unused)) ssa_reg_r10_0_off0 = 0;
    uint32_t __attribute__((unused)) ssa_reg_r11_0_off0 = 0;
    uint64_t __attribute__((unused)) ssa_reg_rdx_0 = 0;
    uint64_t __attribute__((unused)) ssa_reg_r8_0 = 0;
    uint32_t __attribute__((unused)) ssa_r_r8d = (uint32_t)(((uint32_t)(uint64_t)(((uint64_t)Period))));
    if (((uint32_t)(((uint32_t)(uint64_t)(((uint64_t)TolerableDelay)))) == (uint32_t)(0ULL))) {
        ssa_reg_r9_0 = (uint64_t)(((uint64_t)TolerableDelay));
        ssa_reg_r10_0_off0 = (uint8_t)(0ULL);
        ssa_reg_r11_0_off0 = (uint32_t)(ssa_r_r8d);
        ssa_reg_rdx_0 = (uint64_t)(((uint64_t)DueTime));
        goto L_bb_14003787e;
    }
    uint64_t __attribute__((unused)) ssa__69 = (uint64_t)(((uint64_t)(((uint64_t)TolerableDelay)) & (uint64_t)(0xffffffffULL)));
    uint64_t __attribute__((unused)) ssa__71 = (uint64_t)(((uint64_t)(ssa__69) * (uint64_t)(0x2710ULL)));
    if (((uint64_t)(ssa__69) > (uint64_t)(0x673ULL))) {
        goto L_bb_14003789c;
    }
    ssa_reg_r9_1_off0 = (uint32_t)(((uint32_t)(uint64_t)(ssa__71)));
    ssa_reg_r11_1_off0 = (uint32_t)(ssa_r_r8d);
    ssa_reg_rdx_1 = (uint64_t)(((uint64_t)DueTime));
L_bb_140037867:
    ssa_reg_r9_0 = (uint64_t)(0ULL);
    ssa_reg_r10_0_off0 = (uint8_t)(((uint8_t)(((uint8_t)(uint32_t)(((uint32_t)((((uint32_t)(ssa_reg_r9_1_off0) < (uint32_t)(0xfc0000ULL)) ? (uint32_t)(ssa_reg_r9_1_off0) : (uint32_t)(0xfc0000ULL))) >> (uint32_t)(0x10ULL))))) & (uint8_t)(0xfffffffffffffffcULL)));
    ssa_reg_r11_0_off0 = (uint32_t)(ssa_reg_r11_1_off0);
    ssa_reg_rdx_0 = (uint64_t)(ssa_reg_rdx_1);
L_bb_14003787e:
    uint64_t __attribute__((unused)) ssa_call_res = (uint64_t)KiSetTimerEx((void*)(uintptr_t)(((uint64_t)(uintptr_t)Timer)), (int64_t)(ssa_reg_rdx_0), (ULONG)(((uint64_t)(uint32_t)(ssa_reg_r11_0_off0))), (uint64_t)(((uint64_t)(ssa_reg_r9_0) | (uint64_t)(((uint64_t)(uint8_t)(ssa_reg_r10_0_off0))))));
    return (BOOLEAN)(ssa_call_res);
L_bb_14003789c:
    uint64_t __attribute__((unused)) ssa__188 = (uint64_t)(((uint64_t)(ssa__71) + (uint64_t)(0xffffffffff040000ULL)));
    if (((int64_t)(((uint64_t)DueTime)) > (int64_t)(0xffffffffffffffffULL))) {
        goto L_bb_1400378f3;
    }
    uint64_t __attribute__((unused)) ssa__216 = (uint64_t)(((uint64_t)(((uint64_t)DueTime)) - (uint64_t)(ssa__188)));
    ssa_reg_r8_0 = (uint64_t)(((((int64_t)(((uint64_t)(((uint64_t)(ssa__216) ^ (uint64_t)(((uint64_t)DueTime)))) & (uint64_t)(((uint64_t)(ssa__216) ^ (uint64_t)(((uint64_t)(0xfc0000ULL) - (uint64_t)(ssa__71))))))) < (int64_t)(0ULL))) ? (0x8000000000000000ULL) : (ssa__216)));
L_bb_1400378b7:
    if (((uint32_t)(ssa_r_r8d) == (uint32_t)(0ULL))) {
        ssa_reg_r9_1_off0 = (uint32_t)(0xfc0000ULL);
        ssa_reg_r11_1_off0 = (uint32_t)(0ULL);
        ssa_reg_rdx_1 = (uint64_t)(ssa_reg_r8_0);
        goto L_bb_140037867;
    }
    uint64_t __attribute__((unused)) ssa__289 = (uint64_t)(((int64_t)(((uint64_t)(unsigned __int128)(((unsigned __int128)(((unsigned __int128)(((__int128)(int64_t)(ssa__188))) * (unsigned __int128)(0x346dc5d63886594bULL))) >> (unsigned __int128)(0x40ULL))))) >> (int64_t)(11ULL)));
    ssa_reg_r9_1_off0 = (uint32_t)(0xfc0000ULL);
    ssa_reg_r11_1_off0 = (uint32_t)(((((uint32_t)(ssa_r_r8d) + (uint32_t)(((uint32_t)(uint64_t)(((uint64_t)(((uint64_t)(ssa__289) >> (uint64_t)(0x3fULL))) + (uint64_t)(ssa__289)))))) < (uint32_t)(ssa_r_r8d)) ? (uint32_t)0xffffffffULL : ((uint32_t)(ssa_r_r8d) + (uint32_t)(((uint32_t)(uint64_t)(((uint64_t)(((uint64_t)(ssa__289) >> (uint64_t)(0x3fULL))) + (uint64_t)(ssa__289))))))));
    ssa_reg_rdx_1 = (uint64_t)(ssa_reg_r8_0);
    goto L_bb_140037867;
L_bb_1400378f3:
    uint64_t __attribute__((unused)) ssa__361 = (uint64_t)(((uint64_t)(ssa__188) + (uint64_t)(((uint64_t)DueTime))));
    ssa_reg_r8_0 = (uint64_t)(((((int64_t)(((uint64_t)(ssa__361) & (uint64_t)(((uint64_t)(0xfbfff0ULL) - (uint64_t)(ssa__71))))) < (int64_t)(0ULL))) ? (0x7fffffffffffffffULL) : (ssa__361)));
    goto L_bb_1400378b7;
}

/*
 * Function : KeReadStateEvent @ 0x1400f5b60 (4 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.98 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
LONG __attribute__((ms_abi)) KeReadStateEvent(struct _DISPATCHER_HEADER* Event) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)(Event->SignalState);
    return (LONG)(((uint64_t)(uint32_t)(ssa_mem_ld)));
}

/*
 * Function : KeReadStateMutex @ 0x1400f5b60 (4 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.27 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
LONG __attribute__((ms_abi)) KeReadStateMutex(struct _DISPATCHER_HEADER* Mutex) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)(Mutex->SignalState);
    return (LONG)(((uint64_t)(uint32_t)(ssa_mem_ld)));
}

/*
 * Function : KeReadStateSemaphore @ 0x1400f5b60 (4 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.70 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
LONG __attribute__((ms_abi)) KeReadStateSemaphore(struct _DISPATCHER_HEADER* Semaphore) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)(Semaphore->SignalState);
    return (LONG)(((uint64_t)(uint32_t)(ssa_mem_ld)));
}

/*
 * Function : KeReadStateTimer @ 0x1401fbac0 (4 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.30 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
BOOLEAN __attribute__((ms_abi)) KeReadStateTimer(struct _DISPATCHER_HEADER* Timer) {
    uint8_t __attribute__((unused)) ssa_mem_ld = (uint8_t)((*(uint8_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)Timer)) + (uint64_t)(4ULL)))));
    return (BOOLEAN)(((uint64_t)(uint8_t)(ssa_mem_ld)));
}
