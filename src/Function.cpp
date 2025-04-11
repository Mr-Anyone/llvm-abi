#include "Function.h"
#include <iostream>
#include <vector>

using namespace ABI;

FunctionInfo::FunctionInfo(llvm::SmallVector<Type *> args, Type *ret,
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
llvm::Type *ABIArgInfo::getType() const { return type; }

void FunctionInfo::dump() const {

  std::cout << "\n";
  RetInfo.Ty->dump();
  std::cout << " unknown_func(";

  for (std::size_t i = 0; i < Args.size(); ++i) {
    Args[i].Ty->dump();
    if (i != Args.size() - 1)
      std::cout << " , ";
  }

  std::cout << ")";
  std::cout << "\n";
  std::cout << "LLVM IR Parameter type information: " << std::endl;

  for (std::size_t i = 0; i < Args.size(); ++i) {
    llvm::Type *type = Args[i].Info.getType();
    if (!type)
      std::cout << "null(mapping not implemented yet)" << std::endl;
    else
      type->dump();
  }
  std::cout << "LLVM IR Return Type:" << std::endl;
  if (RetInfo.Ty)
    RetInfo.Ty->dump();
  else
    std::cout << "null(unimplemented)" << std::endl;

  std::cout << "\n" << std::endl;
}
