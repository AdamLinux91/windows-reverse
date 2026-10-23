/*
 * Reconstructed Windows NT Kernel Subsystem: Kernel Core Dispatcher, IRQL, DPC, Bootstrap & Synchronization (Ke/Ki/Kz/Hal)
 * Source Binary : ntoskrnl.exe (10.0.16299.15 x64, RSDS 83DB4240-4EFD-4AB6-AFB6-FA864B700CB3)
 * Functions     : 72 Z3-verified / assembly-preserved routines
 */
#include "../../include/nt_reconstructed_types.h"

extern void __nt_asm_ext_14041cad0(void);
extern void __nt_asm_ext_14078d140(void);
extern void __nt_asm_ext_14041e460(void);
extern void __nt_asm_ext_14041b3d0(void);
extern void __nt_asm_ext_1401668d0(void);
extern unsigned char __nt_asm_sym_1403f9130[];
extern unsigned char __nt_asm_sym_1403f90c8[];
extern unsigned char __nt_asm_sym_140169d60[];
extern unsigned char __nt_asm_sym_1403f5780[];
extern unsigned char __nt_asm_sym_140169d10[];
extern unsigned char __nt_asm_sym_1403f5778[];
extern unsigned char __nt_asm_sym_1403f8068[];
extern unsigned char __nt_asm_sym_140353370[];
extern unsigned char __nt_asm_sym_140350ba0[];
extern unsigned char __nt_asm_sym_140350ba8[];
extern unsigned char __nt_asm_sym_1403f9288[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] KiSystemStartup @ 0x14041a010
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Privileged CPU/CSR instruction 'mov r8, cr0' at 0x14041a03c
 *   - Privileged CPU/CSR instruction 'mov r8, cr2' at 0x14041a047
 *   - Privileged CPU/CSR instruction 'mov r8, cr3' at 0x14041a052
 *   - Privileged CPU/CSR instruction 'mov r8, cr4' at 0x14041a05d
 *   - Privileged CPU/CSR instruction 'sgdt [rdx + 0x2d6]' at 0x14041a068
 *   - Privileged CPU/CSR instruction 'sidt [rdx + 0x2e6]' at 0x14041a079
 *   - Privileged CPU/CSR instruction 'str word ptr [rdx + 0x2f0]' at 0x14041a08b
 *   - Privileged CPU/CSR instruction 'sldt word ptr [rdx + 0x2f2]' at 0x14041a092
 *   - Privileged CPU/CSR instruction 'ldmxcsr dword ptr [rdx + 0x180]' at 0x14041a0a3
 *   - Privileged CPU/CSR instruction 'lldt ax' at 0x14041a0d6
 *   - Privileged CPU/CSR instruction 'wrmsr' at 0x14041a102
 *   - Privileged CPU/CSR instruction 'wrmsr' at 0x14041a109
 *   - Privileged CPU/CSR instruction 'mov cr8, rcx' at 0x14041a172
 *   - Privileged CPU/CSR instruction 'rdtsc' at 0x14041a1db
 *   - Non-ABI kernel stack pivot 'mov rsp, rax' at 0x14041a22c
 *   - Privileged/control-register routine (15/117 insns) preserved as native x86-64 assembly
 *   Live-In Registers : r15, rcx
 *   Clobbered State   : r15, rdx, r10, r8, r9, rax, rcx, memory, cc
 */
__attribute__((naked)) void KiSystemStartup(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "sub rsp, 0x38\n\t"
        "mov qword ptr [rsp + 0x30], r15\n\t"
        "mov r15, rsp\n\t"
        "mov qword ptr [rip + __nt_asm_sym_1403f9130], rcx\n\t"
        "mov rdx, qword ptr [rcx + 0x88]\n\t"
        "mov r10, rdx\n\t"
        "sub rdx, 0x180\n\t"
        "mov qword ptr [rdx + 0x18], rdx\n\t"
        "mov qword ptr [rdx + 0x20], r10\n\t"
        "mov r8, cr0\n\t"
        "mov qword ptr [rdx + 0x280], r8\n\t"
        "mov r8, cr2\n\t"
        "mov qword ptr [rdx + 0x288], r8\n\t"
        "mov r8, cr3\n\t"
        "mov qword ptr [rdx + 0x290], r8\n\t"
        "mov r8, cr4\n\t"
        "mov qword ptr [rdx + 0x298], r8\n\t"
        "sgdt [rdx + 0x2d6]\n\t"
        "mov r8, qword ptr [rdx + 0x2d8]\n\t"
        "mov qword ptr [rdx], r8\n\t"
        "sidt [rdx + 0x2e6]\n\t"
        "mov r9, qword ptr [rdx + 0x2e8]\n\t"
        "mov qword ptr [rdx + 0x38], r9\n\t"
        "str word ptr [rdx + 0x2f0]\n\t"
        "sldt word ptr [rdx + 0x2f2]\n\t"
        "mov dword ptr [rdx + 0x180], 0x1f80\n\t"
        "ldmxcsr dword ptr [rdx + 0x180]\n\t"
        "cmp dword ptr [r10 + 0x24], 0\n\t"
        "jne .L_KiSystemStartup_14041a0ba\n\t"
        ".L_KiSystemStartup_14041a0b1:\n\t"
        "mov ax, 0x3c00\n\t"
        "mov word ptr [r8 + 0x50], ax\n\t"
        ".L_KiSystemStartup_14041a0ba:\n\t"
        "mov ax, 0x2b\n\t"
        "mov ds, ax\n\t"
        "mov es, ax\n\t"
        "mov ax, 0x53\n\t"
        "mov fs, ax\n\t"
        "test byte ptr [rip + __nt_asm_sym_1403f90c8], 0xff\n\t"
        "jne .L_KiSystemStartup_14041a0d9\n\t"
        ".L_KiSystemStartup_14041a0d4:\n\t"
        "xor eax, eax\n\t"
        "lldt ax\n\t"
        ".L_KiSystemStartup_14041a0d9:\n\t"
        "mov ax, word ptr [r8 + 0x42]\n\t"
        "mov word ptr [rdx + 8], ax\n\t"
        "mov al, byte ptr [r8 + 0x44]\n\t"
        "mov byte ptr [rdx + 0xa], al\n\t"
        "mov al, byte ptr [r8 + 0x47]\n\t"
        "mov byte ptr [rdx + 0xb], al\n\t"
        "mov eax, dword ptr [r8 + 0x48]\n\t"
        "mov dword ptr [rdx + 0xc], eax\n\t"
        "mov eax, edx\n\t"
        "shr rdx, 0x20\n\t"
        "mov ecx, 0xc0000101\n\t"
        "wrmsr\n\t"
        "mov ecx, 0xc0000102\n\t"
        "wrmsr\n\t"
        "cmp dword ptr [r10 + 0x24], 0\n\t"
        "jne .L_KiSystemStartup_14041a12e\n\t"
        ".L_KiSystemStartup_14041a112:\n\t"
        "lea rcx, [rip + __nt_asm_sym_140169d60]\n\t"
        "mov qword ptr [rip + __nt_asm_sym_1403f5780], rcx\n\t"
        "lea rcx, [rip + __nt_asm_sym_140169d10]\n\t"
        "mov qword ptr [rip + __nt_asm_sym_1403f5778], rcx\n\t"
        ".L_KiSystemStartup_14041a12e:\n\t"
        "mov rcx, qword ptr [rip + __nt_asm_sym_1403f9130]\n\t"
        "call __nt_asm_ext_14041cad0\n\t"
        "xor ecx, ecx\n\t"
        "mov rdx, qword ptr [rip + __nt_asm_sym_1403f9130]\n\t"
        "mov r10, qword ptr [rdx + 0x88]\n\t"
        "cmp dword ptr [r10 + 0x24], 0\n\t"
        "jne .L_KiSystemStartup_14041a156\n\t"
        ".L_KiSystemStartup_14041a151:\n\t"
        "call __nt_asm_ext_14078d140\n\t"
        ".L_KiSystemStartup_14041a156:\n\t"
        "mov rcx, qword ptr [rip + __nt_asm_sym_1403f9130]\n\t"
        "mov r10, qword ptr [rcx + 0x88]\n\t"
        "mov edx, dword ptr [r10 + 0x24]\n\t"
        "call __nt_asm_ext_14041e460\n\t"
        "mov ecx, 0xf\n\t"
        "mov cr8, rcx\n\t"
        "mov ecx, dword ptr [rip + __nt_asm_sym_1403f8068]\n\t"
        "sub rsp, rcx\n\t"
        "and rsp, 0xffffffffffffffc0\n\t"
        "sub rsp, 0x1b0\n\t"
        "mov rax, qword ptr [rip + __nt_asm_sym_1403f9130]\n\t"
        "mov rcx, qword ptr [rax + 0x90]\n\t"
        "mov rdx, qword ptr [rax + 0x98]\n\t"
        "mov r8, qword ptr gs:[8]\n\t"
        "mov r8, qword ptr [r8 + 4]\n\t"
        "mov qword ptr gs:[0x1a8], r8\n\t"
        "mov r9, qword ptr [rax + 0x88]\n\t"
        "mov r10d, dword ptr [r9 + 0x24]\n\t"
        "mov qword ptr [rsp + 0x20], r10\n\t"
        "mov qword ptr [rsp + 0x28], rax\n\t"
        "call __nt_asm_ext_14041b3d0\n\t"
        "cmp word ptr gs:[0x1a4], 0\n\t"
        "jne .L_KiSystemStartup_14041a212\n\t"
        ".L_KiSystemStartup_14041a1db:\n\t"
        "rdtsc\n\t"
        "shl rdx, 0x20\n\t"
        "or rax, rdx\n\t"
        "mov rdx, rax\n\t"
        "ror rdx, 0x31\n\t"
        "xor rax, rdx\n\t"
        "xor rax, qword ptr [rip + __nt_asm_sym_140353370]\n\t"
        "rol rax, 0x10\n\t"
        "mov ax, 0\n\t"
        "ror rax, 0x10\n\t"
        "mov qword ptr [rip + __nt_asm_sym_140350ba0], rax\n\t"
        "not rax\n\t"
        "mov qword ptr [rip + __nt_asm_sym_140350ba8], rax\n\t"
        ".L_KiSystemStartup_14041a212:\n\t"
        "mov rax, qword ptr gs:[0x1a8]\n\t"
        "mov ecx, dword ptr [rip + __nt_asm_sym_1403f8068]\n\t"
        "sub rax, rcx\n\t"
        "and rax, 0xffffffffffffffc0\n\t"
        "sub rax, 0x30\n\t"
        "mov rsp, rax\n\t"
        "mov rcx, qword ptr gs:[0x188]\n\t"
        "mov byte ptr [rcx + 0x186], 2\n\t"
        ".L_KiSystemStartup_14041a23f:\n\t"
        "cmp dword ptr [rip + __nt_asm_sym_1403f9288], 0\n\t"
        "pause\n\t"
        "jne .L_KiSystemStartup_14041a23f\n\t"
        ".L_KiSystemStartup_14041a24a:\n\t"
        "call __nt_asm_ext_1401668d0\n\t"
        "ret\n\t"
        ".att_syntax prefix"
    );
}

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

extern void __nt_asm_ext_140163d20(void);
extern void __nt_asm_ext_140710720(void);
extern void __nt_asm_ext_14016fa40(void);
extern void __nt_asm_ext_140513cd0(void);
extern void __nt_asm_ext_140166eb0(void);
extern void __nt_asm_ext_140164530(void);
extern void __nt_asm_ext_140201a60(void);
extern void __nt_asm_ext_14016fcc0(void);
extern void __nt_asm_ext_140163ca0(void);
extern void __nt_asm_ext_140166a40(void);
extern void __nt_asm_ext_14016f080(void);
extern void __nt_asm_ext_140275690(void);
extern void __nt_asm_ext_140275710(void);
extern unsigned char __nt_asm_sym_140410880[];
extern unsigned char __nt_asm_sym_1403f96c0[];
extern unsigned char __nt_asm_sym_1403f9700[];
extern unsigned char __nt_asm_sym_1402c1210[];
extern unsigned char __nt_asm_sym_1403f9148[];
extern unsigned char __nt_asm_sym_1403f96e0[];

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
 *   - Indirect dispatch / IAT tail-jump 'jmp r11' at 0x14016ebca
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
 *   - Privileged/control-register routine (32/385 insns) preserved as native x86-64 assembly
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
        "lea r10, [rip + __nt_asm_sym_140410880]\n\t"
        "lea r11, [rip + __nt_asm_sym_1403f96c0]\n\t"
        "test dword ptr [rbx + 0x78], 0x80\n\t"
        "je .L_KiSystemCall64_14016eb0e\n\t"
        ".L_KiSystemCall64_14016eafb:\n\t"
        "test dword ptr [rbx + 0x78], 0x200000\n\t"
        "je .L_KiSystemCall64_14016eb0b\n\t"
        ".L_KiSystemCall64_14016eb04:\n\t"
        "lea r11, [rip + __nt_asm_sym_1403f9700]\n\t"
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
        "cmp rsi, qword ptr [rip + __nt_asm_sym_1402c1210]\n\t"
        "cmovae rsi, qword ptr [rip + __nt_asm_sym_1402c1210]\n\t"
        "nop dword ptr [rax]\n\t"
        ".L_KiSystemCall64_14016ebc0:\n\t"
        "lea r11, [rip + .L_KiSystemCall64_14016ec40]\n\t"
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
        "test dword ptr [rip + __nt_asm_sym_1403f9148], 0x40\n\t"
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
        "lea rdi, [rip + __nt_asm_sym_1403f96e0]\n\t"
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
 * Z3 Proof : VERIFIED_EQUIVALENT (11.02 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
struct _KTHREAD* __attribute__((ms_abi)) KeGetCurrentThread(void) {
    return (struct _KTHREAD*)(uintptr_t)(__ntrev_readgsqword(0x188));
}

/*
 * Function : KeGetCurrentProcessorNumberEx @ 0x14001f570 (44 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (7.46 ms, 33 SMT assertions)
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
 * Z3 Proof : VERIFIED_EQUIVALENT (5.00 ms, 33 SMT assertions)
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
 * Z3 Proof : VERIFIED_EQUIVALENT (6.54 ms, 34 SMT assertions)
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
 * Z3 Proof : VERIFIED_EQUIVALENT (3.21 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) KeSetImportanceDpc(struct _KDPC* Dpc, uint32_t Importance) {
    Dpc->Importance = (uint8_t)(((uint8_t)(uint64_t)(((uint64_t)Importance))));
    return;
}

/*
 * Function : KeSetTargetProcessorDpc @ 0x140200ba0 (104 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4030.96 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) KeSetTargetProcessorDpc(struct _KDPC* Dpc, int8_t Number) {
    uint16_t __attribute__((unused)) ssa_reg_rbx_0_off0 = 0;
    uint64_t __attribute__((unused)) ssa_reg_rax_0 = 0;
    uint8_t __attribute__((aligned(16))) ssa_stack_frame[1024];
    (*(uint64_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 776ULL))) = (uint64_t)(0ULL);
    (*(uint64_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 792ULL))) = (uint64_t)(0ULL);
    (*(uint64_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 760ULL))) = (uint64_t)(0ULL);
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
    (*(uint16_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 784ULL))) = (uint16_t)(ssa_reg_rbx_0_off0);
    (*(uint8_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 786ULL))) = (uint8_t)(ssa_r_dl);
    (*(uint8_t*)(uintptr_t)((((uint64_t)(uintptr_t)ssa_stack_frame) + 787ULL))) = (uint8_t)(0ULL);
    uint64_t __attribute__((unused)) ssa_call_res_1 = KeSetTargetProcessorDpcEx(((uint64_t)(uintptr_t)Dpc), (((uint64_t)(uintptr_t)ssa_stack_frame) + 784ULL), 0ULL, 0ULL);
    ssa_reg_rax_0 = (uint64_t)(ssa_call_res_1);
L_bb_140200bf8:
    return;
}

/*
 * Function : KeCheckForZeroPage @ 0x140166a80 (92 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (2625.88 ms, 49 SMT assertions)
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
 * Z3 Proof : VERIFIED_EQUIVALENT (2557.51 ms, 40 SMT assertions)
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
 * Z3 Proof : VERIFIED_EQUIVALENT (3.93 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
LONG __attribute__((ms_abi)) KeReadStateEvent(struct _DISPATCHER_HEADER* Event) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)(Event->SignalState);
    return (LONG)(((uint64_t)(uint32_t)(ssa_mem_ld)));
}

/*
 * Function : KeReadStateMutex @ 0x1400f5b60 (4 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.78 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
LONG __attribute__((ms_abi)) KeReadStateMutex(struct _DISPATCHER_HEADER* Mutex) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)(Mutex->SignalState);
    return (LONG)(((uint64_t)(uint32_t)(ssa_mem_ld)));
}

/*
 * Function : KeReadStateSemaphore @ 0x1400f5b60 (4 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.62 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
LONG __attribute__((ms_abi)) KeReadStateSemaphore(struct _DISPATCHER_HEADER* Semaphore) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)(Semaphore->SignalState);
    return (LONG)(((uint64_t)(uint32_t)(ssa_mem_ld)));
}

/*
 * Function : KeReadStateTimer @ 0x1401fbac0 (4 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.43 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
BOOLEAN __attribute__((ms_abi)) KeReadStateTimer(struct _DISPATCHER_HEADER* Timer) {
    uint8_t __attribute__((unused)) ssa_mem_ld = (uint8_t)((*(uint8_t*)((uint8_t*)(uintptr_t)(Timer) + 0x4)));
    return (BOOLEAN)(((uint64_t)(uint8_t)(ssa_mem_ld)));
}

/*
 * Function : KeRestoreFloatingPointState @ 0x1400260c0 (3 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.54 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) KeRestoreFloatingPointState(void) {
    return (uint64_t)(0ULL);
}

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] KeSweepLocalCaches @ 0x1401175c0
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Privileged CPU/CSR instruction 'wbinvd' at 0x1401175c0
 *   - Privileged/control-register routine (1/2 insns) preserved as native x86-64 assembly
 *   Clobbered State   : memory, cc
 */
void __attribute__((naked, ms_abi)) KeSweepLocalCaches(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "wbinvd\n\t"
        "ret\n\t"
        ".att_syntax prefix"
    );
}

/*
 * Function : KeSignalCallDpcDone @ 0x140111320 (4 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.93 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) KeSignalCallDpcDone(uint32_t* arg1) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))));
    (*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))) = (uint32_t)(((uint32_t)(ssa_mem_ld) + (uint32_t)(0xffffffffffffffffULL)));
    return;
}

/*
 * Function : HalExamineMBR @ 0x1406a96b0 (5 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.47 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) HalExamineMBR(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t* arg4) {
    (*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg4))) = (uint64_t)(0ULL);
    return;
}

/*
 * Function : KeClockTimerPowerChange @ 0x1401fc7e0 (7 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.51 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) KeClockTimerPowerChange(void) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)&KiClockStats))));
    (*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)&KiClockStats))) = (uint32_t)(((uint32_t)(ssa_mem_ld) + (uint32_t)(1ULL)));
    return;
}

/*
 * Function : KeForceEnableNx @ 0x14041eea0 (7 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.14 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint8_t __attribute__((ms_abi)) KeForceEnableNx(void) {
    uint8_t __attribute__((unused)) ssa_mem_ld = (uint8_t)((*(uint8_t*)(uintptr_t)(((uint64_t)(uintptr_t)&KiNxForceEnable))));
    return (uint8_t)(((uint64_t)(uint8_t)(ssa_mem_ld)));
}

/*
 * Function : KeGetClockOwner @ 0x1401fc8e0 (7 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.44 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) KeGetClockOwner(void) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)&KiClockTimerOwner))));
    return (uint32_t)(((uint64_t)(uint32_t)(ssa_mem_ld)));
}

/*
 * Function : KeGetRecommendedSharedDataAlignment @ 0x140025d70 (7 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.39 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) KeGetRecommendedSharedDataAlignment(void) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)&KeLargestCacheLine))));
    return (uint32_t)(((uint64_t)(uint32_t)(ssa_mem_ld)));
}

/*
 * Function : KeQueryTimeIncrement @ 0x1400e9ea0 (7 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.39 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) KeQueryTimeIncrement(void) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)&KeMaximumIncrement))));
    return (uint32_t)(((uint64_t)(uint32_t)(ssa_mem_ld)));
}

/*
 * Function : KeSetLastBranchRecordInUse @ 0x1401fd4e0 (7 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.54 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) KeSetLastBranchRecordInUse(uint8_t arg1) {
    (*(uint8_t*)(uintptr_t)(((uint64_t)(uintptr_t)&KiLastBranchRecordInUse))) = (uint8_t)(((uint8_t)(uint64_t)(((uint64_t)arg1))));
    return;
}

/*
 * Function : KeSetProfileIrql @ 0x14084d250 (7 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.37 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) KeSetProfileIrql(uint8_t arg1) {
    (*(uint8_t*)(uintptr_t)(((uint64_t)(uintptr_t)&KiProfileIrql))) = (uint8_t)(((uint8_t)(uint64_t)(((uint64_t)arg1))));
    return;
}

/*
 * Function : KeSystemFullyCacheCoherent @ 0x140200a30 (7 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.42 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) KeSystemFullyCacheCoherent(void) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)&KiSystemFullyCoherent))));
    return (uint32_t)(((uint64_t)(uint32_t)(ssa_mem_ld)));
}

/*
 * Function : InbvInstallDisplayStringFilter @ 0x1401ecdc0 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.89 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) InbvInstallDisplayStringFilter(uint64_t arg1) {
    (*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)&__nt_data_14037f730))) = (uint64_t)(((uint64_t)arg1));
    return;
}

/*
 * Function : KeQueryActiveGroupCount @ 0x1400e3bc0 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.21 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) KeQueryActiveGroupCount(void) {
    uint16_t __attribute__((unused)) ssa_mem_ld = (uint16_t)((*(uint16_t*)(uintptr_t)(((uint64_t)(uintptr_t)&KiActiveGroups))));
    return (uint32_t)(((uint64_t)(uint16_t)(ssa_mem_ld)));
}

/*
 * Function : KeQueryActiveProcessors @ 0x1401fd800 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.02 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) KeQueryActiveProcessors(void) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)&__nt_data_1403f93e8))));
    return (uint64_t)(ssa_mem_ld);
}

/*
 * Function : KeQueryMaximumGroupCount @ 0x140038830 (8 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.60 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) KeQueryMaximumGroupCount(void) {
    uint16_t __attribute__((unused)) ssa_mem_ld = (uint16_t)((*(uint16_t*)(uintptr_t)(((uint64_t)(uintptr_t)&KiMaximumGroups))));
    return (uint32_t)(((uint64_t)(uint16_t)(ssa_mem_ld)));
}

/*
 * Function : KeQueryHighestNodeNumber @ 0x140024580 (11 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.92 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) KeQueryHighestNodeNumber(void) {
    uint16_t __attribute__((unused)) ssa_mem_ld = (uint16_t)((*(uint16_t*)(uintptr_t)(((uint64_t)(uintptr_t)&KeNumberNodes))));
    return (uint32_t)(((uint64_t)(uint16_t)(((uint16_t)(ssa_mem_ld) + (uint16_t)(0xffffffffffffffffULL)))));
}

/*
 * Function : KeReportCacheIncoherentDevice @ 0x140200a10 (11 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.72 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) KeReportCacheIncoherentDevice(void) {
    (*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)&KiSystemFullyCoherent))) = (uint32_t)(0ULL);
    return;
}

/*
 * Function : InbvEnableDisplayString @ 0x1401ecd40 (13 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.71 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint8_t __attribute__((ms_abi)) InbvEnableDisplayString(uint8_t arg1) {
    uint8_t __attribute__((unused)) ssa_mem_ld = (uint8_t)((*(uint8_t*)(uintptr_t)(((uint64_t)(uintptr_t)&__nt_data_14037f728))));
    (*(uint8_t*)(uintptr_t)(((uint64_t)(uintptr_t)&__nt_data_14037f728))) = (uint8_t)(((uint8_t)(uint64_t)(((uint64_t)arg1))));
    return (uint8_t)(((uint64_t)(uint8_t)(ssa_mem_ld)));
}

/*
 * Function : KeInitializeEnumerationContextFromAffinity @ 0x1401fe730 (13 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (6.35 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) KeInitializeEnumerationContextFromAffinity(PIOV_IRP_TRACE arg1, uint16_t arg2, uint64_t arg3) {
    arg1->Irp = (struct _IRP*)(uintptr_t)(0ULL);
    arg1->KernelApcDisable = (int16_t)(((uint16_t)(uint64_t)(((uint64_t)arg2))));
    arg1->Thread = (struct _KTHREAD*)(uintptr_t)(((uint64_t)arg3));
    return;
}

/*
 * Function : KeIsExecutingDpc @ 0x1401346f0 (14 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.80 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) KeIsExecutingDpc(void) {
    return (uint32_t)(((uint64_t)(uint32_t)(((uint32_t)(__ntrev_readgsqword(0x2fec)) & (uint32_t)(0x10001ULL)))));
}

/*
 * Function : KeQueryPrcbAddress @ 0x140115f10 (14 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (18.88 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) KeQueryPrcbAddress(uint64_t* arg1) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)&KiProcessorBlock)) + (uint64_t)(((uint64_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) << (uint64_t)(3ULL))) & (uint64_t)(0x7fffffff8ULL)))))));
    return (uint64_t)(ssa_mem_ld);
}

/*
 * Function : KeQueryRuntimeThread @ 0x140117200 (15 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.89 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) KeQueryRuntimeThread(PKTHREAD arg1, uint32_t* arg2) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)(arg1->UserTime);
    (*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg2))) = (uint32_t)(ssa_mem_ld);
    uint32_t __attribute__((unused)) ssa_mem_ld_1 = (uint32_t)(arg1->KernelTime);
    return (uint32_t)(((uint64_t)(uint32_t)(ssa_mem_ld_1)));
}

/*
 * Function : KeIsWaitListEmpty @ 0x1406ce3d0 (16 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.09 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
BOOLEAN __attribute__((ms_abi)) KeIsWaitListEmpty(uint64_t* arg1) {
    uint64_t __attribute__((unused)) ssa__24 = (uint64_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(8ULL)));
    uint64_t __attribute__((unused)) ssa_mem_ld_1 = (uint64_t)((*(uint64_t*)(uintptr_t)(ssa__24)));
    return (BOOLEAN)(((uint64_t)(uint8_t)(((uint64_t)(ssa__24) == (uint64_t)(ssa_mem_ld_1)))));
}

/*
 * Function : KeTestSpinLock @ 0x1400e3050 (16 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.60 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
BOOLEAN __attribute__((ms_abi)) KeTestSpinLock(uint64_t* SpinLock) {
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)SpinLock))));
    return (BOOLEAN)(((((uint64_t)(ssa_mem_ld) == (uint64_t)(0ULL))) ? (1ULL) : (((uint64_t)(ssa_mem_ld) & (uint64_t)(0xffffffffffffff00ULL)))));
}

/*
 * Function : KeEnterCriticalRegion @ 0x1400e42a0 (17 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.77 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) KeEnterCriticalRegion(void) {
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    uint16_t __attribute__((unused)) ssa_mem_ld = (uint16_t)(ssa_gs_val->KernelApcDisable);
    ssa_gs_val->KernelApcDisable = (int16_t)(((uint16_t)(ssa_mem_ld) + (uint16_t)(0xffffffffffffffffULL)));
    return (uint64_t)(((uint64_t)(uintptr_t)ssa_gs_val));
}

/*
 * Function : KeEnterGuardedRegion @ 0x1400e50b0 (17 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.97 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) KeEnterGuardedRegion(void) {
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    uint16_t __attribute__((unused)) ssa_mem_ld = (uint16_t)(ssa_gs_val->SpecialApcDisable);
    ssa_gs_val->SpecialApcDisable = (int16_t)(((uint16_t)(ssa_mem_ld) + (uint16_t)(0xffffffffffffffffULL)));
    return (uint64_t)(((uint64_t)(uintptr_t)ssa_gs_val));
}

/*
 * Function : KeGetClockTimerResolution @ 0x1401fc8f0 (17 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.93 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) KeGetClockTimerResolution(uint32_t* arg1, uint32_t* arg2) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)&KeTimeIncrement))));
    (*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg2))) = (uint32_t)(ssa_mem_ld);
    uint32_t __attribute__((unused)) ssa_mem_ld_1 = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)&KiLastRequestedTimeIncrement))));
    (*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))) = (uint32_t)(ssa_mem_ld_1);
    return (uint32_t)(((uint64_t)(uint32_t)(ssa_mem_ld_1)));
}

/*
 * Function : KeQueryGroupAffinityEx @ 0x1401fc3f0 (17 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (20.00 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) KeQueryGroupAffinityEx(uint16_t* arg1, uint16_t arg2) {
    uint64_t __attribute__((unused)) ssa_common_ret_op = 0;
    uint16_t __attribute__((unused)) ssa_mem_ld = (uint16_t)((*(uint16_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))));
    if (((uint16_t)(ssa_mem_ld) > (uint16_t)(((uint16_t)(uint64_t)(((uint64_t)arg2)))))) {
        uint64_t __attribute__((unused)) ssa_mem_ld_1 = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(8ULL))) + (uint64_t)(((uint64_t)(((uint64_t)(((uint64_t)arg2)) << (uint64_t)(3ULL))) & (uint64_t)(0x7fff8ULL)))))));
        ssa_common_ret_op = (uint64_t)(ssa_mem_ld_1);
    }
    else {
        ssa_common_ret_op = (uint64_t)(0ULL);
    }
    return (uint64_t)(ssa_common_ret_op);
}

/*
 * Function : KeInitializeEnumerationContext @ 0x1400f82f0 (18 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (6.38 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) KeInitializeEnumerationContext(PIOV_IRP_TRACE arg1, uint64_t* arg2) {
    arg1->KernelApcDisable = (int16_t)(0ULL);
    uint64_t __attribute__((unused)) ssa_mem_ld = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)arg2)) + (uint64_t)(8ULL)))));
    arg1->Thread = (struct _KTHREAD*)(uintptr_t)(ssa_mem_ld);
    arg1->Irp = (struct _IRP*)(uintptr_t)(((uint64_t)(uintptr_t)arg2));
    return (uint64_t)(ssa_mem_ld);
}

/*
 * Function : KeAreApcsDisabled @ 0x140044d60 (20 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.23 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) KeAreApcsDisabled(void) {
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)(ssa_gs_val->CombinedApcDisable);
    return (uint64_t)(((uint64_t)(((uint64_t)(((uint64_t)(uintptr_t)ssa_gs_val)) & (uint64_t)(0xffffffffffffff00ULL))) | (uint64_t)(((uint64_t)(uint8_t)(((uint32_t)(ssa_mem_ld) != (uint32_t)(0ULL)))))));
}

/*
 * Function : KeCountSetBitsAffinityEx @ 0x140041420 (20 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.45 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) KeCountSetBitsAffinityEx(uint16_t* arg1) {
    return (uint32_t)(0ULL);
}

/*
 * Function : KeInitializeEnumerationContextFromGroup @ 0x140113290 (20 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (8.59 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) KeInitializeEnumerationContextFromGroup(PIOV_IRP_TRACE arg1, PMDL arg2) {
    struct _MDL* __attribute__((unused)) ssa_mem_ld = (struct _MDL*)(uintptr_t)(arg2->Next);
    uint16_t __attribute__((unused)) ssa_mem_ld_1 = (uint16_t)(arg2->Size);
    arg1->Irp = (struct _IRP*)(uintptr_t)(0ULL);
    arg1->KernelApcDisable = (int16_t)(ssa_mem_ld_1);
    arg1->Thread = (struct _KTHREAD*)(uintptr_t)(((uint64_t)(uintptr_t)ssa_mem_ld));
    return (uint32_t)(((uint64_t)(uint16_t)(ssa_mem_ld_1)));
}

/*
 * Function : KeIsAttachedProcess @ 0x14005acf0 (20 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.64 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) KeIsAttachedProcess(void) {
    struct _KTHREAD* __attribute__((unused)) ssa_gs_val = KeGetCurrentThread();
    uint8_t __attribute__((unused)) ssa_mem_ld = (uint8_t)(ssa_gs_val->ApcStateIndex);
    return (uint64_t)(((uint64_t)(((uint64_t)(((uint64_t)(uintptr_t)ssa_gs_val)) & (uint64_t)(0xffffffffffffff00ULL))) | (uint64_t)(((uint64_t)(uint8_t)(((uint8_t)(ssa_mem_ld) == (uint8_t)(1ULL)))))));
}

/*
 * Function : KeSetHeteroCpuPolicyThread @ 0x1401fc1d0 (20 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (3.89 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) KeSetHeteroCpuPolicyThread(uint64_t arg1, uint64_t arg2, uint32_t arg3) {
    uint64_t __attribute__((unused)) ssa_call_res = KiSetHeteroPolicyThread(((uint64_t)arg1), ((uint64_t)arg2), 0ULL, ((uint64_t)(((uint64_t)arg3)) & (uint64_t)(0xffffffffULL)));
    return (uint64_t)(ssa_call_res);
}

extern void __nt_asm_ext_1401fe1d8(void);
extern unsigned char __nt_asm_sym_1403f9146[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] KeReleaseInStackQueuedSpinLock @ 0x1400df670
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Privileged CPU/CSR instruction 'mov cr8, rax' at 0x1400df6af
 *   - Privileged/control-register routine (1/14 insns) preserved as native x86-64 assembly
 *   Live-In Registers : rbx, rcx
 *   Clobbered State   : rbx, rax, rdx, memory, cc
 */
uint32_t __attribute__((naked, ms_abi)) KeReleaseInStackQueuedSpinLock(uint64_t arg1) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "push rbx\n\t"
        "sub rsp, 0x20\n\t"
        "test byte ptr [rip + __nt_asm_sym_1403f9146], 1\n\t"
        "mov rbx, rcx\n\t"
        "jne .L_KeReleaseInStackQueuedSpinLock_1401baa78\n\t"
        ".L_KeReleaseInStackQueuedSpinLock_1400df6ab:\n\t"
        "movzx eax, byte ptr [rbx + 0x10]\n\t"
        "mov cr8, rax\n\t"
        "add rsp, 0x20\n\t"
        "pop rbx\n\t"
        "ret\n\t"
        ".L_KeReleaseInStackQueuedSpinLock_1401baa78:\n\t"
        "mov rdx, qword ptr [rsp + 0x28]\n\t"
        "call __nt_asm_ext_1401fe1d8\n\t"
        "nop\n\t"
        "jmp .L_KeReleaseInStackQueuedSpinLock_1400df6ab\n\t"
        ".att_syntax prefix"
    );
}

/*
 * Function : KeGetCurrentNodeNumber @ 0x1400edbd0 (24 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (7.13 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) KeGetCurrentNodeNumber(void) {
    struct _KPRCB* __attribute__((unused)) ssa_gs_val = KeGetCurrentPrcb();
    struct _KNODE* __attribute__((unused)) ssa_mem_ld = (struct _KNODE*)(uintptr_t)(ssa_gs_val->ParentNode);
    uint16_t __attribute__((unused)) ssa_mem_ld_1 = (uint16_t)(ssa_mem_ld->NodeNumber);
    return (uint64_t)(((uint64_t)(uint16_t)(ssa_mem_ld_1)));
}

/*
 * Function : KeInitializeThreadedDpc @ 0x1401411a0 (25 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (6.99 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) KeInitializeThreadedDpc(PKDPC arg1, uint64_t arg2, uint64_t arg3) {
    arg1->TargetInfoAsUlong = (uint32_t)(0x11aULL);
    arg1->DpcData = (void*)(uintptr_t)(0ULL);
    arg1->ProcessorHistory = (uint64_t)(0ULL);
    arg1->DeferredRoutine = (void**)(uintptr_t)(((uint64_t)arg2));
    arg1->DeferredContext = (void*)(uintptr_t)(((uint64_t)arg3));
    return (uint64_t)(0ULL);
}

/*
 * Function : KeSetTimer @ 0x140037910 (25 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.03 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) KeSetTimer(uint64_t arg1, uint64_t arg2, uint64_t arg3) {
    uint64_t __attribute__((unused)) ssa_call_res = (uint64_t)KiSetTimerEx((void*)(uintptr_t)(((uint64_t)arg1)), (int64_t)(((uint64_t)arg2)), (ULONG)(0ULL), (uint64_t)(0ULL));
    return (uint64_t)(ssa_call_res);
}

/*
 * Function : KeAddProcessorGroupAffinity @ 0x1401393e0 (26 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (24.26 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) KeAddProcessorGroupAffinity(uint64_t* arg1, uint32_t* arg2) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)&KiProcessorIndexToNumberMappingTable)) + (uint64_t)(((uint64_t)(((uint64_t)(((uint64_t)(uintptr_t)arg2)) << (uint64_t)(2ULL))) & (uint64_t)(0x3fffffffcULL)))))));
    uint64_t __attribute__((unused)) ssa_mem_ld_1 = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))));
    uint64_t __attribute__((unused)) ssa__49 = (uint64_t)(((uint64_t)(((uint64_t)(1ULL) << (uint64_t)(((uint64_t)(uint32_t)(((uint32_t)(ssa_mem_ld) & (uint32_t)(0x3fULL))))))) | (uint64_t)(ssa_mem_ld_1)));
    (*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))) = (uint64_t)(ssa__49);
    return (uint64_t)(ssa__49);
}

extern void __nt_asm_ext_1400665c4(void);

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] KeGetEffectiveIrql @ 0x140065390
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Privileged CPU/CSR instruction 'mov rax, cr8' at 0x14006539d
 *   - Privileged/control-register routine (1/9 insns) preserved as native x86-64 assembly
 *   Live-In Registers : rax
 *   Clobbered State   : rax, memory, cc
 */
uint64_t __attribute__((naked, ms_abi)) KeGetEffectiveIrql(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "sub rsp, 0x28\n\t"
        "call __nt_asm_ext_1400665c4\n\t"
        "test al, al\n\t"
        "je .L_KeGetEffectiveIrql_1400653a6\n\t"
        ".L_KeGetEffectiveIrql_14006539d:\n\t"
        "mov rax, cr8\n\t"
        ".L_KeGetEffectiveIrql_1400653a1:\n\t"
        "add rsp, 0x28\n\t"
        "ret\n\t"
        ".L_KeGetEffectiveIrql_1400653a6:\n\t"
        "mov al, 0xf\n\t"
        "jmp .L_KeGetEffectiveIrql_1400653a1\n\t"
        ".att_syntax prefix"
    );
}

/*
 * Function : KeInitializeSemaphore @ 0x140038800 (26 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (6.58 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) KeInitializeSemaphore(struct _RECON_KEINITIALIZESEMAPHORE_RCX* arg1, uint32_t arg2, uint32_t arg3) {
    uint64_t __attribute__((unused)) ssa__26 = (uint64_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) + (uint64_t)(8ULL)));
    arg1->field_0x0 = (uint8_t)(5ULL);
    arg1->field_0x2 = (uint8_t)(8ULL);
    arg1->field_0x4 = (uint32_t)(((uint32_t)(uint64_t)(((uint64_t)arg2))));
    (*(uint64_t*)((uint8_t*)(uintptr_t)(arg1) + 0x10)) = (uint64_t)(ssa__26);
    arg1->field_0x8 = (uint64_t)(ssa__26);
    arg1->field_0x18 = (uint32_t)(((uint32_t)(uint64_t)(((uint64_t)arg3))));
    return (uint64_t)(ssa__26);
}

/*
 * Function : KeRemoveProcessorGroupAffinity @ 0x1401fc430 (26 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (23.47 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) KeRemoveProcessorGroupAffinity(uint64_t* arg1, uint32_t* arg2) {
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)&KiProcessorIndexToNumberMappingTable)) + (uint64_t)(((uint64_t)(((uint64_t)(((uint64_t)(uintptr_t)arg2)) << (uint64_t)(2ULL))) & (uint64_t)(0x3fffffffcULL)))))));
    uint64_t __attribute__((unused)) ssa_mem_ld_1 = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))));
    uint64_t __attribute__((unused)) ssa__50 = (uint64_t)(((uint64_t)(ssa_mem_ld_1) & (uint64_t)(((uint64_t)(((uint64_t)(1ULL) << (uint64_t)(((uint64_t)(uint32_t)(((uint32_t)(ssa_mem_ld) & (uint32_t)(0x3fULL))))))) ^ (uint64_t)(0xffffffffffffffffULL)))));
    (*(uint64_t*)(uintptr_t)(((uint64_t)(uintptr_t)arg1))) = (uint64_t)(ssa__50);
    return (uint64_t)(ssa__50);
}

/*
 * Function : KeInitializeEvent @ 0x140055920 (27 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (6.44 ms, 34 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
void __attribute__((ms_abi)) KeInitializeEvent(struct _KEVENT* Event, uint32_t Type, BOOLEAN State) {
    Event->Header.SignalState = (int32_t)(((uint32_t)(((uint32_t)(uint64_t)(((uint64_t)State)))) & (uint32_t)(0xffULL)));
    uint64_t __attribute__((unused)) ssa__32 = (uint64_t)(((uint64_t)(((uint64_t)(uintptr_t)Event)) + (uint64_t)(8ULL)));
    Event->Header.Type = (uint8_t)(((uint8_t)(uint64_t)(((uint64_t)Type))));
    (*(uint16_t*)((uint8_t*)(uintptr_t)(Event) + 0x1)) = (uint16_t)(0x600ULL);
    Event->Header.WaitListHead.Blink = (struct _LIST_ENTRY*)(uintptr_t)(ssa__32);
    Event->Header.WaitListHead.Flink = (struct _LIST_ENTRY*)(uintptr_t)(ssa__32);
    return;
}

/*
 * Function : KeQueryActiveProcessorAffinity @ 0x14011e8b0 (27 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (4.29 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) KeQueryActiveProcessorAffinity(void) {
    uint64_t __attribute__((unused)) ssa_call_res = KeCopyAffinityEx(0ULL, ((uint64_t)(uintptr_t)&KeActiveProcessors), 0ULL, 0ULL);
    uint32_t __attribute__((unused)) ssa_mem_ld = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)&KeNumberProcessors))));
    return (uint32_t)(((uint64_t)(uint32_t)(ssa_mem_ld)));
}

/*
 * Function : KeQueryGroupAffinity @ 0x14004cbb0 (27 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (18.02 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) KeQueryGroupAffinity(uint64_t* arg1) {
    uint64_t __attribute__((unused)) ssa_common_ret_op = 0;
    uint16_t __attribute__((unused)) ssa_mem_ld = (uint16_t)((*(uint16_t*)(uintptr_t)(((uint64_t)(uintptr_t)&KeActiveProcessors))));
    if (((uint16_t)(ssa_mem_ld) > (uint16_t)(((uint16_t)(uint64_t)(((uint64_t)(uintptr_t)arg1)))))) {
        uint64_t __attribute__((unused)) ssa_mem_ld_1 = (uint64_t)((*(uint64_t*)(uintptr_t)(((uint64_t)(((uint64_t)(uintptr_t)&__nt_rodata_1403f93e8)) + (uint64_t)(((uint64_t)(((uint64_t)(((uint64_t)(uintptr_t)arg1)) << (uint64_t)(3ULL))) & (uint64_t)(0x7fff8ULL)))))));
        ssa_common_ret_op = (uint64_t)(ssa_mem_ld_1);
    }
    else {
        ssa_common_ret_op = (uint64_t)(0ULL);
    }
    return (uint64_t)(ssa_common_ret_op);
}

/*
 * Function : KeInitializeTimer @ 0x14010d030 (28 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (7.44 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) KeInitializeTimer(PPEB arg1) {
    uint64_t __attribute__((unused)) ssa__32 = (uint64_t)(((uint64_t)(((uint64_t)arg1)) + (uint64_t)(8ULL)));
    (*(uint64_t*)((uint8_t*)(uintptr_t)(arg1))) = (uint64_t)(8ULL);
    arg1->ImageBaseAddress = (void*)(uintptr_t)(ssa__32);
    arg1->Mutant = (void*)(uintptr_t)(ssa__32);
    arg1->Ldr = (struct _PEB_LDR_DATA*)(uintptr_t)(0ULL);
    arg1->FastPebLock = (struct _RTL_CRITICAL_SECTION*)(uintptr_t)(0ULL);
    return (uint64_t)(ssa__32);
}

/*
 * Function : KeQueryHeteroCpuPolicyThread @ 0x1401fbf40 (28 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (5.16 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint32_t __attribute__((ms_abi)) KeQueryHeteroCpuPolicyThread(PKTHREAD arg1, uint32_t arg2) {
    uint64_t __attribute__((unused)) ssa_common_ret_op = 0;
    uint8_t __attribute__((unused)) ssa_reg_rax_0_in = 0;
    if (((uint32_t)(((uint32_t)(uint64_t)(((uint64_t)arg2)))) == (uint32_t)(0ULL))) {
        goto L_bb_1401fbf57;
    }
    uint8_t __attribute__((unused)) ssa_mem_ld = (uint8_t)((*(uint8_t*)((uint8_t*)(uintptr_t)(arg1) + 0x7e)));
    uint8_t __attribute__((unused)) ssa_v_0 = (uint8_t)(((uint8_t)(ssa_mem_ld) & (uint8_t)(0x7fULL)));
    if (((uint8_t)(ssa_v_0) == (uint8_t)(8ULL))) {
        goto L_bb_1401fbf50;
    }
    ssa_reg_rax_0_in = (uint8_t)(ssa_v_0);
    goto L_bb_1401fbf5b;
L_common_ret:
    return (uint32_t)(ssa_common_ret_op);
L_bb_1401fbf50:
    uint32_t __attribute__((unused)) ssa_mem_ld_1 = (uint32_t)((*(uint32_t*)(uintptr_t)(((uint64_t)(uintptr_t)&KiDefaultHeteroCpuPolicy))));
    ssa_common_ret_op = (uint64_t)(((uint64_t)(uint32_t)(ssa_mem_ld_1)));
    goto L_common_ret;
L_bb_1401fbf57:
    uint8_t __attribute__((unused)) ssa_mem_ld_2 = (uint8_t)(arg1->SystemHeteroCpuPolicy);
    ssa_reg_rax_0_in = (uint8_t)(ssa_mem_ld_2);
L_bb_1401fbf5b:
    ssa_common_ret_op = (uint64_t)(((uint64_t)(uint8_t)(ssa_reg_rax_0_in)));
    goto L_common_ret;
}

/*
 * Function : KeInitializeTimerEx @ 0x140038460 (31 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (7.69 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) KeInitializeTimerEx(PPEB arg1, uint8_t arg2) {
    uint64_t __attribute__((unused)) ssa__33 = (uint64_t)(((uint64_t)(((uint64_t)arg1)) + (uint64_t)(8ULL)));
    (*(uint64_t*)((uint8_t*)(uintptr_t)(arg1))) = (uint64_t)(0ULL);
    arg1->InheritedAddressSpace = (uint8_t)(((uint8_t)(((uint8_t)(uint64_t)(((uint64_t)arg2)))) + (uint8_t)(8ULL)));
    arg1->ImageBaseAddress = (void*)(uintptr_t)(ssa__33);
    arg1->Mutant = (void*)(uintptr_t)(ssa__33);
    arg1->Ldr = (struct _PEB_LDR_DATA*)(uintptr_t)(0ULL);
    arg1->FastPebLock = (struct _RTL_CRITICAL_SECTION*)(uintptr_t)(0ULL);
    return (uint64_t)(ssa__33);
}

extern void __nt_asm_ext_14001db90(void);
extern void __nt_asm_ext_140084d90(void);
extern void __nt_asm_ext_1400879b0(void);
extern void __nt_asm_ext_14005a1c0(void);
extern void __nt_asm_ext_14005a040(void);
extern void __nt_asm_ext_1401fbe54(void);
extern void __nt_asm_ext_140272d0c(void);
extern unsigned char __nt_asm_sym_1403f9144[];

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] KeSetEvent @ 0x140084960
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Privileged CPU/CSR instruction 'mov r12, cr8' at 0x140084996
 *   - Privileged CPU/CSR instruction 'mov cr8, rax' at 0x14008499f
 *   - Privileged CPU/CSR instruction 'mov rax, cr8' at 0x140084b16
 *   - Privileged CPU/CSR instruction 'mov cr8, rax' at 0x140084b1f
 *   - Privileged CPU/CSR instruction 'mov rax, cr8' at 0x140084c4e
 *   - Privileged CPU/CSR instruction 'mov cr8, rax' at 0x140084c57
 *   - Privileged/control-register routine (6/264 insns) preserved as native x86-64 assembly
 *   Live-In Registers : rbx, rbp, rdi, r14, rcx, r8, rdx, rsi, r12, r13, r15
 *   Clobbered State   : rbx, rbp, rdi, r14, r12, rax, r15, rsi, r13, r9, r8, rdx, rcx, r10, memory, cc
 */
uint64_t __attribute__((naked, ms_abi)) KeSetEvent(struct _RECON_KESETEVENT_RCX* arg1, struct _RECON_KESETEVENT_RDX* arg2, struct _RECON_KESETEVENT_R8* arg3) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "push rbx\n\t"
        "push rbp\n\t"
        "push rdi\n\t"
        "push r14\n\t"
        "sub rsp, 0x68\n\t"
        "test byte ptr [rcx], 0x7f\n\t"
        "movzx edi, r8b\n\t"
        "mov ebp, edx\n\t"
        "mov rbx, rcx\n\t"
        "je .L_KeSetEvent_140084a19\n\t"
        ".L_KeSetEvent_14008497c:\n\t"
        "xor r14d, r14d\n\t"
        ".L_KeSetEvent_14008497f:\n\t"
        "mov qword ptr [rsp + 0x98], rsi\n\t"
        "mov qword ptr [rsp + 0x60], r12\n\t"
        "mov qword ptr [rsp + 0x58], r13\n\t"
        "mov qword ptr [rsp + 0x50], r15\n\t"
        "mov r12, cr8\n\t"
        "mov eax, 2\n\t"
        "mov cr8, rax\n\t"
        "mov r15, qword ptr gs:[0x20]\n\t"
        "call __nt_asm_ext_14001db90\n\t"
        "mov esi, dword ptr [rbx + 4]\n\t"
        "mov dword ptr [rbx + 4], 1\n\t"
        "test esi, esi\n\t"
        "jne .L_KeSetEvent_1400849d1\n\t"
        ".L_KeSetEvent_1400849bf:\n\t"
        "mov r13, qword ptr [rbx + 8]\n\t"
        "test r14d, r14d\n\t"
        "lea r14, [rbx + 8]\n\t"
        "jne .L_KeSetEvent_140084a2e\n\t"
        ".L_KeSetEvent_1400849cc:\n\t"
        "cmp r13, r14\n\t"
        "jne .L_KeSetEvent_140084a40\n\t"
        ".L_KeSetEvent_1400849d1:\n\t"
        "lock and dword ptr [rbx], 0xffffff7f\n\t"
        "neg dil\n\t"
        "mov byte ptr [rsp + 0x20], r12b\n\t"
        "mov r9d, ebp\n\t"
        "mov r8d, 1\n\t"
        "sbb edx, edx\n\t"
        "mov rcx, r15\n\t"
        "and edx, 3\n\t"
        "call __nt_asm_ext_140084d90\n\t"
        "mov r13, qword ptr [rsp + 0x58]\n\t"
        "mov eax, esi\n\t"
        "mov rsi, qword ptr [rsp + 0x98]\n\t"
        "mov r12, qword ptr [rsp + 0x60]\n\t"
        "mov r15, qword ptr [rsp + 0x50]\n\t"
        "add rsp, 0x68\n\t"
        "pop r14\n\t"
        "pop rdi\n\t"
        "pop rbp\n\t"
        "pop rbx\n\t"
        "ret\n\t"
        ".L_KeSetEvent_140084a19:\n\t"
        "cmp dword ptr [rcx + 4], 1\n\t"
        "je .L_KeSetEvent_140084ad3\n\t"
        ".L_KeSetEvent_140084a23:\n\t"
        "mov r14d, 1\n\t"
        "jmp .L_KeSetEvent_14008497f\n\t"
        ".L_KeSetEvent_140084a2e:\n\t"
        "cmp r13, r14\n\t"
        "jne .L_KeSetEvent_140084aa0\n\t"
        ".L_KeSetEvent_140084a33:\n\t"
        "mov qword ptr [r14 + 8], r14\n\t"
        "mov qword ptr [r14], r14\n\t"
        "jmp .L_KeSetEvent_1400849d1\n\t"
        "nop dword ptr [rax]\n\t"
        ".L_KeSetEvent_140084a40:\n\t"
        "mov rdx, r13\n\t"
        "mov r13, qword ptr [r13]\n\t"
        "mov qword ptr [rsp + 0x30], rdx\n\t"
        "mov rcx, qword ptr [rdx]\n\t"
        "mov rax, qword ptr [rdx + 8]\n\t"
        "cmp qword ptr [rcx + 8], rdx\n\t"
        "jne .L_KeSetEvent_14019e9fb\n\t"
        ".L_KeSetEvent_140084a5d:\n\t"
        "cmp qword ptr [rax], rdx\n\t"
        "jne .L_KeSetEvent_14019e9fb\n\t"
        ".L_KeSetEvent_140084a66:\n\t"
        "mov qword ptr [rax], rcx\n\t"
        "mov qword ptr [rcx + 8], rax\n\t"
        "movzx eax, byte ptr [rdx + 0x10]\n\t"
        "cmp al, 1\n\t"
        "jne .L_KeSetEvent_140084aeb\n\t"
        ".L_KeSetEvent_140084a75:\n\t"
        "movzx r8d, word ptr [rdx + 0x12]\n\t"
        "xor r9d, r9d\n\t"
        "mov rcx, r15\n\t"
        "call __nt_asm_ext_1400879b0\n\t"
        "test al, al\n\t"
        "je .L_KeSetEvent_140084cfc\n\t"
        ".L_KeSetEvent_140084a8d:\n\t"
        "sub dword ptr [rbx + 4], 1\n\t"
        "je .L_KeSetEvent_1400849d1\n\t"
        ".L_KeSetEvent_140084a97:\n\t"
        "jmp .L_KeSetEvent_140084cfc\n\t"
        "nop dword ptr [rax]\n\t"
        ".L_KeSetEvent_140084aa0:\n\t"
        "mov rdx, r13\n\t"
        "mov r13, qword ptr [r13]\n\t"
        "mov qword ptr [rsp + 0x38], rdx\n\t"
        "movzx eax, byte ptr [rdx + 0x10]\n\t"
        "cmp al, 1\n\t"
        "jne .L_KeSetEvent_140084c23\n\t"
        ".L_KeSetEvent_140084ab8:\n\t"
        "movzx r8d, word ptr [rdx + 0x12]\n\t"
        ".L_KeSetEvent_140084abd:\n\t"
        "xor r9d, r9d\n\t"
        "mov rcx, r15\n\t"
        "call __nt_asm_ext_1400879b0\n\t"
        ".L_KeSetEvent_140084ac8:\n\t"
        "cmp r13, r14\n\t"
        "je .L_KeSetEvent_140084a33\n\t"
        ".L_KeSetEvent_140084ad1:\n\t"
        "jmp .L_KeSetEvent_140084aa0\n\t"
        ".L_KeSetEvent_140084ad3:\n\t"
        "test dil, dil\n\t"
        "jne .L_KeSetEvent_140084a23\n\t"
        ".L_KeSetEvent_140084adc:\n\t"
        "mov eax, 1\n\t"
        "add rsp, 0x68\n\t"
        "pop r14\n\t"
        "pop rdi\n\t"
        "pop rbp\n\t"
        "pop rbx\n\t"
        "ret\n\t"
        ".L_KeSetEvent_140084aeb:\n\t"
        "cmp al, 2\n\t"
        "jne .L_KeSetEvent_140084ceb\n\t"
        ".L_KeSetEvent_140084af3:\n\t"
        "mov byte ptr [rdx + 0x11], 5\n\t"
        "mov rax, qword ptr [rdx + 0x18]\n\t"
        "mov qword ptr [rsp + 0x90], rax\n\t"
        "add rax, 8\n\t"
        "mov qword ptr [rsp + 0xa8], rax\n\t"
        "mov qword ptr [rdx], 0\n\t"
        "mov rax, cr8\n\t"
        "mov eax, 2\n\t"
        "mov cr8, rax\n\t"
        "mov rax, qword ptr gs:[0x20]\n\t"
        "test dword ptr [rip + __nt_asm_sym_1403f9144], 0x1000000\n\t"
        "mov qword ptr [rsp + 0x38], rax\n\t"
        "mov r9, qword ptr [rax + 8]\n\t"
        "mov qword ptr [rsp + 0x40], r9\n\t"
        "jne .L_KeSetEvent_14019e9da\n\t"
        ".L_KeSetEvent_140084b4a:\n\t"
        "mov rcx, qword ptr [rsp + 0x90]\n\t"
        "call __nt_asm_ext_14001db90\n\t"
        "mov rdx, qword ptr [rsp + 0xa8]\n\t"
        "mov rcx, qword ptr [rsp + 0x90]\n\t"
        "cmp qword ptr [rdx], rdx\n\t"
        "je .L_KeSetEvent_140084baf\n\t"
        ".L_KeSetEvent_140084b6c:\n\t"
        "mov eax, dword ptr [rcx + 0x28]\n\t"
        "cmp eax, dword ptr [rcx + 0x2c]\n\t"
        "jae .L_KeSetEvent_140084baf\n\t"
        ".L_KeSetEvent_140084b74:\n\t"
        "mov r8, qword ptr [rsp + 0x40]\n\t"
        "mov rax, qword ptr [r8 + 0xe8]\n\t"
        "cmp rax, rcx\n\t"
        "je .L_KeSetEvent_140084c13\n\t"
        ".L_KeSetEvent_140084b89:\n\t"
        "mov r8, qword ptr [rsp + 0x30]\n\t"
        "mov rdx, rcx\n\t"
        "mov rcx, qword ptr [rsp + 0x38]\n\t"
        "call __nt_asm_ext_14005a1c0\n\t"
        "mov rcx, qword ptr [rsp + 0x90]\n\t"
        "test al, al\n\t"
        "jne .L_KeSetEvent_140084bfd\n\t"
        ".L_KeSetEvent_140084ba7:\n\t"
        "mov rdx, qword ptr [rsp + 0xa8]\n\t"
        ".L_KeSetEvent_140084baf:\n\t"
        "mov r10d, dword ptr [rcx + 4]\n\t"
        "lea r8, [rcx + 0x18]\n\t"
        "lea eax, [r10 + 1]\n\t"
        "mov dword ptr [rcx + 4], eax\n\t"
        "mov r9, qword ptr [r8 + 8]\n\t"
        "cmp qword ptr [r9], r8\n\t"
        "jne .L_KeSetEvent_14019e9f4\n\t"
        ".L_KeSetEvent_140084bcb:\n\t"
        "mov rax, qword ptr [rsp + 0x30]\n\t"
        "mov qword ptr [rax], r8\n\t"
        "mov qword ptr [rax + 8], r9\n\t"
        "mov qword ptr [r9], rax\n\t"
        "mov qword ptr [r8 + 8], rax\n\t"
        "test r10d, r10d\n\t"
        "jne .L_KeSetEvent_140084bfd\n\t"
        ".L_KeSetEvent_140084be3:\n\t"
        "cmp qword ptr [rdx], rdx\n\t"
        "je .L_KeSetEvent_140084bfd\n\t"
        ".L_KeSetEvent_140084be8:\n\t"
        "mov rdx, rcx\n\t"
        "mov rcx, qword ptr [rsp + 0x38]\n\t"
        "call __nt_asm_ext_14005a040\n\t"
        "mov rcx, qword ptr [rsp + 0x90]\n\t"
        ".L_KeSetEvent_140084bfd:\n\t"
        "lock and dword ptr [rcx], 0xffffff7f\n\t"
        "sub dword ptr [rbx + 4], 1\n\t"
        "je .L_KeSetEvent_1400849d1\n\t"
        ".L_KeSetEvent_140084c0e:\n\t"
        "jmp .L_KeSetEvent_140084cfc\n\t"
        ".L_KeSetEvent_140084c13:\n\t"
        "cmp byte ptr [r8 + 0x283], 0xf\n\t"
        "jne .L_KeSetEvent_140084b89\n\t"
        ".L_KeSetEvent_140084c21:\n\t"
        "jmp .L_KeSetEvent_140084baf\n\t"
        ".L_KeSetEvent_140084c23:\n\t"
        "cmp al, 2\n\t"
        "jne .L_KeSetEvent_140084d15\n\t"
        ".L_KeSetEvent_140084c2b:\n\t"
        "mov byte ptr [rdx + 0x11], 5\n\t"
        "mov rax, qword ptr [rdx + 0x18]\n\t"
        "mov qword ptr [rsp + 0x90], rax\n\t"
        "add rax, 8\n\t"
        "mov qword ptr [rsp + 0xa8], rax\n\t"
        "mov qword ptr [rdx], 0\n\t"
        "mov rax, cr8\n\t"
        "mov eax, 2\n\t"
        "mov cr8, rax\n\t"
        "mov rax, qword ptr gs:[0x20]\n\t"
        "test dword ptr [rip + __nt_asm_sym_1403f9144], 0x1000000\n\t"
        "mov qword ptr [rsp + 0x30], rax\n\t"
        "mov r9, qword ptr [rax + 8]\n\t"
        "mov qword ptr [rsp + 0x40], r9\n\t"
        "jne .L_KeSetEvent_14019ea02\n\t"
        ".L_KeSetEvent_140084c82:\n\t"
        "mov rcx, qword ptr [rsp + 0x90]\n\t"
        "call __nt_asm_ext_14001db90\n\t"
        "mov r8, qword ptr [rsp + 0xa8]\n\t"
        "mov rcx, qword ptr [rsp + 0x90]\n\t"
        "cmp qword ptr [r8], r8\n\t"
        "je .L_KeSetEvent_140084d28\n\t"
        ".L_KeSetEvent_140084ca8:\n\t"
        "mov eax, dword ptr [rcx + 0x28]\n\t"
        "cmp eax, dword ptr [rcx + 0x2c]\n\t"
        "jae .L_KeSetEvent_140084d28\n\t"
        ".L_KeSetEvent_140084cb0:\n\t"
        "mov rdx, qword ptr [rsp + 0x40]\n\t"
        "mov rax, qword ptr [rdx + 0xe8]\n\t"
        "cmp rax, rcx\n\t"
        "je .L_KeSetEvent_140084d0a\n\t"
        ".L_KeSetEvent_140084cc1:\n\t"
        "mov r8, qword ptr [rsp + 0x38]\n\t"
        "mov rdx, rcx\n\t"
        "mov rcx, qword ptr [rsp + 0x30]\n\t"
        "call __nt_asm_ext_14005a1c0\n\t"
        "mov rcx, qword ptr [rsp + 0x90]\n\t"
        "test al, al\n\t"
        "je .L_KeSetEvent_140084d20\n\t"
        ".L_KeSetEvent_140084cdf:\n\t"
        "lock and dword ptr [rcx], 0xffffff7f\n\t"
        "jmp .L_KeSetEvent_140084ac8\n\t"
        ".L_KeSetEvent_140084ceb:\n\t"
        "xor r9d, r9d\n\t"
        "mov r8d, 0x100\n\t"
        "mov rcx, r15\n\t"
        "call __nt_asm_ext_1400879b0\n\t"
        ".L_KeSetEvent_140084cfc:\n\t"
        "cmp r13, r14\n\t"
        "je .L_KeSetEvent_1400849d1\n\t"
        ".L_KeSetEvent_140084d05:\n\t"
        "jmp .L_KeSetEvent_140084a40\n\t"
        ".L_KeSetEvent_140084d0a:\n\t"
        "cmp byte ptr [rdx + 0x283], 0xf\n\t"
        "jne .L_KeSetEvent_140084cc1\n\t"
        ".L_KeSetEvent_140084d13:\n\t"
        "jmp .L_KeSetEvent_140084d28\n\t"
        ".L_KeSetEvent_140084d15:\n\t"
        "mov r8d, 0x100\n\t"
        "jmp .L_KeSetEvent_140084abd\n\t"
        ".L_KeSetEvent_140084d20:\n\t"
        "mov r8, qword ptr [rsp + 0xa8]\n\t"
        ".L_KeSetEvent_140084d28:\n\t"
        "mov r10d, dword ptr [rcx + 4]\n\t"
        "lea rdx, [rcx + 0x18]\n\t"
        "lea eax, [r10 + 1]\n\t"
        "mov dword ptr [rcx + 4], eax\n\t"
        "mov r9, qword ptr [rdx + 8]\n\t"
        "cmp qword ptr [r9], rdx\n\t"
        "jne .L_KeSetEvent_14019ea1c\n\t"
        ".L_KeSetEvent_140084d44:\n\t"
        "mov rax, qword ptr [rsp + 0x38]\n\t"
        "mov qword ptr [rax], rdx\n\t"
        "mov qword ptr [rax + 8], r9\n\t"
        "mov qword ptr [r9], rax\n\t"
        "mov qword ptr [rdx + 8], rax\n\t"
        "test r10d, r10d\n\t"
        "jne .L_KeSetEvent_140084cdf\n\t"
        ".L_KeSetEvent_140084d5c:\n\t"
        "cmp qword ptr [r8], r8\n\t"
        "je .L_KeSetEvent_140084cdf\n\t"
        ".L_KeSetEvent_140084d65:\n\t"
        "mov rdx, rcx\n\t"
        "mov rcx, qword ptr [rsp + 0x30]\n\t"
        "call __nt_asm_ext_14005a040\n\t"
        "mov rcx, qword ptr [rsp + 0x90]\n\t"
        "jmp .L_KeSetEvent_140084cdf\n\t"
        ".L_KeSetEvent_14019e9da:\n\t"
        "mov rcx, r9\n\t"
        "call __nt_asm_ext_1401fbe54\n\t"
        "movzx r8d, al\n\t"
        "mov rcx, r9\n\t"
        "call __nt_asm_ext_140272d0c\n\t"
        "nop\n\t"
        "jmp .L_KeSetEvent_140084b4a\n\t"
        ".L_KeSetEvent_14019e9f4:\n\t"
        "mov ecx, 3\n\t"
        "int 0x29\n\t"
        ".L_KeSetEvent_14019e9fb:\n\t"
        "mov ecx, 3\n\t"
        "int 0x29\n\t"
        ".L_KeSetEvent_14019ea02:\n\t"
        "mov rcx, r9\n\t"
        "call __nt_asm_ext_1401fbe54\n\t"
        "movzx r8d, al\n\t"
        "mov rcx, r9\n\t"
        "call __nt_asm_ext_140272d0c\n\t"
        "nop\n\t"
        "jmp .L_KeSetEvent_140084c82\n\t"
        ".L_KeSetEvent_14019ea1c:\n\t"
        "mov ecx, 3\n\t"
        "int 0x29\n\t"
        ".att_syntax prefix"
    );
}

extern void __nt_asm_ext_140036930(void);

/*
 * [PRESERVED NT KERNEL ASSEMBLY ROUTINE] KeAcquireSpinLockRaiseToSynch @ 0x1401fe400
 * Automated non-C detection prevented destructive C structural forcing:
 *   - Privileged CPU/CSR instruction 'mov rbx, cr8' at 0x1401fe406
 *   - Privileged CPU/CSR instruction 'mov cr8, rax' at 0x1401fe40f
 *   - Privileged/control-register routine (2/10 insns) preserved as native x86-64 assembly
 *   Live-In Registers : rbx
 *   Clobbered State   : rbx, rax, memory, cc
 */
uint32_t __attribute__((naked, ms_abi)) KeAcquireSpinLockRaiseToSynch(void) {
    __asm__ __volatile__ (
        ".intel_syntax noprefix\n\t"
        "push rbx\n\t"
        "sub rsp, 0x20\n\t"
        "mov rbx, cr8\n\t"
        "mov eax, 0xc\n\t"
        "mov cr8, rax\n\t"
        "call __nt_asm_ext_140036930\n\t"
        "mov al, bl\n\t"
        "add rsp, 0x20\n\t"
        "pop rbx\n\t"
        "ret\n\t"
        ".att_syntax prefix"
    );
}

/*
 * Function : KeExpandKernelStackAndCalloutEx @ 0x1400ccdb0 (34 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (11.60 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) KeExpandKernelStackAndCalloutEx(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint32_t arg4) {
    uint64_t __attribute__((unused)) ssa_call_res = KeExpandKernelStackAndCalloutInternal(((uint64_t)arg1), ((uint64_t)arg2), ((uint64_t)arg3), ((((uint8_t)(((uint8_t)(uint64_t)(((uint64_t)arg4)))) == (uint8_t)(0ULL))) ? (0ULL) : (2ULL)));
    return (uint64_t)(ssa_call_res);
}

/*
 * Function : KeInitializeDeviceQueue @ 0x140025a70 (35 bytes)
 * Z3 Proof : VERIFIED_EQUIVALENT (8.27 ms, 33 SMT assertions)
 * Verified : win64_callee_saved_abi, x86_64_to_llvm_ir_equivalence
 */
uint64_t __attribute__((ms_abi)) KeInitializeDeviceQueue(PKDPC arg1) {
    uint64_t __attribute__((unused)) ssa__24 = (uint64_t)(((uint64_t)(((uint64_t)arg1)) + (uint64_t)(8ULL)));
    arg1->TargetInfoAsUlong = (uint32_t)(0x280014ULL);
    arg1->ProcessorHistory = (uint64_t)(ssa__24);
    arg1->DpcListEntry.Next = (struct _SINGLE_LIST_ENTRY*)(uintptr_t)(ssa__24);
    arg1->DeferredRoutine = (void**)(uintptr_t)(0ULL);
    (*(uint8_t*)((uint8_t*)(uintptr_t)(arg1) + 0x20)) = (uint8_t)(0ULL);
    uint64_t __attribute__((unused)) ssa_mem_ld_1 = (uint64_t)(uintptr_t)(arg1->DeferredContext);
    arg1->DeferredContext = (void*)(uintptr_t)(((uint64_t)(ssa_mem_ld_1) & (uint64_t)(0xffULL)));
    return (uint64_t)(ssa__24);
}
