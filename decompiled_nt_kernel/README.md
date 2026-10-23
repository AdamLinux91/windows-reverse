# Decompiled Windows NT Kernel (`ntoskrnl.exe` 10.0.16299.15) — Hardware-Bootable Tree

This self-contained directory holds the complete decompiled, de-optimized, and **Z3-formally-verified** output produced by `ntrev` from the retail x86-64 Microsoft Windows 10 NT Kernel (`ntoskrnl.exe` `10.0.16299.15`, RSDS GUID `83DB4240-4EFD-4AB6-AFB6-FA864B700CB3`) paired with its exact retail Microsoft symbol database (`ntkrnlmp.pdb`), together with everything needed to compile, flash, boot, and verify it directly on **real x86-64 hardware**.

---

## Directory Layout

```text
decompiled_nt_kernel/
├── Makefile                              # One-command build for UEFI, Multiboot2, USB image & silicon test
├── README.md                             # Hardware build, USB flashing & Ring-0 boot guide
├── include/
│   ├── nt_reconstructed_types.h          # PDB-reconstructed _KPCR, _KPRCB, _KTHREAD, _EPROCESS, _IRP, _MDL, _KDPC, etc.
│   ├── nt_baremetal.h                    # Ring-0 x86-64 MSR/CR8/port I/O intrinsics & executive runtime interface
│   └── efi_min.h                         # Freestanding UEFI 2.x SystemTable, ConOut & GOP framebuffer definitions
├── src/
│   ├── ke/ke_kernel.c                    # 18 Core Kernel, IRQL (CR8), TLB (CR3/CR4), Syscall, DPC, Timer & Dispatcher routines
│   ├── ps/ps_process.c                   # 16 Process & Thread Executive (_EPROCESS / _ETHREAD / PPL / System) routines
│   ├── mm/mm_memory.c                    #  4 Virtual Memory Manager & MDL page-span routines
│   ├── ob/ob_object.c                    #  5 Object Manager CONTAINING_RECORD(-0x30) & ObHeaderCookie XOR routines
│   ├── io/io_manager.c                   #  6 I/O Manager device stack, IRP priority, WoW64 PGO & PnP '% 13' routines
│   ├── ex/ex_executive.c                 #  5 Executive 128-bit `lock cmpxchg16b` SList & Rundown Protection routines
│   ├── rtl/rtl_library.c                 # 11 Run-Time Library `repe cmpsq`, 128-bit BitMap, Magic Divide & Epoch routines
│   ├── se/se_kd_security.c               #  4 Security Reference Monitor (6-arg SeTokenIsAdmin) & Kernel Debugger routines
│   └── runtime/
│       ├── nt_reconstructed_globals.c    # Extracted RIP-relative ntoskrnl.exe data & .rdata/.text constants
│       └── nt_kernel_runtime.c           # Freestanding Ring-0 executive tables, UART/VGA/UEFI/GOP console & 8-subsystem test suite
├── arch/x86_64/
│   ├── uefi_entry.c                      # Native x86-64 UEFI bootloader/kernel entry (\EFI\BOOT\BOOTX64.EFI)
│   ├── multiboot2_entry.S                # 32->64-bit Long Mode Multiboot2/1 bootstrap (4 GiB huge-page identity map + GDT)
│   ├── baremetal_main.c                  # Ring-0 Multiboot2 C entry wiring IA32_GS_BASE -> _KPCR
│   └── linker_multiboot2.ld              # 1 MiB physical load linker script
├── scripts/
│   └── make_bootable_usb.py              # Zero-dependency GPT + genuine FAT32 EFI System Partition USB image generator
├── tests/
│   └── silicon_harness.c                 # Live x86-64 CPU silicon harness (`ARCH_SET_GS` -> _KPCR)
├── ir/
│   └── verified_lifted.ll                # Combined optimized LLVM 22 SSA IR across lifted ntoskrnl.exe functions
└── proofs/
    └── z3_equivalence_proofs.json        # Machine-checkable Z3 SMT proof manifest (69/69 VERIFIED_EQUIVALENT)
```

---

## Verified Real `ntoskrnl.exe` Coverage (69 / 69 Functions — 100% Proven)

| Subsystem | Unit | Count | Highlights |
|---|---|---:|---|
| **Core Kernel (`Ke`/`Ki`)** | `src/ke/ke_kernel.c` | 18 | Raw-ASM preserved `KeGetCurrentIrql`, `KzRaiseIrql`, `KzLowerIrql`, `KeRaiseIrqlToDpcLevel`, `KiFlushCurrentTbWorker`, `KiSystemCall64`; Z3-proven `KeGetCurrentThread`, `KeGetCurrentProcessorNumberEx`, `KeQueryPriorityThread`, `KeInitializeDpc`, `KeSetImportanceDpc`, `KeSetTargetProcessorDpc`; **De-Optimized** 16x-unrolled `KeCheckForZeroPage` & 64-bit magic `/ 10000` `KeSetCoalescableTimer`; `KeReadState{Event,Mutex,Semaphore,Timer}` |
| **Process & Thread (`Ps`)** | `src/ps/ps_process.c` | 16 | `PsGetCurrentProcess`, `PsGetCurrentThread`, `PsGetProcessId`, `PsGetProcessInheritedFromUniqueProcessId`, `PsGetProcessImageFileName`, `PsGetProcessExitStatus`, `PsGetProcessPeb`, `PsGetProcessSectionBaseAddress`, `PsIsSystemProcess`, `PsIsProtectedProcess`, `PsIsProtectedProcessLight`, `PsGetThreadId`, `PsGetThreadProcess`, `PsGetThreadProcessId`, `PsGetThreadTeb`, `PsIsSystemThread` |
| **Memory Manager (`Mm`)** | `src/mm/mm_memory.c` | 4 | `MmSizeOfMdl`, `MmGetMaximumFileSectionSize`, `MmIsAddressValid`, `MmFreePagesFromMdl` |
| **Object Manager (`Ob`)** | `src/ob/ob_object.c` | 5 | `CONTAINING_RECORD(-0x30)` + `ObHeaderCookie` XOR `ObGetObjectType`, `ObQueryNameInfo`, `ObGetFilterVersion`, `ObDereferenceObject`, `ObIsKernelHandle` |
| **I/O & PnP (`Io`/`Pi`)** | `src/io/io_manager.c` | 6 | `IoGetAttachedDevice`, `IoGetIoPriorityHint`, `IoGetCurrentProcess`, `IoThreadToProcess`, PGO-split WoW64 `IoIs32bitProcess`, **De-Optimized** `% 13` `PiUEventHashStringIntoBucket` |
| **Executive (`Ex`)** | `src/ex/ex_executive.c` | 5 | Raw-ASM preserved 128-bit `lock cmpxchg16b` `ExpInterlockedPushEntrySList` & `ExpInterlockedPopEntrySList`, `ExQueryDepthSList`, `ExInitializeRundownProtection`, `ExReInitializeRundownProtection` |
| **Run-Time Library (`Rtl`)** | `src/rtl/rtl_library.c` | 11 | Raw-ASM preserved `repe cmpsq` `RtlCompareMemory`, `RtlInitUnicodeString`, `RtlInitAnsiString`, `RtlInitializeBitMap`, `RtlTestBit`, `RtlSetBit`, `RtlClearBit`, 128-bit `RtlExtendedMagicDivide`, `RtlSecondsSince1970ToTime`, `RtlSecondsSince1980ToTime`, `RtlTimeToSecondsSince1970` |
| **Security & KD (`Se`/`Kd`)** | `src/se/se_kd_security.c` | 4 | 6-argument Win64 stack-frame `SeTokenIsAdmin`, `SeTokenIsRestricted`, `SeQueryAuthenticationIdToken`, `.text`-string-probing `KdRefreshDebuggerNotPresent` |

---

## Quick Start: Build All Hardware Targets

Only standard GNU `gcc`, `ld` (`binutils` with `i386pep` + `elf_x86_64`), and `python3` are required — **no root privileges, no EDK2 SDK, and no `mtools`/`dosfstools` needed**:

```bash
cd decompiled_nt_kernel
make all
```

This produces three hardware-ready artifacts inside `build/`:
1. `build/BOOTX64.EFI` — Relocatable PE32+ x86-64 UEFI Application (`Subsystem = 10`, `.reloc` directory present).
2. `build/nt_hybrid_kernel.elf` — Bare-metal 64-bit Ring-0 Multiboot2 + Multiboot1 ELF64 kernel image.
3. `build/nt_hardware_usb.img` — Complete `34.3 MiB` GPT + genuine FAT32 (`>65,525` clusters) bootable USB disk image containing `\EFI\BOOT\BOOTX64.EFI` and `\KERNEL\NTHYBRID.ELF`.

---

## Booting & Testing on Real x86-64 Hardware

### Option 1: Boot Directly from USB on Any UEFI x86-64 PC / Laptop / Server
1. Flash `build/nt_hardware_usb.img` directly onto a USB drive (`/dev/sdX` on Linux, `/dev/diskN` on macOS, or via Rufus DD-mode / BalenaEtcher on Windows):
   ```bash
   sudo dd if=build/nt_hardware_usb.img of=/dev/sdX bs=4M status=progress oflag=sync
   ```
2. Alternatively, copy `build/BOOTX64.EFI` onto any existing FAT32-formatted USB drive at:
   ```text
   \EFI\BOOT\BOOTX64.EFI
   ```
3. In BIOS/UEFI Setup, **temporarily disable Secure Boot** (unsigned research kernel) and select the USB drive in the UEFI Boot Menu.
4. On boot, `efi_main`:
   - Disables the UEFI firmware watchdog timer.
   - Binds the **UEFI Graphics Output Protocol (GOP)** 32-bpp HDMI/DisplayPort linear framebuffer, UEFI `ConOut`, and **COM1 (`0x3F8`, `115200 8N1`)** serial telemetry.
   - Initializes the PDB-reconstructed `_KPCR`, `_KPRCB`, `_ETHREAD`, and `_EPROCESS` structures and writes `_KPCR` into `MSR_IA32_GS_BASE` (`0xC0000101`).
   - Executes all **8 decompiled NT kernel subsystems in Ring-0** (including hardware `CR8` IRQL transitions via `KeGetCurrentIrql` / `KzRaiseIrql` / `KzLowerIrql` and `CR4.PGE`/`CR3` hardware TLB flushes via `KiFlushCurrentTbWorker`).

### Option 2: Boot via GRUB2 / Multiboot2 / PXE on Bare-Metal Hardware
Copy `build/nt_hybrid_kernel.elf` to `/boot/nt_hybrid_kernel.elf` (or use `\KERNEL\NTHYBRID.ELF` from `nt_hardware_usb.img`) and add a GRUB2 menu entry:

```text
menuentry "Decompiled Windows NT 10.0.16299.15 Hybrid Kernel (Ring-0)" {
    multiboot2 /KERNEL/NTHYBRID.ELF
    boot
}
```

`multiboot2_entry.S` transitions the CPU from 32-bit Protected Mode into 64-bit Long Mode with 4 GiB `2 MiB`-page identity paging, enables SSE/FXSR and `CR4.PGE`, zeros `CR8`, programs `IA32_GS_BASE`, renders live Ring-0 results to both **VGA Text Mode (`0xB8000`)** and **COM1 (`115200 8N1`)**, and halts cleanly.

### Option 3: Instant Live x86-64 CPU Silicon Verification (Linux Host)
Verify all decompiled Microsoft x64 ABI (`ms_abi`) subsystems directly on the local x86-64 processor (`ARCH_SET_GS` binds physical `GS.base` to `_KPCR`):

```bash
cd decompiled_nt_kernel
make test-silicon
```
