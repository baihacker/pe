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

SL void GcdTest() {
  assert(Gcd({2, 4, 6}) == 2);
  assert(Gcd(2, 4, 6) == 2);
  assert(Gcd(2L, 4LL, 6ULL) == 2);
}
PE_REGISTER_TEST(&GcdTest, "GcdTest", SMALL);

SL void GetFactorsTest() {
  auto result = GetFactors(1);
  assert(result == std::vector<int64>{1});

  result = GetFactors(2);
  assert(result == std::vector<int64>({1, 2}));

  result = GetFactors(3);
  assert(result == std::vector<int64>({1, 3}));

  result = GetFactors(4);
  assert(result == std::vector<int64>({1, 2, 4}));

  result = GetFactors(12);
  std::sort(all(result));
  assert(result == std::vector<int64>({1, 2, 3, 4, 6, 12}));

  for (int64 limit = -1; limit <= 20; ++limit) {
    auto result = GetFactors(12, limit);
    std::sort(all(result));
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
      std::vector<int64> ans = SquareRootMod(n, p);
      for (auto x : ans) {
        assert(x * x % p == n);
      }
      cnt += sz(ans) > 0;
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
}  // namespace nt_test
