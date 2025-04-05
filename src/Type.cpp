#include "Type.h"
#include <assert.h>
#include <cmath>
#include <cwchar>

using namespace ABI;

Type::Type(TypeKind kind) : Kind(kind) {}
Type::TypeKind Type::getKind() const { return Kind; }

Integer::Integer(uint64_t size) : Size(size), ::Type(TypeKind::Integer) {}
Integer::Integer() : Size(4), ::Type(TypeKind::Integer) {}

bool Integer::isFloat() const { return false; }

bool Integer::isIntegerType() const { return true; }
bool Integer::isAggregateType() const { return false; }

bool Integer::classof(const Type *type) {
  return type->getKind() == TypeKind::Integer;
}

uint64_t Integer::getSize() const { return Size; }

StructType::StructType(std::vector<Type *> elements)
    : elements(elements), ::Type(TypeKind::StructType) {}

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
  assert(false);
  return 0;
}

bool StructType::classof(const Type *type) {
  return type->getKind() == Type::TypeKind::StructType;
}

FloatType::FloatType() : Size(4), Alignment(4), ::Type(TypeKind::FloatType) {}

FloatType::FloatType(uint64_t size)
    : Size(size), Alignment(size), ::Type(TypeKind::FloatType) {}

bool FloatType::isIntegerType() const { return false; }
bool FloatType::isFloat() const { return true; }
bool FloatType::isAggregateType() const { return false; }

bool FloatType::classof(const Type *type) {
  return type->getKind() == Type::TypeKind::FloatType;
}
