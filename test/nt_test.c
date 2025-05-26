#include "pe_test.h"

namespace nt_test {
SL void ParityTest() {
  assert(IsEven(0) == 1);
  assert(IsEven(1) == 0);
  assert(IsEven(2) == 1);
  assert(IsEven(3) == 0);

  assert(IsOdd(0) == 0);
  assert(IsOdd(1) == 1);
  assert(IsOdd(2) == 0);
  assert(IsOdd(3) == 1);

  assert(SameParity(0, 0) == 1);
  assert(SameParity(0, 1) == 0);
  assert(SameParity(0, 2) == 1);
  assert(SameParity(0, 3) == 0);

  assert(SameParity(1, 0) == 0);
  assert(SameParity(1, 1) == 1);
  assert(SameParity(1, 2) == 0);
  assert(SameParity(1, 3) == 1);

  assert(SameParity(2, 0) == 1);
  assert(SameParity(2, 1) == 0);
  assert(SameParity(2, 2) == 1);
  assert(SameParity(2, 3) == 0);

  assert(SameParity(3, 0) == 0);
  assert(SameParity(3, 1) == 1);
  assert(SameParity(3, 2) == 0);
  assert(SameParity(3, 3) == 1);

  for (int i = -100; i <= 100; ++i) assert(IsEven(i) + IsOdd(i) == 1);
}
PE_REGISTER_TEST(&ParityTest, "ParityTest", SMALL);

SL void SqrtITest() {
  int f = 0;
  for (int64 i = 1; i <= 10000000; ++i) {
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
  std::vector<pe::int64> result = GetFactors(1);
  assert(result == std::vector<int64>{1});

  result = GetFactors(2);
  assert(result == std::vector<int64>({1, 2}));

  result = GetFactors(3);
  assert(result == std::vector<int64>({1, 3}));

  result = GetFactors(4);
  assert(result == std::vector<int64>({1, 2, 4}));

  result = GetFactors(12);
  std::sort(std::begin(result), std::end(result));
  assert(result == std::vector<int64>({1, 2, 3, 4, 6, 12}));

  for (int64 limit = -1; limit <= 20; ++limit) {
    std::vector<pe::int64> result = GetFactors(12, limit);
    std::sort(std::begin(result), std::end(result));
    std::vector<int64> expected;
    for (auto iter : {1, 2, 3, 4, 6, 12}) {
      if (limit < 0 || iter <= limit) {
        expected.push_back(iter);
      }
    }
    assert(result == expected);
  }
}

PE_REGISTER_TEST(&GetFactorsTest, "GetFactorsTest", SMALL);

SL int IsSquareFreeNormal(int64 n) {
  for (auto& iter : Factorize(n)) {
    if (iter.second > 1) {
      return 0;
    }
  }
  return 1;
}

SL void IsSquareFreeTest() {
  const int64 n = maxp * 2;
  int64 ans1 = 0;
  {
    TimeRecorder tr;
    for (int i = 1; i <= n; ++i) ans1 += IsSquareFree(i);
    // std::cerr << tr.Elapsed().Format() << std::endl;
  }
  int64 ans2 = 0;
  {
    TimeRecorder tr;
    for (int i = 1; i <= n; ++i) ans2 += IsSquareFreeNormal(i);
    // std::cerr << tr.Elapsed().Format() << std::endl;
  }
  assert(ans1 == ans2);
}

PE_REGISTER_TEST(&IsSquareFreeTest, "IsSquareFreeTest", SMALL);

SL void SquareRootModTest() {
  for (int i = 0; i < 1000; ++i) {
    const int64 p = plist[i];
    int cnt = 0;
    for (int n = 0; n < p; ++n) {
      std::vector<int64> ans = pmod::SquareRootMod(n, p);
      for (auto x : ans) {
        assert(x * x % p == n);
      }
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
SL void TestNTwoSquares(int64 n, int64 expected) {
  int64 real = 0;
  auto f = TwoSquares(n);
  for (auto& iter : f) {
    if (iter.first == 0) {
      real += 4;
    } else {
      real += iter.first == iter.second ? 4 : 8;
    }
  }
  assert(expected == real);
  for (auto& iter : f) assert(sq(iter.first) + sq(iter.second) == n);
}

SL void TwoSquaresTest() {
  auto num_solutions = [=](int64 n) -> int64 {
    int64 ret = 1;
    for (auto& iter : Factorize(n)) {
      int mod4 = iter.first & 3;
      if (mod4 == 3) {
        if (IsOdd(iter.second)) return 0;
      } else if (mod4 == 1) {
        ret *= iter.second + 1;
      }
    }
    return ret * 4;
  };
  int64 offset = 0;
  for (int64 n = 2; n <= 10000; ++n) {
    TestNTwoSquares(offset + n, num_solutions(offset + n));
  }
  // 1e12
  offset = 1000000000000;
  for (int64 n = 2; n <= 10000; ++n) {
    TestNTwoSquares(offset + n, num_solutions(offset + n));
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
  for (int64 n = 0; n <= 10000; ++n) {
    for (int k = -16; k <= 16; ++k) {
      if (Abs(k) >= 2) {
        assert(FromBaseK<int64>(ToBaseK(n, k), k) == n);
      }
    }
  }
  for (int64 n = -100; n <= 100; ++n) {
    for (int k = -200; k <= -2; ++k) {
      if (Abs(k) >= 2) {
        assert(FromBaseK<int64>(ToBaseK(n, k), k) == n);
      }
    }
  }
}
PE_REGISTER_TEST(&BaseKConversionTest, "BaseKConversionTest", SMALL);

SL void CountCoprimeTest() {
  const int64 mod = 17;
  using MT = NMod64<mod>;
  for (int64 a = 1; a <= 100; ++a) {
    auto f = Factorize(a);
    auto rm = GetRadFactorsWithMu(a);
    for (int64 n = 1; n <= 1000; n *= 10) {
      int64 ans0 = 0;
      int64 ans1 = CountCoprime(n, a);
      int64 ans2 = CountCoprime(n, f);
      int64 ans3 = CountCoprime(n, rm);
      for (int64 i = 1; i <= n; ++i) {
        if (Gcd(i, a) == 1) {
          ++ans0;
        }
      }
      assert(ans1 == ans0);
      assert(ans2 == ans0);
      assert(ans3 == ans0);
    }
  }

  for (int64 a = 1; a <= 100; ++a) {
    auto f = Factorize(a);
    auto rm = GetRadFactorsWithMu(a);
    for (int64 n = 1; n <= 1000; n *= 10) {
      int64 ans0 = 0;
      int64 ans1 = CountCoprime<MT>(n, a).value();
      int64 ans2 = CountCoprime<MT>(n, f).value();
      int64 ans3 = CountCoprime<MT>(n, rm).value();
      for (int64 i = 1; i <= n; ++i) {
        if (Gcd(i, a) == 1) {
          ++ans0;
        }
      }
      ans0 %= mod;
      assert(ans1 == ans0);
      assert(ans2 == ans0);
      assert(ans3 == ans0);
    }
  }

  for (int64 a = 1; a <= 100; ++a) {
    auto f = Factorize(a);
    auto rm = GetRadFactorsWithMu(a);
    for (int64 n = 1; n <= 1000; n *= 10) {
      for (int64 remain = 0; remain < 5; ++remain) {
        int64 ans0 = 0;
        int64 ans1 = CountCoprime(n, a, remain, 5);
        int64 ans2 = CountCoprime(n, f, remain, 5);
        int64 ans3 = CountCoprime(n, rm, remain, 5);
        for (int64 i = 1; i <= n; ++i) {
          if (Gcd(i, a) == 1 && i % 5 == remain) {
            ++ans0;
          }
        }
        assert(ans1 == ans0);
        assert(ans2 == ans0);
        assert(ans3 == ans0);
      }
    }
  }

  for (int64 a = 1; a <= 100; ++a) {
    auto f = Factorize(a);
    auto rm = GetRadFactorsWithMu(a);
    for (int64 n = 1; n <= 1000; n *= 10) {
      for (int64 remain = 0; remain < 5; ++remain) {
        int64 ans0 = 0;
        int64 ans1 = CountCoprime<MT>(n, a, remain, 5).value();
        int64 ans2 = CountCoprime<MT>(n, f, remain, 5).value();
        int64 ans3 = CountCoprime<MT>(n, rm, remain, 5).value();
        for (int64 i = 1; i <= n; ++i) {
          if (Gcd(i, a) == 1 && i % 5 == remain) {
            ++ans0;
          }
        }
        ans0 %= mod;
        assert(ans1 == ans0);
        assert(ans2 == ans0);
        assert(ans3 == ans0);
      }
    }
  }
}
PE_REGISTER_TEST(&CountCoprimeTest, "CountCoprimeTest", SMALL);

SL void SumCoprimeTest() {
  const int64 mod = 17;
  using MT = NMod64<mod>;
  for (int64 a = 1; a <= 100; ++a) {
    auto f = Factorize(a);
    auto rm = GetRadFactorsWithMu(a);
    for (int64 n = 1; n <= 1000; n *= 10) {
      int64 ans0 = 0;
      int64 ans1 = SumCoprime(n, a);
      int64 ans2 = SumCoprime(n, f);
      int64 ans3 = SumCoprime(n, rm);
      for (int64 i = 1; i <= n; ++i) {
        if (Gcd(i, a) == 1) {
          ans0 += i;
        }
      }
      assert(ans1 == ans0);
      assert(ans2 == ans0);
      assert(ans3 == ans0);
    }
  }

  for (int64 a = 1; a <= 100; ++a) {
    auto f = Factorize(a);
    auto rm = GetRadFactorsWithMu(a);
    for (int64 n = 1; n <= 1000; n *= 10) {
      int64 ans0 = 0;
      int64 ans1 = SumCoprime<MT>(n, a).value();
      int64 ans2 = SumCoprime<MT>(n, f).value();
      int64 ans3 = SumCoprime<MT>(n, rm).value();
      for (int64 i = 1; i <= n; ++i) {
        if (Gcd(i, a) == 1) {
          ans0 += i;
        }
      }
      ans0 %= mod;
      assert(ans1 == ans0);
      assert(ans2 == ans0);
      assert(ans3 == ans0);
    }
  }

  for (int64 a = 1; a <= 100; ++a) {
    auto f = Factorize(a);
    auto rm = GetRadFactorsWithMu(a);
    for (int64 n = 1; n <= 1000; n *= 10) {
      for (int64 remain = 0; remain < 5; ++remain) {
        int64 ans0 = 0;
        int64 ans1 = SumCoprime(n, a, remain, 5);
        int64 ans2 = SumCoprime(n, f, remain, 5);
        int64 ans3 = SumCoprime(n, rm, remain, 5);
        for (int64 i = 1; i <= n; ++i) {
          if (Gcd(i, a) == 1 && i % 5 == remain) {
            ans0 += i;
          }
        }
        assert(ans1 == ans0);
        assert(ans2 == ans0);
        assert(ans3 == ans0);
      }
    }
  }

  for (int64 a = 1; a <= 100; ++a) {
    auto f = Factorize(a);
    auto rm = GetRadFactorsWithMu(a);
    for (int64 n = 1; n <= 1000; n *= 10) {
      for (int64 remain = 0; remain < 5; ++remain) {
        int64 ans0 = 0;
        int64 ans1 = SumCoprime<MT>(n, a, remain, 5).value();
        int64 ans2 = SumCoprime<MT>(n, f, remain, 5).value();
        int64 ans3 = SumCoprime<MT>(n, rm, remain, 5).value();
        for (int64 i = 1; i <= n; ++i) {
          if (Gcd(i, a) == 1 && i % 5 == remain) {
            ans0 += i;
          }
        }
        ans0 %= mod;
        assert(ans1 == ans0);
        assert(ans2 == ans0);
        assert(ans3 == ans0);
      }
    }
  }
}
PE_REGISTER_TEST(&SumCoprimeTest, "SumCoprimeTest", SMALL);
}  // namespace nt_test
