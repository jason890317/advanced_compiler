#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/Analysis/LoopInfo.h"
using namespace llvm;

namespace {

struct HW1Pass : public PassInfoMixin<HW1Pass> {
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM);
};

PreservedAnalyses HW1Pass::run(Function &F, FunctionAnalysisManager &FAM) {
  // auto &LI = FAM.getResult<LoopAnalysis>(F);
  // llvm::ScalarEvolutionAnalysis SEA;
  // auto &SE = FAM.getResult<ScalarEvolutionAnalysis>(F);
  // for (const auto &L : LI) {
  //     const SCEV *TripCount = SE.getBackedgeTakenCount(&L);
     
  //     if(L->getBounds(SE))
  //     {
  //       errs()<<"\n\n"<<"success"<<"\n";
  //     }
    
  // }

  //get the block of forloop 
  BasicBlock *entry_block;
  BasicBlock *for_body_block; 
  BasicBlock *for_cond_block; 
  for (BasicBlock &BB : F)
  {
    if(BB.getName()=="entry")
    {
      entry_block=&BB;
    }
  }
  auto &LI = FAM.getResult<LoopAnalysis>(F);
  for (const auto &L : LI) {
        for (llvm::Loop::block_iterator BB = L->block_begin(); BB != L->block_end(); ++BB) {
          llvm::BasicBlock *BBinLoop = *BB;
          if(BBinLoop->getName()=="for.body")
          {
              for_body_block=BBinLoop;
          }
          else if(BBinLoop->getName()=="for.cond")
          {
              for_cond_block=BBinLoop;
          }

      }
  }

  //get the iteration of loop
  int iteration_begin;
  int iteration_stripe;
  int iteration_end;
  for (Instruction &I : *entry_block)
  {
    if(auto *II = dyn_cast<StoreInst>(&I))
    {
      // if(II->getOperand(1)->getOperand(0))
      // {
      //   if (ConstantInt* CI = dyn_cast<ConstantInt>(II->getOperand(0))) 
      //   {
      //     if (CI->getBitWidth() <= 32) 
      //     {
      //       int iteration_end = CI->getSExtValue();
      //       errs()<<"iteration_end :"<<iteration_end<<'\n';
      //     }
      //   }   
      // }
      //errs()<<*II->getOperand(1)<<"\n";
      std::string i= "  %i = alloca i32, align 4";  
      std::string str;
      raw_string_ostream stream(str);
      II->getOperand(1)->print(stream);
      str = stream.str();
      if(i.compare(str)==0)
      {
        if (ConstantInt* CI = dyn_cast<ConstantInt>(II->getOperand(0))) 
        {
          if (CI->getBitWidth() <= 32) 
          {
            int iteration_begin = CI->getSExtValue();
            errs()<<"iteration_begin :"<<iteration_begin<<'\n';
          }
        } 
      }
      
    }
  }
  for (Instruction &I : *for_cond_block) 
  { 
    if(auto *II = dyn_cast<ICmpInst>(&I))
    {
      if (ConstantInt* CI = dyn_cast<ConstantInt>(II->getOperand(1))) 
      {
        if (CI->getBitWidth() <= 32) 
        {
          int iteration_end = CI->getSExtValue();
          errs()<<"iteration_end :"<<iteration_end<<'\n';
        }
      } 
    }
  }
        
  
  return PreservedAnalyses::all();
}

} // end anonymous namespace

extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK
llvmGetPassPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "HW1Pass", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "hw1") {
                    FPM.addPass(HW1Pass());
                    return true;
                  }
                  return false;
                });
          }};
}