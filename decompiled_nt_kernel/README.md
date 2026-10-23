# Decompiled Windows NT Kernel (`ntoskrnl.exe` 10.0.16299.15) — Analysis & Hardware Tree

> **Execution boundary:** this committed 527-function tree is a selected analysis/demo tree,
> not a byte-identical or fully proven semantic replacement for Microsoft `ntoskrnl.exe`.
> A C compile, a relocatable link, or a small-function Z3 proof does not establish that a
> booted Windows kernel behaves identically. For exact behavior, run
> `ntrev exact-kernel` on the matching retail PE and use the resulting
> `ntoskrnl.exact.exe`; the whole-kernel C/ASM output is documented in
> [`docs/WHOLE_KERNEL.md`](../docs/WHOLE_KERNEL.md).

This self-contained directory holds a selected decompiled and de-optimized output produced by `ntrev` from the retail x86-64 Microsoft Windows 10 NT Kernel (`ntoskrnl.exe` `10.0.16299.15`, RSDS GUID `83DB4240-4EFD-4AB6-AFB6-FA864B700CB3`) paired with its exact retail Microsoft symbol database (`ntkrnlmp.pdb`). It is useful for analysis and controlled hardware experiments, but it must not be advertised as a complete or exact Microsoft kernel replacement.

1. Generate a new standalone `IMAGE_SUBSYSTEM_NATIVE` PE from the complete whole-kernel object tree; Windows boot/application compatibility is an explicit VM gate, not inferred from compilation.
2. Boot the diagnostic UEFI/Multiboot artifacts on bare-metal x86-64 hardware when testing the low-level runtime, without treating that as Windows compatibility.
3. Verify the selected 16-subsystem analysis tree on x86-64 CPU silicon (`make test-silicon`).

---

## Directory Layout

```text
decompiled_nt_kernel/
├── Makefile                              # Builds diagnostic PE/UEFI/ELF/USB artifacts and silicon suite
├── README.md                             # Generated-image validation, BCD VM testing & low-level diagnostics
├── include/
│   ├── nt_reconstructed_types.h          # PDB-reconstructed _KPCR, _KPRCB, _KTHREAD, _EPROCESS, _IRP, _MDL, _KDPC, etc.
│   ├── nt_baremetal.h                    # Ring-0 x86-64 MSR/CR8/port I/O intrinsics & executive runtime interface
│   └── efi_min.h                         # Freestanding UEFI 2.x SystemTable, ConOut & GOP framebuffer definitions
├── src/                                  # Selected 527-function analysis tree with C candidates and raw-ASM boundaries (16 subsystems)
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
│   ├── build_windows_dropin_ntoskrnl.py  # Historical host-graft research tool; not the standalone implementation
│   ├── Install-DecompiledKernel.ps1      # Safe Administrator PowerShell BCD VM installer & uninstaller
│   ├── Run-StandaloneKernelSmoke.ps1     # Application-facing smoke gate for a VM booted with the generated image
│   ├── install_to_windows.bat            # Double-clickable UAC-elevating batch wrapper for `Install-DecompiledKernel.ps1`
│   └── make_bootable_usb.py              # Zero-dependency GPT + genuine FAT32 EFI System Partition USB image generator
├── tests/
│   └── silicon_harness.c                 # Live x86-64 CPU silicon harness (`ARCH_SET_GS` -> `_KPCR`) across all 16 subsystems
├── ir/
│   └── verified_lifted.ll                # Combined optimized LLVM 22 SSA IR across lifted ntoskrnl.exe functions
└── proofs/
    └── z3_equivalence_proofs.json        # Proof artifacts for selected analysis routines; not a kernel-wide behavior proof
```

---

## Selected analysis coverage (527 functions across 16 subsystems)

The table below describes the committed analysis tree only. It is not a complete
ntoskrnl implementation, a proof of semantic equivalence, or evidence that Windows can
boot or run applications on the generated image.

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

## Building the generated standalone kernel image

Only standard GNU `gcc`, `ld` (`binutils` with `i386pep` + `elf_x86_64`), and `python3`
are required to generate the object tree. The committed 527-function directory is a
selected analysis/diagnostic tree; `make all` and `make test-silicon` do **not** prove
that Windows applications can run on it.

For the actual complete recompiled image, provide the matching retail PE/PDB and run:

```bash
ntrev whole-kernel \
  /path/to/Windows/System32/ntoskrnl.exe \
  --pdb /path/to/symbols/ntkrnlmp.pdb \
  -o build/ntoskrnl_whole \
  --compile \
  --standalone-output build/ntoskrnl_standalone.exe
```

The standalone linker is not the old host-graft path. It requires every function in the
whole-kernel manifest to have a generated object symbol, uses generated
`KiSystemStartup` as the PE entry point, redirects generated exports, binds generated
IAT/data references to the input image's loader contract, and overwrites the input
executable-section bytes with INT3 guards. There is no host-code fallback. Raw assembly
procedures remain generated code because syscall/trap/CPU-state paths cannot safely be
forced into C.

The output is a new `IMAGE_SUBSYSTEM_NATIVE` PE, but a successful structural link is
not yet a Windows boot/application proof. It must be booted in a disposable Windows VM;
then the kernel debugger, loader log, system-service smoke tests, process creation,
file/registry I/O, networking, and a real application workload must pass before this
project can claim Windows-kernel behavior.

The legacy `make windows-dropin` target is retained only as an historical research tool
and is not the implementation requested here.

---

## Testing a Standalone Image in a Disposable Windows VM

> **Important:** this is a boot/application validation procedure, not a claim that the
> selected 527-function tree is complete. Use the generated
> `build/ntoskrnl_standalone.exe` from `ntrev whole-kernel --standalone-output`, not the
> historical host-graft image, when testing the actual recompiled path.

> **Prerequisite**: Temporarily disable **UEFI Secure Boot** in a disposable VM so
> `winload.efi` honors `testsigning` and `nointegritychecks`. Keep a recovery snapshot;
> the stock kernel must remain available as the fallback boot entry.

1. Copy `build/ntoskrnl_standalone.exe` (or another generated standalone image) together with the `scripts/` folder onto the Windows VM.
2. Right-click **`scripts\install_to_windows.bat` -> Run as administrator** (or run from an Administrator PowerShell prompt):
   ```powershell
   powershell -ExecutionPolicy Bypass -File .\scripts\Install-DecompiledKernel.ps1 `
       -KernelPath .\build\ntoskrnl_standalone.exe
   ```
3. What the installer does safely:
   - Verifies PE32+ `MZ`/`PE\0\0`, `Machine == 0x8664`, and `Subsystem == 1 (IMAGE_SUBSYSTEM_NATIVE)`.
   - Copies the kernel side-by-side to `C:\Windows\System32\ntoskrnl_decompiled.exe` (**never** overwrites `C:\Windows\System32\ntoskrnl.exe`).
   - Clones `{current}` via `bcdedit /copy {current}` into a separate boot menu entry (`"Windows NT (ntrev standalone generated kernel)"`) configured with `kernel=ntoskrnl_decompiled.exe`, `testsigning Yes`, `nointegritychecks Yes`, `debug Yes` (COM1 `115200`), and `timeout 10`.
4. Reboot and select the **ntrev standalone generated-kernel** entry on the Windows Boot Manager screen. If it fails before user mode, select the untouched stock entry and inspect the serial/kernel-debug log.
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
