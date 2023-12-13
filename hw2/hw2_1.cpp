#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include<algorithm>
//#include "PassPlugin.h"

using namespace llvm;

namespace {

struct HW2Pass : public PassInfoMixin<HW2Pass> {
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM);
};


/////////////////////  Useful Func.: transform the value class to string  ////////////////////////////////////////////////////////
std::string get_str(Value* val)
{
  std::string str;
  raw_string_ostream stream(str);
  val->print(stream);
  return str;
}
std::string get_str(Type* val)
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



//////////////////////////////////////////////// The Structure for Variable////////////////////////////////////////////////////////////// 
class Var
{
  public:
    std::string name;
    int pt_level;
    int st_num;
    Var():name(""),pt_level(0),st_num(0){};
    Var(std::string n, int m, int v):name(n),pt_level(m),st_num(v){};
    void show()
    {
      errs()<<name<<"\npt: "<<pt_level<<"\nS: "<<st_num<<"\n";
    }
    bool operator==(const Var b)
    {
     return (this->name == b.name && this->pt_level == b.pt_level);
    }
    bool operator<(const Var b)
    {
     return (this->pt_level < b.pt_level);
    }
};


//////////////////////////////////////////////// The Structure for Statement ///////////////////////////////////////////////////////////
using Set=std::set<Var>;
using Set_eqv=std::set<std::pair<Var,Var>>;
class Statement
{
  public:
    BasicBlock::iterator start;
    BasicBlock::iterator end;
    Set Self_TREF;
    Set Self_TGEN;
    Set Self_TDEF;
    Set_eqv Self_TEQUIV;


    Set f(Set aa,bool a)
    { 
      if(a)
      {
        for(auto i:aa)
        {
          i.show();
        }
        return aa;
      }
      else
      {
        return aa;
      }
    }
    Set get_Self_TREF(bool a)
    {
      return f(Self_TREF,a);
    }
    Set get_Self_TGEN(bool a)
    {
      return f(Self_TGEN,a);
    }
    Set get_Self_TDEF(bool a)
    {
      return f(Self_TDEF,a);
    }
    Set_eqv get_Self_TEQUIV(bool a)
    {
      if(a)
      {
        for(auto i:Self_TEQUIV)
        {
          errs()<<"l: ";
          i.first.show();
          errs()<<"==========="<<"\n";
          errs()<<"r: ";
          i.second.show();
          errs()<<"\n";
        }
        return Self_TEQUIV;
      }
      else
      {
        return Self_TEQUIV;
      }
    }
    void output_statement()
    {
      for(BasicBlock::iterator I = start, be = end; I != be; I++)
      {
        errs()<<*I<<"\n";
      }
    }
    BasicBlock::iterator  create_Self_TREF(BasicBlock::iterator I,int i)
    {
      Instruction &rI=*I;
      int st_num=i;
      while(true)
      {
        std::string a=I->getOperand(0)->getName().str();
        int count=0;
        if(a!="" && get_str(I->getOpcodeName())=="load")
        {
          BasicBlock::iterator J=I;
          ++J;
          while(J->getOperand(0)->getName().str()=="" && get_str(J->getOpcodeName())=="load")
          {
            count++;
            ++J;
          }
          this->Self_TREF.emplace(a,count,st_num);
        }
        if(get_str(I->getOpcodeName())=="store")
        {
          break;
        }
        ++I;
      }
      return ++I;
    }
    void create_Self_TGEN()
    {
      BasicBlock::iterator tp=end;
      --tp;
      if(tp->getOperand(1)->getName().str()!="")
      {
        this->Self_TGEN.emplace(tp->getOperand(1)->getName().str(),0,0);
      }
      else if(tp->getOperand(1)->getName().str()=="")
      {
        --tp;
        if(tp->getOperand(0)->getName().str()!="")
        {
          this->Self_TGEN.emplace(tp->getOperand(0)->getName().str(),1,0);
        }
        else
        {
          --tp;
          this->Self_TGEN.emplace(tp->getOperand(0)->getName().str(),2,0);
        }
      } 
    }
    void create_Self_TEQUIV()
    {
      bool t=true;
      for(BasicBlock::iterator i=start;i!=end;i++)
      {
        if(get_str(i->getOpcodeName())!="store" && get_str(i->getOpcodeName())!="load")
        {
          t=false;
          //errs()<<get_str(i->getOpcodeName())<<"\n";
        }
      }
      if(t)
      {
        for(BasicBlock::iterator i=start;i!=end;i++)
        {
          if(get_str(i->getOperand(0)->getType())=="i32" )
          {
            if(ConstantInt *ci = dyn_cast<ConstantInt>(i->getOperand(0)))
            {
              //errs()<<*ci<<"\n";
              continue;
            }
          }
          if(i->getOperand(0)->getName().str().substr(0,3)=="add")
          {
            continue;
          }
          int count_l=0;
          int count_r=0;
          Var lhs;
          Var rhs;
          BasicBlock::iterator a=start;
          BasicBlock::iterator b=end;
        
          if(a==--b && get_str(i->getOpcodeName())=="store")
          {
            lhs.name=i->getOperand(1)->getName().str();
            rhs.name=i->getOperand(0)->getName().str();

            lhs.pt_level=++count_l;
            rhs.pt_level=count_r;

            auto temp=std::make_pair(lhs,rhs);
            this->Self_TEQUIV.insert(temp);
          }
          if(get_str(i->getOpcodeName())=="store" && i->getOperand(0)->getName().str()=="" && i->getOperand(1)->getName().str()=="")
          {
            for(;b->getOperand(0)->getName().str()=="";--b)
            {
              count_l++;
            }
            lhs.name=b->getOperand(0)->getName().str();
            
            --b;
            for(;b->getOperand(0)->getName().str()=="";--b)
            {
              count_r++;
            }
            rhs.name=b->getOperand(0)->getName().str();
            
            if(count_r>0 && count_l>0)
            {
              --count_r;
              --count_l;
            }
            lhs.pt_level=count_l;
            rhs.pt_level=count_r;
            auto temp=std::make_pair(lhs,rhs);
            this->Self_TEQUIV.insert(temp);
          }
          if(get_str(i->getOpcodeName())=="store" && i->getOperand(0)->getName().str()=="" && i->getOperand(1)->getName().str()!="")
          {
            lhs.name=b->getOperand(1)->getName().str();
            lhs.pt_level=count_l;
            for(;b->getOperand(0)->getName().str()=="";--b)
            {
              count_r++;
            }
            count_r--;
            rhs.name=b->getOperand(0)->getName().str();
            rhs.pt_level=count_r;
            
            auto temp=std::make_pair(lhs,rhs);
            this->Self_TEQUIV.insert(temp);
          }
          if(get_str(i->getOpcodeName())=="store" && i->getOperand(0)->getName().str()!="" && i->getOperand(1)->getName().str()=="")
          {
            
            rhs.name=b->getOperand(0)->getName().str();
            rhs.pt_level=count_r;
            --b;
            count_l++;
            for(;b->getOperand(0)->getName().str()=="";--b)
            {
              errs()<<*b<<"\n";
              count_l++;
            }
            lhs.name=b->getOperand(0)->getName().str();
            lhs.pt_level=count_l;
            
            auto temp=std::make_pair(lhs,rhs);
            this->Self_TEQUIV.insert(temp);
          }
        }
      }
      
      
    }

    void create_Self_TDEF(int i)
    {
      Var temp;
      int counter=1;
      BasicBlock::iterator I=--end;
      if(I->getOperand(1)->getName().str()!="")
      {
        //errs()<<I->getOperand(1)->getName().str()<<"\n";
        this->Self_TDEF.emplace(I->getOperand(1)->getName().str(),0,i);
      }
      else
      {
        --I;
        while(I->getOperand(0)->getName().str()=="")
        {
          ++counter;
          --I;
        }
        //errs()<<counter<<"\n";
        this->Self_TDEF.emplace(I->getOperand(0)->getName().str(),counter,i);
      }
    }
};

//////////////////////////////////// manage TEQUIV /////////////////////////////////////////////////////////////////////////////////



void manage_TEQUIV(std::vector<Set_eqv> t)
{
    
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// RUN ///////////////////////////////////////////////////////////////////////////////// 
PreservedAnalyses HW2Pass::run(Function &F, FunctionAnalysisManager &FAM) {
 
 
///////////////////////// Initializing //////////////////////////////////////////////////////////////////////////////////
  BasicBlock *entry_block;
  std::set<Var> variables;
  std::set<Set> TREF;
  std::set<Set> TGEN;
  std::set<Set> TDEF;
  std::set<Set_eqv> TEQUIV;
  int S_num=0;
  std::vector<Statement> S;

  for(auto &B: F)
  {
    entry_block=&B;
  }


///////////////////////// Get the number of statements //////////////////////////////////////////////////////////////// 
  
  for(BasicBlock::iterator I = entry_block->begin(), be = entry_block->end(); I != be; I++)
  {
    Instruction &rI=*I;
    if(auto *II=dyn_cast<StoreInst>(&rI))
    {
      S_num++;
    }
  }  

  S.reserve(S_num);
////////////////////// Get the list of Variable /////////////////////////////////////////////////////////////////////////

  std::string ptr="";
  for(BasicBlock::iterator I = entry_block->begin(), be = entry_block->end(); I != be; I++)
  {
    std::string str;
    llvm::raw_string_ostream Inst(str);
    Inst << *I;
    std::string n;
    bool ptr;
    auto index_eql=find(Inst.str(),'=');
    auto index_pe=find(Inst.str(),'%');
    if(Inst.str().find("alloca")!=-1)
    {
      n=Inst.str().substr(Inst.str().find('%')+1,index_eql-index_pe-2);
      if(Inst.str().find("ptr")!=-1)
      {
        ptr=true;
      }
      else
      {
        ptr=false;
      }
      variables.emplace(n,ptr,0);
    }
  }
////////////////////// Handling Statement : Self_Self_TREF ///////////////////////////////////////////////////////////////////////////////////
  
  BasicBlock::iterator a = entry_block->begin();
  BasicBlock::iterator b;
  while(get_str(a->getOpcodeName())=="alloca")
  {
    ++a;
  }
  for(int i=0;i<S_num;i++)
  {
    Statement tp;
    tp.start=a;
    a=tp.create_Self_TREF(a,i);
    tp.end=a;
    // tp.get_Self_TREF(true);
    // errs()<<"\n";
    S.push_back(tp);
    
  }
//////////////////// Handling Statement :Self_TGEN /////////////////////////////////////////////////////////////////////////////////////
  
  for(auto i:S)
  {
    i.create_Self_TGEN();
    // i.get_Self_TGEN(true);
    // errs()<<"\n";
  }
  
/////////////////// Handling Statement :Self_TEQUIV /////////////////////////////////////////////////////////////////////////////////////

  for(auto i:S)
  {
    i.create_Self_TEQUIV();
    i.get_Self_TEQUIV(true); 
  }

/////////////////// Handling Statement :Self_TDEF ///////////////////////////////////////////////////////////////////////////////////////

  for(int i=0;i<S_num;i++)
  {
    S[i].create_Self_TDEF(i);
    //S[i].get_Self_TDEF(true);
  }

////////////////// Composing TEQUIV //////////////////////////////////////////////////////////////////////////////////////////////////////
  
  for(int i=0;i<S_num;i++)
  {
    TEQUIV.insert(S[i].Self_TEQUIV);
    //manage_TEQUIV(TEQUIV);
  }

  return PreservedAnalyses::all();
}

} // end anonymous namespace

extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK
llvmGetPassPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "HW2Pass", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "hw2") {
                    FPM.addPass(HW2Pass());
                    return true;
                  }
                  return false;
                });
          }};
}
