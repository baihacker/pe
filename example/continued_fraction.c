#include "pe.hpp"
using namespace pe;

template <typename T>
void demo() {
  std::vector<int> data{1, 2, 2, 2, 2, 2, 2, 2, 2, 2};
  for (int i = 0; i < 10; ++i) {
    std::cout << FromCf<T>(data, i) << std::endl;
  }
  std::cout << FromCfN<T>(data) << std::endl;
  for (int i = 50; i <= 50; ++i) {
    std::cout << i << " " << ToCf<T>(0, 1, 6, 1, i) << std::endl;
  }

  std::cout << ToCf<T>(0, 1, 2, 1, 10) << std::endl;
  std::cout << FromCf<T>(ToCf<T>(0, 1, 2, 1, 50)) << std::endl;

  std::cout << ToCf<T>(123456, 654321) << std::endl;
  std::cout << FromCf<T>(ToCf<T>(123456, 654321)) << std::endl;
}

int main() {
  demo<BigInteger>();
#if ENABLE_GMP
  demo<MpInteger>();
#endif
  return 0;
}
