#include <iostream>
#include <memory>

void TestOne() {
  // this should probably be a wrapper!
  // 64 int
  // std::shared_ptr<ABI::Type> = std::make_shared<ABI::Integer>(4);

  std::cout << "Passed test one!" << std::endl;
}

void RunTest() { TestOne(); }

int main() { RunTest(); }
