#ifndef TYPE_H
#define TYPE_H

#include <stdint.h>

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

} // namespace ABI

#endif
