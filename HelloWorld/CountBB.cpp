//=============================================================================
// FILE:
//    CountBB.cpp
//
// DESCRIPTION:
//    Visits all functions in a module, prints their names and the number of
//    arguments via stderr. Strictly speaking, this is an analysis pass (i.e.
//    the functions are not modified). However, in order to keep things simple
//    there's no 'print' method here (every analysis pass should implement it).
//
// USAGE:
//    1. Legacy PM
//      opt -load libCountBB.dylib -legacy-count-bb -disable-output `\`
//        <input-llvm-file>
//    2. New PM
//      opt -load-pass-plugin=libCountBB.dylib -passes="count-bb" `\`
//        -disable-output <input-llvm-file>
//
//
// License: MIT
//=============================================================================
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

//-----------------------------------------------------------------------------
// CountBB implementation
//-----------------------------------------------------------------------------
// No need to expose the internals of the pass to the outside world - keep
// everything in an anonymous namespace.
namespace {

// This method implements what the pass does
void visitor(Function &F) {
    bool Changed = false;

    errs() << "(llvm-tutor) Hello from: "<< F.getName() << "\n";
    errs() << "(llvm-tutor)   number of arguments: " << F.arg_size() << "\n";
    unsigned int bb_cnt = 0;
    for (auto &BB : F) {
      bb_cnt += 1;
      errs() << "第" << bb_cnt << "个BasicBlock:\n";
      errs() << BB << "\n";
      unsigned int ins_cnt = 0;
      // for (auto &Inst: BB) {
      for (auto Inst = BB.begin(), IE = BB.end(); Inst != IE; ++Inst) {
        ins_cnt += 1;
        // errs() << Inst << "\n";
        auto *BinOp = dyn_cast<BinaryOperator>(Inst);
        if (!BinOp)
          continue;
        errs() << "第" << ins_cnt << "个Instruction是二进制操作\n";
        errs() << *Inst << "\n";
        if (BinOp->getOpcode() != Instruction::Add)
          continue;
        errs() << "  并且是一个+操作\n";
        errs() << "    " << *BinOp->getOperand(1) << "\n";
        Value *LHS = BinOp->getOperand(0);
        Value *RHS = BinOp->getOperand(1);
        errs() << "    RHS" << RHS << "\n";  // 仅仅是个值,但是不知道怎么判断是不是0
        errs() << "    *RHS" << *RHS << "\n";  // 具体的值
        if (ConstantInt* CI = dyn_cast<ConstantInt>(RHS)) {
          errs() << "    并且右侧是常数\n";
          errs() << CI->getValue() << "\n";
        }
        // Value *Val0 = Value(BinOp->getType(), 0);
        // if (*RHS == Val0) {
        // }
        // errs() << *RHS->Value << "\n";  // 
        // Value Zero;
        // if (*RHS == Zero || *LHS == Zero) {
        // // if (*BinOp->getOperand(1) == 0) {
        //   errs() << "竟然加了一个0\n";
        //   // Instruction *NewValue = BinaryOperator::CreateAdd
        // }
        // errs() << BinOp->getOperand(1) << "\n";
        // errs() << *BinOp << "\n";
      }
    }
    errs() << F.getName() << "有" << bb_cnt << "个BasicBlock\n";
}

// New PM implementation
struct CountBB : PassInfoMixin<CountBB> {
  // Main entry point, takes IR unit to run the pass on (&F) and the
  // corresponding pass manager (to be queried if need be)
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {
    visitor(F);
    return PreservedAnalyses::all();
  }
};

} // namespace

//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
llvm::PassPluginLibraryInfo getCountBBPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "CountBB", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "count-bb") {
                    FPM.addPass(CountBB());
                    return true;
                  }
                  return false;
                });
          }};
}

// This is the core interface for pass plugins. It guarantees that 'opt' will
// be able to recognize CountBB when added to the pass pipeline on the
// command line, i.e. via '-passes=count-bb'
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getCountBBPluginInfo();
}
