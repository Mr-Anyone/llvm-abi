#include "Type.h"
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
  FloatType original_type(4);
  Type *sometype = &original_type;
  FloatType *test_cast = llvm::dyn_cast<FloatType>(sometype);
}

int main() {
  test_one();
  test_two();
  std::cout << "You have passed all test!" << std::endl;
}
