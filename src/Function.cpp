#include "Function.h"
#include <vector>

using namespace ABI;

FunctionInfo::FunctionInfo(std::vector<Type *> args, Type *ret,
                           CallingConvention conv)
    : Args(), RetInfo{ret, ABIArgInfo()}, Conv(conv) {

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

ABIArgInfo::ABIArgInfo(ABIArgKind kind) : Kind(kind) {}

FunctionArgInfo FunctionInfo::getReturnInfo() const { return RetInfo; }

void FunctionInfo::setABIReturnInfo(const ABIArgInfo &info) {
  RetInfo.Info = info;
}
