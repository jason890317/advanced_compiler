#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include<algorithm> 

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
using var_collection=std::set<Var>;
using var_collection_equiv=std::set<std::pair<Var,Var>>;
class statement
{
  public:
    var_collection self_TREF;
    var_collection self_TGEN;
    var_collection self_TDEF;
    var_collection_equiv self_TEQUIV;
    llvm::BasicBlock::iterator start;
    llvm::BasicBlock::iterator end;

};