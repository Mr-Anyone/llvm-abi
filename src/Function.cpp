#include "Function.h"
#include <vector>

using namespace ABI;

FunctionInfo::FunctionInfo(std::vector<std::shared_ptr<Type>> args,
                           std::shared_ptr<Type> ret, CallingConvention conv)
    : Args(), RetInfo{ret, ABIArgInfo()}, Conv(conv) {

  // initialize the argument
  for (const std::shared_ptr<Type> ty : args) {
    Args.push_back({ty, ABIArgInfo()});
  }
}

FunctionInfo::ArgIter FunctionInfo::GetArgBegin() { return Args.begin(); }

FunctionInfo::ArgIter FunctionInfo::GetArgEnd() { return Args.end(); }

ABIArgInfo::ABIArgInfo() {}
ABIArgKind ABIArgInfo::GetKind() const { return Kind; }

ABIArgInfo::ABIArgInfo(ABIArgKind kind) : Kind(kind) {}

FunctionArgInfo FunctionInfo::getReturnInfo() const { return RetInfo; }

void FunctionInfo::setABIArgInfo(ABIArgInfo info) const {
  RetInfo.Info = info;
}
