#include "Type.h"
#include <cmath>
using namespace ABI;

Integer::Integer(uint64_t size) : Size(size), ::Type() {}
Integer::Integer() : Size(4), ::Type() {}

bool Integer::isIntegerType() const { return true; }
bool Integer::isAggregateType() const { return false; }

uint64_t Integer::getSize() const { return Size; }
