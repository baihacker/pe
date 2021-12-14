#include <pe.hpp>
using namespace pe;

const int64 mod = 1000000007;
int main() {
  {
    // Polynomial inverse
    NModPoly x{{1LL, 96LL}, 97};
    std::cout << x.Inv(20) << std::endl;
    std::cout << x.Inv(20) * x << std::endl;
  }

  {
    // Polynomial inverse
    NModPoly x{{}, 97};
    for (int i = 1; i < 100; ++i) x.data.push_back(i % 97);
    std::cout << x.Inv(100) * x << std::endl;
  }

  {
    // Fibonacci sequence
    NModPoly x{{1, mod - 1, mod - 1}, mod};

    TimeRecorder tr;
    std::cout << x.Inv(100000)[99999] << std::endl;
    std::cout << tr.Elapsed().Format() << std::endl;
  }

  {
    // Fibonacci sequence
    std::vector<int64> A{1, -1, -1};
    std::vector<int64> B{1};

    TimeRecorder tr;
    std::cerr << GfAt({A, mod}, {B, mod}, 99999) << std::endl;
    std::cout << tr.Elapsed().Format() << std::endl;
  }
  return 0;
}