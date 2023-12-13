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

//transform the value class to string
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
  return str;
}

//resolving the eqution from the structure given
void get_equ(int* a, int* c, std::string equ)
{
  if(equ=="i")
  {
    *a=1;
    *c=0;
  }
  else if(equ[1]=='+')
  {
    *a=1;
    *c=stoi(equ.substr(2,equ.length()));
  }
  else if(equ[1]=='-')
  {
    *a=1;
    *c=-1*stoi(equ.substr(2,equ.length()));
  }
  else if(equ[1]=='*')
  {
    auto f=equ.find("-");
    auto g=equ.find("+");
    if(f!=-1)
    {
      *a=stoi(equ.substr(2,f));
      *c=-1*stoi(equ.substr(f+1,equ.length()));
    }
    else if(g!=-1)
    {
      *a=stoi(equ.substr(2,equ.length()));
      *c=-1*stoi(equ.substr(g+1,equ.length()));
    }
    else if(f==-1 && g==-1)
    {
      *a=stoi(equ.substr(2,equ.length()));
      *c=0;
    }
  }
}

//getting the gcd, and u, v
int xGCD(int a, int b, int &x, int &y) {
    if(b == 0) {
       x = 1;
       y = 0;
       return a;
    }

    int x1, y1, gcd = xGCD(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return gcd;
}

//geting the bounds of t which is intermediate of diophantine
void get_b(double &l_x,double &u_x, double &l_y, double &u_y,double &L,double &U)
{
  double ar[]={l_x,l_y,u_x,u_y};
  if(l_x>u_x)
  {
    std::swap(l_x,u_x);
  }
  if(l_y>u_y)
  {
    std::swap(l_y,u_y);
  }
  if(l_y>u_x || l_x>u_y)
  {
    L=0;
    U=-1;
  }
  else
  {
    sort(ar,ar+4);
    L=ceil(ar[1]);
    U=floor(ar[2]);
  }
}

//diophantine fuction
std::vector<int*> diophantine(int low_b,int up_b,std::string equ_a, std::string equ_b)
{
  int u,v;
  int a,b,c,c_a,c_b;
  int g;
  int t;
  std::vector<int*> result;
  get_equ(&a,&c_a,equ_a);
  get_equ(&b,&c_b,equ_b);
  c=-1*(c_a-c_b);
 
  b=b*-1;
  g=xGCD(a,b,u,v);
  
  int x[2],y[2];
  x[0]=u*c/g;
  x[1]=b/g;
  y[0]=v*c/g;
  y[1]=(-1)*a/g;
  
  double L=low_b,U=up_b;

  if(low_b>=up_b)
  {
    U=low_b;
    L=up_b;
  }

  double l_x,u_x;
  double l_y,u_y;
  
  l_x=(L-x[0])/x[1];
  u_x=(U-x[0])/x[1];

  l_y=(L-y[0])/y[1];
  u_y=(U-y[0])/y[1];

  get_b(l_x,u_x,l_y,u_y,L,U);
  b=b*-1;
  for(int i=L;i<=U;i++)
  {
    int* vec = new int[2];
    vec[0]=x[0]+x[1]*i;
    vec[1]=y[0]+y[1]*i;
    result.push_back(vec);  
  }
  return result;
}
bool greater(int* s,int* t)
{
  return s[0]<t[0];
}

//the struct to store the info of instructions
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

PreservedAnalyses HW1Pass::run(Function &F, FunctionAnalysisManager &FAM) 
{ 
  BasicBlock *entry_block;
  BasicBlock *for_body_block; 
  BasicBlock *for_inc_block;

  //get the iteration of loop
  auto &LI = FAM.getResult<LoopAnalysis>(F);
  auto &SE =FAM.getResult<ScalarEvolutionAnalysis>(F);
  int iteration_begin=0;
  int iteration_stripe=0;
  int iteration_end=0;
  for (const auto &L : LI) 
  {
    if(ConstantInt *TI = dyn_cast<ConstantInt>(&L->getBounds(SE)->getInitialIVValue()))
    {
      iteration_begin = TI->getSExtValue();
    }
    if(ConstantInt *TI = dyn_cast<ConstantInt>(&L->getBounds(SE)->getFinalIVValue()))
    {
      iteration_end = TI->getSExtValue();
    }
    if(ConstantInt *TI = dyn_cast<ConstantInt>(L->getBounds(SE)->getStepValue()))
    {
      iteration_stripe = TI->getSExtValue();
    }
  }

  //find the body block and inc block
  for (const auto &L : LI) {
        for (llvm::Loop::block_iterator BB = L->block_begin(); BB != L->block_end(); ++BB) {
          llvm::BasicBlock *BBinLoop = *BB;
          if(BBinLoop->getName()=="for.body")
          {
              for_body_block=BBinLoop;
              
          }
          else if(BBinLoop->getName()=="for.inc")
          {
              for_inc_block=BBinLoop;
          }
      }
  }

  //seperate S1,S2,..., and store them into the struct
  std::vector<S> s;
  int S_num=0;
  
  //initialize the structure
  for(BasicBlock::iterator I = for_body_block->begin(), be = for_body_block->end(); I != be; I++)
  {
    Instruction &rI=*I;
    if(auto *II=dyn_cast<StoreInst>(&rI))
    {
      s.emplace_back("","","","");
      S_num++;
    }
  }

  //the process of getting the array name of each instruction
  int i=0;
  for(BasicBlock::iterator I = for_body_block->begin(), be = for_body_block->end(); I != be && i<S_num; I++)
  {
    
    if(get_str(I->getOpcodeName())=="getelementptr" && s[i].r_name=="" )
    {
      s[i].r_name=get_str(I->getOperand(0))[3];
    }
    else if(get_str(I->getOpcodeName())=="getelementptr" && s[i].r_name!="")
    {
      s[i].l_name=get_str(I->getOperand(0))[3];
    }
    else if(s[i].r_name!="" && s[i].l_name!="" )
    {
      ++i;
    }
  }

  //the process of getting the idx
  i=0;
  std::string equ="i";
  for(BasicBlock::iterator I = for_body_block->begin(), be = for_body_block->end(); I != be; I++)
  {
    BasicBlock::iterator cur=I;
    if(get_str(I->getOperand(0)).substr(3,1)== equ || get_str(I->getOpcodeName())== "mul")
    {
      while(get_str(cur->getOpcodeName())!="getelementptr" && i<S_num)
      {
          std::string opt=get_str(cur->getOpcodeName());
          if(opt=="mul")
          {
            std::string st=&get_str(cur->getOperand(0))[4];
            equ+="*"+st;
          }
          else if(opt=="add")
          {
            std::string st=&get_str(cur->getOperand(1))[4];
            equ+="+"+st;
          }
          else if(opt=="sub")
          {
            std::string st=&get_str(cur->getOperand(1))[4];
            equ+="-"+st;
          }
          else if(opt=="div")
          {
            std::string st=&get_str(cur->getOperand(0))[4];
            equ+="/"+st;
          }
          cur++;
      }
      if(s[i].r_idx=="" && s[i].l_idx=="")
      {
        s[i].r_idx=equ;
      }
      else if(s[i].r_idx!="" && s[i].l_idx=="")
      {
        s[i].l_idx=equ;
        i++;
      }
      equ="i";  
    }
  }

  //running the diophantine func, getting the result of the dependency, and stroe them
  //into the string.
  std::string flow_dependence="====Flow Dependency====\n";
  std::string anti_dependence="====Anti-Dependency====\n";
  std::string output_dependence="====Output Dependency====\n";
  std::vector<int*> result;

  //part 1,left to right
  for(int i=0;i<S_num;i++)
  {
    for(int j=i;j<S_num && j<i+3;j++)
    {
      if(s[i].l_name==s[j].r_name)
      {
        result=diophantine(iteration_begin,iteration_end-1,s[i].l_idx,s[j].r_idx);
        std::sort(result.begin(),result.end(),greater);
        for(auto item=result.begin();item!=result.end();item++)
        {
          if((*item)[0]-(*item)[1]<=0)
          {
            flow_dependence+="(i="+std::to_string((*item)[0])+",i="+std::to_string((*item)[1])+")\n";
            flow_dependence+=s[i].l_name+":S"+std::to_string(i+1)+" -----> S"+std::to_string(j+1)+"\n";

          }
          else{ 
            anti_dependence+="(i="+std::to_string((*item)[1])+",i="+std::to_string((*item)[0])+")\n";
            anti_dependence+=s[i].l_name+":S"+std::to_string(j+1)+" --A--> S"+std::to_string(i+1)+"\n";
          }
        }
      }
    }
  }
  
  //part 2,right to left
  for(int i=0;i<S_num;i++)
  {
    for(int j=i+1;j<S_num && j<i+3;j++)
    {
      if(s[i].r_name==s[j].l_name)
      {
        result=diophantine(iteration_begin,iteration_end-1,s[i].r_idx,s[j].l_idx);
        std::sort(result.begin(),result.end(),greater);
        for(auto item=result.begin();item!=result.end();item++)
        {
          if((*item)[0]-(*item)[1]>0)
          {
            flow_dependence+="(i="+std::to_string((*item)[0])+",i="+std::to_string((*item)[1])+")\n";
            flow_dependence+=s[i].l_name+":S"+std::to_string(i+1)+" -----> S"+std::to_string(j+1)+"\n";

          }
          else{ 
            anti_dependence+="(i="+std::to_string((*item)[1])+",i="+std::to_string((*item)[0])+")\n";
            anti_dependence+=s[i].l_name+":S"+std::to_string(j+1)+" --A--> S"+std::to_string(i+1)+"\n";
          }
        }
      }
    }
  }

  //part 3,left to left
  for(int i=0;i<S_num;i++)
  {
    for(int j=i+1;j<S_num && j<i+3;j++)
    {
      if(s[i].l_name==s[j].l_name)
      {
        result=diophantine(iteration_begin,iteration_end-1,s[i].l_idx,s[j].l_idx);
        std::sort(result.begin(),result.end(),greater);
        for(auto item=result.begin();item!=result.end();item++)
        {
          if((*item)[0]-(*item)[1]<=0)
          {
            output_dependence+="(i="+std::to_string((*item)[0])+",i="+std::to_string((*item)[1])+")\n";
            output_dependence+=s[i].l_name+":S"+std::to_string(i+1)+" --O--> S"+std::to_string(j+1)+"\n";

          }
          else{ 
            output_dependence+="(i="+std::to_string((*item)[1])+",i="+std::to_string((*item)[0])+")\n";
            output_dependence+=s[i].l_name+":S"+std::to_string(j+1)+" --O--> S"+std::to_string(i+1)+"\n";
          }
        }
      }
    }
  }
  // output the result
  errs()<<flow_dependence;
  errs()<<anti_dependence;
  errs()<<output_dependence;
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