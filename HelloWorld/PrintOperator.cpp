//=============================================================================
// FILE:
//    DeleteInstruction.cpp
//
// DESCRIPTION:
//    Visits all functions in a module, prints their names and the number of
//    arguments via stderr. Strictly speaking, this is an analysis pass (i.e.
//    the functions are not modified). However, in order to keep things simple
//    there's no 'print' method here (every analysis pass should implement it).
//
// USAGE:
//    1. Legacy PM
//      opt -load libDeleteInstruction.dylib -legacy-delete-instruction -disable-output `\`
//        <input-llvm-file>
//    2. New PM
//      opt -load-pass-plugin=libDeleteInstruction.dylib -passes="delete-instruction" `\`
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
// DeleteInstruction implementation
//-----------------------------------------------------------------------------
// No need to expose the internals of the pass to the outside world - keep
// everything in an anonymous namespace.
namespace {

// This method implements what the pass does
bool visitor(Function &F) {
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
    //   ins_cnt += 1;
    //   StringRef Name = Inst.getOpcodeName();
    //   errs() << "第" << ins_cnt << "条指令: " << Inst << "\n";
    //   errs() << "    " << Name << "\n";
    // }
    ins_cnt = 0;
    for (auto Inst = BB.begin(), IE = BB.end(); Inst != IE; ++Inst) {
      ins_cnt += 1;
      errs() << "第" << ins_cnt << "个指令是: ";
      errs() << "    " << *Inst << "\n";
      errs() << Inst->getOpcodeName() << "\n";
      errs() << ins_cnt << ": ";
      // continue;
      if (auto *BinOp = dyn_cast<BinaryOperator>(Inst)) {
        errs() << "BinaryOperator\n";
        continue;
      }
      if (auto *Call = dyn_cast<CallBase>(Inst)) {
        errs() << "CallBase\n";
        continue;
      }
      if (auto *Store = dyn_cast<StoreInst>(Inst)) {
        errs() << "Store\n";
        continue;
      }
      if (auto *Ret = dyn_cast<ReturnInst>(Inst)) {
        errs() << "Return\n";
        continue;
      }
      if (auto *RMW = dyn_cast<AtomicRMWInst>(Inst)) {
        errs() << "AtomicRMWInst\n";
        continue;
      }
      if (auto *Unary = dyn_cast<UnaryInstruction>(Inst)) {
        errs() << "UnaryInstruction\n";
        continue;
      }
      errs() << "???\n";
      continue;
    }
  }
  errs() << F.getName() << "有" << bb_cnt << "个BasicBlock\n";
  return Changed;
}

// New PM implementation
struct DeleteInstruction : PassInfoMixin<DeleteInstruction> {
  // Main entry point, takes IR unit to run the pass on (&F) and the
  // corresponding pass manager (to be queried if need be)
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {
    bool Changed = visitor(F);
    return (Changed ? PreservedAnalyses::none()
                    : PreservedAnalyses::all());
  }
};

} // namespace

//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
llvm::PassPluginLibraryInfo getDeleteInstructionPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "DeleteInstruction", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "delete-instruction") {
                    FPM.addPass(DeleteInstruction());
                    return true;
                  }
                  return false;
                });
          }};
}

// This is the core interface for pass plugins. It guarantees that 'opt' will
// be able to recognize DeleteInstruction when added to the pass pipeline on the
// command line, i.e. via '-passes=delete-instruction'
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getDeleteInstructionPluginInfo();
}
