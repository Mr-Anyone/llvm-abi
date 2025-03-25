#include "Call.h"
#include "Function.h"
#include "Type.h"
#include <assert.h>
#include <iostream>
#include <memory>

void TestOne() {
  // handled by front end
  std::shared_ptr<ABI::Type> argOne = std::make_shared<ABI::Integer>(4);
  std::shared_ptr<ABI::Type> argTwo = std::make_shared<ABI::Integer>(4);
  std::vector<std::shared_ptr<ABI::Type>> args = {argOne, argTwo};

  std::shared_ptr<ABI::Type> returnType = std::make_shared<ABI::Integer>(8);
  ABI::FunctionInfo FI(args, returnType, ABI::CallingConvention::C);

  ABI::X86_64ABIInfo abiLowering;
  abiLowering.ComputeInfo(FI);


  std::cout << "Passed test one!" << std::endl;
}

void RunTest() { TestOne(); }

int main() { RunTest(); }
