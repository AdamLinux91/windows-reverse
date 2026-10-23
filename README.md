# `windows-reverse` (`ntrev`)

**Automated Windows NT Binary Decompilation & Z3 Semantic Equivalence Engine**  
*Phase 1 Foundation for the Core Integrated Hybrid Kernel Architecture*

---

## Overarching System Vision

```text
+-------------------------------------------------------------+
|          User Space (Applications & Games)                  |
|   [Native Linux Apps (ELF)]     [Native Windows Apps (PE)]  |
+-------------------------------------------------------------+
|          Unified Executive API Layer                        |
|   [Linux Syscall Interface]     [Windows NT System Services]|
+-------------------------------------------------------------+
|          The Core Integrated Hybrid Kernel                  |
|   * Unified Super-Scheduler (Manages CPU Cores)             |
|   * Monolithic Page Table Manager (Shared RAM Pool)         |
|   * Combined WDDM / DRM Subsystem (Direct GPU Access)       |
+-------------------------------------------------------------+
|                      Bare-Metal Hardware                    |
+-------------------------------------------------------------+
```

Before unifying the Windows NT Executive (`ntoskrnl.exe`) with a monolithic Linux-compatible hybrid kernel, we require **mathematically provable binary-to-source recovery** of NT system services, executive structures, and kernel primitives — without hand-wave approximations or broken decompiler heuristics.

`ntrev` implements **Phase 1: Automated Decompilation & Binary-to-Source Semantic Equivalence**:

```text
                [Windows Binary (ntoskrnl.exe / .sys)]
                                  │
                                  ▼
      1. Lifter / IR Translator (Assembly ──► LLVM 22 IR + Raw ASM Detector)
                                  │
                                  ▼
      2. Symbol & Structural Mapper (MSF 7.00 PDBs, Ghidra API, NT Structs)
                                  │
                                  ▼
      3. Type Inference, De-Optimizer & Z3 Theorem Prover (Formal Equivalence)
                                  │
                                  ▼
                [Exact Compilable C/C++ Source Replica]
```

---

## Key Capabilities

### 1. Stage 1 — Lifter, IR Translator & Non-C Kernel Assembly Preserver
* **PE32+ (`ntoskrnl.exe` / `.sys` / `.dll`) & `.pdata` Unwind Loader** (`src/ntrev/frontend/pe_loader.py`): Parses `IMAGE_FILE_MACHINE_AMD64` binaries, executable kernel sections (`.text`, `PAGE`, `INIT`, `KVASCODE`), `.pdata` `RUNTIME_FUNCTION` + `UNWIND_INFO` exception tables, exports/imports, and `RSDS` CodeView PDB70 debug signatures (with automatic Microsoft Symbol Server URL generation).
* **Hand-Written NT Kernel Assembly Detector** (`src/ntrev/frontend/asm_detector.py`): Parts of `ntoskrnl.exe` (`KiSystemCall64`, `KiSwapContext`, trap entry stubs, CR3/MSR/TLB routines) are written in raw assembly rather than C. `ntrev` detects ring-0 / non-C instructions (`swapgs`, `sysretq`, `iretq`, `rdmsr`, `wrmsr`, `mov cr0..cr8`, `invlpg`, `lgdt`, `lidt`, `cli`/`sti`, and kernel stack pivots `mov rsp, gs:[0x1a8]`) and **preserves them verbatim as `__attribute__((naked))` or inline `__asm__ __volatile__` blocks** instead of mangling them into invalid C.
* **Native x86-64 ──► LLVM 22 SSA Lifter + Remill / RetDec Adapters** (`src/ntrev/lifter/`):
  * Lifts x86-64 CFGs directly to verified LLVM 22 IR via `llvmlite` (`llvm.parse_assembly` + `ModuleRef.verify()` + LLVM NewPassManager `PassBuilder` SROA/`mem2reg`/GVN/InstCombine).
  * Models non-escaping stack frames via GEP into `%stack_frame = alloca [4096 x i8]`, enabling LLVM 22's SROA pass to eliminate `push`/`pop` prologue spills and promote stack locals into pure SSA registers automatically.
  * Includes pluggable adapters & intrinsic normalizers (`ExternalLifterAdapter`) for **Trail of Bits Remill** (`remill-lift`) and **RetDec** (`retdec-decompiler --stop-after bin2llvmir`), plus a native C++ LLVM NewPM pass plugin (`llvm_passes/NtLoopRerollPass.cpp`).

### 2. Stage 2 — PDB v7.00 Symbol & Structural Mapper + Ghidra API Bridge
* **Genuine Binary MSF 7.00 `.pdb` Parser & Builder** (`src/ntrev/symbols/pdb_parser.py`): Parses SuperBlock, multi-block Stream Directory, PDB Info GUID/Age, TPI (`LF_STRUCTURE`, `LF_UNION`, `LF_FIELDLIST`, `LF_MEMBER`, `LF_POINTER`, `LF_BITFIELD`, `LF_ARRAY`, `LF_PROCEDURE`, `LF_ARGLIST`), and DBI Symbol Streams (`S_GPROC32`, `S_LPROC32`, `S_PUB32`, `S_LOCAL`).
* **Ghidra Headless Python & Java Programmatic API Scripts** (`ghidra_scripts/ExportNtAnalysis.py`, `ghidra_scripts/GhidraNtBridge.java`, `src/ntrev/frontend/ghidra_bridge.py`): Extracts `DataTypeManager` structs, `DecompInterface` `HighFunction` signatures, and `PcodeOpAST` metadata via `analyzeHeadless`.
* **NT Kernel Pointer & `CONTAINING_RECORD` Mapper** (`src/ntrev/symbols/struct_mapper.py`): Resolves `gs:[0x188]` to `KeGetCurrentThread()`, tracks chained struct dereferences (`Thread->ApcStateProcess->UniqueProcessId`), recovers negative-displacement `CONTAINING_RECORD(ptr, struct _EPROCESS, ActiveProcessLinks)` macros, and synthesizes `_RECON_*` struct definitions from raw displacement patterns when symbols are stripped.

### 3. Stage 3 — Automated Type Inference, Loop Reroller & Z3 SMT Solver
* **Constraint-Based Type Inference** (`src/ntrev/analysis/type_inference.py`): Backward CFG liveness for Windows x64 `ms_abi` registers (`rcx`, `rdx`, `r8`, `r9`) combined with opcode signedness (`sar`/`idiv`/`jl` vs `shr`/`div`/`jb`), pointer stride inference, and `NTSTATUS` / `BOOLEAN` return detection.
* **Compiler De-Optimization & Multi-Branch Loop Collapse** (`src/ntrev/analysis/deoptimizer.py`):
  * **Multi-Branch Unrolled Loop Collapse**: Recognizes loops unrolled by optimizing compilers into N distinct branches (e.g., 5-way count dispatch trees or Duff's-device fallthrough cascades) and collapses them back into a single clean `for` loop.
  * **Linear Strided Loop Rerolling**: Collapses `-O3 -funroll-loops` sequential memory reduction chains back into clean `for` loops.
  * **Magic-Multiplier Division Recovery**: Recovers `x / D` from 64-bit & 32-bit widening `mul`/`imul` + shift reciprocal constants (including Hacker's Delight `(N+1)`-bit overflow-add sequences).
* **Microsoft Z3 Formal Equivalence Verifier** (`src/ntrev/analysis/smt_verifier.py`): Proves `ForAll inputs: x86_64_semantics(inputs) == reconstructed_IR_and_C(inputs)` (`z3.unsat`) over 64-bit bit-vectors, EFLAGS (`ZF`, `CF`, `SF`, `OF`), byte-addressable little-endian Z3 `Array` memory, and Presburger `QF_LIA` arithmetic for 128-bit magic reciprocals. Produces concrete hex counterexamples whenever any divergence exists.

### 4. Stage 4 — Custom Verified LLVM IR ──► Exact C/C++ Emitter
* **Compilable Layout-Exact C11/C++ Output** (`src/ntrev/backend/c_emitter.py`): Emits `reconstructed_types.h` (with byte-exact explicit padding so `offsetof(...)` matches the Windows binary byte-for-byte), `reconstructed_nt.c`, `verified_lifted.ll`, and `z3_equivalence_proofs.json`.

---

## Repository Structure

```text
windows-reverse/
├── decompiled_nt_kernel/              # ★ Self-contained real ntoskrnl.exe (10.0.16299.15) hardware-bootable kernel tree:
│   ├── Makefile                       #   Builds BOOTX64.EFI, nt_hybrid_kernel.elf, nt_hardware_usb.img & silicon_harness
│   ├── README.md                      #   Step-by-step UEFI / GRUB2 / USB flashing & Ring-0 hardware guide
│   ├── include/                       #   PDB-reconstructed NT structs (_KPCR, _KPRCB, _EPROCESS, _IRP, ...) & UEFI/MSR headers
│   ├── src/{ke,ps,mm,ob,io,ex,rtl,se} #   69/69 Z3-proven & raw-ASM-preserved retail ntoskrnl.exe routines across 8 subsystems
│   ├── src/runtime/                   #   Freestanding Ring-0 executive globals, UART/VGA/GOP console & 8-subsystem self-test
│   ├── arch/x86_64/                   #   Native UEFI entry + 32->64-bit Long Mode Multiboot2 bootstrap & linker script
│   ├── scripts/make_bootable_usb.py   #   Zero-dependency GPT + genuine FAT32 EFI System Partition USB image builder
│   ├── ir/verified_lifted.ll          #   Combined optimized LLVM 22 SSA IR for lifted ntoskrnl.exe routines
│   └── proofs/z3_equivalence_proofs.json # Machine-checkable Z3 SMT proof manifest (69/69 verified)
├── docs/
│   └── ARCHITECTURE.md                # Deep-dive technical & mathematical design
├── ghidra_scripts/
│   ├── ExportNtAnalysis.py            # Ghidra Python / PyGhidra headless exporter
│   └── GhidraNtBridge.java            # Ghidra Java GhidraScript headless exporter
├── llvm_passes/
│   ├── CMakeLists.txt                 # Builds libNtDeoptPass.so against LLVM Core
│   ├── NtLoopRerollPass.h
│   └── NtLoopRerollPass.cpp           # C++ LLVM NewPM loop-reroll & magic-div pass
├── src/ntrev/
│   ├── models.py                      # Pipeline IR, CFG, PDB, & Z3 proof models
│   ├── pipeline.py                    # End-to-end 4-stage orchestrator + modular subsystem tree exporter
│   ├── cli.py                         # Rich CLI (`ntrev`, including `--modular-tree`)
│   ├── frontend/                      # PE32+ loader, PGO cold-chunk CFG, ASM detector, Ghidra bridge
│   ├── lifter/                        # Native x86-64 -> LLVM 22 lifter & Remill/RetDec bridges
│   ├── symbols/                       # Binary MSF 7.00 PDB parser/builder & NT struct mapper
│   ├── analysis/                      # Type inference, loop de-optimizer, & Z3 SMT verifier
│   └── backend/                       # Verified LLVM IR + Struct Map -> C/C++ emitter
├── examples/
│   └── build_ntoskrnl_fixture.py      # Compiles realistic PE32+ kernel fixture + MSF 7.00 PDB
└── tests/                             # 22 unit, real-kernel Z3 manifest & live x86-64 silicon tests
```

---

## Real Windows NT Kernel (`ntoskrnl.exe` 10.0.16299.15) — Build & Boot on Real Hardware

All decompiled output from the retail x86-64 Windows 10 NT Kernel (`ntoskrnl.exe` `10.0.16299.15`, RSDS GUID `83DB4240-4EFD-4AB6-AFB6-FA864B700CB3`) and its matching `ntkrnlmp.pdb` (**69 / 69 functions = 100% formally proven / assembly-preserved** across `Ke`, `Ps`, `Mm`, `Ob`, `Io`/`Pi`, `Ex`, `Rtl`, and `Se`/`Kd`) resides in [`decompiled_nt_kernel/`](decompiled_nt_kernel/README.md):

```bash
cd decompiled_nt_kernel

# 1. Build UEFI BOOTX64.EFI, Multiboot2 ELF64 kernel, and bootable GPT+FAT32 USB image:
make all

# 2. Execute all 8 decompiled NT kernel subsystems directly on live x86-64 CPU silicon:
make test-silicon

# 3. Flash onto a physical USB drive to boot on real UEFI or GRUB2/Multiboot2 hardware:
#    sudo dd if=build/nt_hardware_usb.img of=/dev/sdX bs=4M status=progress oflag=sync
```

See [`decompiled_nt_kernel/README.md`](decompiled_nt_kernel/README.md) for complete UEFI GOP/Serial/VGA hardware boot instructions.

---

## Quickstart

### Installation

```bash
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
pip install -e .
```

### Run the End-to-End Live Kernel Demo

Synthesizes a real `ms_abi`-compiled Windows x64 PE32+ binary (`ntoskrnl_mini.exe`) + binary MSF 7.00 symbol database (`ntoskrnl_mini.pdb`) — including `_EPROCESS`/`_KTHREAD` routines, a 6x linearly unrolled loop, a 5-branch compiler-unrolled dispatch loop, a 64-bit magic-constant division, and a ring-0 `KiSystemCall64Entry` (`swapgs` / `rdmsr` / `sysretq`) stub — then runs all 4 pipeline stages and proves semantic equivalence with Z3:

```bash
ntrev demo -o output/demo
```

### Decompile a Windows PE32+ Binary (`ntoskrnl.exe` / `.sys`)

```bash
# With matching Microsoft PDB:
ntrev decompile ntoskrnl.exe --pdb ntkrnlmp.pdb -o output/ntoskrnl --show-c

# With exported Ghidra Headless analysis bundle:
ntrev decompile driver.sys --ghidra-json ghidra_export.json -o output/driver

# Inspect PE32+ .pdata unwind table & RSDS Symbol Server URL:
ntrev inspect-pe ntoskrnl.exe

# Inspect binary MSF 7.00 PDB TPI structures & procedure symbols:
ntrev inspect-pdb ntkrnlmp.pdb
```

### Run the Test Suite

```bash
pytest -v
```
