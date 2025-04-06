#include "Call.h"
#include "Function.h"
#include "Type.h"
#include <assert.h>
#include <iostream>
#include <memory>

using namespace ABI;
void TestOne() {
  // int func1(int, int);
  Integer arg(4);
  Integer arg_two(4);
  std::vector<Type *> args{&arg, &arg_two};
  Integer returnType(8);
  FunctionInfo FI(args, &returnType, ABI::CallingConvention::C);

  X86_64ABIInfo abiLowering;
  abiLowering.ComputeInfo(FI);

  assert(FI.getReturnInfo().Info.GetKind() == Direct);

  auto ArgIterator = FI.GetArgBegin();
  assert(ArgIterator->Info.GetKind() == Direct);

  ++ArgIterator;
  assert(ArgIterator->Info.GetKind() == Direct);
  std::cout << "Passed test one" << std::endl;
}

void TestTwo() {
  // handled by front end
  Integer arg(/*size*/ 8);
  std::vector<Type *> record{&arg, &arg, &arg, &arg, &arg};
  StructType arg_one(record);

  Integer returnType = ABI::Integer(/*size*/ 8);
  FunctionInfo FI({&arg_one}, &returnType, ABI::CallingConvention::C);
  X86_64ABIInfo abiLowering;
  abiLowering.ComputeInfo(FI);

  assert(FI.getReturnInfo().Info.GetKind() == Direct);

  // struct larger than 4 bytes in the memory
  auto ArgIterator = FI.GetArgBegin();
  ABIArgInfo abiInfo = ArgIterator->Info;
  assert(abiInfo.GetKind() == Indirect);

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
  FloatType c(4);
  FloatType d(4);
  std::vector<Type *> record{&a, &b, &c, &d};
  StructType arg(record);
  Integer return_type(4);

  FunctionInfo FI({&arg}, &return_type, ABI::CallingConvention::C);
  X86_64ABIInfo abiLowering;
  abiLowering.ComputeInfo(FI);

  assert(FI.getReturnInfo().Info.GetKind() == Direct);

  auto ArgIterator = FI.GetArgBegin();
  ABIArgInfo abiInfo = ArgIterator->Info;
  assert(abiInfo.GetKind() == Direct);

  std::cout << "Passed Test Three" << std::endl;
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
  FloatType b(4);
  Integer c(4);
  FloatType d(4);
  std::vector<Type *> record{&a, &b, &c, &d};
  StructType arg(record);
  Integer return_type(4);

  FunctionInfo FI({&arg}, &return_type, ABI::CallingConvention::C);
  X86_64ABIInfo abiLowering;
  abiLowering.ComputeInfo(FI);

  assert(FI.getReturnInfo().Info.GetKind() == Direct);

  auto ArgIterator = FI.GetArgBegin();
  ABIArgInfo abiInfo = ArgIterator->Info;
  assert(abiInfo.GetKind() == Direct);

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
  FloatType b(4);
  Integer c(4);
  FloatType d(4);
  Integer e(4);
  std::vector<Type *> record{&a, &b, &c, &d, &e};
  StructType arg(record);
  Integer return_type(4);

  FunctionInfo FI({&arg}, &return_type, ABI::CallingConvention::C);
  X86_64ABIInfo abiLowering;
  abiLowering.ComputeInfo(FI);

  assert(FI.getReturnInfo().Info.GetKind() == Direct);

  auto ArgIterator = FI.GetArgBegin();
  ABIArgInfo abiInfo = ArgIterator->Info;
  assert(abiInfo.GetKind() == Indirect);

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
  X86_64ABIInfo abiLowering;
  abiLowering.ComputeInfo(FI);

  assert(FI.getReturnInfo().Info.GetKind() == Direct);
  std::cout << "Passed test six" << std::endl;
}

void RunTest() {
  TestOne();
  TestTwo();
  TestThree();
  TestFour();
  TestFive();
  TestSix();
}

int main() { RunTest(); }
