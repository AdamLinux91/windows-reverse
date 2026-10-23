# Whole-kernel decompilation (`ntoskrnl.exe`)

The original 69/527 hand-selected routine tree is not a whole-kernel decompilation. The
`ntrev whole-kernel` pipeline now discovers the complete function universe from the
**matching PE image, x64 `.pdata`, GFIDS, exports, and MSF/PDB procedure/public symbols**,
then emits it in bounded-memory shards.

## Why this is the reliable boundary

A Windows kernel has three different kinds of targets and they must not be conflated:

1. **Normal compiler-generated procedures** — lifted instruction-by-instruction to C11
   with explicit 8/16/32/64-bit register semantics, Win64 stack arguments, memory
   operands, flags, atomic operations, SSE/scalar floating point, and recovered switch
   tables.
2. **Non-C entry/exit and CPU-state procedures** — emitted as relocatable Intel inline
   assembly. This includes syscall/trap paths, `swapgs`, `iretq`/`sysretq`, CR/MSR/IRQL
   paths, interrupt linkages, retpolines, string instructions, `cmpxchg16b`, XSave,
   and functions with non-standard live-in registers. These are not guessed C.
3. **RIP-relative literals, jump tables, import-directory slots, and code mid-labels** —
   resolved to local relocatable labels or exact byte blobs. A raw-ASM `lea`/indirect
   jump therefore never silently becomes an unresolved absolute address.

The emitter is two-pass and streaming. Pass 1 keeps only compact metadata and builds
collision-free C names, PDB/PE symbol aliases, Win64 arity/stack-frame votes, IAT
imports, DIR64 relocation records, mid-ASM label ownership, and subsystem shards. Pass 2
re-disassembles one function at a time and flushes a shard as soon as it is complete.
It does not retain a 1.9-million-instruction AST in memory.

## Command

Use a retail image and its **matching** PDB. The PDB is deliberately loaded with the PE
image so section numbers are mapped correctly; loading an unrelated PDB or loading it
without the image can turn inline data into false functions.

```bash
ntrev whole-kernel \
  /path/to/Windows/System32/ntoskrnl.exe \
  --pdb /path/to/symbols/ntkrnlmp.pdb \
  --output-dir build/ntoskrnl_whole \
  --compile
```

The output tree contains:

```text
build/ntoskrnl_whole/
├── include/{reconstructed_types.h,nt_whole_kernel_runtime.h,nt_whole_kernel_protos.h}
├── src/<subsystem>/*.c                 # verified-C lift or preserved raw ASM shards
├── src/globals/nt_data_sections.S       # reconstructed non-executable data and raw-ASM blobs
├── src/globals/nt_reconstructed_globals.c
├── specs/nt_whole_kernel_rvas.json      # every function, RVA, shard, arity, aliases, raw-ASM state
├── specs/whole_kernel_summary.json
├── obj/*.o                              # created by --compile
└── whole_kernel_reloc.o                 # `ld -r` output created by --compile
```

`--compile` is a hard gate: every generated translation unit must compile, and the
relocatable link must have no unexpected unresolved symbol. Only the normal GCC 128-bit
integer helpers (`__divti3`, `__modti3`, `__udivti3`, `__umodti3`) and the GOT marker
are allowed at this intermediate stage; unresolved raw-ASM/data symbols fail the gate.

The Python API is also available:

```python
from ntrev.whole_kernel import WholeKernelCLifter

lifter = WholeKernelCLifter()
index, summary = lifter.run("ntoskrnl.exe", "ntkrnlmp.pdb", "build/ntoskrnl_whole")
compile_result = lifter.compile_tree("build/ntoskrnl_whole")
assert compile_result.compiled_units == compile_result.translation_units
assert not compile_result.unexpected_unresolved_symbols
```

## Real-kernel run used to validate the pipeline

Against a public retail Windows 10 x64 `10.0.17763.107` image and matching
`ntkrnlmp.pdb` (RSDS `69247313-0560-76BB-CB34-11FD96428714`), the run produced:

| Gate | Result |
|---|---:|
| PE executable bytes analyzed | 8,086,016 bytes |
| `.pdata` primary procedures | 22,199 |
| discovered emitted functions | **22,198** |
| exported functions | 2,760 |
| internal/non-exported functions | **19,438** |
| PDB-named internal procedures | 18,289 |
| preserved raw-ASM procedures | 3,163 |
| normal C-lifted procedures | 19,035 |
| recovered MSVC switch tables | 48 |
| recovered magic-multiplier divisions | 706 |
| recovered loops | 1 |
| PDB reconstructed structures available | 1,478 |
| named data symbols emitted | 6,314 |
| IAT imports indexed | 222 |
| DIR64 relocations indexed | 8,806 |
| generated shards | 39 |
| generated translation units compiled | **41 / 41** |
| relocatable whole-kernel link | **passed** |
| unexpected unresolved symbols | **0** |
| peak Python RSS during streaming pass | approximately 289 MiB |

The run covered all executable sections, not merely exported routines: `.text`,
`PAGE`, `INIT`, `KVASCODE`, `RETPOL`, `PAGELK`, `PAGEKD`, `PAGEVRFY`, `PAGEHDLS`,
`PAGEBGFX`, `INITKDBG`, and `POOLCODE`.

## What “exact” means and what is tested

The pipeline has strong, testable exactness invariants:

- every emitted function is backed by an image entry from `.pdata`/PDB/exports;
- no executable function bytes are silently dropped by the CFG builder;
- every raw-ASM target, RIP-relative data reference, IAT slot, and mid-function label
  resolves in the generated relocatable tree;
- all integer widths, x86-64 zero-extension rules, stack arguments, condition flags,
  atomic operations, and recovered switch targets are represented explicitly;
- every generated shard compiles and the complete tree links relocatably;
- the original instruction stream remains available in the source comments/ASM path and
  in the input image for byte-level audit.

A byte-for-byte identity claim for the **compiled C** routines is intentionally not made:
Microsoft's original C source and compiler/optimization configuration are unavailable,
and C recompilation is not expected to reproduce Microsoft's bytes. For routines that
cannot be safely represented as C, the emitter preserves the original instruction
sequence as raw assembly. Semantic equivalence beyond these structural/build gates must
be established with a Windows VM differential harness (same image build, same CPU
feature mask, and the same kernel entry/driver workload); `ntrev` does not label an
unmeasured C routine “formally equivalent.”

This distinction prevents the previous failure mode where a small manually chosen
function set was reported as a complete drop-in kernel.
