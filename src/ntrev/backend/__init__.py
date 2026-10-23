"""Stage 4 backend: Custom Verified LLVM IR + Structural Map -> Exact C/C++ Emitter."""

from .c_emitter import VerifiedCEmitter

__all__ = ["VerifiedCEmitter"]
