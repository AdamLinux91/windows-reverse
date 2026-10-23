/*
 * Native x86-64 UEFI Hardware Entry Point (\EFI\BOOT\BOOTX64.EFI)
 * Boots directly on any UEFI Class 2/3 x86-64 PC, laptop, workstation, or server,
 * wires MSR IA32_GS_BASE (0xC0000101) to the reconstructed _KPCR, and executes
 * all 8 decompiled Windows NT 10.0.16299.15 kernel subsystems in Ring-0.
 */
#include "../../include/nt_baremetal.h"
#include "../../include/efi_min.h"

static struct _KPCR     g_uefi_kpcr    __attribute__((aligned(4096)));
static struct _KPRCB    g_uefi_kprcb   __attribute__((aligned(64)));
static struct _ETHREAD  g_uefi_ethread __attribute__((aligned(64)));
static struct _EPROCESS g_uefi_eproc   __attribute__((aligned(64)));

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable);

/* Force a non-empty PE32+ .reloc directory (DIR64 entry) required by EDK2/Aptio PeCoffLoader */
__attribute__((used)) void * volatile __efi_reloc_anchor = (void *)(uintptr_t)&efi_main;

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    (void)ImageHandle;

    NtConsoleInitSerial();

    if (SystemTable && SystemTable->BootServices && SystemTable->BootServices->SetWatchdogTimer) {
        SystemTable->BootServices->SetWatchdogTimer(0, 0, 0, NULL);
    }

    if (SystemTable && SystemTable->ConOut) {
        if (SystemTable->ConOut->ClearScreen) {
            SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
        }
        NtConsoleBindUefiConOut(SystemTable->ConOut);
    }

    /* Attempt to bind 32-bpp UEFI Graphics Output Protocol (GOP) linear framebuffer */
    if (SystemTable && SystemTable->BootServices && SystemTable->BootServices->LocateProtocol) {
        EFI_GUID gop_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
        EFI_GRAPHICS_OUTPUT_PROTOCOL *gop = NULL;
        if (SystemTable->BootServices->LocateProtocol(&gop_guid, NULL, (void **)&gop) == EFI_SUCCESS &&
            gop && gop->Mode && gop->Mode->Info &&
            (gop->Mode->Info->PixelFormat == PixelBlueGreenRedReserved8BitPerColor ||
             gop->Mode->Info->PixelFormat == PixelRedGreenBlueReserved8BitPerColor)) {
            NtConsoleBindGopFramebuffer(
                (uint32_t *)(uintptr_t)gop->Mode->FrameBufferBase,
                gop->Mode->Info->HorizontalResolution,
                gop->Mode->Info->VerticalResolution,
                gop->Mode->Info->PixelsPerScanLine
            );
        }
    }

    NtConsoleWrite("[UEFI] Booted Decompiled Windows NT 10.0.16299.15 Hybrid Kernel (x86-64 Ring-0)\n");

    /* Save UEFI firmware GS.base, bind reconstructed _KPCR to IA32_GS_BASE, and run Ring-0 suite */
    uint64_t saved_gs = nt_rdmsr(MSR_IA32_GS_BASE);
    NtRuntimeInitializeExecutive(&g_uefi_kpcr, &g_uefi_kprcb, &g_uefi_ethread, &g_uefi_eproc);
    nt_wrmsr(MSR_IA32_GS_BASE, (uint64_t)(uintptr_t)&g_uefi_kpcr);

    int failures = NtRunHardwareVerificationSuite(1 /* Ring-0 CR8/CR4/CR3 enabled */);

    nt_wrmsr(MSR_IA32_GS_BASE, saved_gs);

    NtConsoleWrite("[UEFI] Hardware verification complete. Holding display for 15s before return...\n");
    if (SystemTable && SystemTable->BootServices && SystemTable->BootServices->Stall) {
        SystemTable->BootServices->Stall(15000000ULL);
    }

    return (failures == 0) ? EFI_SUCCESS : 1ULL;
}
