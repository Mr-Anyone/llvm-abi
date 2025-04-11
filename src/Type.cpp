#include "Type.h"
#include <assert.h>
#include <cmath>
#include <cwchar>
#include <iostream>

using namespace ABI;

Type::Type(TypeKind kind) : Kind(kind) {}
Type::TypeKind Type::getKind() const { return Kind; }

uint64_t Type::getSize() const {
  switch (Kind) {
  case FloatType:
    return llvm::cast<ABI::FloatType>(this)->getSize();
  case StructType:
    return llvm::cast<ABI::StructType>(this)->getSize();
  case Integer:
    return llvm::cast<ABI::Integer>(this)->getSize();
  default:
    assert(false && "what type are you having?");
    return 0;
  }
}

void Type::dump() const {
  switch (Kind) {
  case Integer:
    llvm::cast<ABI::Integer>(this)->dump();
    break;
  case FloatType:
    llvm::cast<ABI::FloatType>(this)->dump();
    break;
  case StructType:
    llvm::cast<ABI::StructType>(this)->dump();
    break;
  case PointerType:
    llvm::cast<ABI::PointerType>(this)->dump();
    break;
  default:
    assert(false && "cannot dump tpye. Unknown type");
  }
}

Integer::Integer(uint64_t size) : ::Type(TypeKind::Integer), Size(size) {}
Integer::Integer() : ::Type(TypeKind::Integer), Size(4) {}

bool Integer::isFloat() const { return false; }

bool Integer::isIntegerType() const { return true; }
bool Integer::isAggregateType() const { return false; }

bool Integer::classof(const Type *type) {
  return type->getKind() == TypeKind::Integer;
}

uint64_t Integer::getSize() const { return Size; }

void Integer::dump() const { std::cout << "Integer" << Size; }

// FIXME: clean this up?
static void pushElements(llvm::SmallVector<Type *> &records,
                         StructType::ElementIterator it,
                         StructType::ElementIterator ie) {

  for (; it != ie; ++it) {
    // recursively process struct type
    if (llvm::isa<StructType>(*it)) {
      StructType *some_type = llvm::dyn_cast<StructType>(*it);
      pushElements(records, some_type->getStart(), some_type->getEnd());
      continue;
    }

    records.push_back(*it);
  }
}

StructType::StructType(llvm::SmallVector<Type *> elements)
    : ::Type(TypeKind::StructType) {

  // recursively push elements
  pushElements(this->elements, elements.begin(), elements.end());
}

bool StructType::isAggregateType() const { return true; }

bool StructType::isIntegerType() const {
  assert(false && "This comparison makes no sense");
  return false;
}

StructType::ElementIterator StructType::getStart() { return elements.begin(); }

StructType::ElementIterator StructType::getEnd() { return elements.end(); }

uint64_t StructType::getSize() const {
  uint64_t size = 0;
  for (Type *ele : elements) {
    ABI::FloatType *some_type;
    ABI::Integer *int_type;
    ABI::StructType *struct_type;

    if ((some_type = llvm::dyn_cast<ABI::FloatType>(ele))) {
      size += some_type->getSize();
    } else if ((int_type = llvm::dyn_cast<ABI::Integer>(ele))) {
      size += int_type->getSize();
    } else if ((struct_type = llvm::dyn_cast<ABI::StructType>(ele))) {
      size += struct_type->getSize();
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

void StructType::dump() const {
  // ece
  std::cout << "{ ";
  for (const Type *type : elements) {
    type->dump();
    std::cout << ",";
  }
  std::cout << " }";
}

FloatType::FloatType() : ::Type(TypeKind::FloatType), Size(4), Alignment(4) {}

FloatType::FloatType(uint64_t size)
    : ::Type(TypeKind::FloatType), Size(size), Alignment(size) {}

bool FloatType::isIntegerType() const { return false; }
bool FloatType::isFloat() const { return true; }
bool FloatType::isAggregateType() const { return false; }
uint64_t FloatType::getSize() const { return Size; }

bool FloatType::classof(const Type *type) {
  return type->getKind() == Type::TypeKind::FloatType;
}

void FloatType::dump() const { std::cout << "Float" << Size; }

PointerType::PointerType() : ::Type(TypeKind::PointerType) {}

bool PointerType::isIntegerType() const {
  assert(false && "not sure for now");
  return true;
}

bool PointerType::isFloat() const { return false; }
bool PointerType::isAggregateType() const { return false; }
void PointerType::dump() const { std::cout << "PTR" << Size; }
