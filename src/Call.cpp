#include "Call.h"

using namespace ABI;

ABIArgInfo X86_64ABIInfo::ClassifyArgumentType(std::shared_ptr<Type *> type) {

  return ABIArgInfo();
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

  // unsigned NumRequiredArgs = FI.getNumRequiredArgs();

  // AMD64-ABI 3.2.3p3: Once arguments are classified, the registers
  // get assigned (in left-to-right order) for passing as follows...
  unsigned ArgNo = 0;
  for (FunctionInfo::ArgIter it = FI.GetArgBegin(), ie = FI.GetArgEnd();
       it != ie; ++it, ++ArgNo) {

    it->Info = ClassifyArgumentType(it->Ty);

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
