#ifndef CALL_H
#define CALL_H

#include "Function.h"
#include "Type.h"
#include <memory>
#include <stdint.h>

namespace ABI {
class ABIArgInfo;

// for a target to implement basically
class ABIInfo {
public:
  virtual void ComputeInfo(FunctionInfo &FI);

private:
};

// System-V AMD 64 ABI
class X86_64ABIInfo : ABIInfo {
public:
  // type are defined in the system-v abi
  enum Class {
    Integer = 0,
    SSE,
    SSEUp,
    X87,
    X87Up,
    ComplexX87,
    NoClass,
    Memory,

    Num
  };

  virtual void ComputeInfo(FunctionInfo &FI) override;

  ABIArgInfo ClassifyArgumentType(std::shared_ptr<Type *> type);
private:
};
} // namespace ABI

#endif
