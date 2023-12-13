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

////////////////////////////////////////////// The Var structure ///////////////////////////////////////////////////////////////////
class Var
{
  public:
    std::string name;
    int pt_level;
    int st_num;
    Var():name("nothing"),pt_level(0),st_num(0){};
    Var(std::string n, int m, int v):name(n),pt_level(m),st_num(v){};
    void show()
    {
      errs()<<name<<"\npt: "<<pt_level<<"\nS: "<<st_num<<"\n";
    }
    
};
//////////////////////////////////////////// The structure of statement ////////////////////////////////////////////////////////////
using var_collection=std::set<Var>;
using var_collection_equiv=std::pair<Var,Var>;

class Statement
{
  public:
    var_collection self_TREF;
    var_collection self_TGEN;
    var_collection self_TDEF;
    var_collection_equiv self_TEQUIV;
    BasicBlock::iterator start;
    BasicBlock::iterator end;
    void show_self_TREF()
    {
      for(auto i:self_TREF)
      {
        i.show();
      }
    }
    void show_self_TGEN()
    {
      for(auto i:self_TGEN)
      {
        i.show();
      }
    }
    void show_self_TDEF()
    {
      for(auto i:self_TDEF)
      {
        i.show();
      }
    }
    void show_self_TEQUIV()
    {
      self_TEQUIV.first.show();
      errs()<<"===============\n";
      self_TEQUIV.second.show();
      
    }
////////////////////////////// create_self_TREF ///////////////////////////////////////////////////////////////////////////////////////////////
    BasicBlock::iterator  create_self_TREF(BasicBlock::iterator I,int i)
    {
      int st_num=i;
      Var temp;
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
            ++count;
            ++J;
          }
          
          temp.name=a;
          temp.pt_level=count;
          temp.st_num=st_num;
          if(!this->self_TREF.insert(temp).second)
          {
            errs()<<"fail"<<"\n";
          }
        }
        if(get_str(I->getOpcodeName())=="store")
        {
          break;
        }
        ++I;
      }
      return ++I;
    }
////////////////////////////////////////// create_self_TGEN /////////////////////////////////////////////////////////////////////
    void create_self_TGEN()
    {
      BasicBlock::iterator tp=end;
      --tp;
      if(tp->getOperand(1)->getName().str()!="")
      {
        this->self_TGEN.emplace(tp->getOperand(1)->getName().str(),0,0);
      }
      else if(tp->getOperand(1)->getName().str()=="")
      {
        --tp;
        if(tp->getOperand(0)->getName().str()!="")
        {
          this->self_TGEN.emplace(tp->getOperand(0)->getName().str(),1,0);
        }
        else
        {
          --tp;
          this->self_TGEN.emplace(tp->getOperand(0)->getName().str(),2,0);
        }
      } 
    }
////////////////////////////////////// create_self_TEQUIV ////////////////////////////////////////////////////////////////////////
    void create_self_TEQUIV()
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
            // lhs.show();
            // errs()<<"===============\n";
            // rhs.show();
            this->self_TEQUIV.first=lhs;
            this->self_TEQUIV.second=rhs;
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
            //auto temp=std::make_pair(lhs,rhs);
            // lhs.show();
            // errs()<<"===============\n";
            // rhs.show();
            if(rhs.pt_level>lhs.pt_level)
            {
              this->self_TEQUIV.first=rhs;
              this->self_TEQUIV.second=lhs;
            }
            else
            {
              this->self_TEQUIV.first=lhs;
              this->self_TEQUIV.second=rhs;
            }
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
            
            //auto temp=std::make_pair(lhs,rhs);
            // lhs.show();
            // errs()<<"===============\n";
            // rhs.show();
            if(rhs.pt_level>lhs.pt_level)
            {
              this->self_TEQUIV.first=rhs;
              this->self_TEQUIV.second=lhs;
            }
            else
            {
              this->self_TEQUIV.first=lhs;
              this->self_TEQUIV.second=rhs;
            }
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
            ++count_l;
            lhs.name=b->getOperand(0)->getName().str();
            lhs.pt_level=count_l;
            
            
            // lhs.show();
            // errs()<<"===============\n";
            // rhs.show();
            if(rhs.pt_level>lhs.pt_level)
            {
              this->self_TEQUIV.first=rhs;
              this->self_TEQUIV.second=lhs;
            }
            else
            {
              this->self_TEQUIV.first=lhs;
              this->self_TEQUIV.second=rhs;
            }
          }
          
        }
      }
      
      
    }


/////////////////////////////////////// create_self_TDEF////////////////////////////////////////////////////////////////////////////////////////////////
    
    void create_self_TDEF(int i)
    {
      Var temp;
      int counter=1;
      BasicBlock::iterator I=--end;
      if(I->getOperand(1)->getName().str()!="")
      {
        //errs()<<I->getOperand(1)->getName().str()<<"\n";
        if(!this->self_TDEF.emplace(I->getOperand(1)->getName().str(),0,i).second)
        {
          errs()<<"fail\n";
        }
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
        if(!this->self_TDEF.emplace(I->getOperand(0)->getName().str(),counter,i).second)
        {
          errs()<<"fail\n";
        }
        
      }
    }


};
////////////////////////////////////////////// The structure for TEQUIV ///////////////////////////////////////////////////////////////////
class TEQUIV_set
{
  public:
    std::map<std::string,std::string*> T;
    std::vector<std::string> list;
    void initialize(std::string a)
    {
      list.push_back(a);
      std::string *i= new std::string[2];
      T[a]=i;
    }
    
    
};
////////////////////////////////////////////// OverLoad ///////////////////////////////////////////////////////////////////////////////////
bool operator==(const Var a,const Var b)
{
  return (a.name == b.name && a.pt_level == b.pt_level);
}
bool operator<(const Var a,const Var b)
{
  if(a.name != b.name && a.pt_level<=b.pt_level)
  {
    return true;
  }
  else if(a.name == b.name && a.pt_level<b.pt_level)
  {
    return true;
  }
  else
  {
    return false;
  }
}
bool operator>(const Var a,const Var b)
{
  if(a.name != b.name && a.pt_level>b.pt_level)
  {
    return true;
  }
  else if(a.name == b.name && a.pt_level>b.pt_level)
  {
    return true;
  }
  else
  {
    return false;
  }
}
bool operator==(const std::pair<Var,Var> a, const std::pair<Var,Var> b)
{
  if(a.first==b.first && a.second==b.second)
  {
    return true;
  }
  else if(a.first==b.second && a.second==b.first)
  {
    return true;
  }
  else{
    return false;
  }
}
bool operator<(const std::pair<Var,Var> a, const std::pair<Var,Var> b)
{
  
  if(a==b)
  {
    return false;
  }
  // if(a.first <b.first)
  // {
  //   return true;
  // }
  // else if (a.first==b.first && a.second<b.second)
  // {
  //   return true;
  // }
  // else{
  //   return false;
  // }
  else
  {
    return true;
  }
  
}
bool operator>(const std::pair<Var,Var> a, const std::pair<Var,Var> b)
{
  if(a==b)
  {
    return false;
  }
  if(a.first >b.first)
  {
    return true;
  }
  else if (a.first==b.first && a.second>b.second)
  {
    return true;
  }
  else{
    return false;
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// RUN ///////////////////////////////////////////////////////////////////////////////// 
PreservedAnalyses HW2Pass::run(Function &F, FunctionAnalysisManager &FAM) 
{
  BasicBlock *entry_block;
  std::vector<Statement> S;
  var_collection TGEN;
  var_collection TREF;
  var_collection TDEF;
  TEQUIV_set TEQUIV;
  int S_num=0;
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
    a=tp.create_self_TREF(a,i);
    tp.end=a;
    // tp.show_self_TREF();
    // errs()<<"\n";
    S.push_back(tp);
  }

  
//////////////////// Handling Statement :self_TGEN /////////////////////////////////////////////////////////////////////////////////////
  
  for(auto &i:S)
  {
    i.create_self_TGEN();
    // i.show_self_TGEN();
    // errs()<<"\n";
  }

/////////////////// Handling Statement :self_TEQUIV /////////////////////////////////////////////////////////////////////////////////////
  int o=0;
  for(auto &i:S)
  {
    //errs()<<"statement "<<o<<"\n";
    i.create_self_TEQUIV();
    // i.show_self_TEQUIV(); 
    // errs()<<"\n";
    // o++;
  }
  
/////////////////// Handling Statement :self_TDEF ///////////////////////////////////////////////////////////////////////////////////////

  for(int i=0;i<S_num;i++)
  {
    // errs()<<"statement :"<<i<<"\n";
    S[i].create_self_TDEF(i);
    // S[i].show_self_TDEF();
    // errs()<<"\n";
  }
/////////////////// Managing TEQUIV //////////////////////////////////////////////////////////////////////////////////////////////////////

  // for(auto i:S)
  // {
  //   errs()<<"======TGEN======: \n";
  //   i.show_self_TGEN(); 
  //   errs()<<"======TREf======: \n";
  //   i.show_self_TREF();
  //   errs()<<"======TDEF======: \n";
  //   i.show_self_TDEF();
  //   errs()<<"======TEQUIV======: \n";
  //   i.show_self_TEQUIV();
  //   errs()<<"====================\n\n";
  // }
  TEQUIV.initialize("b");
  

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
