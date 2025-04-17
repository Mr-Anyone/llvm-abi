#include "Type.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/Casting.h"
#include <iostream>

using namespace ABI;

void test_one() {
  Integer uncasted_type = Integer(4);
  Type *sometype = &uncasted_type;
  Integer *casted_type = llvm::dyn_cast<Integer>(sometype);

  assert(casted_type);
  assert(casted_type->getSize() == 4);
}

void test_two() {
  Float original_type(4);
  Type *sometype = &original_type;
  Float *test_cast = llvm::dyn_cast<Float>(sometype);
}

void test_three() {
  Float original_type(4);
  Type *copy_type = &original_type;
  Float *some_type = llvm::dyn_cast<Float>(copy_type);

  assert(some_type->getSize() == 4);
}

void test_four() {
  Integer a(4);
  Float b(4);
  Integer c(4);
  Float d(4);
  llvm::SmallVector<Type *> layout{&a, &b, &c, &d};
  StructType arg(layout);
  llvm::LLVMContext context{};
  arg.toLLVM(context)->dump();

  std::cout << "I have passed test four" << std::endl;
}

int main() {
  test_one();
  test_two();
  test_three();
  test_four();
  std::cout << "You have passed all test!" << std::endl;
}
