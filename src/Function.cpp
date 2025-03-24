#include "Function.h"
#include <vector>

using namespace ABI;

FunctionInfo::FunctionInfo(std::vector<std::shared_ptr<Type *>> args, Type *ret,
                           CallingConvention conv)
    : Args(), Ret(ret), Conv(conv) {

        // initialize the argument
  for (const std::shared_ptr<Type *> ty : args) {
    Args.push_back({ty, ABIArgInfo()});
  }
}

FunctionInfo::ArgIter FunctionInfo::GetArgBegin() { return Args.begin(); }

FunctionInfo::ArgIter FunctionInfo::GetArgEnd() { return Args.end(); }
