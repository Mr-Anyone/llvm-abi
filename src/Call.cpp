#include "Call.h"
#include "Type.h"

#include <cassert>
#include <iostream>
#include <memory>

using namespace ABI;

// The arugments here is in think
X86_64ABIInfo::Class
X86_64ABIInfo::ClassifyArgumentType(std::shared_ptr<Type> type) {

  assert(type && "null type is undefined!");

  if (type->isIntegerType()) {
    std::shared_ptr<::Integer> intergerType =
        std::dynamic_pointer_cast<::Integer>(type);

    if (intergerType->getSize() <= 8)
      return Integer;

    // FIXME: what about types that are _BitInt(128), etc.
    assert(false && "how to represent type that are less than 8 bytes  ");
  }

  assert(false && "Todo: b re");
}

// Mainly taken from clang
void X86_64ABIInfo::ComputeInfo(FunctionInfo &FI) {
  // Keep track of the number of assigned registers.
  unsigned FreeIntRegs = 6;
  unsigned FreeSSERegs = 8;
  unsigned NeededInt = 0, NeededSSE = 0, MaxVectorWidth = 0;

  // TODO: fix me
  // if (IsRegCall && FI.getReturnType()->getTypePtr()->isRecordType() &&
  //    !FI.getReturnType()->getTypePtr()->isUnionType()) {
  //  FI.getReturnInfo() = classifyRegCallStructType(
  //      FI.getReturnType(), NeededInt, NeededSSE, MaxVectorWidth);
  //  if (FreeIntRegs >= NeededInt && FreeSSERegs >= NeededSSE) {
  //    FreeIntRegs -= NeededInt;
  //    FreeSSERegs -= NeededSSE;
  //  } else {
  //    FI.getReturnInfo() = getIndirectReturnResult(FI.getReturnType());
  //  }
  //} else if (IsRegCall && FI.getReturnType()->getAs<ComplexType>() &&
  //           getContext().getCanonicalType(
  //               FI.getReturnType()->getAs<ComplexType>()->getElementType())
  //               == getContext().LongDoubleTy)
  //  // Complex Long Double Type is passed in Memory when Regcall
  //  // calling convention is used.
  //  FI.getReturnInfo() = getIndirectReturnResult(FI.getReturnType());
  // else
  //  FI.getReturnInfo() = classifyReturnType(FI.getReturnType());

  // If the return value is indirect, then the hidden argument is consuming one
  // integer register.
  // if (FI.getReturnInfo().isIndirect())
  //   --FreeIntRegs;
  // else if (NeededSSE && MaxVectorWidth > 0)
  //   FI.setMaxVectorWidth(MaxVectorWidth);

  // // The chain argument effectively gives us another free register.
  // if (FI.isChainCall())
  //   ++FreeIntRegs;

  // unsigned NumRequiredArgs = FI.getNumR!equiredArgs();

  // AMD64-ABI 3.2.3p3: Once arguments are classified, the registers
  // get assigned (in left-to-right order) for passing as follows...
  unsigned ArgNo = 0;
  for (FunctionInfo::ArgIter it = FI.GetArgBegin(), ie = FI.GetArgEnd();
       it != ie; ++it, ++ArgNo) {

    Class type = ClassifyArgumentType(it->Ty);
    std::cout << "I am processing this arugment: " << ArgNo << std::endl;

    switch (type) {
    case ABI::X86_64ABIInfo::Class::Integer:
      break;
    default:
      assert(false && "unreachable statement. Not implemented yet");
    }

    // FIXME: what about structure type?
    // if (IsRegCall && it->type->isStructureOrClassType())
    //   it->info = classifyRegCallStructType(it->type, NeededInt, NeededSSE,
    //                                        MaxVectorWidth);
    // else
    // classifyArgumentType(it->type, FreeIntRegs, NeededInt, NeededSSE,
    // IsNamedArg);

    // AMD64-ABI 3.2.3p3: If there are no
    // registers available for any eightbyte of
    // an argument, the whole argument is passed
    // on the stack. If registers have already
    // been assigned for some eightbytes of such
    // an argument, the assignments get
    // reverted. if (FreeIntRegs >= NeededInt &&
    // FreeSSERegs >= NeededSSE) {
    //   FreeIntRegs -= NeededInt;
    //   FreeSSERegs -= NeededSSE;
    //   if (MaxVectorWidth >
    //   FI.getMaxVectorWidth())
    //     FI.setMaxVectorWidth(MaxVectorWidth);
    // } else {
    //   it->info = getIndirectResult(it->type,
    //   FreeIntRegs);
    // }
  }
}
