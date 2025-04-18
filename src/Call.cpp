#include "Call.h"
#include "Function.h"
#include "Type.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/Casting.h"

#include <cassert>

using namespace ABI;

X86_64ABIInfo::Class X86_64ABIInfo::Merge(Class one, Class two) {
  // for a 8 bytes
  // (a) If both classes are equal, this is the resulting class.
  // (b) If one of the classes is NO_CLASS, the resulting class is the other
  // class.
  // (c) If one of the classes is MEMORY, the result is the MEMORY class.
  // (d) If one of the classes is INTEGER, the result is the INTEGER.
  // (e) If one of the classes is X87, X87UP, COMPLEX_X87 class, MEMORY is used
  // as class.
  // (f) Otherwise class SSE is used.
  if (one == two)
    return one;
  else if (one == NoClass)
    return two;
  else if (two == NoClass)
    return one;
  else if (one == Memory || two == Memory)
    return Memory;
  else if (one == Integer || two == Integer)
    return Integer;
  else if (one == X87 || two == X87)
    return Memory;
  else if (one == X87Up || two == X87Up)
    return Memory;
  else if (one == Complex || two == Complex)
    return Complex;

  return SSE;
}

void X86_64ABIInfo::PostMerger(Class &Low, Class &High, uint64_t size) {
  //     5. Then a post merger cleanup is done:
  // (a) If one of the classes is MEMORY, the whole argument is passed in
  // memory.
  // (b) If X87UP is not preceded by X87, the whole argument is passed in
  // memory.
  // (c) If the size of the aggregate exceeds two eightbytes and the first
  // eight- byte isn’t SSE or any other eightbyte isn’t SSEUP, the whole
  // argument is passed in memory.

  if (Low == Memory && High == Memory) {
    Low = High = Memory;
  } else if (High == X87Up && Low != X87) {
    Low = High = Memory;
  } else if (High == SSEUp && Low != SSE) {
    High = SSEUp;
  } else if (Low != SSE && High != SSEUp && size > 16) {
    High = Low = Memory;
  }
}

// What about High and Low?
void X86_64ABIInfo::Classify(Type *type, Class &Low, Class &High) {
  Low = NoClass;
  High = NoClass;

  // Trivial cases

  // page 17, Classification rule one:
  // 1) Arguments of types (signed and unsigned) _Bool, char, short, int, long,
  // long long, and pointers are in the INTEGER class.
  ABI::Integer *integer_type;
  if ((integer_type = llvm::dyn_cast<::Integer>(type))) {
    if (integer_type->getSize() <= 8) {
      Low = Integer;
      return;
    } else if (integer_type->getSize() == 16) {
      // Arguments of type __int128 offer the same operations as INTEGERs,
      // yet they do not fit into one general purpose register but require two
      //    registers.For classification purpose
      Low = Integer;
      High = Integer;

      return;
    }
  }

  ABI::Float *float_type;
  // basically the following type as of so far:
  // float, double, _Decimal32, _Decimal64 and __m64 are in class SSE.
  if ((float_type = llvm::dyn_cast<::Float>(type))) {
    if (float_type->getSize() <= 8) {
      Low = SSE;
      return;
    }
  }

  // Arguments of types __float128, _Decimal128 and __m128 are split
  // into two halves. The least significant ones belong to class SSE, the most
  // significant one to class SSEUP.
  if (float_type) {
    // FIXME: what about _m256?
    assert(float_type->getSize() != 32 && "unimplemented for m256");

    if (float_type->getSize() == 16) {
      Low = SSE;
      High = SSEUp;
      return;
    }
  }

  // FIXME: we seem to need to be able to parse the difference between int long
  // double and __float128 The 64-bit mantissa of arguments of type long double
  // belongs to class X87, the 16-bit exponent plus 6 bytes of padding belongs
  // to class X87UP

  // rename to record type?
  ABI::StructType *record_type;
  if ((record_type = llvm::dyn_cast<ABI::StructType>(type))) {
    // FIXME: there are cases such as 256 and 512 bit vector where this is not
    // true
    if (record_type->getSize() > 16) {
      // The entire class is on memory
      Low = High = Memory;
      return;
    }

    // assert(record_type->getSize() > 8 && "FIXME as well");
    // FIXME: check for alignment as well!

    assert(Low == NoClass && High == NoClass);
    uint64_t current_offset = 0;
    for (ABI::StructType::ElementIterator it = record_type->getStart(),
                                          e = record_type->getEnd();
         it != e; ++it) {

      Class FieldLow, FieldHigh;
      ABI::Integer *integer_type;
      ABI::Float *float_type;
      ABI::StructType *struct_type;

      // calculate lowering type
      // FIXME: use switch statement instead
      if ((integer_type = llvm::dyn_cast<ABI::Integer>(*it))) {
        assert(integer_type->getSize() <= 8);

        Classify(integer_type, FieldLow, FieldHigh);
        current_offset += integer_type->getSize();

        // FIXME: do some code cleanup, there is a bit of repetition
        if (current_offset <= 8) {
          Low = Merge(Low, FieldLow);
        } else {
          High = Merge(High, FieldLow);
        }
      } else if ((float_type = llvm::dyn_cast<ABI::Float>(*it))) {
        // something here
        assert(float_type->getSize() <= 8);
        Classify(float_type, FieldLow, FieldHigh);
        current_offset += float_type->getSize();

        // FIXME: do some code cleanup, there is a bit of repetition
        if (current_offset <= 8) {
          Low = Merge(Low, FieldLow);
        } else {
          // FIXME: as of current, classify does not understand that offset
          // matters and sometime should be returned to high
          // this is a jank fix, FieldLow
          High = Merge(High, FieldLow);
        }
      } else if ((struct_type = llvm::dyn_cast<ABI::StructType>(*it))) {
        Classify(struct_type, FieldLow, FieldHigh);
        current_offset += struct_type->getSize();

        // FIXME: cleanup
        Low = Merge(Low, FieldLow);
        High = Merge(High, FieldHigh);
      } else {
        assert(false && "don't know how this happened!");
      }

      assert(current_offset > 0 && "how can this be possible?");
    }

    PostMerger(Low, High, current_offset);
    return;
  }

  assert(false && "unimplemented!");
}

llvm::Type *X86_64ABIInfo::getIntegerType(Type *type, uint64_t offset) {
  // trivial case
  ABI::Integer *int_type;
  if ((int_type = llvm::dyn_cast<ABI::Integer>(type))) {
    if (int_type->getSize() == 4) {
      return llvm::Type::getInt32Ty(Context);
    } else if (int_type->getSize() == 8) {
      return llvm::Type::getInt64Ty(Context);

    } else {
      assert(false && "don't know how to do this");
    }
  }

  // not trivial case
  ABI::StructType *struct_type;
  if ((struct_type = llvm::dyn_cast<ABI::StructType>(type))) {
    uint64_t current_offset = 0;

    // increment the iterator until we reach the offset we want
    auto it = struct_type->getStart(), ie = struct_type->getEnd();
    while (it != ie) {
      if (current_offset == offset)
        break;
      current_offset += (*it)->getSize();
      ++it;
    }
    assert(current_offset == offset &&
           "invalid offset is provided. Are you sure?");

    // we can have two float
    Type *first = *it;
    Type *second = nullptr;
    if (it != ie)
      second = *(++it);

    // trivial case: there is only one float
    assert(llvm::isa<ABI::Integer>(first));
    // FIXME: what about uint64_t?
    if (first && second)
      return llvm::Type::getInt64Ty(Context);

    if (first && !second)
      return llvm::Type::getInt32Ty(Context);

    // if (llvm::isa<ABI::Integer>(first) && llvm::isa<ABI::Integer>(second))
    //   if ((first->getSize() + second->getSize()) == 8)
    //     return llvm::Type::getInt64Ty(Context);
  }

  assert(false && "not yet implemented");
  return nullptr;
}

// offset: the offset required for record type
llvm::Type *X86_64ABIInfo::getSSEType(Type *type, uint64_t offset) {
  Float *float_type;
  if ((float_type = llvm::dyn_cast<Float>(type))) {
    if (float_type->getSize() == 4)
      return llvm::Type::getFloatTy(Context);
    else
      assert("how did we get here?" && false);
  }

  StructType *struct_type;
  if ((struct_type = llvm::dyn_cast<StructType>(type))) {
    uint64_t current_offset = 0;

    // increment the iterator until we reach the offset we want
    auto it = struct_type->getStart(), ie = struct_type->getEnd();
    while (it != ie) {
      if (current_offset == offset)
        break;
      current_offset += (*it)->getSize();
      ++it;
    }
    assert(current_offset == offset &&
           "invalid offset is provided. Are you sure?");

    // we can have two float
    Type *first = *it;
    Type *second = nullptr;
    if (it != ie)
      second = *(++it);

    // trivial case: there is only one float
    assert(llvm::isa<Float>(first));
    if (second && !llvm::isa<Float>(second))
      return llvm::Type::getFloatTy(Context);

    // FIXME: what about __fp16 or _Float16?
    if (llvm::isa<Float>(first) && llvm::isa<Float>(second))
      return llvm::FixedVectorType::get(llvm::Type::getFloatTy(Context), 2);
  }

  assert(false && "what did you input?");
  return nullptr;
}

void X86_64ABIInfo::ComputeInfo(FunctionInfo &FI) {
  // Keep track of the number of assigned registers.
  unsigned FreeIntRegs = 6;
  unsigned FreeSSERegs = 8;

  Class ReturnHigh, ReturnLow;
  Classify(FI.getReturnInfo().Ty, ReturnLow, ReturnHigh);
  switch (ReturnLow) {
  case Memory:
    // fixme: this is not correct
    FI.setABIReturnInfo(ABIArgInfo(Indirect, nullptr));
    break;
  case X87:
  case SSE:
  case Complex:
  case Integer:
    // fixme: this is not correct
    FI.setABIReturnInfo(ABIArgInfo(Direct, nullptr));
    break;
  default:
    assert(false && "how did we get here?");
  }

  // AMD64-ABI 3.2.3p3: Once arguments are classified, the registers
  // get assigned (in left-to-right order) for passing as follows...
  unsigned ArgNo = 0;
  for (FunctionInfo::ArgIter it = FI.GetArgBegin(), ie = FI.GetArgEnd();
       it != ie; ++it, ++ArgNo) {

    Class low;
    Class high;
    Classify(it->Ty, low, high);
    assert(low != NoClass);

    // dealing with low
    // move this into its own logic
    llvm::Type *low_type = nullptr;
    switch (low) {
    case ABI::X86_64ABIInfo::Class::Integer:
      // defined by AMD64-ABI:
      low_type = getIntegerType(it->Ty, 0);
      if (FreeIntRegs > 0) {
        // Use Register
        // TODO: fixme, nullptr is definitely not correct
        it->Info = ABIArgInfo(Direct, low_type);
      } else {
        // Pushed to the stack
        // TODO: fixme, nullptr is definitely not correct
        it->Info = ABIArgInfo(Indirect, low_type);
      }
      --FreeIntRegs;
      break;
    case ABI::X86_64ABIInfo::Class::SSE:
      // defined by AMD64-ABI:
      low_type = getSSEType(it->Ty, 0);
      if (FreeSSERegs > 0) {
        // Use Register
        it->Info = ABIArgInfo(Direct, low_type);
      } else {
        // Pushed to the stack
        // TODO: fixme, nullptr is definitely not correct
        it->Info = ABIArgInfo(Indirect, low_type);
      }
      --FreeSSERegs;
      break;
      // 5. If the class is X87, X87UP or COMPLEX_X87, it is passed in memory.
    case ABI::X86_64ABIInfo::Class::Complex:
    case ABI::X86_64ABIInfo::Class::X87:
    case ABI::X86_64ABIInfo::Class::Memory:
      it->Info = ABIArgInfo(Indirect, it->Ty->toLLVM(Context));
      break;
    default:
      assert(false && "unreachable statement. Not implemented yet");
    }

    llvm::Type *high_type = nullptr;
    switch (high) {
    case ABI::X86_64ABIInfo::Class::SSE:
      // this may not always be correct
      high_type = getSSEType(it->Ty, 8);
      break;
    case ABI::X86_64ABIInfo::Class::Integer:
      high_type = getIntegerType(it->Ty, 8);
      break;
    case ABI::X86_64ABIInfo::Class::NoClass:
      break;
    case ABI::X86_64ABIInfo::Class::Memory:
      // has been handled in low!
      break;
    default:
      assert(false && "how did we get here?");
    }

    // we are making a pair it is direct
    if (high_type && it->Info.GetKind() == Direct) {
      assert(low_type && high_type &&
             "high and low type cannot both be nullptr");
      llvm::StructType *final_type = llvm::StructType::get(low_type, high_type);
      it->Info = ABIArgInfo(Direct, final_type);
    }

    // assert(high == NoClass);
  }
}
