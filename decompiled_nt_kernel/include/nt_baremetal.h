/*
 * Ring-0 Hardware & Console Primitives for Decompiled Windows NT Kernel
 * Supports:
 *   1. Bare-metal x86-64 UEFI hardware (ConOut + GOP 32-bpp framebuffer + COM1 UART)
 *   2. Bare-metal x86-64 Multiboot2/GRUB2 hardware (VGA 0xB8000 + COM1 16550 UART)
 *   3. Live host x86-64 CPU silicon verification harness (ARCH_SET_GS + stdout)
 */
#ifndef NT_BAREMETAL_H
#define NT_BAREMETAL_H

#include "nt_reconstructed_types.h"

#define MSR_IA32_GS_BASE        0xC0000101U
#define MSR_IA32_KERNEL_GS_BASE 0xC0000102U
#define COM1_IOPORT             0x3F8U

/* NT IRQL constants */
#define PASSIVE_LEVEL           0U
#define APC_LEVEL               1U
#define DISPATCH_LEVEL          2U
#define HIGH_LEVEL              15U

#ifndef NT_HOST_SILICON_TEST

static inline void nt_outb(uint16_t port, uint8_t val) {
    __asm__ __volatile__ ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t nt_inb(uint16_t port) {
    uint8_t ret;
    __asm__ __volatile__ ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void nt_wrmsr(uint32_t msr, uint64_t val) {
    uint32_t lo = (uint32_t)(val & 0xFFFFFFFFULL);
    uint32_t hi = (uint32_t)(val >> 32);
    __asm__ __volatile__ ("wrmsr" : : "c"(msr), "a"(lo), "d"(hi) : "memory");
}

static inline uint64_t nt_rdmsr(uint32_t msr) {
    uint32_t lo, hi;
    __asm__ __volatile__ ("rdmsr" : "=a"(lo), "=d"(hi) : "c"(msr));
    return ((uint64_t)hi << 32) | lo;
}

static inline uint64_t nt_read_cr8(void) {
    uint64_t v;
    __asm__ __volatile__ ("mov %%cr8, %0" : "=r"(v));
    return v;
}

static inline void nt_write_cr8(uint64_t v) {
    __asm__ __volatile__ ("mov %0, %%cr8" : : "r"(v) : "memory");
}

#endif /* !NT_HOST_SILICON_TEST */

#ifndef NTAPI
#define NTAPI __attribute__((ms_abi))
#endif

/* Kernel console & hardware runtime entry points */
void NTAPI NtConsoleInitSerial(void);
void NTAPI NtConsoleInitVgaText(void);
void NTAPI NtConsoleBindUefiConOut(void *con_out);
void NTAPI NtConsoleBindBootvid(void (NTAPI *display_str_fn)(const char *));
void NTAPI NtConsoleBindGopFramebuffer(uint32_t *fb_base, uint32_t width, uint32_t height, uint32_t pitch);
void NTAPI NtConsolePutChar(char c);
void NTAPI NtConsoleWrite(const char *s);
void NTAPI NtConsolePrintHex(uint64_t val);
void NTAPI NtConsolePrintDec(int64_t val);

/* Executive global & KPCR/KPRCB initialization */
void NTAPI NtRuntimeInitializeExecutive(
    struct _KPCR    *kpcr_out,
    struct _KPRCB   *kprcb_out,
    struct _ETHREAD *init_thread_out,
    struct _EPROCESS *sys_proc_out
);

/* Full 8-subsystem hardware verification suite (returns 0 if 100% pass, >0 failures) */
int NTAPI NtRunHardwareVerificationSuite(int is_ring0_hardware);

/* Captured telemetry from internal callees for end-to-end hardware assertions */
extern uint64_t g_nt_last_timer_hand;
extern int64_t  g_nt_last_timer_duetime;
extern ULONG    g_nt_last_timer_period;
extern uint32_t g_nt_kd_printed_len;
extern char     g_nt_kd_printed_buf[128];

#endif /* NT_BAREMETAL_H */
