/*
 * Bare-Metal x86-64 Multiboot2 + Multiboot1 Dual-Header Ring-0 Bootstrap
 * Transitions physical x86-64 hardware from 32-bit Protected Mode (GRUB2 / PXE / Coreboot)
 * into 64-bit Long Mode with 4 GiB 2MiB-page identity paging, SSE/FXSR, and calls
 * `nt_baremetal_main` (Microsoft x64 ABI).
 */

    .section .multiboot, "a"
    .align 8

/* Multiboot2 Header (GRUB2 `multiboot2 /KERNEL/NT_HYBRID.ELF`) */
mb2_hdr_start:
    .long 0xE85250D6                                  /* Multiboot2 magic */
    .long 0                                           /* Architecture: i386/x86 */
    .long (mb2_hdr_end - mb2_hdr_start)               /* Header length */
    .long -(0xE85250D6 + 0 + (mb2_hdr_end - mb2_hdr_start))
    /* End tag */
    .short 0
    .short 0
    .long 8
mb2_hdr_end:

    .align 4
/* Multiboot1 Header (`multiboot` / QEMU `-kernel` / SeaBIOS) */
    .long 0x1BADB002
    .long 0x00000003
    .long -(0x1BADB002 + 0x00000003)

    .section .text.boot, "ax"
    .code32
    .globl _start
_start:
    cli
    cld
    movl $boot_stack_top, %esp

    /* Zero 6 x 4KiB page-table frames (PML4, PDPT, 4 x PD covering 0..4 GiB) */
    movl $boot_pml4, %edi
    xorl %eax, %eax
    movl $(6 * 4096 / 4), %ecx
    rep stosl

    /* PML4[0] -> boot_pdpt (Present | Read/Write) */
    movl $boot_pdpt, %eax
    orl  $0x03, %eax
    movl %eax, boot_pml4

    /* PDPT[0..3] -> boot_pd0..3 (4 x 1 GiB = 0..4 GiB physical identity map) */
    movl $boot_pd0, %eax
    orl  $0x03, %eax
    movl %eax, boot_pdpt + 0
    addl $0x1000, %eax
    movl %eax, boot_pdpt + 8
    addl $0x1000, %eax
    movl %eax, boot_pdpt + 16
    addl $0x1000, %eax
    movl %eax, boot_pdpt + 24

    /* Populate 2048 x 2 MiB huge-page entries (PS=0x83: Present | RW | HugePage) */
    movl $boot_pd0, %edi
    movl $0x00000083, %eax
    movl $2048, %ecx
1:
    movl %eax, (%edi)
    movl $0, 4(%edi)
    addl $0x00200000, %eax
    adcl $0, 4(%edi)
    addl $8, %edi
    decl %ecx
    jnz  1b

    /* Fix high 32 bits for entries >= 2 GiB (entries 1024..2047 remain < 4 GiB so hi=0) */

    /* Load CR3 = boot_pml4 */
    movl $boot_pml4, %eax
    movl %eax, %cr3

    /* Enable PAE (bit 5), PGE (bit 7), OSFXSR (bit 9), OSXMMEXCPT (bit 10) in CR4 */
    movl %cr4, %eax
    orl  $((1 << 5) | (1 << 7) | (1 << 9) | (1 << 10)), %eax
    movl %eax, %cr4

    /* Enable Long Mode (EFER.LME = bit 8) in IA32_EFER (0xC0000080) */
    movl $0xC0000080, %ecx
    rdmsr
    orl  $(1 << 8), %eax
    wrmsr

    /* Enable Paging (CR0.PG = bit 31) + Write Protect (bit 16) + MP (bit 1), clear EM (bit 2) */
    movl %cr0, %eax
    andl $~(1 << 2), %eax
    orl  $((1 << 31) | (1 << 16) | (1 << 1)), %eax
    movl %eax, %cr0

    /* Load 64-bit GDT and far-jump into 64-bit Long Mode */
    lgdt gdt64_ptr
    ljmp $0x08, $long_mode_entry

    .code64
long_mode_entry:
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %ss
    xorw %ax, %ax
    movw %ax, %fs
    movw %ax, %gs

    /* Align RSP to 16 bytes and reserve 32-byte Microsoft x64 shadow space */
    movq $boot_stack_top, %rsp
    andq $-16, %rsp
    subq $32, %rsp

    /* Zero CR8 (IRQL = PASSIVE_LEVEL) */
    xorq %rax, %rax
    movq %rax, %cr8

    call nt_baremetal_main

halt_forever:
    cli
    hlt
    jmp halt_forever

    .align 16
gdt64:
    .quad 0x0000000000000000 /* 0x00: Null descriptor */
    .quad 0x00AF9A000000FFFF /* 0x08: 64-bit Ring-0 code segment */
    .quad 0x00CF92000000FFFF /* 0x10: 64-bit Ring-0 data segment */
gdt64_end:

gdt64_ptr:
    .word (gdt64_end - gdt64 - 1)
    .long gdt64

    .section .bss
    .align 4096
boot_pml4:
    .skip 4096
boot_pdpt:
    .skip 4096
boot_pd0:
    .skip (4 * 4096)

    .align 16
boot_stack_bottom:
    .skip 65536
boot_stack_top:

    .section .note.GNU-stack, "", @progbits

