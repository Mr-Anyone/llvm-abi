#include "Function.h"
#include "Type.h"
#include <iostream>

void TestOne() {

  std::cout << "I have passed test one" << std::endl;
}

void RunTest() { TestOne(); }

int main() { RunTest(); }
