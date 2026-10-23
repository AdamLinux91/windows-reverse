/*
 * Real Windows Boot Manager (`winload.efi` / `winload.exe`) Ring-0 Entry Point
 * Invoked directly by `OslArchTransferToKernel(LoaderBlock, NtKiSystemStartupEntry)`
 * in 64-bit Long Mode with `RCX = PLOADER_PARAMETER_BLOCK`.
 */
    .intel_syntax noprefix
    .text

    .globl NtKiSystemStartupEntry
NtKiSystemStartupEntry:
    cli
    cld
    mov rbx, rcx
    lea rsp, [rip + __nt_winload_stack_top]
    and rsp, -16
    sub rsp, 0x28
    mov rcx, rbx
    call NtWindowsBootEntry
.L_winload_halt:
    hlt
    jmp .L_winload_halt

/* Safe Control Flow Guard (`LOAD_CONFIG`) check & dispatch handlers */
    .globl __nt_guard_check_icall_nop
__nt_guard_check_icall_nop:
    ret

    .globl __nt_guard_dispatch_icall_nop
__nt_guard_dispatch_icall_nop:
    jmp rax

/* Safe NTSTATUS=0 (`STATUS_SUCCESS` / `NULL`) fallback stub for un-decompiled exports */
    .globl __nt_export_fallback_stub
__nt_export_fallback_stub:
    xor eax, eax
    ret

    .bss
    .balign 4096
__nt_winload_stack_bottom:
    .space 65536
    .globl __nt_winload_stack_top
__nt_winload_stack_top:

    .section .note.GNU-stack,"",@progbits
