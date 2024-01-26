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
          
          for(int i=count;i>-1;i--)
          {
            temp.name=a;
            temp.pt_level=i;
            temp.st_num=st_num;
            if(!this->self_TREF.insert(temp).second)
            {
              errs()<<"fail"<<"\n";
            }
          
          }
          // if(!this->self_TREF.insert(temp).second)
          // {
          //   errs()<<"fail"<<"\n";
          // }
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
    void create_self_TGEN(int i)
    {
      BasicBlock::iterator tp=end;
      --tp;
      if(tp->getOperand(1)->getName().str()!="")
      {
        this->self_TGEN.emplace(tp->getOperand(1)->getName().str(),0,i);
      }
      else if(tp->getOperand(1)->getName().str()=="")
      {
        --tp;
        if(tp->getOperand(0)->getName().str()!="")
        {
          this->self_TGEN.emplace(tp->getOperand(0)->getName().str(),1,i);
        }
        else
        {
          --tp;
          this->self_TGEN.emplace(tp->getOperand(0)->getName().str(),2,i);
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
    
    // void create_self_TDEF(int i)
    // {
    //   Var temp;
    //   int counter=1;
    //   BasicBlock::iterator I=--end;
    //   if(I->getOperand(1)->getName().str()!="")
    //   {
    //     //errs()<<I->getOperand(1)->getName().str()<<"\n";
    //     self_TDEF.name=I->getOperand(1)->getName().str();
    //     self_TDEF.pt_level=0;
    //     self_TDEF.st_num=i;
    //     // if(!this->self_TDEF.emplace(I->getOperand(1)->getName().str(),0,i).second)
    //     // {
    //     //   errs()<<"fail\n";
    //     // }
    //   }
    //   else
    //   {
    //     --I;
    //     while(I->getOperand(0)->getName().str()=="")
    //     {
    //       ++counter;
    //       --I;
    //     }
    //     //errs()<<counter<<"\n";
    //     self_TDEF.name=I->getOperand(0)->getName().str();
    //     self_TDEF.pt_level=counter;
    //     self_TDEF.st_num=i;
    //     // if(!this->self_TDEF.emplace(I->getOperand(0)->getName().str(),counter,i).second)
    //     // {
    //     //   errs()<<"fail\n";
    //     // }
        
    //   }
    // }


};
////////////////////////////////////////////// The structure for TEQUIV ///////////////////////////////////////////////////////////////////
class TEQUIV_set
{
  public:
    std::map<std::string,std::string*> T;
    std::vector<std::pair<Var,Var>> set;
    std::vector<std::string> list;
    std::set<std::pair<std::string,std::string>> relation;
    void initialize(std::string a)
    {
      list.push_back(a);
      std::string *i= new std::string[2];
      T[a]=i;
      T[a][0]="";
      T[a][1]="";
    }
    
    void show_list()
    {
      for(auto i:list)
      {
        errs()<<i<<"\n";
      }
    }
    bool check_in_list(std::string b)
    {
      for(auto i:list)
      {
        if(i==b)
        {
          return true;
        }
      }
      return false;
    }
    void add(var_collection_equiv a)
    {
      if(check_in_list(a.first.name) )
      {
        if(relation.empty())
        {
          
          //errs()<<a.first.name<<"\n";
          T[a.first.name][a.first.pt_level-1]=a.second.name;
        }
        else
        {
          
          for(auto i:relation)
          {
            //errs()<<i.first<<" "<<i.second<<"\n";
            if(a.first.name == i.first && a.second.name != i.second)
            {
              T[i.first][1]=a.second.name;
              T[i.second][0]=a.second.name;
            }
            else if(a.first.name == i.second && a.second.name != i.first)
            {
              T[i.second][0]=a.second.name;
              T[i.first][1]=a.second.name;
            }
          }
        }
      }
      if(check_in_list(a.first.name) && check_in_list(a.second.name) && relation.emplace(a.first.name,a.second.name).second)
      {
        
        T[a.first.name][1]=T[a.second.name][0];
      }
      put_into_set();
    }

    void show_set()
    {
      
      for(auto i:set)
      {
        errs()<<"(";
        for(int j=0;j<i.first.pt_level;j++)
        {
          errs()<<"*";
        }
        errs()<<i.first.name<<",";
        for(int j=0;j<i.second.pt_level;j++)
        {
          errs()<<"*";
        }
        errs()<<i.second.name;
        errs()<<"), ";
      }
      
    }
    void show()
    {
      for(auto i:T)
      {
        errs()<<i.first<<"\n======\n";
        errs()<<i.second[0]<<" "<<i.second[1]<<"\n";
      }
    }
    void put_into_set()
    {
      set.clear();
      for(auto i:list)
      {
        Var temp1;
        Var temp2;
        for(int j=0;j<2;j++)
        {
          if(T[i][j]!="")
          {
            temp1.name=i;
            temp1.pt_level=1+j;
            temp1.st_num=0;

            temp2.name=T[i][j];
            temp2.pt_level=0;
            temp2.st_num=0;
            set.push_back(std::make_pair(temp1,temp2));
          }
        }
        
      }
    }
    bool empty()
    {
      return T.empty();
    }
};
////////////////////////////////////////////// OverLoad ///////////////////////////////////////////////////////////////////////////////////
bool operator<(const std::pair<std::string,std::string> a, const std::pair<std::string,std::string> b)
{
  if(a.first == b.first && a.second==b.second)
  {
    return false;
  }
  else if(a.first == b.second && a.second==b.first)
  {
    return false;
  }
  else{
    return true;
  }
}
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
// bool operator>(const Var a,const Var b)
// {
//   if(a.name != b.name && a.pt_level>b.pt_level)
//   {
//     return true;
//   }
//   else if(a.name == b.name && a.pt_level>b.pt_level)
//   {
//     return true;
//   }
//   else
//   {
//     return false;
//   }
// }
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
// bool operator>(const std::pair<Var,Var> a, const std::pair<Var,Var> b)
// {
//   if(a==b)
//   {
//     return false;
//   }
//   if(a.first >b.first)
//   {
//     return true;
//   }
//   else if (a.first==b.first && a.second>b.second)
//   {
//     return true;
//   }
//   else{
//     return false;
//   }
// }
//////////////////////////////////////////////  TDEF ///////////////////////////////////////////////////////////////////////////
class TDEF_set
{ 
  public:
    std::vector<Var> TDEF;
    void show()
    {
      for(auto i:TDEF)
      {
        errs()<<"(";
        for(int j=0;j<i.pt_level;j++)
        {
          errs()<<"*";
        }
        errs()<<i.name<<", S"<<i.st_num+1<<"), ";
      }
     
    }

    void add(Var item,int now_st)
    {
      bool check=false;
      if(TDEF.empty())
      {
        item.st_num=now_st;
        TDEF.push_back(item);
      }
      else
      {
        for(auto &i:TDEF)
        {
          if(i.name==item.name && i.pt_level==item.pt_level)
          {
            //errs()<<"the same "<<i.name<<"\n";
            i.st_num=now_st;
            check=true;
          } 
        }
        if(!check)
        {
          item.st_num=now_st;
          TDEF.push_back(item);
        }
      }
     
    }
};
//////////////////////////////////////////////show_collection ///////////////////////////////////////////////////////////////////////
void show_collection(std::set<Var> a)
{
  for(auto i:a)
  {
    for(int j=0;j<i.pt_level;j++)
    {
      errs()<<"*";
    }
    errs()<<i.name<<",";
  }
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// RUN ///////////////////////////////////////////////////////////////////////////////// 
PreservedAnalyses HW2Pass::run(Function &F, FunctionAnalysisManager &FAM) 
{
  BasicBlock *entry_block;
  std::vector<Statement> S;
  std::set<Var> TGEN;
  std::set<Var> TREF;
  TDEF_set TDEF;
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
////////////////////// Get the list of Variable & Initializing TEQUIV/////////////////////////////////////////////////////////////////////////

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
        TEQUIV.initialize(n); ////initializing TEQUIV
      }
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
    a=tp.create_self_TREF(a,i);
    tp.end=a;
    // errs()<<"stm :"<<i+1<<"\n";
    // tp.show_self_TREF();
    // errs()<<"\n";
    S.push_back(tp);
  }

  
//////////////////// Handling Statement :self_TGEN /////////////////////////////////////////////////////////////////////////////////////
  
  for(int i=0;i<S_num;i++)
  {
    S[i].create_self_TGEN(i);
    // i.show_self_TGEN();
    // errs()<<"\n";
  }

/////////////////// Handling Statement :self_TEQUIV /////////////////////////////////////////////////////////////////////////////////////
  int o=0;
  for(auto &i:S)
  {
    // errs()<<"statement "<<o<<"\n";
    i.create_self_TEQUIV();
    // i.show_self_TEQUIV(); 
    // errs()<<"\n";
    // o++;
  }
  
/////////////////// Handling Statement :self_TDEF ///////////////////////////////////////////////////////////////////////////////////////

  for(int i=0;i<S_num;i++)
  {
    //S[i].create_self_TDEF(i);
    //errs()<<"statement :"<<i<<"\n";
    // S[i].show_self_TDEF();
    // errs()<<"\n";
  }
/////////////////// Managing  //////////////////////////////////////////////////////////////////////////////////////////////////////
  // for(int i=0;i<S_num;i++)
  // {
    
  //   TDEF.add(S[i].self_TDEF);
  //   errs()<<"stm :"<<i+1<<"\n";
  //   TDEF.show();
  //   errs()<<"\n";
  // }
  
  for(int i=0;i<S_num;i++)
  {
    TEQUIV.add(S[i].self_TEQUIV);

    errs()<<"S"<<i+1<<":--------------------"<<"\n";

    errs()<<"TREF:{ ";
    for(auto i:S[i].self_TREF)
    {
      TREF.insert(i);
      for(auto k:TEQUIV.set)
      {
        if(i.name==k.first.name && i.pt_level==k.first.pt_level)
        {
          TREF.insert(k.second);
          for(auto m:TEQUIV.set)
          {
            if(k.second==m.first)
            {
              TREF.insert(m.second);
            }
            if(k.second==m.second)
            {
              TREF.insert(m.first);
            }
          }
        }
        if(i.name==k.second.name && i.pt_level==k.second.pt_level)
        {
          TREF.insert(k.first);
          for(auto m:TEQUIV.set)
          {
            if(k.first==m.first)
            {
              TREF.insert(m.second);
            }
            if(k.first==m.second)
            {
              TREF.insert(m.first);
            }
          }
        }
      }
    }
   
    show_collection(TREF);
    S[i].self_TREF=TREF;
    
    errs()<<"\b"<<" }\n";

    errs()<<"TGEN:{ ";
    for(auto i:S[i].self_TGEN)
    {
      TGEN.insert(i);
      for(auto k:TEQUIV.set)
      {
        if(i.name==k.first.name && i.pt_level==k.first.pt_level)
        {
          TGEN.insert(k.second);
          
        }
        if(i.name==k.second.name && i.pt_level==k.second.pt_level)
        {
          TGEN.insert(k.first);
          
        }
      }
    }
   
    show_collection(TGEN);
    S[i].self_TGEN=TGEN;
    
    errs()<<"\b"<<" }\n";

    
    
    errs()<<"DEP:{\n";
    for(auto j:TDEF.TDEF)
    { 
      for(auto k:TREF)
      {
        if(j.name==k.name && j.pt_level==k.pt_level)
        {
          errs()<<"     ";
          for(int m=0;m<j.pt_level;m++)
          {
            errs()<<"*";
          }
          errs()<<j.name<<": S"<<j.st_num+1<<"------>S"<<i+1<<"\n";
        }
      }
    }
    for(auto j:TDEF.TDEF)
    { 
      for(auto k:TGEN)
      {
        if(k.pt_level==0 && j.name==k.name && j.pt_level==k.pt_level)
        {
          errs()<<"     ";
          for(int m=0;m<j.pt_level;m++)
          {
            errs()<<"*";
          }
          errs()<<j.name<<": S"<<j.st_num+1<<"---o-->S"<<i+1<<"\n";
        }
      }
    } 
    errs()<<"}\n";

    errs()<<"TDEF:{";

    S[i].self_TDEF=S[i].self_TGEN;
    for(auto j:S[i].self_TDEF)
    {
      TDEF.add(j,i);
    }
    TDEF.show();
    errs()<<"\b\b";
    errs()<<"}\n";

    errs()<<"TEQUIV:{";
    TEQUIV.show_set();
    errs()<<"\b\b"<<" }\n";


    

    TREF.clear();
    TGEN.clear();
    errs()<<"\n";
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
