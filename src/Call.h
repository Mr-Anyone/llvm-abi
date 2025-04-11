#ifndef CALL_H
#define CALL_H

#include "Function.h"
#include "Type.h"
#include "llvm/IR/LLVMContext.h"
#include <memory>
#include <stdint.h>

namespace ABI {
class ABIArgInfo;

// for a target to implement basically
class ABIInfo {
public:
  virtual void ComputeInfo(FunctionInfo &FI) = 0;

private:
};

// System-V AMD 64 ABI
class X86_64ABIInfo : public ABIInfo {
public:
  // type are defined in the system-v abi
  enum Class {
    Integer = 0,
    SSE,
    SSEUp,
    X87,
    X87Up,
    Complex,
    NoClass,
    Memory,
  };

  X86_64ABIInfo(llvm::LLVMContext &context) : Context(context) {}

  virtual void ComputeInfo(FunctionInfo &FI) override;

private:
  llvm::LLVMContext &Context;

  llvm::Type *getSSEType(Type *type, uint64_t offset);
  llvm::Type *getIntegerType(Type *type, uint64_t offset);

  void Classify(Type *type, Class &Low, Class &High);
  Class Merge(Class one, Class two);
  void PostMerger(Class &Low, Class &High, uint64_t size);
};
} // namespace ABI

#endif
