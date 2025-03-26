#include "Type.h"
#include <assert.h>
#include <cmath>
using namespace ABI;

Integer::Integer(uint64_t size) : Size(size), ::Type() {}
Integer::Integer() : Size(4), ::Type() {}

bool Integer::isIntegerType() const { return true; }
bool Integer::isAggregateType() const { return false; }

uint64_t Integer::getSize() const { return Size; }

StructType::StructType(std::vector<std::shared_ptr<Type>> elements)
    : elements(elements) {}

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
  // FIXME: this is not always correct!
  uint64_t size = 0;
  for (std::shared_ptr<Type> type : elements) {
    if (type->isIntegerType()) {
      std::shared_ptr<Integer> temp = std::dynamic_pointer_cast<Integer>(type);
      size += temp->getSize();

    } else {
      assert(false && "Fix me!");
    }
  }

  return size;
}
