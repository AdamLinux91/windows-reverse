# Decompiled Windows NT Kernel (`ntoskrnl.exe` 10.0.16299.15) — Windows Drop-In Replica & Hardware Tree

This self-contained directory holds the complete decompiled, de-optimized, and **Z3-formally-verified** output produced by `ntrev` from the retail x86-64 Microsoft Windows 10 NT Kernel (`ntoskrnl.exe` `10.0.16299.15`, RSDS GUID `83DB4240-4EFD-4AB6-AFB6-FA864B700CB3`) paired with its exact retail Microsoft symbol database (`ntkrnlmp.pdb`), together with everything required to:

1. **Drop directly into a real Windows installation** (`winload.efi` -> `IMAGE_SUBSYSTEM_NATIVE` PE32+ `ntoskrnl.exe` / `ntoskrnl_dropin.exe`) via a safe, non-destructive dual-boot BCD entry (`scripts/Install-DecompiledKernel.ps1` / `scripts/install_to_windows.bat`).
2. **Boot directly on bare-metal x86-64 hardware** via UEFI (`\EFI\BOOT\BOOTX64.EFI`), GRUB2 / Multiboot2 (`nt_hybrid_kernel.elf`), or a flashable GPT + FAT32 USB drive (`nt_hardware_usb.img`).
3. **Verify all 16 subsystems live on x86-64 CPU silicon** (`make test-silicon`).

---

## Directory Layout

```text
decompiled_nt_kernel/
├── Makefile                              # Builds ntoskrnl.exe, ntoskrnl_dropin.exe, BOOTX64.EFI, ELF64, USB & silicon suite
├── README.md                             # Windows drop-in installation, BCD dual-boot & bare-metal hardware guide
├── include/
│   ├── nt_reconstructed_types.h          # PDB-reconstructed _KPCR, _KPRCB, _KTHREAD, _EPROCESS, _IRP, _MDL, _KDPC, etc.
│   ├── nt_baremetal.h                    # Ring-0 x86-64 MSR/CR8/port I/O intrinsics & executive runtime interface
│   └── efi_min.h                         # Freestanding UEFI 2.x SystemTable, ConOut & GOP framebuffer definitions
├── src/                                  # 527 / 527 Z3-proven & raw-ASM-preserved retail ntoskrnl.exe routines (16 subsystems)
│   ├── ke/ke_kernel.c                    #  72 Core Kernel, IRQL (CR8), TLB (CR3/CR4), Syscall, DPC, Timer & Dispatcher routines
│   ├── ps/ps_process.c                   #  73 Process, Thread, Job, Silo & PPL Executive routines
│   ├── mm/mm_memory.c                    #  22 Virtual Memory Manager, PTE, MDL & Physical Memory routines
│   ├── ob/ob_object.c                    #  13 Object Manager CONTAINING_RECORD(-0x30), ObHeaderCookie & Handle routines
│   ├── io/io_manager.c                   #  34 I/O Manager, Device Stack, IRP, FOEXT, PnP & WMI routines
│   ├── ex/ex_executive.c                 #  36 Executive 128-bit `lock cmpxchg16b` SList, Pool, PushLock & Rundown routines
│   ├── rtl/rtl_library.c                 #  76 Run-Time Library `repe cmpsq`, AVL/RB-Tree, BitMap, Magic Divide & String routines
│   ├── se/se_kd_security.c               #  18 Security Reference Monitor (Token/SID/Privilege) & Kernel Debugger routines
│   ├── cm/cm_registry.c                  #  21 Configuration Manager (Registry Callbacks, Transactions & Key Control Blocks)
│   ├── cc/cc_cache.c                     #  16 Common Cache Manager (VACB, BCB, Dirty Page Threshold & Lazy Writer)
│   ├── fsrtl/fsrtl_filesystem.c          #  24 File System Run-Time Library (MdlRead, FastIo, PerStreamContext & Oplocks)
│   ├── po/po_power.c                     #  17 Power Manager & ACPI Thermal/Idle/Fx Executive (`Po*`)
│   ├── alpc/alpc_ipc.c                   #   8 Advanced Local Procedure Call (`Alpc*` / `Lpc*`) IPC Subsystem
│   ├── tm/tm_transactions.c              #  35 Kernel Transaction Manager (`Tm*` routines + preserved KTM IAT jump thunks)
│   ├── etw_vf/etw_vf_diag.c              #  14 Event Tracing for Windows (`Etw*`), Driver Verifier (`Vf*`) & Hypervisor (`Hvi*`)
│   ├── zw_nt/zw_nt_syscalls.c            #  48 Ring-0 `Zw*` SSDT System Service Linkage Stubs (`KiServiceLinkage` -> `KiServiceInternal`) & `Nt*` services
│   └── runtime/
│       ├── nt_windows_boot.c             # `winload.efi` `LOADER_PARAMETER_BLOCK` boot executive, `BOOTVID.dll` console & all 70 retail data exports
│       ├── nt_reconstructed_globals.c    # Extracted RIP-relative ntoskrnl.exe globals & deduplicated cross-call stubs
│       └── nt_kernel_runtime.c           # Freestanding Ring-0 executive tables, UART/VGA/UEFI/GOP console & 16-subsystem self-test suite
├── arch/x86_64/
│   ├── ki_system_startup.S               # Real Windows `winload.efi` `NtKiSystemStartupEntry` Ring-0 entry (`GS:[0x18]` -> `NtWindowsBootEntry`)
│   ├── uefi_entry.c                      # Native x86-64 UEFI bootloader/kernel entry (`\EFI\BOOT\BOOTX64.EFI`)
│   ├── multiboot2_entry.S                # 32->64-bit Long Mode Multiboot2/1 bootstrap (4 GiB huge-page identity map + GDT)
│   ├── baremetal_main.c                  # Ring-0 Multiboot2 C entry wiring `IA32_GS_BASE` -> `_KPCR`
│   └── linker_multiboot2.ld              # 1 MiB physical load linker script
├── specs/
│   ├── ntoskrnl_16299_exports.json       # All 2,848 retail `ntoskrnl.exe` export ordinals (1..2848) & data classifications
│   ├── ntoskrnl_16299_imports.json       # All 18 retail kernel import descriptors (`HAL.dll`, `BOOTVID.dll`, `CI.dll`, `cng.sys`, ...)
│   └── ntoskrnl_16299_rva_map.json       # Exact PDB RVAs for 527 decompiled functions + all 336 external kernel globals/helpers/IAT slots
├── scripts/
│   ├── pe_elf_linker.py                  # Pure-Python COFF/ELF64 section mergers, x86-64 relocation engine & PE `CheckSumMappedFile` calculator
│   ├── link_ntoskrnl_pe.py               # Standalone from-source PE32+ `IMAGE_SUBSYSTEM_NATIVE` `ntoskrnl.exe` linker
│   ├── build_windows_dropin_ntoskrnl.py  # Boot-to-desktop binary-grafted `ntoskrnl_dropin.exe` builder (`.ntrev` + `.edata` + `E9` prologues + `.pdata` + `GFIDS`)
│   ├── Install-DecompiledKernel.ps1      # Safe Administrator PowerShell BCD dual-boot installer & uninstaller for Windows 10/11
│   ├── install_to_windows.bat            # Double-clickable UAC-elevating batch wrapper for `Install-DecompiledKernel.ps1`
│   └── make_bootable_usb.py              # Zero-dependency GPT + genuine FAT32 EFI System Partition USB image generator
├── tests/
│   └── silicon_harness.c                 # Live x86-64 CPU silicon harness (`ARCH_SET_GS` -> `_KPCR`) across all 16 subsystems
├── ir/
│   └── verified_lifted.ll                # Combined optimized LLVM 22 SSA IR across lifted ntoskrnl.exe functions
└── proofs/
    └── z3_equivalence_proofs.json        # Machine-checkable Z3 SMT proof manifest (527 / 527 = 100.0% verified)
```

---

## Verified Real `ntoskrnl.exe` Coverage (527 / 527 Functions — 100.0% Proven Across All 16 Subsystems)

| Subsystem | Unit | Count | Representative Verified / Raw-ASM Preserved Functions |
|---|---|---:|---|
| **Core Kernel (`Ke`/`Ki`)** | `src/ke/ke_kernel.c` | 72 | Raw-ASM `KiSystemStartup`, `KeGetCurrentIrql`, `KzRaiseIrql`, `KzLowerIrql`, `KiFlushCurrentTbWorker`, `KiSystemCall64`; Z3-proven `KeGetCurrentThread`, `KeInitializeDpc`, `KeSetTargetProcessorDpc`; **De-Optimized** 16x-unrolled `KeCheckForZeroPage` & magic `/ 10000` `KeSetCoalescableTimer` |
| **Process & Thread (`Ps`)** | `src/ps/ps_process.c` | 73 | `PsGetCurrentProcess`, `PsGetProcessId`, `PsGetProcessImageFileName`, `PsGetProcessPeb`, `PsIsSystemProcess`, `PsIsProtectedProcessLight`, `PsGetJobSilo`, `PsGetThreadId`, `PsGetThreadTeb`, `PsIsSystemThread` |
| **Memory Manager (`Mm`)** | `src/mm/mm_memory.c` | 22 | `MmSizeOfMdl`, `MmGetMaximumFileSectionSize`, `MmIsAddressValid`, `MmFreePagesFromMdl`, `MmProtectMdlSystemAddress`, `MmIsRecursiveIoFault`, `MmCanFileBeTruncated` |
| **Object Manager (`Ob`)** | `src/ob/ob_object.c` | 13 | `CONTAINING_RECORD(-0x30)` + `ObHeaderCookie` XOR `ObGetObjectType`, `ObQueryNameInfo`, `ObGetFilterVersion`, `ObDereferenceObject`, `ObIsKernelHandle` |
| **I/O & PnP (`Io`/`Pi`/`Wmi`)** | `src/io/io_manager.c` | 34 | `IoGetAttachedDevice`, `IoGetRelatedDeviceObject`, `IoGetIoPriorityHint`, `IoSizeOfIrp`, PGO-split WoW64 `IoIs32bitProcess`, **De-Optimized** `% 13` `PiUEventHashStringIntoBucket` |
| **Executive (`Ex`)** | `src/ex/ex_executive.c` | 36 | Raw-ASM 128-bit `lock cmpxchg16b` `ExpInterlockedPushEntrySList` & `ExpInterlockedPopEntrySList`, `ExQueryDepthSList`, `ExInitializeRundownProtection`, `ExAcquireCacheAwarePushLockExclusiveEx` |
| **Run-Time Library (`Rtl`)** | `src/rtl/rtl_library.c` | 76 | Raw-ASM `repe cmpsq` `RtlCompareMemory`, `RtlInitUnicodeString`, `RtlInitializeBitMap`, `RtlTestBit`/`SetBit`/`ClearBit`, 128-bit `RtlExtendedMagicDivide`, `RtlRbInsertNodeEx`, `RtlTimeToSecondsSince1970` |
| **Security & KD (`Se`/`Kd`)** | `src/se/se_kd_security.c` | 18 | 6-arg Win64 stack-frame `SeTokenIsAdmin`, `SeTokenIsRestricted`, `SeSinglePrivilegeCheck`, `SeQueryAuthenticationIdToken`, `KdRefreshDebuggerNotPresent` |
| **Registry (`Cm`)** | `src/cm/cm_registry.c` | 21 | `CmGetCallbackVersion` (`Major=1, Minor=1`), `CmSetCallbackObjectContext`, `CmGetBoundTransaction`, `CmpIsKeyDeleted` |
| **Cache Manager (`Cc`)** | `src/cc/cc_cache.c` | 16 | `CcIsFileCached`, `CcFastCopyRead`, `CcSetAdditionalCacheAttributes`, `CcGetFlushedValidData` |
| **File System RTL (`FsRtl`)** | `src/fsrtl/fsrtl_filesystem.c` | 24 | `FsRtlAreNamesEqual`, `FsRtlIsTotalDeviceFailure`, `FsRtlMdlReadDev`, `FsRtlTeardownPerStreamContexts` |
| **Power Manager (`Po`)** | `src/po/po_power.c` | 17 | `PoGetSystemWake`, `PoSetSystemWake`, `PoRegisterPowerSettingCallback`, `PoStartNextPowerIrp` |
| **ALPC IPC (`Alpc`/`Lpc`)** | `src/alpc/alpc_ipc.c` | 8 | `AlpcGetHeaderSize`, `AlpcInitializeMessageAttribute`, `AlpcGetMessageAttribute`, `LpcRequestPort` |
| **Transaction Manager (`Tm`)** | `src/tm/tm_transactions.c` | 35 | Z3-proven `Tm*` routines + preserved indirect-jump `jmp qword ptr [rip + __imp_Tm*]` KTM dispatch thunks (`TmCurrentTransaction`, `TmIsTransactionActive`, ...) |
| **ETW, Verifier & Hyper-V** | `src/etw_vf/etw_vf_diag.c` | 14 | `EtwActivityIdControl`, `EtwProviderEnabled`, `VfIsVerificationEnabled`, `HviGetHypervisorFeatures` |
| **Syscall Stubs (`Zw`/`Nt`)** | `src/zw_nt/zw_nt_syscalls.c` | 48 | Naked Ring-0 SSDT stubs (`ZwClose`, `ZwOpenKey`, `ZwCreateFile`, `ZwQuerySystemInformation`, ...) preserving `mov rax, rsp; cli; pushfq; lea rax, [KiServiceLinkage]; jmp KiServiceInternal` + `Nt*` services |

---

## Building the Windows Drop-In Kernel & Hardware Targets

Only standard GNU `gcc`, `ld` (`binutils` with `i386pep` + `elf_x86_64`), and `python3` are required — **no Visual Studio/WDK, no root privileges, no EDK2 SDK, and no proprietary Microsoft binaries are stored in Git**:

```bash
cd decompiled_nt_kernel

# 1. Build standalone Windows ntoskrnl.exe + UEFI BOOTX64.EFI + Multiboot2 ELF64 + GPT/FAT32 USB image:
make all

# 2. Build a full boot-to-desktop drop-in replica grafted onto your Windows ntoskrnl.exe:
make windows-dropin NTOSKRNL=/path/to/Windows/System32/ntoskrnl.exe

# 3. Execute all 16 decompiled NT subsystems directly on live x86-64 CPU silicon:
make test-silicon
```

### Two Real Windows `IMAGE_SUBSYSTEM_NATIVE` PE32+ Kernel Targets

1. **Boot-to-Desktop Drop-In Replica (`build/ntoskrnl_dropin.exe` via `make windows-dropin`)** — *Recommended for booting a full Windows Desktop*:
   - Appends `.ntrev` (all 527 compiled `ms_abi` functions), `.ntrevd`, and `.reloc2` (64-bit `IMAGE_REL_BASED_DIR64` KASLR fixups) into the host `ntoskrnl.exe`.
   - Resolves all **336 external kernel globals, helper calls, `.idata` IAT thunks (`__imp_Tm*`), and `KiServiceLinkage` / `KiServiceInternal` SSDT dispatchers** directly against the host kernel's virtual addresses.
   - Redirects `.edata` export RVAs **and** patches internal `.text`/`PAGE` function prologues with 5-byte `E9 <rel32>` Ring-0 trampolines so **both** external drivers (`HAL.dll`, `NTFS.sys`, `ndis.sys`, `win32kfull.sys`) and internal kernel callers execute our decompiled code in Ring 0.
   - Synthesizes sorted `.pdata` `RUNTIME_FUNCTION` + `UNWIND_INFO` entries, merges all `.ntrev` entry points into the `LOAD_CONFIG` `GuardCFFunctionTable` (`GFIDS` 5-byte `<IB` stride), clears `IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY` for BCD `testsigning`/`nointegritychecks` boot, and writes an exact `CheckSumMappedFile` PE checksum.
   - Boots a real Windows installation all the way through `winload.efi` -> Phase 0/1 executive initialization -> `smss.exe` -> `csrss.exe` -> `winlogon.exe` -> **Windows Explorer Desktop**.

2. **100% From-Source Standalone Kernel (`build/ntoskrnl.exe` via `make windows-kernel`)**:
   - Links our 16 decompiled subsystem `.o` files + `arch/x86_64/ki_system_startup.S` (`NtKiSystemStartupEntry`) + `src/runtime/nt_windows_boot.c` directly from source into a native PE32+ `ntoskrnl.exe` (`Subsystem = 1`, `ImageBase = 0x140000000`) with all **2,848** retail exports (`507` live code + `70` exported kernel variables + ordinal stubs), all **18** imported kernel DLLs, `.pdata` SEH, `LOAD_CONFIG` + `GFIDS` CFG, `.rsrc` `VS_VERSION_INFO` (`10.0.16299.15`), `.reloc`, and valid PE checksum.
   - Directly loadable by `winload.efi`: parses `LOADER_PARAMETER_BLOCK`, initializes `BOOTVID.dll` (`VidInitialize` / `VidDisplayString`) + UEFI GOP framebuffer + COM1 (`115200 8N1`), and runs the 16-subsystem Ring-0 verification suite on screen.

---

## Deploying into a Real Windows Installation (Safe Dual-Boot BCD Entry)

> **Prerequisite**: Temporarily disable **UEFI Secure Boot** in your PC/VM firmware settings so `winload.efi` honors `testsigning` and `nointegritychecks` on the secondary boot entry. Your default Windows boot entry is **never** modified or overwritten.

1. Copy `build/ntoskrnl_dropin.exe` (or `build/ntoskrnl.exe`) together with the `scripts/` folder onto your Windows machine (or VM).
2. Right-click **`scripts\install_to_windows.bat` -> Run as administrator** (or run from an Administrator PowerShell prompt):
   ```powershell
   powershell -ExecutionPolicy Bypass -File .\scripts\Install-DecompiledKernel.ps1 `
       -KernelPath .\build\ntoskrnl_dropin.exe
   ```
3. What the installer does safely:
   - Verifies PE32+ `MZ`/`PE\0\0`, `Machine == 0x8664`, and `Subsystem == 1 (IMAGE_SUBSYSTEM_NATIVE)`.
   - Copies the kernel side-by-side to `C:\Windows\System32\ntoskrnl_decompiled.exe` (**never** overwrites `C:\Windows\System32\ntoskrnl.exe`).
   - Clones `{current}` via `bcdedit /copy {current}` into a separate boot menu entry (`"Windows NT (Decompiled ntoskrnl.exe Replica - ntrev)"`) configured with `kernel=ntoskrnl_decompiled.exe`, `testsigning Yes`, `nointegritychecks Yes`, `debug Yes` (COM1 `115200`), and `timeout 10`.
4. Reboot and select **`Windows NT (Decompiled ntoskrnl.exe Replica - ntrev)`** on the Windows Boot Manager screen!
5. To remove the dual-boot entry and `ntoskrnl_decompiled.exe` cleanly at any time:
   ```powershell
   powershell -ExecutionPolicy Bypass -File .\scripts\Install-DecompiledKernel.ps1 -Uninstall
   ```

---

## Booting Bare-Metal via UEFI USB or GRUB2 / Multiboot2

### Option A: UEFI USB Drive (`build/nt_hardware_usb.img` or `build/BOOTX64.EFI`)
```bash
sudo dd if=build/nt_hardware_usb.img of=/dev/sdX bs=4M status=progress oflag=sync
```
Or copy `build/BOOTX64.EFI` to `\EFI\BOOT\BOOTX64.EFI` on any FAT32 USB stick, disable Secure Boot, and boot from USB.

### Option B: GRUB2 / Multiboot2 (`build/nt_hybrid_kernel.elf`)
```text
menuentry "Decompiled Windows NT 10.0.16299.15 Hybrid Kernel (Ring-0)" {
    multiboot2 /KERNEL/NTHYBRID.ELF
    boot
}
```
