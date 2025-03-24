#include "Type.h"
#include <cmath>

using namespace ABI;

Integer::Integer(uint64_t size) : Size(size) {}

bool Integer::isIntegerType() const { return true; }
bool Integer::isAggregateType() const { return false; }
