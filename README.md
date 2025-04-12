# LLVM ABI Library Prototype  

This is a prototype LLVM ABI lowering library focused on the **System V ABI**. It provides a simple, self-contained implementation for call ABI. 

## Features  

- **System V ABI lowering implementation** following AMD-SystemV conventions  (see [`Call.cpp`](./src/Call.cpp))  
- A **basic type system** designed specifically for ABI lowering (see [`Type.cpp`](./src/Type.cpp))  
- Can be extended to different calling conventions and targets.

## Overview  

This library is designed to assist **language frontend developers** by providing ABI lowering information based on basic type descriptions. The frontend supplies type information, and the library determines how the types should be passed according to the **System V ABI** (e.g., in registers or on the stack).  Additionally, llvm type information will be provided.

### Updates Since V1

**LLVM Type Information** are now also given in [`ABIArgInfo.cpp`](./src/Function.cpp). See below for an example.

### Example Usage  

A frontend might generate type information like this:  

```cpp
Integer a(4);
Integer b(4);
FloatType c(4);
FloatType d(4);
llvm::SmallVector<Type *> layout{&a, &b, &c, &d}; // layout
StructType arg(layout);
Integer return_type(4);

FunctionInfo FI({&arg}, &return_type, ABI::CallingConvention::C);
llvm::LLVMContext context;
X86_64ABIInfo abiLowering(context);
abiLowering.ComputeInfo(FI);
```

This represents the following C function signature:

```c
typedef struct {
    int a, b;
    float c, d;
} some_type_t;

int some_func(some_type_t a);
```

and this IR function prototype:
```
define dso_local i32 @some_func(i64 %0, <2 x float> %1) ...
```

By calling `ComputeInfo`, the library produces an `ABIInfo` object, which specifies how arguments should be passed (e.g., in registers or on the stack).

Calling `getType()` method on `ABIArgInfo` return the native llvm type will be given.

```c
ABIArgInfo abi_return_info = FI.getReturnInfo().Info;
assert(abi_return_info.GetKind() == Direct);

auto ArgIterator = FI.GetArgBegin();
ABIArgInfo abiInfo = ArgIterator->Info;
assert(abiInfo.GetKind() == Direct);

// {i64, < 2 x float>}
assert(abiInfo.getType() ==
     llvm::StructType::get(
         llvm::Type::getInt64Ty(context),
         llvm::FixedVectorType::get(llvm::Type::getFloatTy(context), 2)));

FI.dump();

// Outputs the following: 
// Integer32 unknown_func({ Integer32,Integer32,Float32,Float32 })
// LLVM IR Parameter type information:
// { i64, <2 x float> }
// LLVM IR Return Type:
// Integer32 
```


See [`Test.cpp`](./test/test.cpp) for more example on how this is done.
## Building  

Note: **llvm must be installed on your computer**. If you are running ubuntu, you probably have to do 
something like the following: 

```
sudo apt install llvm-14
```

As far as I can tell, the version of llvm doesn't seem to matter that much. It is prove to work on llvm-14.


To build the project, run:  

```sh
mkdir build
cd build
cmake .. 
make -j
```

## Running Tests

```
./test/test 
``` 
