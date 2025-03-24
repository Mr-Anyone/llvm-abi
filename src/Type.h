#ifndef TYPE_H
#define TYPE_H

#include <stdint.h>

namespace ABI {

class Type {
public:
  virtual bool isIntegerType() const;
  virtual bool isAggregateType() const;
};

class Integer : Type {
public:
  Integer(uint64_t size);
  virtual bool isIntegerType() const override;
  virtual bool isAggregateType() const override;

private:
  // size of the integer in bytes
  uint64_t Size;
};

} // namespace ABI

#endif
