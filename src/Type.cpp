#include "Type.h"
#include <assert.h>
#include <cmath>
#include <cwchar>

using namespace ABI;

Type::Type(TypeKind kind) : Kind(kind) {}
Type::TypeKind Type::getKind() const { return Kind; }

Integer::Integer(uint64_t size) : ::Type(TypeKind::Integer), Size(size) {}
Integer::Integer() : ::Type(TypeKind::Integer), Size(4) {}

bool Integer::isFloat() const { return false; }

bool Integer::isIntegerType() const { return true; }
bool Integer::isAggregateType() const { return false; }

bool Integer::classof(const Type *type) {
  return type->getKind() == TypeKind::Integer;
}

uint64_t Integer::getSize() const { return Size; }

StructType::StructType(std::vector<Type *> elements)
    : ::Type(TypeKind::StructType), elements(elements) {}

bool StructType::isAggregateType() const {
  // FIXME: as of current we can only represent interger
  // we need a way to represent other types
  return true;
}

bool StructType::isIntegerType() const {
  // FIXME: as of current we can only represent interger
  // we need a way to represent other types
  return true;
}

StructType::ElementIterator StructType::getStart() { return elements.begin(); }

StructType::ElementIterator StructType::getEnd() { return elements.end(); }

uint64_t StructType::getSize() const {
  uint64_t size = 0;
  for (Type *ele : elements) {
    ABI::FloatType *some_type;
    ABI::Integer *int_type;

    if ((some_type = llvm::dyn_cast<ABI::FloatType>(ele))) {
      size += some_type->getSize();
    } else if ((int_type = llvm::dyn_cast<ABI::Integer>(ele))) {
      size += int_type->getSize();
    } else {
      assert(0 && "don't konw how to calculate size for struct type!");
    }
  }
  return size;
}

bool StructType::isFloat() const { return false; }

bool StructType::classof(const Type *type) {
  return type->getKind() == Type::TypeKind::StructType;
}

FloatType::FloatType() : ::Type(TypeKind::FloatType), Size(4), Alignment(4) {}

FloatType::FloatType(uint64_t size)
    : Size(size), Alignment(size), ::Type(TypeKind::FloatType) {}

bool FloatType::isIntegerType() const { return false; }
bool FloatType::isFloat() const { return true; }
bool FloatType::isAggregateType() const { return false; }
uint64_t FloatType::getSize() const { return Size; }

bool FloatType::classof(const Type *type) {
  return type->getKind() == Type::TypeKind::FloatType;
}
