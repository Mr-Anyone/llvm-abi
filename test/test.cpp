#include "Call.h"
#include "Function.h"
#include "Type.h"
#include <assert.h>
#include <iostream>
#include <memory>

void TestOne() {
  // handled by front end
  std::vector<std::shared_ptr<ABI::Type>> args{};
  for (int i = 0; i < 7; ++i) {
    std::shared_ptr<ABI::Type> arg = std::make_shared<ABI::Integer>(4);
    args.push_back(arg);
  }

  std::shared_ptr<ABI::Type> returnType = std::make_shared<ABI::Integer>(8);
  ABI::FunctionInfo FI(args, returnType, ABI::CallingConvention::C);

  ABI::X86_64ABIInfo abiLowering;
  abiLowering.ComputeInfo(FI);

  assert(FI.getReturnInfo().Info.GetKind() == ABI::Direct);

  auto ArgIterator = FI.GetArgBegin();
  // rdi, rsi, rdx, rcx, r8, r9
  // the first 6 argument are passed on the register
  for (int i = 0; i < 6; ++i) {
    assert(ArgIterator->Info.GetKind() == ABI::Direct);
    ++ArgIterator;
  }
  // the last argument in passed stack
  assert(ArgIterator->Info.GetKind() == ABI::Indirect);

  std::cout << "Passed test one!" << std::endl;
}

void RunTest() { TestOne(); }

int main() { RunTest(); }
