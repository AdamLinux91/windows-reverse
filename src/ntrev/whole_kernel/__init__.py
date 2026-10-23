"""
Automated Whole-Kernel Decompilation, Compositional Z3 Verification & Sharded Emission.

Handles all 22,000+ exported and internal functions in retail `ntoskrnl.exe` within
bounded memory (< 450 MiB RSS) via 2-pass streaming disassembly, interprocedural
Win64 ABI arity & symbol recovery, dead-flag-eliminated C11 lifting, raw-ASM
preservation, compositional per-block Z3 bisimulation proofs, and lockstep differential
binary execution.
"""

from ntrev.whole_kernel.lifter_c import (
    FunctionMeta,
    WholeKernelCLifter,
    WholeKernelCompileResult,
    WholeKernelEmitSummary,
    WholeKernelSymbolIndex,
)

__all__ = [
    "FunctionMeta",
    "WholeKernelCLifter",
    "WholeKernelCompileResult",
    "WholeKernelEmitSummary",
    "WholeKernelSymbolIndex",
]
