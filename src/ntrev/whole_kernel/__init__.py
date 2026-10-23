"""
Automated Whole-Kernel Decompilation, Compositional Z3 Verification & Sharded Emission.

Handles all 22,000+ exported and internal functions in retail `ntoskrnl.exe` within
bounded memory (< 450 MiB RSS) via 2-pass streaming disassembly, interprocedural
Win64 ABI arity & symbol recovery, dead-flag-aware C11 lifting, raw-ASM preservation,
and relocatable build checks. The C tree is an analysis/reconstruction candidate, not
an unconditional behavior proof. The `exact` API creates the byte-preserving PE to use
when execution must retain the supplied Microsoft build exactly.
"""

from ntrev.whole_kernel.exact import (
    ExactKernelArtifact,
    ExactVerificationResult,
    export_exact_kernel,
    verify_exact_kernel,
)
from ntrev.whole_kernel.lifter_c import (
    FunctionMeta,
    WholeKernelCLifter,
    WholeKernelCompileResult,
    WholeKernelEmitSummary,
    WholeKernelSymbolIndex,
)

__all__ = [
    "ExactKernelArtifact",
    "ExactVerificationResult",
    "FunctionMeta",
    "WholeKernelCLifter",
    "WholeKernelCompileResult",
    "WholeKernelEmitSummary",
    "WholeKernelSymbolIndex",
    "export_exact_kernel",
    "verify_exact_kernel",
]
