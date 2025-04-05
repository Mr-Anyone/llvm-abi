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
    X87,     // FIXME: add this later
    X87Up,   // FIXME: add this later
    Complex, // FIXME: add this laterX87,
    NoClass,
    Memory,
  };

  virtual void ComputeInfo(FunctionInfo &FI) override;

private:
  void Classify(Type *type, Class &Low, Class &High);
  Class Merge(Class one, Class two);
  void PostMerger(Class &Low, Class &High);
};
} // namespace ABI

#endif
