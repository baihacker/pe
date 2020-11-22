#include <pe.hpp>

const int64 mod = 1000000007;
using MT = NMod64<mod>;

/**
 * Computes
 * |1 1|^n *  |1|
 * |1 0|      |0|
 */

// mod is specified at compiling time.
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

// Use a customized type which depends on mod specified at compiling time.
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

// Mod is specified at runtime.
// If int128 is available, use int128 as element type, different threads use
// different mod. Otherwise, use DefaultMod, All the threads use the same mod.
int64 solve2(int64 n, int64 mod) {
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

// Mod is specified at runtime.
// Different threads use different mod.
int64 solve3(int64 n, int64 rmod) {
  TLMod<int64>::Set(rmod);
  auto ans = MatrixPowerMod<TLNMod64<>>(
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
// All the threads use the same mod.
int64 solve4(int64 n, int64 rmod) {
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

int main() {
  PE_INIT(maxp = 2000000);

  for (int64 n = 1; n <= 1000000000; n *= 10) {
    cout << "n = " << n << endl;
    int64 ans0 = solve0(n);
    int64 ans1 = solve1(n);
    int64 ans2 = solve2(n, mod);
    int64 ans3 = solve3(n, mod);
    int64 ans4 = solve4(n, mod);
    cout << "Ans0 = " << ans0 << endl;
    cout << "Ans1 = " << ans1 << endl;
    cout << "Ans2 = " << ans2 << endl;
    cout << "Ans3 = " << ans3 << endl;
    cout << "Ans4 = " << ans4 << endl;
    cout << endl;
  }

  for (int64 rmod = mod; rmod <= mod + 100; ++rmod)
    if (IsPrime(rmod)) {
      const int64 n = 1000000000;
      cout << "n = " << n << endl;
      cout << "rmod = " << rmod << endl;
      int64 ans2 = solve2(n, rmod);
      int64 ans3 = solve3(n, rmod);
      int64 ans4 = solve4(n, rmod);
      cout << "Ans2 = " << ans2 << endl;
      cout << "Ans3 = " << ans3 << endl;
      cout << "Ans4 = " << ans4 << endl;
      cout << endl;
    }

  return 0;
}