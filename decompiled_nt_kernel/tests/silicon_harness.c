/*
 * Live x86-64 CPU Silicon Execution Harness (`make test-silicon`)
 * Binds the physical x86-64 CPU `GS.base` segment register to our PDB-reconstructed
 * `_KPCR` via `ARCH_SET_GS` and executes all 8 decompiled Windows NT 10.0.16299.15
 * kernel subsystems (`ms_abi`, `gs:[0x188]`, `gs:[0x20]`, `lock cmpxchg16b`, `repe cmpsq`)
 * natively on real hardware.
 */
#define _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <asm/prctl.h>

#include "../include/nt_baremetal.h"

static struct _KPCR     g_host_kpcr    __attribute__((aligned(4096)));
static struct _KPRCB    g_host_kprcb   __attribute__((aligned(64)));
static struct _ETHREAD  g_host_ethread __attribute__((aligned(64)));
static struct _EPROCESS g_host_eproc   __attribute__((aligned(64)));

int main(void) {
    NtRuntimeInitializeExecutive(&g_host_kpcr, &g_host_kprcb, &g_host_ethread, &g_host_eproc);

    /* Bind physical CPU GS.base to reconstructed _KPCR so real `mov rax, gs:[0x188]` works! */
    if (syscall(SYS_arch_prctl, ARCH_SET_GS, (unsigned long)(uintptr_t)&g_host_kpcr) != 0) {
        perror("arch_prctl(ARCH_SET_GS) failed");
        return 2;
    }

    int failures = NtRunHardwareVerificationSuite(0 /* Ring-3 silicon mode */);
    return (failures == 0) ? 0 : 1;
}
