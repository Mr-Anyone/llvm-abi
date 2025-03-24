#ifndef TYPE_H
#define TYPE_H

#include <stdint.h>

namespace ABI {

class Type {
public:
  // TODO: are we really going to use integer type?
  virtual bool isIntegerType() const;
  virtual bool isAggregateType() const;
};

class Integer : Type {
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
