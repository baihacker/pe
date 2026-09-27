#include "pe_test.h"

namespace fft_test {
// ntt64::PolyMul is the reference: it is always available and its capacity
// ((mod-1)^2*n < 3.5e35) covers every case below.
SL std::vector<uint64> Reference(const std::vector<uint64>& x,
                                 const std::vector<uint64>& y, int64 mod) {
  return ntt64::PolyMul(x, y, mod);
}

SL std::vector<uint64> RandomPoly(int n, int64 mod) {
  std::vector<uint64> ret(n);
  for (auto& v : ret) v = (uint64)CRand63() % mod;
  return ret;
}

SL void RandomTest() {
  srand(123456789);
  {
    // 1e5+19, mod * n = 7e8
    const int64 mod = 100019;
    const std::vector<uint64> x = RandomPoly(7000, mod), y = RandomPoly(7000, mod);
    const std::vector<uint64> expected = Reference(x, y, mod);
    assert(fft::PolyMulFft(x, y, mod) == expected);
    assert(fft::PolyMulFftSmall(x, y, mod) == expected);
  }
  {
    // 1e9+7
    const int64 mod = 1000000007;
    const std::vector<uint64> x = RandomPoly(100000, mod),
                              y = RandomPoly(100000, mod);
    assert(fft::PolyMulFft(x, y, mod) == Reference(x, y, mod));
  }
  {
    // 1e10+19, mod * n = 8e14
    const int64 mod = 10000000019;
    const std::vector<uint64> x = RandomPoly(80000, mod),
                              y = RandomPoly(80000, mod);
    assert(fft::PolyMulFft(x, y, mod) == Reference(x, y, mod));
  }
}

// All coefficients are mod - 1: the limits in the comment of pe_fft.
SL void LimitTest() {
  {
    // 10018*10018*2048=205537943552 2.06e11
    const int64 mod = 100019;
    const std::vector<uint64> x(2048, mod - 1), y(2048, mod - 1);
    const std::vector<uint64> expected = Reference(x, y, mod);
    assert(fft::PolyMulFft(x, y, mod) == expected);
    assert(fft::PolyMulFftSmall(x, y, mod) == expected);
  }
  {
    // 1000000007*339750=339750002378250=3.39e14
    const int64 mod = 1000000007;
    const std::vector<uint64> x(339750, mod - 1), y(339750, mod - 1);
    assert(fft::PolyMulFft(x, y, mod) == Reference(x, y, mod));
  }
  {
    // 10000000019*44064=440640000837216=4.4e14
    const int64 mod = 10000000019;
    const std::vector<uint64> x(44064, mod - 1), y(44064, mod - 1);
    assert(fft::PolyMulFft(x, y, mod) == Reference(x, y, mod));
  }
}

SL void SmallTest() {
  // Small and unequal sizes against a naive product, both APIs.
  fft::InitFftK();  // already initialized: no effect
  for (int64 mod : {int64(2), int64(97), int64(100019), int64(1000000007)}) {
    for (int n : {1, 2, 3, 17, 64, 100}) {
      for (int m : {1, 5, 64, 129}) {
        for (int zeros = 0; zeros < 2; ++zeros) {
          std::vector<uint64> x = RandomPoly(n, mod), y = RandomPoly(m, mod);
          if (zeros) std::fill(std::begin(x), std::end(x), 0);
          std::vector<uint64> expected(n + m - 1);
          for (int i = 0; i < n; ++i)
            for (int j = 0; j < m; ++j)
              expected[i + j] = (expected[i + j] + x[i] * y[j] % mod) % mod;

          assert(fft::PolyMulFft(x, y, mod) == expected);
          std::vector<uint64> result(n + m - 1);
          fft::PolyMulFft(x.data(), n, y.data(), m, result.data(), mod);
          assert(result == expected);
          if (mod <= 100019) {
            assert(fft::PolyMulFftSmall(x, y, mod) == expected);
            fft::PolyMulFftSmall(x.data(), n, y.data(), m, result.data(), mod);
            assert(result == expected);
          }
          // Signed element type
          const std::vector<int64> xs(std::begin(x), std::end(x));
          const std::vector<int64> ys(std::begin(y), std::end(y));
          const std::vector<int64> es(std::begin(expected),
                                      std::end(expected));
          assert(fft::PolyMulFft(xs, ys, mod) == es);
        }
      }
    }
  }
}

SL void FftTest() {
  SmallTest();
  RandomTest();
  LimitTest();
}
PE_REGISTER_TEST(&FftTest, "FftTest", SMALL);
}  // namespace fft_test
