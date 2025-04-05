#ifndef TYPE_H
#define TYPE_H

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

  // TODO: handle this better later on
  virtual bool isIntegerType() const = 0;
  virtual bool isFloat() const = 0;
  virtual bool isAggregateType() const = 0;
};

// TODO: what is the point of this class for now?
class PointerType : public Type {
public:
  PointerType();
  bool isIntegerType() const override;
  bool isFloat() const override;
  bool isAggregateType() const override;

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
    Long_Float,
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

  bool isIntegerType() const override;
  bool isFloat() const override;
  bool isAggregateType() const override;

  static bool classof(const Type *type);
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

  bool isIntegerType() const override;
  bool isFloat() const override;
  bool isAggregateType() const override;

  uint64_t getSize() const;
  uint64_t getAlignment() const;

  static bool classof(const Type *type);
};

// rename this to record type?
class StructType : public Type {
public:
  using ElementIterator = std::vector<Type *>::iterator;

  // FIXME: think of a more intelligent way of passing parameter
  // there is also a question of alignmnet?
  StructType(std::vector<Type *> elements);

  bool isHomogeneousType();
  uint64_t getSize() const;
  uint64_t getOffset() const;

  ElementIterator getStart();
  ElementIterator getEnd();

  bool isIntegerType() const override;
  bool isAggregateType() const override;
  bool isFloat() const override;

  static bool classof(const Type *type);

private:
  std::vector<Type *> elements;
};

} // namespace ABI

#endif
