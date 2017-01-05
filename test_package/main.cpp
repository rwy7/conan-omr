#include "Jit.hpp"
#include "ilgen/MethodBuilder.hpp"
#include "ilgen/TypeDictionary.hpp"
#include <cstdint>
#include <iostream>

class SimpleMethod : public TR::MethodBuilder {
public:
  SimpleMethod(TR::TypeDictionary *types);
  virtual bool buildIL() override;
};

SimpleMethod::SimpleMethod(TR::TypeDictionary *types)
    : TR::MethodBuilder(types) {
  DefineFile(__FILE__);
  DefineLine(LINETOSTR(__LINE__));

  // int32_t increment(int32_t value);
  DefineName("increment");
  DefineParameter("value", Int32);
  DefineReturnType(Int32);
}

bool SimpleMethod::buildIL() {
  std::cout << "SimpleMethod::buildIL() running!\n";
  // return (value + 1);
  Return(Add(Load("value"), ConstInt32(1)));
  return true;
}

extern "C" int main(int argc, char **argv) {
  std::cout << "Step 1: initialize JIT" << std::endl;
  auto initialized = initializeJit();
  if (!initialized) {
    std::cerr << "FAIL: could not initialize JIT" << std::endl;
    exit(1);
  }

  std::cout << "Step 2: define type dictionary" << std::endl;
  TR::TypeDictionary types;

  std::cout << "Step 3: compile method builder\n";
  SimpleMethod method(&types);
  uint8_t *entry = 0;
  auto rc = compileMethodBuilder(&method, &entry);
  if (rc != 0) {
    std::cerr << "FAIL: compilation error " << rc << std::endl;
    exit(2);
  }

  std::cout << "Step 4: invoke compiled code and print results" << std::endl;
  auto increment = (int32_t(*)(int32_t))(entry);
  for (int32_t x : {1, 2, 10, -15}) {
    auto y = increment(x);
    std::cout << "increment(" << x << ") == " << y << std::endl;
  }

  std::cout << "Step 5: shutdown JIT" << std::endl;
  shutdownJit();
  return 0;
}
