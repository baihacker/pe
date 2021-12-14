#include <pe.hpp>
using namespace pe;

int main() {
  PE_INIT(maxp = 2000000);
  // Convert mathematica expression to cpp
  // For mathematica function, it only supports Sqrt
  for (auto a : mma::Compile(
           "Sqrt(-A^4 + 2 A^2 B^2 - B^4 + 2 A^2 x1^2 + 2 B^2 x1^2 - x1^4 - 4 "
           "A^2 x1 x2 - 4 B^2 x1 x2 + 4 x1^3 x2 + 2 A^2 x2^2 + 2 B^2 x2^2 - "
           "6 x1^2 x2^2 + 4 x1 x2^3 - x2^4)/(2 Sqrt(x1^2 - 2 x1 x2 + "
           "x2^2))")) {
    std::cout << a << std::endl;
  }
  // output:
  // sqrt(-A * A * A * A + 2 * A * A * B * B - B * B * B * B + 2 * A * A * x1 *
  // x1 + 2 * B * B * x1 * x1 - x1 * x1 * x1 * x1 - 4 * A * A * x1 * x2 - 4 * B
  // * B * x1 * x2 + 4 * x1 * x1 * x1 * x2 + 2 * A * A * x2 * x2 + 2 * B * B *
  // x2 * x2 - 6 * x1 * x1 * x2 * x2 + 4 * x1 * x2 * x2 * x2 - x2 * x2 * x2 *
  // x2) / (2
  // * sqrt(x1 * x1 - 2 * x1 * x2 + x2 * x2))
  // Convert mathematica expression to cpp using modular arithmetic.
  for (auto a : mma::CompileMod("(a^4+a b)*7/b")) {
    std::cout << a << std::endl;
  }
  // output:
  // int64 foo(int64 a, int64 b, int64 mod) {
  //   const int64 t0 = a % mod;
  //   const int64 t1 = 4;
  //   const int64 t2 = PowerModEx(t0, t1, mod) % mod;
  //   const int64 t3 = b % mod;
  //   const int64 t4 = (t0 * t3) % mod;
  //   const int64 t5 = (t2 + t4) % mod;
  //   const int64 t6 = 7 % mod;
  //   const int64 t7 = (t5 * t6) % mod;
  //   const int64 t8 = t7 * InvOf(t3, mod) % mod;
  //   return t8;
  // }
  return 0;
}