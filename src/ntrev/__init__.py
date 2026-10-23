"""
ntrev — Automated Windows NT Binary Decompilation & Z3 Semantic Equivalence Engine.

Phase 1 foundation for the Core Integrated Hybrid Kernel architecture:
  1. Lifter / IR Translator (x86-64 PE32+ -> Verified LLVM IR + Inline-Asm Detector + Remill/RetDec Bridges)
  2. Symbol & Structural Mapper (MSF 7.00 PDB Parser, Ghidra Programmatic API Bridge, NT Struct Reconstructor)
  3. Type Inference, De-Optimization & Z3 SMT Equivalence Prover (Loop Reroller, Magic-Div Recovery, Formal Z3 Proofs)
  4. Verified LLVM IR + Structural Map -> Exact Compilable C/C++ Replica Emitter
"""

__version__ = "0.1.0"
