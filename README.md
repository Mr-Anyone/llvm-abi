# LLVM ABI Library Prototype  

This is a prototype LLVM ABI lowering library focused on the **System V ABI**. It provides a simple, self-contained implementation for call ABI. 

## Features  

- A **basic type system** designed specifically for ABI lowering (see [`Type.cpp`](./src/Type.h))  
- Can be extended to different calling conventions and targets.
- **System V ABI lowering implementation** following AMD-SystemV conventions  (see [`Call.cpp`](./src/Call.h))  

## Overview  

This library is designed to assist **language frontend developers** by providing ABI lowering information based on basic type descriptions. The frontend supplies type information, and the library determines how the types should be passed according to the **System V ABI** (e.g., in registers or on the stack).  

### Example Usage  

A frontend might generate type information like this:  

```cpp
Integer arg(/*size*/ 8);
std::vector<Type *> record{&arg, &arg, &arg, &arg, &arg};
StructType arg_one(record);

Integer returnType (/*size*/ 8);
FunctionInfo FI({&arg_one}, &returnType, CallingConvention::C);
```

This represents the following C function signature:

```c
typedef struct {
    uint64_t a, b, c, d, e; 
} some_type_t;

uint64_t some_func(some_type_t type);
```
By calling `ComputeInfo`, the library produces an `ABIInfo` object, which specifies how arguments should be passed (e.g., in registers or on the stack).

```c
assert(FI.getReturnInfo().Info.GetKind() == Direct);

// struct larger than 4 bytes in the memory
auto ArgIterator = FI.GetArgBegin();
ABIArgInfo abiInfo = ArgIterator->Info;
assert(abiInfo.GetKind() == Indirect);
```

See [`Test.cpp`](./test/test.cpp) for more example on how this is done.
## Building  

Note: **llvm must be installed on your computer**. If you are running ubuntu, you probably have to do 
something like the following: 

```
sudo apt install llvm-14
```


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

