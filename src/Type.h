#ifndef TYPE_H
#define TYPE_H

#include <memory>
#include <stdint.h>
#include <vector>

namespace ABI {

class Type {
public:
  // TODO: handle this better later on
  virtual bool isIntegerType() const = 0;
  virtual bool isAggregateType() const = 0;

private:
};

class Integer : public Type {
public:
  Integer(uint64_t size);

  // default size of 4 bytes
  Integer();

  virtual bool isIntegerType() const override;
  virtual bool isAggregateType() const override;

  uint64_t getSize() const;

private:
  // size of the integer in bytes
  uint64_t Size;
};

class StructType : public Type {
public:
  using ElementIterator = std::vector<std::shared_ptr<Type>>::iterator;

  // TODO: do we have to make a copy?
  StructType(std::vector<std::shared_ptr<Type>> elements);

  bool isHomogeneousType();
  uint64_t getSize() const;

  ElementIterator getStart();
  ElementIterator getEnd();

  virtual bool isIntegerType() const override;
  virtual bool isAggregateType() const override;

private:
  std::vector<std::shared_ptr<Type>> elements;
};

} // namespace ABI

#endif
