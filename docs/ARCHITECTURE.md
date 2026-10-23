# Architecture & Formal Verification Specification

## 1. Role in the Integrated Hybrid Kernel Roadmap

The long-term objective of `windows-reverse` is a bare-metal **Core Integrated Hybrid Kernel** capable of executing both native Linux (`ELF`) and native Windows (`PE32+`) user-space workloads atop a unified executive layer:

1. **Unified Executive API Layer**: Exposes both the Linux syscall ABI and Windows NT System Services (`Nt*` / `Zw*` SSDT dispatch) without user-space translation overhead.
2. **Core Integrated Hybrid Kernel**:
   * **Unified Super-Scheduler**: Schedules Linux tasks (`task_struct`) and NT kernel threads (`_KTHREAD` / `_ETHREAD`) across physical cores under unified priority/IRQL semantics.
   * **Monolithic Page Table Manager**: Manages shared physical pages and x86-64 4/5-level paging structures across POSIX `mm_struct` and NT `_EPROCESS.DirectoryTableBase` VAD trees.
   * **Combined WDDM / DRM Subsystem**: Bridges Windows Display Driver Model (`dxgkrnl`) scheduling and Linux DRM/KMS GPU command submission.

Achieving drop-in compatibility with undocumented or optimization-heavy NT kernel internals requires **Phase 1 (`ntrev`)**: an automated binary-to-source pipeline where every lifted function, reconstructed struct layout, and collapsed compiler optimization carries a **machine-checkable Z3 equivalence proof**.

---

## 2. Detailed 4-Stage Pipeline Design

### Stage 1: Frontend Disassembly, Non-C ASM Classification & LLVM 22 Lifting

1. **PE32+ & `.pdata` Boundary Discovery (`frontend/pe_loader.py`, `frontend/disassembler.py`)**
   * On Windows x64, every non-leaf C/C++ function is registered in `.pdata` (`IMAGE_DIRECTORY_ENTRY_EXCEPTION`) as a 12-byte `RUNTIME_FUNCTION (BeginAddress, EndAddress, UnwindInfoAddress)`.
   * `ntrev` fuses `.pdata` unwind bounds, PDB `S_GPROC32`/`S_PUB32` symbols, and PE exports across all executable sections (`.text`, `PAGE`, `INIT`, `KVASCODE`), strips alignment padding (`0xCC` / multi-byte NOPs), and constructs basic-block CFGs via Capstone (`CS_ARCH_X86`, `CS_MODE_64`).

2. **Non-C Kernel Assembly Detection (`frontend/asm_detector.py`)**
   * Hand-written `.asm` routines in `ntoskrnl.exe` violate C ABI invariants or execute privileged ring-0 instructions.
   * **Whole-function naked ASM preservation** triggers when a function resides in trap/shadow sections (`KVASCODE`), executes non-C entry/exit opcodes (`swapgs`, `sysretq`, `iretq`, `retf`), performs non-ABI stack pivots (`mov rsp, gs:[0x1a8]`), or lacks standard `ret` paths while executing privileged instructions.
   * **Hybrid inline ASM splicing** triggers when an otherwise standard `ms_abi` C function executes isolated privileged instructions (e.g., `invlpg [rcx]`, `mov cr3, rcx`, `wrmsr`, `cli`/`sti`). Surrounding blocks lift to C while the privileged instruction is emitted as a memory/flag-fenced `__asm__ __volatile__` statement.

3. **LLVM 22 Core Lifting & SROA Stack Promotion (`lifter/x86_to_llvm.py`)**
   * Register state (`rax`..`r15`) and condition flags (`ZF`, `CF`, `SF`, `OF`) are allocated in the entry block and lifted with exact x86-64 sub-register rules (writing 32-bit `eax` zero-extends to 64-bit `rax`; writing 16-bit `ax` or 8-bit `al`/`ah` preserves upper bits).
   * `%reg_rsp` (and `%reg_rbp` when frame-pointer-based) indexes into a non-escaped `%stack_frame = alloca [4096 x i8]` via `getelementptr` rather than `inttoptr`. When LLVM 22's `PassBuilder` (`speed_level=2`) runs `SROA` + `GVN` + `InstCombine` + `DCE`, all callee-saved register spills (`push rbx` / `pop rbx`) and local shadow-space slots collapse into pure SSA `phi` nodes and registers.

---

### Stage 2: Binary MSF 7.00 PDB & Ghidra Structural Mapping

1. **MSF 7.00 Binary Container & CodeView TPI/DBI Streams (`symbols/pdb_parser.py`)**
   * Parses the 32-byte MSF 7.00 magic header, SuperBlock, BlockMap, and multi-block Stream Directory.
   * Decodes TPI (`Stream 2`) type indices starting at `0x1000`, resolving forward references (`prop & 0x80`), 64-bit pointers (`LF_POINTER`), field lists (`LF_FIELDLIST` / `LF_MEMBER`), bitfields (`LF_BITFIELD`), arrays (`LF_ARRAY`), and procedure prototypes (`LF_PROCEDURE` / `LF_ARGLIST`).
   * Decodes DBI (`Stream 3`) and Symbol Record Streams (`S_GPROC32`, `S_LPROC32`, `S_PUB32`, `S_LOCAL`).

2. **Chained Pointer & `CONTAINING_RECORD` Resolution (`symbols/struct_mapper.py`)**
   * Resolves `gs:[0x188]` to `KeGetCurrentThread()` (`struct _KTHREAD*`) and propagates pointee types across loads (`Thread->ApcStateProcess` ──► `struct _EPROCESS*` ──► `UniqueProcessId`).
   * Detects negative-displacement `lea` idioms (`lea rax, [rcx - 0x2e8]`) matching embedded `LIST_ENTRY` fields and reconstructs `CONTAINING_RECORD(ptr, struct _EPROCESS, ActiveProcessLinks)`.
   * Synthesizes padded `_RECON_*` C structs for untyped pointer parameters when PDB symbols are unavailable.

---

### Stage 3: De-Optimization & Z3 Theorem Prover Equivalence

1. **Multi-Branch & Linear Loop Rerolling (`analysis/deoptimizer.py`)**
   * Optimizing compilers frequently unroll bounded loops into either:
     * **Multi-branch dispatch trees / Duff's-device cascades** (e.g., 5 branches handling `count == 1..5`), or
     * **Intra-block strided instruction chains** (`ptr[0]..ptr[K-1]`).
   * `CompilerDeoptimizer` projects acyclic CFG paths into normalized strided micro-operation sequences, verifies arithmetic progression of byte offsets `{d0 + i * stride}`, determines exact upper-bound clamping vs default-zero semantics by concretely tracing `count = max_k + 1`, and collapses the branches into a single `for` loop.

2. **Magic-Multiplier Division Recovery & Presburger `QF_LIA` Verification (`analysis/smt_verifier.py`)**
   * Compilers replace unsigned/signed division by constant `D` with 128-bit widening multiplication by reciprocal `M` plus shifts (or Hacker's Delight `(N+1)`-bit `sub/shr/add/shr` sequences).
   * Rather than bit-blasting a 128-bit hardware multiplier against a 64-bit divider in CNF SAT (which suffers exponential blowup), `Z3EquivalenceVerifier` uses a **two-step compositional proof**:
     1. **Z3 BitVec Equivalence**: Proves `x86_cfg(x)` is bit-identically equal to the shift-multiply formula over `BitVec(64)` / `BitVec(128)`.
     2. **Z3 `QF_LIA` Presburger Proof**: Encodes `0 <= x < 2^W` and `q * 2^S <= x * M < (q + 1) * 2^S` (strictly linear in `x` and `q` since `M` and `2^S` are constants!) and proves `(q * D > x) OR (x - q * D >= D)` is **`unsat`** in ~5–10 ms across all `2^64` inputs.

---

### Stage 4: Layout-Verified C11/C++ Code Emission (`backend/c_emitter.py`)

* Emits `struct __attribute__((packed))` definitions with explicit gap padding arrays (`uint8_t _pad_...[...]`) between sparse PDB offsets so `offsetof(struct _EPROCESS, Field)` in recompiled C is guaranteed to match the original Windows binary down to the byte.
* Structures reducible CFGs into clean `if / else`, natural `while` loops, and rerolled `for` loops, annotating every emitted function with its Z3 proof status, solve time, and verified stages.
