#include "Call.h"
#include "Function.h"
#include "Type.h"
#include <assert.h>
#include <iostream>
#include <memory>

// void TestOne() {
//   // Create a function with 7 uint32_t
//   std::vector<std::shared_ptr<ABI::Type>> args{};
//   for (int i = 0; i < 7; ++i) {
//     std::shared_ptr<ABI::Type> arg = std::make_shared<ABI::Integer>(4);
//     args.push_back(arg);
//   }
// 
//   std::shared_ptr<ABI::Type> returnType = std::make_shared<ABI::Integer>(8);
//   ABI::FunctionInfo FI(args, returnType, ABI::CallingConvention::C);
// 
//   ABI::X86_64ABIInfo abiLowering;
//   abiLowering.ComputeInfo(FI);
// 
//   assert(FI.getReturnInfo().Info.GetKind() == ABI::Direct);
// 
//   auto ArgIterator = FI.GetArgBegin();
//   // rdi, rsi, rdx, rcx, r8, r9
//   // the first 6 argument are passed on the register
//   for (int i = 0; i < 6; ++i) {
//     assert(ArgIterator->Info.GetKind() == ABI::Direct);
//     ++ArgIterator;
//   }
//   // the last argument in passed stack
//   assert(ArgIterator->Info.GetKind() == ABI::Indirect);
// 
//   std::cout << "Passed test one!" << std::endl;
// }

// void TestTwo() {
//   // Test Two
//   // handled by front end
//   std::shared_ptr<ABI::Type> arg = std::make_shared<ABI::Integer>(/*size*/ 8);
//   std::vector<std::shared_ptr<ABI::Type>> args{arg, arg, arg, arg, arg};
//   std::shared_ptr<ABI::StructType> arg_one =
//       std::make_shared<ABI::StructType>(args);
// 
//   std::shared_ptr<ABI::Type> returnType =
//       std::make_shared<ABI::Integer>(/*size*/ 8);
//   ABI::FunctionInfo FI({arg_one}, returnType, ABI::CallingConvention::C);
// 
//   ABI::X86_64ABIInfo abiLowering;
//   abiLowering.ComputeInfo(FI);
// 
//   assert(FI.getReturnInfo().Info.GetKind() == ABI::Direct);
// 
//   // struct larger than 4 bytes in the memory
//   auto ArgIterator = FI.GetArgBegin();
//   ABI::ABIArgInfo abiInfo = ArgIterator->Info;
//   assert(abiInfo.GetKind() == ABI::Indirect);
// 
//   std::cout << "Passed test two" << std::endl;
// }

void RunTest() {
  // TestOne();
  // TestTwo();
}
 
int main() { RunTest(); }
