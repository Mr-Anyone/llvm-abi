#ifndef TYPE_H
#define TYPE_H

#include "llvm/ADT/SmallVector.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/Casting.h"
#include <stdint.h>
#include <vector>

namespace ABI {

class Type {
public:
  enum TypeKind { BuiltIn, Integer, StructType, Float, PointerType, Size };

private:
  const TypeKind Kind;

public:
  Type(TypeKind Kind);
  TypeKind getKind() const;

  // Is this even a correct approach?
  // FIXME: make this a const
  llvm::Type *toLLVM(llvm::LLVMContext &Context);
  uint64_t getSize() const;

  // write to stdin
  void dump() const;
};

// TODO: what is the point of this class for now?
class PointerType : public Type {
public:
  PointerType();
  bool isIntegerType() const;
  bool isFloat() const;
  bool isAggregateType() const;

  // write to stdin
  void dump() const;

  static bool classof(const Type *Ty) {
    return Ty->getKind() == TypeKind::PointerType;
  }

private:
  uint64_t size;
};

class Float : public Type {
public:
  Float();

  uint64_t getSize() const;
  uint64_t getAlignment() const;

  bool isIntegerType() const;
  bool isFloat() const;
  bool isAggregateType() const;

  static bool classof(const Type *Ty);

  // write to stdin
  void dump() const;

private:
  uint64_t Size;
  uint64_t Alignment;
};

class Integer : public Type {
public:
  Integer(uint64_t Size);

  // default size of 4 bytes
  Integer();

  bool isIntegerType() const;
  bool isFloat() const;
  bool isAggregateType() const;

  uint64_t getSize() const;
  uint64_t getAlignment() const;

  static bool classof(const Type *Ty);

  // write to stdin
  void dump() const;

private:
  // size of the integer in bytes
  uint64_t Size;
  uint64_t Alignment;
};

// FIXME: this type need to store two things:
// 1. The underlying type information
// 2. The memory layout for each specific type
class StructType : public Type {
public:
  using ElementIterator = llvm::SmallVector<Type *>::iterator;
  using ElementIteratorConst = llvm::SmallVector<Type *>::const_iterator;

  // FIXME: think of a more intelligent way of passing parameter
  // there is also a question of alignmnet?
  StructType(llvm::SmallVector<Type *> Elements);

  bool isHomogeneousType();
  uint64_t getSize() const;
  uint64_t getOffset() const;

  ElementIterator getStart();
  ElementIterator getEnd();

  bool isIntegerType() const;
  bool isAggregateType() const;
  bool isFloat() const;

  static bool classof(const Type *Ty);

  // write to stdin
  void dump() const;

private:
  llvm::SmallVector<Type *> Elements;
};

} // namespace ABI

#endif
