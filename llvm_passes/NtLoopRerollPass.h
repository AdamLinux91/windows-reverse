#pragma once

#include "llvm/IR/PassManager.h"

namespace ntrev {

/// LLVM NewPassManager pass that recovers high-level C constructs from
/// compiler-optimized lifted NT kernel IR:
///   1. Collapses linearly unrolled strided memory reduction chains back into
///      canonical counted loops (`PHINode` induction variable + backedge).
///   2. Rewrites 128-bit widening magic-multiplier + right-shift sequences
///      back into clean `udiv` instructions.
class NtLoopRerollPass : public llvm::PassInfoMixin<NtLoopRerollPass> {
public:
  llvm::PreservedAnalyses run(llvm::Function &F,
                              llvm::FunctionAnalysisManager &FAM);
};

} // namespace ntrev
