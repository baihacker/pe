#include "pe_test.h"

namespace misc_test {
SL void MiscTest() {
  GaussianEliminationSolver solver;
  solver.Init(10, 10);
  for (int i = 0; i < 10; ++i) {
    solver.At(i, 10) = 10 - i;
    for (int j = i; j < 10; ++j) {
      solver.At(i, j) = 1;
    }
  }
  auto v = solver.Solve();
  for (int i = 0; i < 10; ++i) {
    assert(FAbs(v[i] - 1) < 1e-10);
  }

  auto vtos = [=](const std::vector<int64>& vec) {
    std::stringstream ss;
    ss << vec;
    return ss.str();
  };

  std::vector<int64> vec;
  assert(vtos(vec) == "{}");

  vec.push_back(1);
  assert(vtos(vec) == "{1}");

  vec.push_back(2);
  assert(vtos(vec) == "{1, 2}");

  vec.push_back(3);
  assert(vtos(vec) == "{1, 2, 3}");
}

PE_REGISTER_TEST(&MiscTest, "MiscTest", SMALL);

SL void CountPtInCircleTest() {
  for (int64 n = 0; n <= 100; ++n) {
    int64 u = CountPtInCircle(n);
    int64 v = CountPtInCircleBf(n);
    int64 ans = 0;
    const int t = (int)SqrtI(n);
    for (int x = -t; x <= t; ++x) {
      for (int y = -t; y <= t; ++y) ans += sq(x) + sq(y) <= n;
    }
    assert(u == ans);
    assert(v == ans);
  }
#if 1
  for (int64 i = 1; i <= 10000; ++i) {
    int64 u = CountPtInCircleQ1(i);
    int64 v = CountPtInCircleQ1Bf(i);
    if (u != v) {
      std::cerr << i << " " << u << " " << v << std::endl;
    }
    assert(u == v);
  }
#endif

#if !defined(CONTINUOUS_INTEGRATION_TEST)
  // 9999999999999907 7853981733966909 7853981733966913
  for (int64 i = 10000; i <= 100000000000000000; i = i * 10) {
    for (int64 j = -3; j <= 3; ++j) {
      int64 target = i + j;
      int64 u = CountPtInCircleQ1(target);
      int64 v = CountPtInCircleQ1Bf(target);
      if (u != v) {
        std::cerr << target << " " << u << " " << v << std::endl;
      }
      assert(u == v);
    }
  }
#endif
}

PE_REGISTER_TEST(&CountPtInCircleTest, "CountPtInCircleTest", MEDIUM);

#if PE_HAS_INT128
SL void SumSigma0Test() {
#if 1
  for (int64 i = 1; i <= 10000; ++i) {
    int64 u = SumSigma0(i);
    int64 v = SumSigma0Bf(i);
    auto w = min25::sigma0_sum_fast(i);
    if (u != v || v != w || u != w) {
      std::cerr << i << " " << u << " " << v << " " << w << std::endl;
    }
    assert(u == v);
    assert(u == w);
  }
#endif

  // 9999999999999907 7853981733966909 7853981733966913
  for (int64 i = 10000; i <= 100000000000000000; i = i * 10) {
    for (int64 j = -3; j <= 3; ++j) {
      int64 target = i + j;
      int64 u = SumSigma0(target);
      int64 v = SumSigma0Bf(target);
      auto w = min25::sigma0_sum_fast(target);
      if (u != v || v != w || u != w) {
        std::cerr << target << " " << u << " " << v << " " << w << std::endl;
      }
      assert(u == v);
      assert(u == w);
    }
  }
}

PE_REGISTER_TEST(&SumSigma0Test, "SumSigma0Test", SUPER);
#endif

SL int64 IntDivFloor(int64 a, int64 b) {
  if (b < 0) a = -a;
  if (a % b == 0) return a / b;
  if (a >= 0) return a / b;
  return a / b - 1;
}

SL void SolveInequatilityGE2Test() {
  for (int64 x1 = -1000; x1 <= 1000; ++x1) {
    for (int64 x2 = x1; x2 <= 1000; ++x2) {
      // (100 x-x1)(100 x-x2) >= 0
      // 10000 x^2-(100 x1 + 100 x2) x + x1 x2 >= 0
      const int64 A = 10000;
      const int64 B = -(100 * x1 + 100 * x2);
      const int64 C = x1 * x2;
      int64 u = IntDivFloor(x1, 100);
      int64 v = x2 % 100 == 0 ? IntDivFloor(x2, 100) : IntDivFloor(x2, 100) + 1;
      auto ans = SolveInequatilityGE2<int64>(A, B, C);
      if (u == v || u + 1 == v) {
        assert(std::size(ans) == 1);
        assert(ans[0].x1 == -IntegerRange64::inf);
        assert(ans[0].x2 == IntegerRange64::inf);
      } else {
        assert(std::size(ans) == 2);
        assert(ans[0].x1 == -IntegerRange64::inf);
        assert(ans[0].x2 == u);
        assert(ans[1].x1 == v);
        assert(ans[1].x2 == IntegerRange64::inf);
      }
    }
  }
}
PE_REGISTER_TEST(&SolveInequatilityGE2Test, "SolveInequatilityGE2Test", SMALL);

SL void SolveInequatilityG2Test() {
  for (int64 x1 = -1000; x1 <= 1000; ++x1) {
    for (int64 x2 = x1; x2 <= 1000; ++x2) {
      // (100 x-x1)(100 x-x2) > 0
      // 10000 x^2-(100 x1 + 100 x2) x + x1 x2 > 0
      const int64 A = 10000;
      const int64 B = -(100 * x1 + 100 * x2);
      const int64 C = x1 * x2;
      int64 u = x1 % 100 == 0 ? IntDivFloor(x1, 100) - 1 : IntDivFloor(x1, 100);
      int64 v = IntDivFloor(x2, 100) + 1;
      auto ans = SolveInequatilityG2<int64>(A, B, C);
      if (u == v || u + 1 == v) {
        assert(std::size(ans) == 1);
        assert(ans[0].x1 == -IntegerRange64::inf);
        assert(ans[0].x2 == IntegerRange64::inf);
      } else {
        assert(std::size(ans) == 2);
        assert(ans[0].x1 == -IntegerRange64::inf);
        assert(ans[0].x2 == u);
        assert(ans[1].x1 == v);
        assert(ans[1].x2 == IntegerRange64::inf);
      }
    }
  }
}
PE_REGISTER_TEST(&SolveInequatilityG2Test, "SolveInequatilityG2Test", SMALL);

SL void SolveInequatilityLE2Test() {
  for (int64 x1 = -1000; x1 <= 1000; ++x1) {
    for (int64 x2 = x1; x2 <= 1000; ++x2) {
      // (100 x-x1)(100 x-x2) <= 0
      // 10000 x^2-(100 x1 + 100 x2) x + x1 x2 <= 0
      const int64 A = 10000;
      const int64 B = -(100 * x1 + 100 * x2);
      const int64 C = x1 * x2;
      int64 u = x1 % 100 == 0 ? IntDivFloor(x1, 100) : IntDivFloor(x1, 100) + 1;
      int64 v = IntDivFloor(x2, 100);
      auto ans = SolveInequatilityLE2<int64>(A, B, C);
      if (u > v) {
        assert(std::size(ans) == 0);
      } else {
        assert(std::size(ans) == 1);
        assert(ans[0].x1 == u);
        assert(ans[0].x2 == v);
      }
    }
  }
}
PE_REGISTER_TEST(&SolveInequatilityLE2Test, "SolveInequatilityLE2Test", SMALL);

SL void SolveInequatilityL2Test() {
  for (int64 x1 = -1000; x1 <= 1000; ++x1) {
    for (int64 x2 = x1; x2 <= 1000; ++x2) {
      // (100 x-x1)(100 x-x2) < 0
      // 10000 x^2-(100 x1 + 100 x2) x + x1 x2 < 0
      const int64 A = 10000;
      const int64 B = -(100 * x1 + 100 * x2);
      const int64 C = x1 * x2;
      int64 u = IntDivFloor(x1, 100) + 1;
      int64 v = x2 % 100 == 0 ? IntDivFloor(x2, 100) - 1 : IntDivFloor(x2, 100);
      auto ans = SolveInequatilityL2<int64>(A, B, C);
      if (u > v) {
        assert(std::size(ans) == 0);
      } else {
        assert(std::size(ans) == 1);
        assert(ans[0].x1 == u);
        assert(ans[0].x2 == v);
      }
    }
  }
}
PE_REGISTER_TEST(&SolveInequatilityL2Test, "SolveInequatilityL2Test", SMALL);
}  // namespace misc_test
