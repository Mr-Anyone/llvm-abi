#ifndef FUNCTION_H
#define FUNCTION_H

#include "Type.h"
#include <memory>
#include <vector>

namespace ABI {
// Taken from clang and the system-v abi
enum ABIArgKind : uint8_t {
  /// Direct - Pass the argument directly using the normal converted LLVM
  /// type, or by coercing to another specified type stored in
  /// 'CoerceToType').  If an offset is specified (in UIntData), then the
  /// argument passed is offset by some number of bytes in the memory
  /// representation. A dummy argument is emitted before the real argument
  /// if the specified type stored in "PaddingType" is not zero.
  Direct,

  /// Extend - Valid only for integer argument types. Same as 'direct'
  /// but also emit a zero/sign extension attribute.
  Extend,

  /// Indirect - Pass the argument indirectly via a hidden pointer with the
  /// specified alignment (0 indicates default alignment) and address space.
  Indirect,

  /// IndirectAliased - Similar to Indirect, but the pointer may be to an
  /// object that is otherwise referenced.  The object is known to not be
  /// modified through any other references for the duration of the call, and
  /// the callee must not itself modify the object.  Because C allows
  /// parameter variables to be modified and guarantees that they have unique
  /// addresses, the callee must defensively copy the object into a local
  /// variable if it might be modified or its address might be compared.
  /// Since those are uncommon, in principle this convention allows programs
  /// to avoid copies in more situations.  However, it may introduce *extra*
  /// copies if the callee fails to prove that a copy is unnecessary and the
  /// caller naturally produces an unaliased object for the argument.
  IndirectAliased,

  /// Ignore - Ignore the argument (treat as void). Useful for void and
  /// empty structs.
  Ignore,

  /// Expand - Only valid for aggregate argument types. The structure should
  /// be expanded into consecutive arguments for its constituent fields.
  /// Currently expand is only allowed on structures whose fields
  /// are all scalar types or are themselves expandable types.
  Expand,

  /// CoerceAndExpand - Only valid for aggregate argument types. The
  /// structure should be expanded into consecutive arguments corresponding
  /// to the non-array elements of the type stored in CoerceToType.
  /// Array elements in the type are assumed to be padding and skipped.
  CoerceAndExpand,

  /// InAlloca - Pass the argument directly using the LLVM inalloca attribute.
  /// This is similar to indirect with byval, except it only applies to
  /// arguments stored in memory and forbids any implicit copies.  When
  /// applied to a return type, it means the value is returned indirectly via
  /// an implicit sret parameter stored in the argument struct.
  InAlloca,
  KindFirst = Direct,
  KindLast = InAlloca
};

class ABIArgInfo {
public:
private:
  ABIArgKind Kind;

public:
  // set Kind to Direct for Register
  ABIArgInfo();
  ABIArgInfo(ABIArgKind kind);

  ABIArgKind GetKind() const;
};

enum CallingConvention {
  // define Calling convention
  C,

  Size
};

struct FunctionArgInfo {
  std::shared_ptr<Type> Ty;
  ABIArgInfo Info;
};

class FunctionInfo {
public:
  using ArgIter = std::vector<FunctionArgInfo>::iterator;

  // TODO: you are missing a lot of stuff, variadic, etc
  FunctionInfo(std::vector<std::shared_ptr<Type>> args,
               std::shared_ptr<Type> ret, CallingConvention conv);

  ArgIter GetArgBegin();
  ArgIter GetArgEnd();

  FunctionArgInfo getReturnInfo() const;
  void setABIReturnInfo(const ABIArgInfo &info);

private:
  CallingConvention Conv;

  std::vector<FunctionArgInfo> Args;
  FunctionArgInfo RetInfo;
};
} // namespace ABI

#endif
