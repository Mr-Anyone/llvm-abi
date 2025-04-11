#include "Function.h"
#include <vector>

using namespace ABI;

FunctionInfo::FunctionInfo(std::vector<Type *> args, Type *ret,
                           CallingConvention conv)
    : Conv(conv), Args(), RetInfo{ret, ABIArgInfo()} {

  // initialize the argument
  for (Type *ty : args) {
    FunctionArgInfo info{ty, ABIArgInfo()};
    Args.push_back(info);
  }
}

FunctionInfo::ArgIter FunctionInfo::GetArgBegin() { return Args.begin(); }

FunctionInfo::ArgIter FunctionInfo::GetArgEnd() { return Args.end(); }

ABIArgInfo::ABIArgInfo() {}

ABIArgKind ABIArgInfo::GetKind() const { return Kind; }

ABIArgInfo::ABIArgInfo(ABIArgKind kind, llvm::Type *type)
    : Kind(kind), type(type) {}

FunctionArgInfo FunctionInfo::getReturnInfo() const { return RetInfo; }

void FunctionInfo::setABIReturnInfo(const ABIArgInfo &info) {
  RetInfo.Info = info;
}
  llvm::Type *ABIArgInfo::getType() const{
      return type;

  }
