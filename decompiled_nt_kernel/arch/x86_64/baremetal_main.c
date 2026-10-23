/*
 * Bare-Metal 64-bit Long Mode Ring-0 Entry (`nt_baremetal_main`)
 * Invoked by `multiboot2_entry.S` on GRUB2 / Multiboot2 x86-64 hardware.
 */
#include "../../include/nt_baremetal.h"

static struct _KPCR     g_mb_kpcr    __attribute__((aligned(4096)));
static struct _KPRCB    g_mb_kprcb   __attribute__((aligned(64)));
static struct _ETHREAD  g_mb_ethread __attribute__((aligned(64)));
static struct _EPROCESS g_mb_eproc   __attribute__((aligned(64)));

void __attribute__((ms_abi)) nt_baremetal_main(void) {
    NtConsoleInitSerial();
    NtConsoleInitVgaText();

    NtConsoleWrite("[BAREMETAL] Entered 64-bit Long Mode Ring-0 via Multiboot2\n");

    NtRuntimeInitializeExecutive(&g_mb_kpcr, &g_mb_kprcb, &g_mb_ethread, &g_mb_eproc);
    nt_wrmsr(MSR_IA32_GS_BASE, (uint64_t)(uintptr_t)&g_mb_kpcr);
    nt_wrmsr(MSR_IA32_KERNEL_GS_BASE, (uint64_t)(uintptr_t)&g_mb_kpcr);

    int failures = NtRunHardwareVerificationSuite(1 /* Ring-0 CR8/CR4/CR3 enabled */);

    if (failures == 0) {
        NtConsoleWrite("[BAREMETAL] Kernel boot & Ring-0 verification SUCCEEDED. Halting CPU cleanly.\n");
    } else {
        NtConsoleWrite("[BAREMETAL] Kernel verification reported failures. Halting CPU.\n");
    }

    for (;;) {
        __asm__ __volatile__ ("hlt");
    }
}
