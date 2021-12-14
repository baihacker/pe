#include "pe.hpp"
using namespace pe;
template <typename GBI>
void demo() {
  vi data{1, 2, 2, 2, 2, 2, 2, 2, 2, 2};
  for (int i = 0; i < 10; ++i) {
    std::cout << FromCf<GBI>(data, i) << std::endl;
  }
  std::cout << FromCfN<GBI>(data) << std::endl;
  for (int i = 50; i <= 50; ++i) {
    std::cout << i << " " << ToCf<GBI>(1, 6, i) << std::endl;
  }

  std::cout << ToCf<GBI>(1, 2, 10) << std::endl;

  std::cout << FromCf<GBI>(ToCf<GBI>(1, 2, 50)) << std::endl;
}

int main() {
#if ENABLE_GMP
  demo<bi>();
  demo<Mpz>();
#endif
  return 0;
}
