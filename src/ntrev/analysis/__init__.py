"""Stage 3 analysis passes: Type Inference, Compiler De-Optimizer, and Z3 SMT Verifier."""

from .deoptimizer import CompilerDeoptimizer
from .smt_verifier import Z3EquivalenceVerifier
from .type_inference import TypeInferenceEngine

__all__ = [
    "CompilerDeoptimizer",
    "TypeInferenceEngine",
    "Z3EquivalenceVerifier",
]
