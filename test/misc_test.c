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
}

PE_REGISTER_TEST(&CountPtInCircleTest, "CountPtInCircleTest", SUPER);

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
}  // namespace misc_test
