"""LLVM IR lifters: built-in x86-64 -> LLVM 22 SSA lifter and Remill / RetDec bridges."""

from .external_lifters import ExternalLifterAdapter
from .x86_to_llvm import LiftedModuleResult, X86ToLLVMLifter

__all__ = [
    "ExternalLifterAdapter",
    "LiftedModuleResult",
    "X86ToLLVMLifter",
]
