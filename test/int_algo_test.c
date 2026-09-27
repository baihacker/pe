#include "pe_test.h"

namespace int_algo_test {

SL void ContinuedFractionTest() {
  // 3/2 = [1; 2]
  {
    auto cf = ToContinuedFraction<int64>(int64(3), int64(2));
    assert(std::size(cf) == 2 && cf[0] == 1 && cf[1] == 2);
    auto f = FromContinuedFraction<int64>(cf);
    assert(f.a == 3 && f.b == 2);
  }

  // 22/7 = [3; 7]
  {
    auto cf = ToContinuedFraction<int64>(int64(22), int64(7));
    assert(std::size(cf) == 2 && cf[0] == 3 && cf[1] == 7);
    auto f = FromContinuedFraction<int64>(cf);
    assert(f.a == 22 && f.b == 7);
  }

  // Integer: 5/1 = [5]
  {
    auto cf = ToContinuedFraction<int64>(int64(5), int64(1));
    assert(std::size(cf) == 1 && cf[0] == 5);
    auto f = FromContinuedFraction<int64>(cf);
    assert(f.a == 5 && f.b == 1);
  }

  // Convergents of [1; 1, 1, 1, ...] are consecutive Fibonacci ratios
  // h_k / k_k: 1/1, 2/1, 3/2, 5/3, 8/5, 13/8, 21/13, 34/21, 55/34, 89/55
  {
    std::vector<int64> cf(10, int64(1));
    auto convs = FromContinuedFractionN<int64>(cf);
    const int64 fib_num[] = {1, 2, 3, 5, 8, 13, 21, 34, 55, 89};
    const int64 fib_den[] = {1, 1, 2, 3, 5, 8, 13, 21, 34, 55};
    for (int i = 0; i < 10; ++i) {
      assert(convs[i].a == fib_num[i]);
      assert(convs[i].b == fib_den[i]);
    }
    // FromContinuedFractionN with pos parameter
    auto convs5 = FromContinuedFractionN<int64>(cf, 4);
    assert(std::size(convs5) == 5);
    assert(convs5.back().a == fib_num[4] && convs5.back().b == fib_den[4]);
  }

  // Round-trip: p/q -> ToCf -> FromCf -> p/q for random fractions
  srand(42);
  for (int iter = 0; iter < 200; ++iter) {
    int64 p = rand() % 999 + 1;
    int64 q = rand() % 999 + 1;
    int64 g = Gcd(p, q);
    p /= g;
    q /= g;
    auto cf = ToContinuedFraction<int64>(p, q);
    auto frac = FromContinuedFraction<int64>(cf);
    assert(frac.a == p && frac.b == q);
  }

  // ToCf with truncation limit n
  {
    // 355/113 truncated to first 3 terms
    auto cf3 = ToContinuedFraction<int64>(int64(355), int64(113), 3);
    assert(std::size(cf3) == 3);
    assert(cf3[0] == 3);
  }
}

PE_REGISTER_TEST(&ContinuedFractionTest, "ContinuedFractionTest", SMALL);

SL void ExGcdExtTest() {
  // The Bezout identity of the extended-int overloads is also checked in
  // extended_signed_int_test; this covers all three forms.
  using Int128 = pe::ExtendedSignedInt<uint64>;
  for (int a = -15; a <= 15; ++a) {
    for (int b = -15; b <= 15; ++b) {
      if (a == 0 && b == 0) continue;
      const Int128 ia(a), ib(b);
      Int128 x, y;
      const Int128 d = ExGcd(ia, ib, x, y);
      assert(Abs(d).ToInt<int>() == Gcd(std::abs(a), std::abs(b)));
      assert(ia * x + ib * y == d);

      auto [d1, x1, y1] = ExGcd(ia, ib);
      assert(d1 == d && ia * x1 + ib * y1 == d);

      Int128 x2;
      assert(ExGcd(ia, ib, x2) == d && x2 == x);
    }
  }
}

PE_REGISTER_TEST(&ExGcdExtTest, "ExGcdExtTest", SMALL);

SL void CrtNExtTest() {
  using Int128 = pe::ExtendedSignedInt<uint64>;
  // Three moduli, not pairwise coprime: pointer and vector overloads.
  for (int a = 0; a < 6; ++a) {
    for (int b = 0; b < 10; ++b) {
      for (int c = 0; c < 4; ++c) {
        Int128 val[] = {Int128(a), Int128(b), Int128(c)};
        Int128 mod[] = {Int128(6), Int128(10), Int128(4)};
        const auto ans = CrtN<Int128>(val, mod, 3);
        const auto ans_v = CrtN(std::vector<Int128>(val, val + 3),
                                std::vector<Int128>(mod, mod + 3));
        int found = -1;
        for (int x = 0; x < 60; ++x) {
          if (x % 6 == a && x % 10 == b && x % 4 == c) {
            found = x;
            break;
          }
        }
        assert(ans.ok() == (found >= 0) && ans_v.ok() == (found >= 0));
        if (found >= 0) {
          assert(ans.value == found && ans.mod == 60);
          assert(ans_v.value == found && ans_v.mod == 60);
        }
      }
    }
  }
}

PE_REGISTER_TEST(&CrtNExtTest, "CrtNExtTest", SMALL);

SL void ContinuedFractionMoreTest() {
  // Negative values: every term after the first is positive (floor division),
  // and a negative denominator is normalized.
  assert((ToContinuedFraction<int64>(int64(-7), int64(3)) ==
          std::vector<int64>{-3, 1, 2}));
  assert((ToContinuedFraction<int64>(int64(7), int64(-3)) ==
          std::vector<int64>{-3, 1, 2}));
  for (int64 p = -60; p <= 60; ++p) {
    for (int64 q = -13; q <= 13; ++q) {
      if (q == 0) continue;
      const std::vector<int64> cf = ToCf<int64>(p, q);
      for (size_t i = 1; i < std::size(cf); ++i) assert(cf[i] > 0);
      const Fraction<int64> f = FromCf<int64>(cf);
      assert(f == Fraction<int64>(p, q));
      // All convergents; the last one is the value.
      const std::vector<Fraction<int64>> convs = FromCfN<int64>(cf);
      assert(std::size(convs) == std::size(cf) && convs.back() == f);
      // A larger n pads with zeros, which FromContinuedFraction skips.
      const std::vector<int64> padded = ToCf<int64>(p, q, 8);
      assert(std::size(padded) == 8);
      assert(FromContinuedFraction<int64>(padded) == f);
    }
  }
  // pos selects a convergent.
  const std::vector<int64> pi = {3, 7, 15, 1, 292};
  assert(FromContinuedFraction<int64>(pi, 1) == Fraction<int64>(22, 7));
  assert(FromContinuedFraction<int64>(pi, 3) == Fraction<int64>(355, 113));
  assert(FromContinuedFraction<int64>(pi, 100) ==
         FromContinuedFraction<int64>(pi));
  assert(std::empty(FromContinuedFractionN<int64>(std::vector<int64>{})));
  // Big numerators with small terms.
  {
    std::vector<int64> ones(100, 1);
    const Fraction<BigInteger> f = FromContinuedFraction<BigInteger>(ones);
    assert(f.a == "573147844013817084101"_bi && f.b == "354224848179261915075"_bi);
  }

  // Quadratic irrationals (x + y sqrt(w)) / z against a long double
  // expansion; perfect squares and y == 0 take the rational path.
  for (int64 x = -4; x <= 4; ++x) {
    for (int64 y = -2; y <= 2; ++y) {
      for (int64 w : {2, 3, 5, 7, 9, 13, 16}) {
        for (int64 z : {-3, -2, -1, 1, 2, 5}) {
          const int n = 10;
          const std::vector<int64> cf = ToCf<int64>(x, y, w, z, n);
          assert(static_cast<int>(std::size(cf)) == n);
          const int64 s = SqrtI(w);
          if (y == 0 || s * s == w) {
            assert(cf == ToCf<int64>(x + y * s, z, n));
            continue;
          }
          // The long double error grows with the product of the terms, so
          // only the first terms are compared.
          long double v = (x + y * std::sqrt(static_cast<long double>(w))) / z;
          for (int i = 0; i < 6; ++i) {
            const int64 a = static_cast<int64>(std::floor(v));
            assert(cf[i] == a);
            v = 1 / (v - a);
          }
        }
      }
    }
  }
  assert((ToContinuedFraction<int64>(0, 1, 2, 1, 5) ==
          std::vector<int64>{1, 2, 2, 2, 2}));
  assert((ToContinuedFraction<int64>(1, 1, 5, 2, 5) ==
          std::vector<int64>{1, 1, 1, 1, 1}));
}

PE_REGISTER_TEST(&ContinuedFractionMoreTest, "ContinuedFractionMoreTest",
                 SMALL);

template <typename T>
SL void TestFactPPowerModer() {
  // n! mod p^e where the multiples of p contribute 1.
  for (auto [p, e] : std::vector<std::pair<int64, int64>>{
           {2, 1}, {2, 3}, {2, 5}, {3, 1}, {3, 4}, {5, 2}, {7, 3}}) {
    const int64 mod = Power(p, e);
    FactPPowerModer<T> moder(p, e);
    int64 expected = 1 % mod;
    for (int64 n = 0; n <= 400; ++n) {
      if (n > 0 && n % p != 0) expected = expected * n % mod;
      assert(moder.Cal(T(n)) == expected);
    }
  }
}

SL void FactPPowerModerTest() {
  // Extended integers don't compile (see the TODO(bug) in pe_int_algo).
  TestFactPPowerModer<BigInteger>();
#if ENABLE_GMP
  TestFactPPowerModer<MpInteger>();
#endif
}

PE_REGISTER_TEST(&FactPPowerModerTest, "FactPPowerModerTest", SMALL);

}  // namespace int_algo_test
