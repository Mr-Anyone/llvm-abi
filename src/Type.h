#ifndef TYPE_H
#define TYPE_H

#include "llvm/ADT/SmallVector.h"
#include "llvm/Support/Casting.h"
#include <stdint.h>
#include <vector>

namespace ABI {

class Type {
public:
  enum TypeKind { BuiltIn, Integer, StructType, FloatType, PointerType, Size };

private:
  const TypeKind Kind;

public:
  Type(TypeKind kind);
  TypeKind getKind() const;

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

  static bool classof(const Type *type) {
    return type->getKind() == TypeKind::PointerType;
  }

private:
  uint64_t size;
};

class FloatType : public Type {
public:
  // some way to do stuff
  // Taken from System-v abi, these
  // are called Fundamental Type
  enum FloatingType {
    Float = 0,
    Double,
    Long_Double,
    Float_128,
    Decimal32,
    Decimal64,
    Decimal128,

    Num
  };

private:
  uint64_t Size;
  uint64_t Alignment;

public:
  FloatType();
  // FIXME: add this later on
  FloatType(FloatingType type);

  FloatType(uint64_t size);

  uint64_t getSize() const;
  uint64_t getAlignment() const;

  bool isIntegerType() const;
  bool isFloat() const;
  bool isAggregateType() const;

  static bool classof(const Type *type);

  // write to stdin
  void dump() const;
};

class Integer : public Type {
public:
  // Fundamental type taken from 3.1 system v abi
  enum IntegerType {

  };

private:
  // size of the integer in bytes
  uint64_t Size;
  uint64_t Alignment;

public:
  Integer(uint64_t size);

  // default size of 4 bytes
  Integer();

  bool isIntegerType() const;
  bool isFloat() const;
  bool isAggregateType() const;

  uint64_t getSize() const;
  uint64_t getAlignment() const;

  static bool classof(const Type *type);

  // write to stdin
  void dump() const;
};

class StructType : public Type {
public:
  using ElementIterator = llvm::SmallVector<Type *>::iterator;

  // FIXME: think of a more intelligent way of passing parameter
  // there is also a question of alignmnet?
  StructType(llvm::SmallVector<Type *> elements);

  bool isHomogeneousType();
  uint64_t getSize() const;
  uint64_t getOffset() const;

  ElementIterator getStart();
  ElementIterator getEnd();

  bool isIntegerType() const;
  bool isAggregateType() const;
  bool isFloat() const;

  static bool classof(const Type *type);

  // write to stdin
  void dump() const;

private:
  llvm::SmallVector<Type *> elements;
};

} // namespace ABI

#endif
