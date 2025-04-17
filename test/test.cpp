#include "Call.h"
#include "Function.h"
#include "Type.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/LLVMContext.h"
#include <assert.h>
#include <iostream>
#include <memory>

using namespace ABI;

void TestOne() {
  // int func1(int, int);
  Integer arg(4);
  Integer arg_two(4);
  llvm::SmallVector<Type *> args{&arg, &arg_two};
  Integer returnType(8);
  FunctionInfo FI(args, &returnType, ABI::CallingConvention::C);

  llvm::LLVMContext context_g;
  X86_64ABIInfo abiLowering(context_g);
  abiLowering.ComputeInfo(FI);

  assert(FI.getReturnInfo().Info.GetKind() == Direct);

  auto ArgIterator = FI.GetArgBegin();
  assert(ArgIterator->Info.GetKind() == Direct);

  ++ArgIterator;
  assert(ArgIterator->Info.GetKind() == Direct);
  std::cout << "Passed test one" << std::endl;

  FI.dump();
}

void TestTwo() {
  // handled by front end
  Integer arg(/*size*/ 8);
  llvm::SmallVector<Type *> layout{&arg, &arg, &arg, &arg, &arg};
  StructType arg_one(layout);

  Integer returnType = ABI::Integer(/*size*/ 8);
  FunctionInfo FI({&arg_one}, &returnType, ABI::CallingConvention::C);
  llvm::LLVMContext context_g;
  X86_64ABIInfo abiLowering(context_g);
  abiLowering.ComputeInfo(FI);

  assert(FI.getReturnInfo().Info.GetKind() == Direct);

  // struct larger than 4 bytes in the memory
  auto ArgIterator = FI.GetArgBegin();
  ABIArgInfo abiInfo = ArgIterator->Info;
  assert(abiInfo.GetKind() == Indirect);

  FI.dump();

  std::cout << "Passed test two" << std::endl;
}

void TestThree() {
  // typedef struct {
  //      int a, b;
  //      float c, d;
  //  } some_type_t;

  // int some_func(some_type_t a);
  Integer a(4);
  Integer b(4);
  Float c(4);
  Float d(4);
  llvm::SmallVector<Type *> layout{&a, &b, &c, &d};
  StructType arg(layout);
  Integer return_type(4);

  FunctionInfo FI({&arg}, &return_type, ABI::CallingConvention::C);
  llvm::LLVMContext context;
  X86_64ABIInfo abiLowering(context);
  abiLowering.ComputeInfo(FI);

  assert(FI.getReturnInfo().Info.GetKind() == Direct);

  auto ArgIterator = FI.GetArgBegin();
  ABIArgInfo abiInfo = ArgIterator->Info;
  assert(abiInfo.GetKind() == Direct);

  FI.dump();

  std::cout << "Passed Test three" << std::endl;
}

void TestFour() {
  // typedef struct {
  //      int a;
  //      float b;
  //    int c,
  //    float dd;
  //  } some_type_t;

  // int some_func(some_type_t a);
  Integer a(4);
  Float b(4);
  Integer c(4);
  Float d(4);
  llvm::SmallVector<Type *> layout{&a, &b, &c, &d};
  StructType arg(layout);
  Integer return_type(4);

  FunctionInfo FI({&arg}, &return_type, ABI::CallingConvention::C);
  llvm::LLVMContext context;
  X86_64ABIInfo abiLowering(context);
  abiLowering.ComputeInfo(FI);

  assert(FI.getReturnInfo().Info.GetKind() == Direct);

  auto ArgIterator = FI.GetArgBegin();
  ABIArgInfo abiInfo = ArgIterator->Info;
  assert(abiInfo.GetKind() == Direct);

  FI.dump();

  std::cout << "Passed test four" << std::endl;
}

void TestFive() {
  // typedef struct {
  //      int a;
  //      float b;
  //    int c,
  //    float d;
  //  int e
  //  } some_type_t;

  // int some_func(some_type_t a);
  Integer a(4);
  Float b(4);
  Integer c(4);
  Float d(4);
  Integer e(4);
  llvm::SmallVector<Type *> layout{&a, &b, &c, &d, &e};
  StructType arg(layout);
  Integer return_type(4);

  FunctionInfo FI({&arg}, &return_type, ABI::CallingConvention::C);
  llvm::LLVMContext context;
  X86_64ABIInfo abiLowering(context);
  abiLowering.ComputeInfo(FI);

  assert(FI.getReturnInfo().Info.GetKind() == Direct);

  auto ArgIterator = FI.GetArgBegin();
  ABIArgInfo abiInfo = ArgIterator->Info;
  assert(abiInfo.GetKind() == Indirect);

  FI.dump();

  std::cout << "Passed test five" << std::endl;
}

void TestSix() {
  // typedef struct {
  //   uint32_t a;
  //   uint32_t b;
  // } nested_type_t;

  // typedef struct {
  //   nested_type_t a;
  //   uint32_t c;

  // } some_type_t;

  Integer a(4);
  Integer b(4);
  StructType inner({&a, &b});
  Integer c(4);
  StructType outer({&inner, &c});

  FunctionInfo FI({&outer}, &a, ABI::CallingConvention::C);
  llvm::LLVMContext context;
  X86_64ABIInfo abiLowering(context);
  abiLowering.ComputeInfo(FI);

  FI.dump();

  assert(FI.getReturnInfo().Info.GetKind() == Direct);
  std::cout << "Passed test six" << std::endl;
}

void TestSeven() {
  // typedef struct {
  //      float a;
  //      float b;
  //  } some_type_t;

  // int some_func(some_type_t a);
  Float a(4);
  Float b(4);
  llvm::SmallVector<Type *> layout{&a, &b}; // layout
  StructType arg(layout);
  Integer return_type(4);

  FunctionInfo FI({&arg}, &return_type, ABI::CallingConvention::C);
  llvm::LLVMContext context;
  X86_64ABIInfo abiLowering(context);
  abiLowering.ComputeInfo(FI);

  assert(FI.getReturnInfo().Info.GetKind() == Direct);

  auto ArgIterator = FI.GetArgBegin();
  ABIArgInfo abiInfo = ArgIterator->Info;
  assert(abiInfo.GetKind() == Direct);
  // <2 x float>
  assert(abiInfo.getType() ==
         llvm::FixedVectorType::get(llvm::Type::getFloatTy(context), 2));

  FI.dump();

  std::cout << "Passed test seven" << std::endl;
}

void TestEight() {
  // typedef struct {
  //   int a, b;
  //   float c, d;
  // } some_type_t;

  // int some_func(some_type_t a);
  Integer a(4);
  Integer b(4);
  Float c(4);
  Float d(4);
  llvm::SmallVector<Type *> layout{&a, &b, &c, &d}; // layout
  StructType arg(layout);
  Integer return_type(4);

  FunctionInfo FI({&arg}, &return_type, ABI::CallingConvention::C);
  llvm::LLVMContext context;
  X86_64ABIInfo abiLowering(context);
  abiLowering.ComputeInfo(FI);

  ABIArgInfo abi_return_info = FI.getReturnInfo().Info;
  assert(abi_return_info.GetKind() == Direct);

  auto ArgIterator = FI.GetArgBegin();
  ABIArgInfo abiInfo = ArgIterator->Info;
  assert(abiInfo.GetKind() == Direct);

  // {i64, < 2 x float>}
  assert(abiInfo.getType() ==
         llvm::StructType::get(
             llvm::Type::getInt64Ty(context),
             llvm::FixedVectorType::get(llvm::Type::getFloatTy(context), 2)));

  FI.dump();

  std::cout << "Passed test eight" << std::endl;
}

void RunTest() {
   TestOne();
  TestTwo();
   TestThree();
   TestFour();
   TestFive();
   TestSix();
   TestSeven();
   TestEight();
}

int main() { RunTest(); }
