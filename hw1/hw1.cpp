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

std::string get_str(Value* val)
{
  std::string str;
  raw_string_ostream stream(str);
  val->print(stream);
  return str;
}
std::string get_str(const char *val)
{
  std::string str;
  str= val;
  //errs()<<str<<"\n";
  return str;
}
int gcd(int a, int b) 
{ 
    // Everything divides 0 
    if (a == 0) 
        return b; 
    if (b == 0) 
        return a; 
  
    // base case 
    if (a == b) 
        return a; 
  
    // a is greater 
    if (a > b) 
        return gcd(a - b, b); 
    return gcd(a, b - a); 
} 
void diophantine(int low_b,int up_b,std::string equ_a, std::string equ_b)
{
  int u,v,g,a,b,c;
  if(equ_a=="i")
  {
    a=1;
  }
  
  if(equ_b=="i")
  {
    b=1;
  }
  
}
PreservedAnalyses HW1Pass::run(Function &F, FunctionAnalysisManager &FAM) 
{
  // auto &LI = FAM.getResult<LoopAnalysis>(F);
  // llvm::ScalarEvolutionAnalysis SEA;
  // auto &SE = FAM.getResult<ScalarEvolutionAnalysis>(F);
  
  // for (const auto &L : LI) {
  //   if (auto LB = L->getBounds(SE)) {
  //     outs() << "Valid loop found" << "\n";
  //     outs() << LB->getInitialIVValue() << " "
  //            << LB->getStepInst() << "\n"; 
  //   } 
  // }
  

  //get the block of forloop 
  BasicBlock *entry_block;
  BasicBlock *for_body_block; 
  BasicBlock *for_cond_block; 
  BasicBlock *for_inc_block;
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
          else if(BBinLoop->getName()=="for.inc")
          {
              for_inc_block=BBinLoop;
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
  for (Instruction &I : *for_inc_block)
  {
    if(auto *II = dyn_cast<AddOperator>(&I))
    {
      if (ConstantInt* TI = dyn_cast<ConstantInt>(II->getOperand(1))) 
      {
        if (TI->getBitWidth() <= 32) 
        {
          int iteration_stripe = TI->getSExtValue();
          errs()<<"iteration_stripe :"<<iteration_stripe<<'\n';
        }
      } 
      
    }
  }
  
  //seperate S1,S2,...
  using ar_name=std::string;
  using idx=std::string;
  struct S
  {
    ar_name l_name;
    ar_name r_name;
    idx l_idx;
    idx r_idx;
    S(ar_name l_n, ar_name r_n, idx l_i, idx r_i):l_name(l_n),
    r_name(r_n),l_idx(l_i),r_idx(r_i){};
  };
  std::vector<S> s;
  int S_num=0;
  
  for(BasicBlock::iterator I = for_body_block->begin(), be = for_body_block->end(); I != be; I++)
  {
    Instruction &rI=*I;
    if(auto *II=dyn_cast<StoreInst>(&rI))
    {
      s.emplace_back("","","","");
      S_num++;
    }
  }
  int i=0;
  for(BasicBlock::iterator I = for_body_block->begin(), be = for_body_block->end(); I != be && i<S_num; I++)
  {
    
    if(get_str(I->getOpcodeName())=="getelementptr" && s[i].r_name=="" )
    {
      //errs()<<*I->getOperand(0)<<"\n";
      s[i].r_name=get_str(I->getOperand(0));
    }
    else if(get_str(I->getOpcodeName())=="getelementptr" && s[i].r_name!="")
    {
      //errs()<<*I->getOperand(0)<<"\n";
      s[i].l_name=get_str(I->getOperand(0));
    }
    else if(s[i].r_name!="" && s[i].l_name!="" )
    {
      //errs()<<s[i].l_name<<" "<<s[i].r_name<<"\n";
      ++i;
    }
  }
  i=0;
  std::string equ="i";
  for(BasicBlock::iterator I = for_body_block->begin(), be = for_body_block->end(); I != be; I++)
  {
    
    BasicBlock::iterator cur=I;
    if(get_str(I->getOperand(0))=="  %i = alloca i32, align 4")
    {
      cur++;
      //Instruction &rI=*cur;
      while(get_str(cur->getOpcodeName())!="sext" && i<S_num)
      {
          std::string opt=get_str(cur->getOpcodeName());
          if(opt=="mul")
          {
            // errs()<<"mul"<<"\n";
            std::string st=&get_str(cur->getOperand(0))[4];
            equ+="*"+st;
          }
          else if(opt=="add")
          {
            // errs()<<"add"<<"\n";
            std::string st=&get_str(cur->getOperand(1))[4];
            equ+="+"+st;
          }
          else if(opt=="sub")
          {
            // errs()<<"sub"<<"\n";
            std::string st=&get_str(cur->getOperand(1))[4];
            equ+="-"+st;
          }
          else if(opt=="div")
          {
            // errs()<<"div"<<"\n";
            std::string st=&get_str(cur->getOperand(0))[4];
            equ+="/"+st;
          }
          cur++;
      }
      //errs()<<i<<" "<<equ<<"\n";
      if(s[i].r_idx=="" && s[i].l_idx=="")
      {
        s[i].r_idx=equ;
        //errs()<<i<<" "<<s[i].r_idx<<"\n";
      }
      else if(s[i].r_idx!="" && s[i].l_idx=="")
      {
        s[i].l_idx=equ;
        //errs()<<i<<" "<<s[i].l_idx<<"\n";
        i++;
      }
      equ="i";
    }
  }
  
  diophantine(0,1,"i*2-12","i+5");
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