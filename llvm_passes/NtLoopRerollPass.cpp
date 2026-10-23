#include "NtLoopRerollPass.h"

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/PatternMatch.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

#include <cmath>
#include <cstdint>
#include <vector>

using namespace llvm;
using namespace llvm::PatternMatch;

namespace ntrev {

namespace {

/// Attempt to recover unsigned 64-bit constant division from:
///   %ext  = zext i64 %x to i128
///   %prod = mul nuw i128 %ext, <MagicConst>
///   %sh   = lshr i128 %prod, <64 + S>
///   %res  = trunc i128 %sh to i64
bool recoverMagicUnsignedDiv(Function &F) {
  bool Changed = false;
  std::vector<Instruction *> DeadInsts;

  for (BasicBlock &BB : F) {
    for (Instruction &I : BB) {
      auto *Trunc = dyn_cast<TruncInst>(&I);
      if (!Trunc || !Trunc->getType()->isIntegerTy(64))
        continue;

      Value *X = nullptr;
      const APInt *MagicAP = nullptr;
      const APInt *ShiftAP = nullptr;

      if (!match(Trunc->getOperand(0),
                 m_LShr(m_Mul(m_ZExt(m_Value(X)), m_APInt(MagicAP)),
                        m_APInt(ShiftAP))))
        continue;

      if (!X->getType()->isIntegerTy(64) || MagicAP->getBitWidth() != 128)
        continue;

      uint64_t TotalShift = ShiftAP->getLimitedValue();
      if (TotalShift < 64 || TotalShift > 126)
        continue;

      uint64_t Magic64 = MagicAP->trunc(64).getZExtValue();
      if (Magic64 == 0)
        continue;

      // Estimate divisor D ~= 2^(TotalShift) / Magic64
      long double Numer = std::ldexp(1.0L, static_cast<int>(TotalShift));
      uint64_t CandDiv =
          static_cast<uint64_t>(std::llround(Numer / static_cast<long double>(Magic64)));
      if (CandDiv <= 1)
        continue;

      // Verify exactness on ceiling formula: Magic64 == ceil(2^TotalShift / CandDiv)
      APInt TwoPow(192, 1);
      TwoPow = TwoPow.shl(static_cast<unsigned>(TotalShift));
      APInt DivAP(192, CandDiv);
      APInt Quot = (TwoPow + DivAP - 1).udiv(DivAP);
      if (Quot.ugt(APInt::getMaxValue(64).zext(192)) ||
          Quot.getZExtValue() != Magic64)
        continue;

      IRBuilder<> Builder(Trunc);
      Value *UDiv =
          Builder.CreateUDiv(X, ConstantInt::get(Trunc->getType(), CandDiv),
                             "recovered_udiv");
      Trunc->replaceAllUsesWith(UDiv);
      DeadInsts.push_back(Trunc);
      Changed = true;
    }
  }

  for (Instruction *Dead : DeadInsts)
    Dead->eraseFromParent();

  return Changed;
}

struct StridedSlice {
  LoadInst *Load;
  BinaryOperator *AccumOp;
  int64_t ByteOffset;
};

/// Detect K >= 3 consecutive strided loads feeding an associative reduction
/// in a single basic block and collapse them into a canonical LLVM loop.
bool rerollLinearReductionInBlock(BasicBlock &BB) {
  std::vector<StridedSlice> Chain;
  Value *BaseAddr = nullptr;
  Instruction::BinaryOps AssocOpcode = Instruction::Add;

  for (Instruction &I : BB) {
    auto *BO = dyn_cast<BinaryOperator>(&I);
    if (!BO)
      continue;
    if (BO->getOpcode() != Instruction::Add &&
        BO->getOpcode() != Instruction::Xor &&
        BO->getOpcode() != Instruction::Or)
      continue;

    LoadInst *LI = dyn_cast<LoadInst>(BO->getOperand(0));
    if (!LI)
      LI = dyn_cast<LoadInst>(BO->getOperand(1));
    if (!LI || !LI->isSimple())
      continue;

    Value *PtrOp = LI->getPointerOperand();
    Value *CandBase = nullptr;
    int64_t Offset = 0;

    if (auto *ITP = dyn_cast<IntToPtrInst>(PtrOp)) {
      const APInt *DispAP = nullptr;
      if (match(ITP->getOperand(0), m_Add(m_Value(CandBase), m_APInt(DispAP)))) {
        Offset = DispAP->getSExtValue();
      } else {
        CandBase = ITP->getOperand(0);
        Offset = 0;
      }
    } else {
      continue;
    }

    if (Chain.empty()) {
      BaseAddr = CandBase;
      AssocOpcode = BO->getOpcode();
      Chain.push_back({LI, BO, Offset});
    } else if (CandBase == BaseAddr && BO->getOpcode() == AssocOpcode) {
      Chain.push_back({LI, BO, Offset});
    }
  }

  if (Chain.size() < 4)
    return false;

  int64_t Stride = Chain[1].ByteOffset - Chain[0].ByteOffset;
  if (Stride <= 0)
    return false;
  for (size_t i = 2; i < Chain.size(); ++i) {
    if (Chain[i].ByteOffset - Chain[i - 1].ByteOffset != Stride)
      return false;
  }

  // Split block and build counted loop over [0, Chain.size())
  Function *F = BB.getParent();
  LLVMContext &Ctx = F->getContext();
  Type *I64Ty = Type::getInt64Ty(Ctx);
  Type *ElemTy = Chain[0].Load->getType();
  Type *AccTy = Chain[0].AccumOp->getType();

  Instruction *SplitPt = Chain[0].Load;
  BasicBlock *TailBB = BB.splitBasicBlock(SplitPt, "reroll.exit");
  BasicBlock *LoopBB = BasicBlock::Create(Ctx, "reroll.loop", F, TailBB);

  // Redirect preheader terminator to LoopBB
  BB.getTerminator()->setSuccessor(0, LoopBB);

  IRBuilder<> LoopB(LoopBB);
  PHINode *IdxPhi = LoopB.CreatePHI(I64Ty, 2, "reroll.iv");
  PHINode *AccPhi = LoopB.CreatePHI(AccTy, 2, "reroll.acc");

  Value *InitAcc = ConstantInt::get(AccTy, 0);
  IdxPhi->addIncoming(ConstantInt::get(I64Ty, 0), &BB);
  AccPhi->addIncoming(InitAcc, &BB);

  Value *ByteOff = LoopB.CreateAdd(
      ConstantInt::get(I64Ty, Chain[0].ByteOffset),
      LoopB.CreateMul(IdxPhi, ConstantInt::get(I64Ty, Stride)));
  Value *EA = LoopB.CreateAdd(BaseAddr, ByteOff);
  Value *Ptr = LoopB.CreateIntToPtr(EA, PointerType::getUnqual(Ctx));
  Value *Elem = LoopB.CreateLoad(ElemTy, Ptr, "reroll.ld");
  Value *ElemExt = (ElemTy == AccTy) ? Elem : LoopB.CreateZExt(Elem, AccTy);
  Value *NextAcc = LoopB.CreateBinOp(AssocOpcode, AccPhi, ElemExt, "reroll.next");
  Value *NextIdx = LoopB.CreateAdd(IdxPhi, ConstantInt::get(I64Ty, 1), "reroll.inc");

  IdxPhi->addIncoming(NextIdx, LoopBB);
  AccPhi->addIncoming(NextAcc, LoopBB);

  Value *Cond = LoopB.CreateICmpULT(
      NextIdx, ConstantInt::get(I64Ty, Chain.size()), "reroll.cond");
  LoopB.CreateCondBr(Cond, LoopBB, TailBB);

  Chain.back().AccumOp->replaceAllUsesWith(NextAcc);
  for (auto It = Chain.rbegin(); It != Chain.rend(); ++It) {
    if (It->AccumOp->use_empty())
      It->AccumOp->eraseFromParent();
    if (It->Load->use_empty())
      It->Load->eraseFromParent();
  }
  return true;
}

} // namespace

PreservedAnalyses NtLoopRerollPass::run(Function &F,
                                        FunctionAnalysisManager &) {
  bool Changed = recoverMagicUnsignedDiv(F);

  for (BasicBlock &BB : F) {
    if (rerollLinearReductionInBlock(BB)) {
      Changed = true;
      break;
    }
  }

  return Changed ? PreservedAnalyses::none() : PreservedAnalyses::all();
}

} // namespace ntrev

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "NtDeoptPass", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "nt-deopt") {
                    FPM.addPass(ntrev::NtLoopRerollPass());
                    return true;
                  }
                  return false;
                });
          }};
}
