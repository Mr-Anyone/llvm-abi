#include "Call.h"
#include "Function.h"
#include "Type.h"

#include <cassert>
#include <iostream>
#include <memory>

using namespace ABI;

X86_64ABIInfo::Class
X86_64ABIInfo::ClassifyArgumentType(std::shared_ptr<Type> type) {

  assert(type && "null type is undefined for now!");
  if (type->isIntegerType() && !type->isAggregateType()) {
    std::shared_ptr<::Integer> intergerType =
        std::dynamic_pointer_cast<::Integer>(type);

    if (intergerType->getSize() <= 8)
      return Integer;

    // FIXME: what about types that are _BitInt(128), etc.
    assert(false && "how to represent type that are less than 8 bytes  ");
  }

  if (type->isAggregateType()) {
    std::shared_ptr<::StructType> structType =
        std::dynamic_pointer_cast<::StructType>(type);

    // TODO: check if this is 4 strict or less strict
    if (structType->getSize() >= 4 * 8) {
      return Memory;
    }
  }

  assert(false && "Todo: b re");
}

// Mainly taken from clang
void X86_64ABIInfo::ComputeInfo(FunctionInfo &FI) {
  // Keep track of the number of assigned registers.
  unsigned FreeIntRegs = 6;
  unsigned FreeSSERegs = 8;
  unsigned NeededInt = 0, NeededSSE = 0, MaxVectorWidth = 0;

  // Lower Return
  switch (ClassifyArgumentType(FI.getReturnInfo().Ty)) {
  case Class::Integer:
    FI.setABIReturnInfo(ABIArgInfo(Direct)); // eax register
    break;
  default:
    assert(false && "Please implememnt other types");
  }

  // AMD64-ABI 3.2.3p3: Once arguments are classified, the registers
  // get assigned (in left-to-right order) for passing as follows...
  unsigned ArgNo = 0;
  for (FunctionInfo::ArgIter it = FI.GetArgBegin(), ie = FI.GetArgEnd();
       it != ie; ++it, ++ArgNo) {

    Class type = ClassifyArgumentType(it->Ty);

    switch (type) {
    case ABI::X86_64ABIInfo::Class::Integer:
      // defined by AMD64-ABI:
      if (FreeIntRegs > 0) {
        // Use Register
        it->Info = ABIArgInfo(Direct);
      } else {
        // Pushed to the stack
        it->Info = ABIArgInfo(Indirect);
      }
      --FreeIntRegs;
      break;
    case ABI::X86_64ABIInfo::Class::Memory:
      it->Info = ABIArgInfo(Indirect);
      break;
    default:
      assert(false && "unreachable statement. Not implemented yet");
    }
  }
}
