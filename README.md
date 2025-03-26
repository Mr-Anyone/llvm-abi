# LLVM ABI Library Prototype  

⚠ **Project Status: Incomplete** ⚠  

This is a prototype LLVM ABI lowering library focused on the **System V ABI**. It provides a simple, self-contained implementation for call ABI. 

## Features  

- A **basic type system** designed specifically for ABI lowering (see [`Type.cpp`](./src/Type.h))  
- Can be extended to different calling conventions and targets.
- **System V ABI lowering implementation** following AMD-SystemV conventions  (see [`Call.cpp`](./src/Call.h))  
- **No external dependencies** – the library is fully self-contained  

## Overview  

This library is designed to assist **language frontend developers** by providing ABI lowering information based on basic type descriptions. The frontend supplies type information, and the library determines how the types should be passed according to the **System V ABI** (e.g., in registers or on the stack).  

### Example Usage  

A frontend might generate type information like this:  

```cpp
std::shared_ptr<ABI::Type> arg = std::make_shared<ABI::Integer>(/*size*/ 8);
std::vector<std::shared_ptr<ABI::Type>> args{arg, arg, arg, arg, arg};
std::shared_ptr<ABI::StructType> arg_one =
    std::make_shared<ABI::StructType>(args);

std::shared_ptr<ABI::Type> returnType = std::make_shared<ABI::Integer>(8);
ABI::FunctionInfo FI(args, returnType, ABI::CallingConvention::C);
```

This represents the following C function signature:

```c
typedef struct {
    uint64_t a, b, c, d, e; 
} some_type_t;

uint64_t some_func(some_type_t type);
```
By calling `ComputeInfo`, the library produces an `ABIInfo` object, which specifies how arguments should be passed (e.g., in registers or on the stack).

```
ABI::X86_64ABIInfo abiLowering;
abiLowering.ComputeInfo(FI);

// Check return type handling
assert(FI.getReturnInfo().Info.GetKind() == ABI::Direct);

// Verify struct passing strategy
auto ArgIterator = FI.GetArgBegin();
assert(ArgIterator->Info.GetKind() == ABI::Indirect); // Structs larger than 4 bytes are passed by reference
```

See [`Test.cpp`](./test/test.cpp) for more example on how this is done.
## Building  

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

