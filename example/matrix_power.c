#include <pe.hpp>
using namespace pe;

const int64 mod = 316227766016779;
using MT = NMod64<mod>;

// Computes
// |1 1|^n *  |1|
// |1 0|      |0|

// Mod is specified at compiling time.
// The element type is choosed internally.
int64 solve0(int64 n) {
  auto ans = MatrixPowerMod<mod>(
      [=](auto& m, auto& v) {
        m(0, 0) = 1;
        m(0, 1) = 1;
        m(1, 0) = 1;
        v[0] = 1;
        v[1] = 0;
      },
      2, n);
  return ans[0];
}

// Mod is associated with T at compiling time.
int64 solve1(int64 n) {
  auto ans = MatrixPowerMod<MT>(
      [=](auto& m, auto& v) {
        m(0, 0) = 1;
        m(0, 1) = 1;
        m(1, 0) = 1;
        v[0] = 1;
        v[1] = 0;
      },
      2, n);
  return ans[0].value();
}

// Mod is associated with T at runtime.
// Different threads use different mod.
int64 solve2(int64 n, int64 rmod) {
  Eigen::setNbThreads(1);
  TLMod64::Set(rmod);
  auto ans = MatrixPowerMod<TLNMod64<>>(
      [=](auto& m, auto& v) {
        m(0, 0) = 1;
        m(0, 1) = 1;
        m(1, 0) = 1;
        v[0] = 1;
        v[1] = 0;
      },
      2, n);
  Eigen::setNbThreads(0);
  return ans[0].value();
}

// Mod is associated with T at runtime.
// All the threads use the same mod.
int64 solve3(int64 n, int64 rmod) {
  using T = NModNumber<DefaultMod>;
  DefaultMod::Set(rmod);
  auto ans = MatrixPowerMod<T>(
      [=](auto& m, auto& v) {
        m(0, 0) = 1;
        m(0, 1) = 1;
        m(1, 0) = 1;
        v[0] = 1;
        v[1] = 0;
      },
      2, n);
  return ans[0].value();
}

// Mod is specified at runtime.
// If int128 is available, use int128 as element type, different threads use
// different mod.
// Otherwise, use DefaultMod, all the threads use the same mod, and this is same
// as solve3 and the difference is solve3 needs to set the default mod
// explicitly and solve4 will set it automatically.
int64 solve4(int64 n, int64 mod) {
  auto ans = MatrixPowerMod(
      [=](auto& m, auto& v) {
        m(0, 0) = 1;
        m(0, 1) = 1;
        m(1, 0) = 1;
        v[0] = 1;
        v[1] = 0;
      },
      2, n, mod);
  return ans[0];
}

int main() {
  PE_INIT(maxp = 200000000);

  for (int64 n = 1; n <= 1000000000; n *= 10) {
    std::cout << "n = " << n << std::endl;
    int64 ans0 = solve0(n);
    int64 ans1 = solve1(n);
    int64 ans2 = solve2(n, mod);
    int64 ans3 = solve3(n, mod);
    int64 ans4 = solve4(n, mod);
    std::cout << "Ans0 = " << ans0 << std::endl;
    std::cout << "Ans1 = " << ans1 << std::endl;
    std::cout << "Ans2 = " << ans2 << std::endl;
    std::cout << "Ans3 = " << ans3 << std::endl;
    std::cout << "Ans4 = " << ans4 << std::endl;
    std::cout << std::endl;
  }

  for (int64 rmod = mod; rmod <= mod + 100; ++rmod)
    if (IsPrime(rmod)) {
      const int64 n = 1000000000;
      std::cout << "n = " << n << std::endl;
      std::cout << "rmod = " << rmod << std::endl;
      int64 ans2 = solve2(n, rmod);
      int64 ans3 = solve3(n, rmod);
      int64 ans4 = solve4(n, rmod);
      std::cout << "Ans2 = " << ans2 << std::endl;
      std::cout << "Ans3 = " << ans3 << std::endl;
      std::cout << "Ans4 = " << ans4 << std::endl;
      std::cout << std::endl;
    }

  return 0;
}