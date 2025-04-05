#include "Call.h"
#include "Function.h"
#include "Type.h"
#include <assert.h>
#include <iostream>
#include <memory>

void TestOne() {
  // Create a function with 7 uint32_t
  ABI::Integer arg(4);
  ABI::Integer arg_two(4);
  std::vector<ABI::Type *> args{&arg, &arg_two};

  ABI::Integer returnType(8);
  ABI::FunctionInfo FI(args, &returnType, ABI::CallingConvention::C);

  ABI::X86_64ABIInfo abiLowering;
  abiLowering.ComputeInfo(FI);

  assert(FI.getReturnInfo().Info.GetKind() == ABI::Direct);

  auto ArgIterator = FI.GetArgBegin();
  // rdi, rsi, rdx, rcx, r8, r9
  // the first 6 argument are passed on the register
  assert(ArgIterator->Info.GetKind() == ABI::Direct);
  ++ArgIterator;
  assert(ArgIterator->Info.GetKind() == ABI::Direct);

  std::cout << "Passed test one!" << std::endl;
}

void TestTwo() {
  // handled by front end
  ABI::Integer arg(/*size*/ 8);
  std::vector<ABI::Type *> record{&arg, &arg, &arg, &arg, &arg};
  ABI::StructType arg_one(record);

  ABI::Integer returnType = ABI::Integer(/*size*/ 8);
  ABI::FunctionInfo FI({&arg_one}, &returnType, ABI::CallingConvention::C);

  ABI::X86_64ABIInfo abiLowering;
  abiLowering.ComputeInfo(FI);

  assert(FI.getReturnInfo().Info.GetKind() == ABI::Direct);

  // struct larger than 4 bytes in the memory
  auto ArgIterator = FI.GetArgBegin();
  ABI::ABIArgInfo abiInfo = ArgIterator->Info;
  assert(abiInfo.GetKind() == ABI::Indirect);

  std::cout << "Passed test two" << std::endl;
}

void RunTest() {
  TestOne();
  TestTwo();
}

int main() { RunTest(); }
