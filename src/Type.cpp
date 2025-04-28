#include "Type.h"
#include <assert.h>
#include <cmath>
#include <cwchar>
#include <iostream>

#include <llvm/IR/DerivedTypes.h>

using namespace ABI;

llvm::Type *Type::toLLVM(llvm::LLVMContext &Context) {
  switch (Kind) {
  case Integer: {
    uint64_t IntegerSize = (llvm::cast<ABI::Integer>(this))->getSize() * 8;
    return llvm::Type::getIntNTy(Context, IntegerSize);
  }
  case Float: {
    return llvm::Type::getFloatTy(Context);
  }
  case StructType: {
    ABI::StructType *CurrentType = llvm::cast<ABI::StructType>(this);
    llvm::SmallVector<llvm::Type *> Elements{};
    for (auto it = CurrentType->getStart(), ie = CurrentType->getEnd();
         it != ie; ++it) {
      Elements.push_back((*it)->toLLVM(Context));
    }

    return llvm::StructType::create(Elements);
  }
  default:
    assert(false && "don't know how to transform this into the llvm type!");
  }
}

Type::Type(TypeKind Kind) : Kind(Kind) {}
Type::TypeKind Type::getKind() const { return Kind; }

uint64_t Type::getSize() const {
  switch (Kind) {
  case Float:
    return llvm::cast<ABI::Float>(this)->getSize();
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
  case Float:
    llvm::cast<ABI::Float>(this)->dump();
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

Integer::Integer(uint64_t Size) : ::Type(TypeKind::Integer), Size(Size) {}
Integer::Integer() : ::Type(TypeKind::Integer), Size(4) {}

bool Integer::isFloat() const { return false; }

bool Integer::isIntegerType() const { return true; }
bool Integer::isAggregateType() const { return false; }

bool Integer::classof(const Type *Ty) {
  return Ty->getKind() == TypeKind::Integer;
}

uint64_t Integer::getSize() const { return Size; }

void Integer::dump() const { std::cout << "Integer" << Size * 8; }

// FIXME: clean this up?
static void pushElements(llvm::SmallVector<Type *> &Records,
                         StructType::ElementIterator It,
                         StructType::ElementIterator Ie) {

  for (; It != Ie; ++It) {
    // recursively process struct type
    if (llvm::isa<StructType>(*It)) {
      StructType *some_type = llvm::dyn_cast<StructType>(*It);
      pushElements(Records, some_type->getStart(), some_type->getEnd());
      continue;
    }

    Records.push_back(*It);
  }
}

StructType::StructType(llvm::SmallVector<Type *> Elements)
    : ::Type(TypeKind::StructType) {

  // recursively push elements
  pushElements(this->Elements, Elements.begin(), Elements.end());
}

bool StructType::isAggregateType() const { return true; }

bool StructType::isIntegerType() const {
  assert(false && "This comparison makes no sense");
  return false;
}

StructType::ElementIterator StructType::getStart() { return Elements.begin(); }

StructType::ElementIterator StructType::getEnd() { return Elements.end(); }

uint64_t StructType::getSize() const {
  uint64_t Size = 0;
  for (Type *Element : Elements) {
    ABI::Float *SomeType;
    ABI::Integer *IntType;
    ABI::StructType *StructTy;

    if ((SomeType = llvm::dyn_cast<ABI::Float>(Element))) {
      Size += SomeType->getSize();
    } else if ((IntType = llvm::dyn_cast<ABI::Integer>(Element))) {
      Size += IntType->getSize();
    } else if ((StructTy = llvm::dyn_cast<ABI::StructType>(Element))) {
      Size += StructTy->getSize();
    } else {
      assert(0 && "don't konw how to calculate size for struct type!");
    }
  }
  return Size;
}

bool StructType::isFloat() const { return false; }

bool StructType::classof(const Type *Ty) {
  return Ty->getKind() == Type::TypeKind::StructType;
}

void StructType::dump() const {
  // ece
  std::cout << "{ ";
  for (std::size_t i = 0; i < Elements.size(); ++i) {
    Elements[i]->dump();
    if (i != Elements.size() - 1)
      std::cout << ",";
  }
  std::cout << " }";
}

Float::Float() : ::Type(TypeKind::Float), Size(4), Alignment(4) {}

bool Float::isIntegerType() const { return false; }
bool Float::isFloat() const { return true; }
bool Float::isAggregateType() const { return false; }
uint64_t Float::getSize() const { return Size; }

bool Float::classof(const Type *Ty) {
  return Ty->getKind() == Type::TypeKind::Float;
}

void Float::dump() const { std::cout << "Float" << Size * 8; }

PointerType::PointerType() : ::Type(TypeKind::PointerType) {}

bool PointerType::isIntegerType() const {
  assert(false && "not sure for now");
  return true;
}

bool PointerType::isFloat() const { return false; }
bool PointerType::isAggregateType() const { return false; }
void PointerType::dump() const { std::cout << "PTR" << Size * 8; }
