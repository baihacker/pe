#include "pe_test.h"

namespace nt_test {
SL void ParityTest() {
  for (int i = -5; i <= 5; ++i) {
    assert(IsEven(i) == (i % 2 == 0));
    assert(IsOdd(i) == (i % 2 != 0));
    for (int j = -5; j <= 5; ++j) {
      assert(SameParity(i, j) == ((i - j) % 2 == 0));
    }
  }
}
PE_REGISTER_TEST(&ParityTest, "ParityTest", SMALL);

SL void SqrtITest() {
  // Dense for small i, then sparse up to 3e9 (i^2 close to 2^63).
  for (int64 i = 1; i <= 3000000000; i += i / 10000 + 1) {
    const int64 num = i * i;
    for (int64 offset = -10; offset <= 10; ++offset) {
      int64 n = offset + num;
      if (n < 0) {
        continue;
      }
      int64 u = SqrtI(n);
      assert(u * u <= n);
      assert((u + 1) * (u + 1) > n);
    }
  }

  assert(SqrtI(9999999999999999) == 99999999);
  assert(SqrtI(9999999999999999 + 1) == 99999999 + 1);
  assert(SqrtI(999999999999999999) == 999999999);
  assert(SqrtI(999999999999999999 + 1) == 999999999 + 1);

  {
    const int64 x = 3037000499;
    assert(SqrtI(x * x + 1) == x);
    assert(SqrtI(x * x) == x);
    assert(SqrtI(x * x - 1) == x - 1);
  }
  {
    const uint64 x = 4294967295;
    assert(SqrtI(x * x + 1) == x);
    assert(SqrtI(x * x) == x);
    assert(SqrtI(x * x - 1) == x - 1);
  }
}

PE_REGISTER_TEST(&SqrtITest, "SqrtITest", SMALL);

SL void RootITest() {
  for (int64 v = 2, k = 1; k <= 50; v *= 2, ++k) {
    for (int i = 2; i <= 16; ++i) {
      int64 ans = RootI(v, i);
      int64 val1 = Power(ans, i);
      int64 val2 = Power(ans + 1, i);
      assert(val1 <= v);
      assert(val2 > v);
    }
  }
  for (int64 v = 10, k = 1; k <= 15; v *= 10, ++k) {
    for (int i = 2; i <= 16; ++i) {
      int64 ans = RootI(v, i);
      int64 val1 = Power(ans, i);
      int64 val2 = Power(ans + 1, i);
      assert(val1 <= v);
      assert(val2 > v);
    }
  }
}
PE_REGISTER_TEST(&RootITest, "RootITest", SMALL);

SL void LogITest() {
  for (int i = 2; i <= 16; ++i) {
    for (int64 n = 2, k = 1; k <= 50; n *= 2, ++k) {
      int ans = LogI(i, n);
      int64 val1 = Power(i, ans);
      int64 val2 = Power(i, ans + 1);
      assert(val1 <= n);
      assert(val2 > n);
    }
    for (int64 n = 10, k = 1; k <= 15; n *= 10, ++k) {
      int ans = LogI(i, n);
      int64 val1 = Power(i, ans);
      int64 val2 = Power(i, ans + 1);
      assert(val1 <= n);
      assert(val2 > n);
    }
  }
}
PE_REGISTER_TEST(&LogITest, "LogITest", SMALL);

SL void GcdTest() {
  assert(Gcd({2, 4, 6}) == 2);
  assert(Gcd(2, 4, 6) == 2);
  assert(Gcd(2L, 4LL, 6ULL) == 2);
}
PE_REGISTER_TEST(&GcdTest, "GcdTest", SMALL);

SL void GetFactorsTest() {
  for (int64 n = 1; n <= 100; ++n) {
    std::vector<int64> result = GetFactors(n);
    std::sort(std::begin(result), std::end(result));
    std::vector<int64> expected;
    for (int64 d = 1; d <= n; ++d) {
      if (n % d == 0) expected.push_back(d);
    }
    assert(result == expected);
  }

  for (int64 limit = -1; limit <= 20; ++limit) {
    std::vector<pe::int64> result = GetFactors(12, limit);
    std::sort(std::begin(result), std::end(result));
    std::vector<int64> expected;
    for (int64 iter : {1, 2, 3, 4, 6, 12}) {
      if (limit < 0 || iter <= limit) {
        expected.push_back(iter);
      }
    }
    assert(result == expected);
  }
}

PE_REGISTER_TEST(&GetFactorsTest, "GetFactorsTest", SMALL);

SL int IsSquareFreeNormal(int64 n) {
  for (std::pair<int64, int>& iter : Factorize(n)) {
    if (iter.second > 1) {
      return 0;
    }
  }
  return 1;
}

SL void IsSquareFreeTest() {
  // pmask path, around maxp, and trial division path.
  for (int64 start : {int64(1), maxp - 5000, int64(1000000000000)}) {
    for (int64 n = start; n <= start + 2000; ++n) {
      assert(IsSquareFree(n) == IsSquareFreeNormal(n));
    }
  }
}

PE_REGISTER_TEST(&IsSquareFreeTest, "IsSquareFreeTest", SMALL);

SL void CalModOrderTest() {
  const int64 mod = 97;
  std::map<int64, int64> order_to_cnt;
  for (int64 n = 1; n < mod; ++n) {
    int64 ans1 = CalModOrder<int64>(n, mod);
    auto s =
        MakePeriodicSequence1<int64>(n, [=](int64 a) { return a * n % mod; });
    int64 ans2 = s.end - s.start;
    assert(ans1 == ans2);
    ++order_to_cnt[ans1];
  }
  for (auto [order, cnt] : order_to_cnt) {
    assert(cnt == CalPhi(order));
  }
}

PE_REGISTER_TEST(&CalModOrderTest, "CalModOrderTest", SMALL);

SL void SquareRootModTest() {
  for (int i = 0; i < 200; ++i) {
    const int64 p = plist[i];
    int cnt = 0;
    for (int n = 0; n < p; ++n) {
      std::vector<int64> ans = pmod::SquareRootMod(n, p);
      for (int64 x : ans) {
        assert(x * x % p == n);
      }
#if ENABLE_FLINT && GMP_LIMB_BITS == 64
      {
        std::vector<int64> ans = flint::SquareRootMod(n, p);
        for (int64 x : ans) {
          assert(x * x % p == n);
        }
      }
#endif
      cnt += !std::empty(ans);
    }
    if (p > 2) {
      assert(cnt * 2 == p + 1);
    } else {
      assert(cnt == 2);
    }
  }
}

PE_REGISTER_TEST(&SquareRootModTest, "SquareRootModTest", SMALL);

#if PE_HAS_INT128
SL void TestTwoSquaresImpl(int64 n, int64 expected_count) {
  int64 actual_count = 0;
  std::vector<std::pair<int64, int64>> f = TwoSquares(n);
  for (std::pair<int64, int64>& iter : f) {
    if (iter.first == 0) {
      actual_count += 4;
    } else {
      actual_count += iter.first == iter.second ? 4 : 8;
    }
  }
  assert(expected_count == actual_count);
  for (std::pair<int64, int64>& iter : f) {
    assert(sq(iter.first) + sq(iter.second) == n);
  }
}

SL void TwoSquaresTest() {
  auto num_solutions = [=](int64 n) -> int64 {
    int64 ret = 1;
    for (std::pair<int64, int>& iter : Factorize(n)) {
      int mod4 = iter.first & 3;
      if (mod4 == 3) {
        if (IsOdd(iter.second)) return 0;
      } else if (mod4 == 1) {
        ret *= iter.second + 1;
      }
    }
    return ret * 4;
  };
  for (int64 offset : {int64(0), int64(1000000000000)}) {
    for (int64 n = 2; n <= 2000; ++n) {
      TestTwoSquaresImpl(offset + n, num_solutions(offset + n));
    }
  }
}

PE_REGISTER_TEST(&TwoSquaresTest, "TwoSquaresTest", SMALL);
#endif

SL void BaseKConversionTest() {
  for (int64 n = 0; n <= 100; ++n) {
    for (int k = -200; k <= 200; ++k) {
      if (Abs(k) >= 2) {
        assert(FromBaseK<int64>(ToBaseK(n, k), k) == n);
      }
    }
  }
  for (int64 n = 0; n <= 1000000; n += n / 100 + 1) {
    for (int k = -16; k <= 16; ++k) {
      if (Abs(k) >= 2) {
        assert(FromBaseK<int64>(ToBaseK(n, k), k) == n);
      }
    }
  }
  // Negative n only works with a negative base.
  for (int64 n = -100; n < 0; ++n) {
    for (int k = -200; k <= -2; ++k) {
      assert(FromBaseK<int64>(ToBaseK(n, k), k) == n);
    }
  }
}
PE_REGISTER_TEST(&BaseKConversionTest, "BaseKConversionTest", SMALL);

SL void CountCoprimeTest() {
  const int64 mod = 17;
  using MT = NModCC64<mod>;
  for (int64 a = 1; a <= 100; ++a) {
    IntegerFactorization f = Factorize(a);
    std::vector<std::pair<int64, int>> rm = GetRadFactorsWithMu(a);
    for (int64 n = 1; n <= 1000; n *= 10) {
      int64 ans0 = 0;
      for (int64 i = 1; i <= n; ++i) {
        if (Gcd(i, a) == 1) {
          ++ans0;
        }
      }
      assert(CountCoprime(n, a) == ans0);
      assert(CountCoprime(n, f) == ans0);
      assert(CountCoprime(n, rm) == ans0);
      assert(CountCoprime<MT>(n, a).value() == ans0 % mod);
      assert(CountCoprime<MT>(n, f).value() == ans0 % mod);
      assert(CountCoprime<MT>(n, rm).value() == ans0 % mod);

      for (int64 remain = 0; remain < 5; ++remain) {
        int64 ans1 = 0;
        for (int64 i = 1; i <= n; ++i) {
          if (Gcd(i, a) == 1 && i % 5 == remain) {
            ++ans1;
          }
        }
        assert(CountCoprime(n, a, remain, 5) == ans1);
        assert(CountCoprime(n, f, remain, 5) == ans1);
        assert(CountCoprime(n, rm, remain, 5) == ans1);
        assert(CountCoprime<MT>(n, a, remain, 5).value() == ans1 % mod);
        assert(CountCoprime<MT>(n, f, remain, 5).value() == ans1 % mod);
        assert(CountCoprime<MT>(n, rm, remain, 5).value() == ans1 % mod);
      }
    }
  }
}
PE_REGISTER_TEST(&CountCoprimeTest, "CountCoprimeTest", SMALL);

SL void SumCoprimeTest() {
  const int64 mod = 17;
  using MT = NModCC64<mod>;
  for (int64 a = 1; a <= 100; ++a) {
    IntegerFactorization f = Factorize(a);
    std::vector<std::pair<int64, int>> rm = GetRadFactorsWithMu(a);
    for (int64 n = 1; n <= 1000; n *= 10) {
      int64 ans0 = 0;
      for (int64 i = 1; i <= n; ++i) {
        if (Gcd(i, a) == 1) {
          ans0 += i;
        }
      }
      assert(SumCoprime(n, a) == ans0);
      assert(SumCoprime(n, f) == ans0);
      assert(SumCoprime(n, rm) == ans0);
      assert(SumCoprime<MT>(n, a).value() == ans0 % mod);
      assert(SumCoprime<MT>(n, f).value() == ans0 % mod);
      assert(SumCoprime<MT>(n, rm).value() == ans0 % mod);

      for (int64 remain = 0; remain < 5; ++remain) {
        int64 ans1 = 0;
        for (int64 i = 1; i <= n; ++i) {
          if (Gcd(i, a) == 1 && i % 5 == remain) {
            ans1 += i;
          }
        }
        assert(SumCoprime(n, a, remain, 5) == ans1);
        assert(SumCoprime(n, f, remain, 5) == ans1);
        assert(SumCoprime(n, rm, remain, 5) == ans1);
        assert(SumCoprime<MT>(n, a, remain, 5).value() == ans1 % mod);
        assert(SumCoprime<MT>(n, f, remain, 5).value() == ans1 % mod);
        assert(SumCoprime<MT>(n, rm, remain, 5).value() == ans1 % mod);
      }
    }
  }
}
PE_REGISTER_TEST(&SumCoprimeTest, "SumCoprimeTest", SMALL);

SL void LcmTest() {
  // Mixed types
  assert(Lcm(4, 6LL) == 12LL);

  // Variadic (3 arguments)
  assert(Lcm((int64)4, (int64)6, (int64)10) == 60);

  // initializer_list
  assert(Lcm({(int64)4, (int64)6, (int64)10}) == 60);

  // vector
  std::vector<int64> v = {4, 6, 10};
  assert(Lcm(v) == 60);

  // Property: Lcm(a,b) * Gcd(a,b) == a * b
  for (int a = 1; a <= 20; ++a) {
    for (int b = 1; b <= 20; ++b) {
      assert(Lcm((int64)a, (int64)b) * Gcd((int64)a, (int64)b) ==
             (int64)a * b);
    }
  }
}

PE_REGISTER_TEST(&LcmTest, "LcmTest", SMALL);

SL void ExGcdBuiltinTest() {
  // Verify the Bezout identity of all three forms for small inputs
  for (int64 a = 1; a <= 20; ++a) {
    for (int64 b = 1; b <= 20; ++b) {
      int64 x, y;
      const int64 d = ExGcd(a, b, x, y);
      assert(d == Gcd(a, b));
      assert(a * x + b * y == d);

      auto [d1, x1, y1] = ExGcd(a, b);
      assert(d1 == d && a * x1 + b * y1 == d);

      int64 x2;
      assert(ExGcd(a, b, x2) == d);
      assert(x2 == x);
    }
  }

  // Negative operands
  {
    int64 x, y;
    int64 d = ExGcd((int64)-24, (int64)36, x, y);
    assert(Abs(d) == 12);
    assert(-24 * x + 36 * y == d);
  }
}

PE_REGISTER_TEST(&ExGcdBuiltinTest, "ExGcdBuiltinTest", SMALL);

SL void SolveLinearEquationBuiltinTest() {
  // Brute-force check for all small moduli
  for (int64 m = 1; m <= 15; ++m) {
    for (int64 a = 0; a < m; ++a) {
      for (int64 b = 0; b < m; ++b) {
        auto ans = SolveLinearEquation(a, b, m);
        int64 first_sol = -1;
        for (int64 x = 0; x < m; ++x) {
          if ((a * x % m + m) % m == b) {
            first_sol = x;
            break;
          }
        }
        if (first_sol < 0) {
          assert(!ans.ok());
        } else {
          // Solutions are periodic with period m / gcd(a, m).
          assert(ans.ok());
          assert(ans.value == first_sol);
          assert(ans.mod == m / Gcd(a, m));
        }
      }
    }
  }
}

PE_REGISTER_TEST(&SolveLinearEquationBuiltinTest,
                 "SolveLinearEquationBuiltinTest", SMALL);

SL void Crt2CrtNBuiltinTest() {
  // CrtN: x = 1 (mod 2), x = 2 (mod 3), x = 3 (mod 5) -> x = 23 (mod 30)
  {
    int64 val[] = {1, 2, 3};
    int64 mod[] = {2, 3, 5};
    auto ans = CrtN<int64>(val, mod, 3);
    assert(ans.ok());
    assert(ans.value == 23 && ans.mod == 30);
  }

  // CrtN vector overload
  {
    std::vector<int64> val = {1, 2, 3};
    std::vector<int64> mod = {2, 3, 5};
    auto ans = CrtN(val, mod);
    assert(ans.ok() && ans.value == 23);
  }

  // Brute-force correctness for small moduli
  for (int64 m1 = 1; m1 <= 8; ++m1) {
    for (int64 m2 = 1; m2 <= 8; ++m2) {
      for (int64 a = 0; a < m1; ++a) {
        for (int64 b = 0; b < m2; ++b) {
          auto ans = Crt2(a, m1, b, m2);
          int64 found = -1;
          for (int64 x = 0; x < m1 * m2; ++x) {
            if (x % m1 == a && x % m2 == b) {
              found = x;
              break;
            }
          }
          if (found < 0) {
            assert(!ans.ok());
          } else {
            assert(ans.ok());
            assert(ans.value == found && ans.mod == Lcm(m1, m2));
          }
        }
      }
    }
  }
}

PE_REGISTER_TEST(&Crt2CrtNBuiltinTest, "Crt2CrtNBuiltinTest", SMALL);

SL void ArithFuncTest() {
  auto bf_phi = [](int64 n) -> int64 {
    int64 r = n, tmp = n;
    for (int64 p = 2; p * p <= tmp; ++p) {
      if (tmp % p == 0) {
        r -= r / p;
        while (tmp % p == 0) tmp /= p;
      }
    }
    if (tmp > 1) r -= r / tmp;
    return r;
  };

  auto bf_mu = [](int64 n) -> int64 {
    int k = 0;
    for (int64 p = 2; p * p <= n; ++p) {
      if (n % p == 0) {
        ++k;
        n /= p;
        if (n % p == 0) return 0;
      }
    }
    if (n > 1) ++k;
    return (k & 1) ? -1 : 1;
  };

  auto bf_sigma0 = [](int64 n) -> int64 {
    int64 cnt = 0;
    for (int64 d = 1; d * d <= n; ++d) {
      if (n % d == 0) {
        ++cnt;
        if (d != n / d) ++cnt;
      }
    }
    return cnt;
  };

  auto bf_sigma1 = [](int64 n) -> int64 {
    int64 s = 0;
    for (int64 d = 1; d * d <= n; ++d) {
      if (n % d == 0) {
        s += d;
        if (d != n / d) s += n / d;
      }
    }
    return s;
  };

  auto bf_rad = [](int64 n) -> int64 {
    int64 r = 1, tmp = n;
    for (int64 p = 2; p * p <= tmp; ++p) {
      if (tmp % p == 0) {
        r *= p;
        while (tmp % p == 0) tmp /= p;
      }
    }
    if (tmp > 1) r *= tmp;
    return r;
  };

  for (int64 n = 1; n <= 1000; ++n) {
    assert(CalPhi(n) == bf_phi(n));
    assert(CalMu(n) == bf_mu(n));
    assert(CalSigma0(n) == bf_sigma0(n));
    assert(CalSigma1(n) == bf_sigma1(n));
    assert(CalRad(n) == bf_rad(n));
  }
}

PE_REGISTER_TEST(&ArithFuncTest, "ArithFuncTest", SMALL);

SL void ExtractFactorInvOfTest() {
  // ExtractFactor(A, B): returns {A / B^k, k} for the largest k with B^k | A
  {
    auto [q, k] = ExtractFactor(int64(12), int64(2));
    assert(q == 3 && k == 2);
  }
  {
    auto [q, k] = ExtractFactor(int64(1), int64(7));
    assert(q == 1 && k == 0);
  }
  {
    int64 n = 7 * 7 * 7;
    auto [q, k] = ExtractFactor(n, int64(7));
    assert(q == 1 && k == 3);
  }
  {
    auto [q, k] = ExtractFactor(int64(8 * 25), int64(2));
    assert(q == 25 && k == 3);
  }

  // InvOf(x, p) = x^(p-2) mod p (Fermat's little theorem)
  for (int i = 0; i < 20; ++i) {
    int64 p = plist[i];
    for (int64 x = 1; x < std::min(p, int64(10)); ++x) {
      assert(x * InvOf(x, p) % p == 1);
    }
  }
}

PE_REGISTER_TEST(&ExtractFactorInvOfTest, "ExtractFactorInvOfTest", SMALL);

SL void CountSumMultipleModValueTest() {
  assert(CountMultiple(int64(0), int64(5)) == 0);
  assert(SumMultiple(int64(0), int64(5)) == 0);

  // Verified by brute force; r == 0 also checks CountMultiple / SumMultiple
  for (int64 n = 1; n <= 50; ++n) {
    for (int64 m = 1; m <= 10; ++m) {
      for (int64 r = 0; r < m; ++r) {
        int64 ecnt = 0, esum = 0;
        for (int64 x = 1; x <= n; ++x) {
          if (x % m == r) {
            ++ecnt;
            esum += x;
          }
        }
        assert(CountModValue(n, r, m) == ecnt);
        assert(SumModValue(n, r, m) == esum);
        if (r == 0) {
          assert(CountMultiple(n, m) == ecnt);
          assert(SumMultiple(n, m) == esum);
        }
      }
    }
  }
}

PE_REGISTER_TEST(&CountSumMultipleModValueTest, "CountSumMultipleModValueTest",
                 SMALL);

SL void IsPrimeTest() {
  auto bf_is_prime = [](int64 n) -> int {
    if (n <= 1) return 0;
    for (int64 d = 2; d * d <= n; ++d) {
      if (n % d == 0) return 0;
    }
    return 1;
  };
  // Below maxp (pmask), just above maxp (trial division / Miller-Rabin) and
  // near 1e12.
  for (int64 start : {int64(-10), maxp - 1000, int64(1000000000000)}) {
    for (int64 n = start; n <= start + 500; ++n) {
      const int expected = bf_is_prime(n);
      assert(IsPrime(n) == expected);
      assert(IsPrimeEx(n) == expected);
    }
  }

  // Strong pseudoprimes for the Miller-Rabin bases (OEIS A014233).
  for (int64 n : {int64(2047), int64(1373653), int64(25326001),
                  int64(3215031751), int64(2152302898747),
                  int64(3474749660383), int64(341550071728321),
                  int64(3825123056546413051)}) {
    assert(IsPrimeEx(n) == 0);
  }
  assert(MrTest(2047, 2) == 1);
  assert(MrTest(2047, 3) == 0);

  assert(IsPrimeEx(1000000007) == 1);
  assert(IsPrimeEx(998244353) == 1);
  assert(IsPrimeEx(1000000000000000003) == 1);
  assert(IsPrimeEx(2305843009213693951) == 1);  // 2^61 - 1
  assert(IsPrimeEx(int64(1000000007) * 998244353) == 0);
  assert(IsPrimeEx(int64(3037000493) * 3037000493) == 0);
}

PE_REGISTER_TEST(&IsPrimeTest, "IsPrimeTest", SMALL);

SL void FactorizationUtilTest() {
  for (int64 start : {int64(1), int64(1000000000000)}) {
    for (int64 n = start; n <= start + 300; ++n) {
      const IntegerFactorization f = Factorize(n);
      assert(f.GetValue<int64>() == n);

      // The hint only changes the search order, not the result.
      const IntegerFactorization fh = Factorize(n, {6, 35});
      assert(fh.ToMap() == f.ToMap());

      const IntegerFactorization fp = FactorizePower(n, 3);
      assert(std::size(fp) == std::size(f));
      for (int i = 0; i < static_cast<int>(std::size(f)); ++i) {
        assert(fp[i].first == f[i].first && fp[i].second == f[i].second * 3);
      }

      int big_omega = 0;
      std::vector<int64> primes;
      for (auto [p, e] : f) {
        assert(IsPrimeEx(p));
        big_omega += e;
        primes.push_back(p);
      }
      assert(CalSmallOmega(n) == static_cast<int>(std::size(f)));
      assert(CalBigOmega(n) == big_omega);
      assert(GetPrimeFactors(n) == primes);

      if (start == 1) {
        std::vector<int64> rad = GetRadFactors(n);
        std::sort(std::begin(rad), std::end(rad));
        std::vector<int64> expected;
        for (int64 d = 1; d <= n; ++d) {
          if (n % d == 0 && CalMu(d) != 0) expected.push_back(d);
        }
        assert(rad == expected);
      }
    }
  }

  // operator*
  for (int64 a = 1; a <= 60; ++a) {
    for (int64 b = 1; b <= 60; ++b) {
      const auto expected = Factorize(a * b).ToMap();
      assert((Factorize(a) * Factorize(b)).ToMap() == expected);
      assert((Factorize(a) * b).ToMap() == expected);
      assert((a * Factorize(b)).ToMap() == expected);
      IntegerFactorization f(a);
      f *= b;
      assert(f.ToMap() == expected);
    }
  }
}

PE_REGISTER_TEST(&FactorizationUtilTest, "FactorizationUtilTest", SMALL);

SL void ModInvTest() {
  for (int64 m = 2; m <= 50; ++m) {
    for (int64 a = 1; a < m; ++a) {
      if (Gcd(a, m) != 1) continue;
      const int64 inv = ModInv(a, m);
      assert(0 <= inv && inv < m);
      assert(a * inv % m == 1);
    }
  }
  assert(ModInv(int64(2), int64(1000000007)) == 500000004);
}

PE_REGISTER_TEST(&ModInvTest, "ModInvTest", SMALL);

SL void PrimitiveRootIndTest() {
  assert(FindPrimitiveRoot(2) == 1);
  for (int i = 1; i < 30; ++i) {
    const int64 p = plist[i];
    const int64 g = FindPrimitiveRoot(p);
    assert(g == FindPrimitiveRoot(p, Factorize(p - 1)));
    assert(CalModOrder<int64>(g, p) == p - 1);

    int64 cnt = 0;
    for (int64 x = 1; x < p; ++x) {
      const bool is_root = IsPrimitiveRoot(x, p);
      assert(is_root == (CalModOrder<int64>(x, p) == p - 1));
      cnt += is_root;
    }
    assert(cnt == CalPhi(p - 1));

    IndSolver sv(p);
    assert(sv.pr() == g);
    for (int64 a = 1; a < p; ++a) {
      const int64 k = Ind(a, g, p);
      assert(0 <= k && k < p - 1);
      assert(PowerMod(g, k, p) == a);
      assert(sv(a) == k);
    }
  }

  {
    const int64 p = 1000000007;
    assert(FindPrimitiveRoot(p) == 5);
    IndSolver sv(p, 5);
    for (int64 k : {int64(0), int64(1), int64(12345), p - 2}) {
      const int64 a = PowerMod(int64(5), k, p);
      assert(Ind(a, 5, p) == k);
      assert(sv(a) == k);
    }
  }
}

PE_REGISTER_TEST(&PrimitiveRootIndTest, "PrimitiveRootIndTest", SMALL);

SL void RootModTest() {
  for (int i = 0; i < 25; ++i) {
    const int64 p = plist[i];
    for (int64 n = 1; n <= 6; ++n) {
      for (int64 a = 0; a < p; ++a) {
        std::vector<int64> expected;
        for (int64 x = 0; x < p; ++x) {
          if (PowerMod(x, n, p) == a) expected.push_back(x);
        }
        std::vector<int64> ans = pmod::RootMod(a, n, p);
        std::sort(std::begin(ans), std::end(ans));
        assert(ans == expected);
      }
    }
  }
  // x^4 = 1 (mod 13) has 4 roots, only 2 are requested.
  assert(std::size(pmod::RootMod(1, 4, 13, 2)) == 2);
  // a is reduced first.
  assert(pmod::RootMod(-12, 2, 13) == pmod::RootMod(1, 2, 13));
  assert(pmod::RootMod(26, 3, 13) == std::vector<int64>{0});
}

PE_REGISTER_TEST(&RootModTest, "RootModTest", SMALL);

#if PE_HAS_INT128
SL void TwoSquaresFullTest() {
  for (int64 n = 0; n <= 2000; ++n) {
    std::vector<std::pair<int64, int64>> expected;
    for (int64 x = -SqrtI(n); x * x <= n; ++x) {
      for (int64 y = -SqrtI(n); y * y <= n; ++y) {
        if (x * x + y * y == n) expected.emplace_back(x, y);
      }
    }
    std::vector<std::pair<int64, int64>> ans = TwoSquaresFull(n);
    std::sort(std::begin(ans), std::end(ans));
    assert(ans == expected);
  }
}

PE_REGISTER_TEST(&TwoSquaresFullTest, "TwoSquaresFullTest", SMALL);
#endif

SL void SieveTest() {
  // end must not exceed maxp^2.
  for (int64 start : {int64(0), int64(1000000000000)}) {
    const int64 end = start + 1000;
    auto [rest, factorization] = Sieve(start, end);
    assert(static_cast<int64>(std::size(factorization)) == end - start + 1);
    for (int64 i = start; i <= end; ++i) {
      std::vector<std::pair<int64, int>> expected = Factorize(i);
      assert(factorization[i - start] == expected);
    }
  }
}

PE_REGISTER_TEST(&SieveTest, "SieveTest", SMALL);

SL void GetPrimesInRangeTest() {
  for (int64 start : {int64(-5), int64(0), int64(2), int64(3), maxp - 100,
                      int64(1000000000000)}) {
    for (int64 len : {int64(0), int64(1), int64(1000)}) {
      const int64 end = start + len;
      std::vector<int64> expected;
      for (int64 n = start; n <= end; ++n) {
        if (IsPrime(n)) expected.push_back(n);
      }
      assert(GetPrimesInRange(start, end) == expected);
      assert(GetPrimesInRangePe(start, end) == expected);

      // A non-positive end means an unbounded enumeration.
      if (end >= 2) {
        std::vector<int64> enumerated;
        for (int64 p : PrimeEnumeratorPe<int64>(start, end)) {
          enumerated.push_back(p);
        }
        assert(enumerated == expected);
      }
    }
  }
  assert(std::empty(GetPrimesInRange(10, 5)));
}

PE_REGISTER_TEST(&GetPrimesInRangeTest, "GetPrimesInRangeTest", SMALL);
}  // namespace nt_test
