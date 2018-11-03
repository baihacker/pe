#include "pe_test.h"

namespace misc_test {
SL void misc_test() {
  GaussianEliminationSolver solver;
  solver.init(10, 10);
  for (int i = 0; i < 10; ++i) {
    solver.at(i, 10) = 10 - i;
    for (int j = i; j < 10; ++j) {
      solver.at(i, j) = 1;
    }
  }
  auto v = solver.solve();
  for (int i = 0; i < 10; ++i) {
    assert(fabs(v[i] - 1) < 1e-10);
  }

  auto vtos = [=](const vector<int64>& vec) {
    stringstream ss;
    ss << vec;
    return ss.str();
  };

  vector<int64> vec;
  assert(vtos(vec) == "{}");

  vec.push_back(1);
  assert(vtos(vec) == "{1}");

  vec.push_back(2);
  assert(vtos(vec) == "{1, 2}");

  vec.push_back(3);
  assert(vtos(vec) == "{1, 2, 3}");
}

PE_REGISTER_TEST(&misc_test, "misc_test", SMALL);

SL void count_pt_in_circle_test() {
  for (int64 n = 0; n <= 100; ++n) {
    int64 u = count_pt_in_circle(n);
    int64 v = count_pt_in_circle_bf(n);
    int64 ans = 0;
    const int t = sqrti(n);
    for (int x = -t; x <= t; ++x)
      for (int y = -t; y <= t; ++y) ans += sq(x) + sq(y) <= n;
    assert(u == ans);
    assert(v == ans);
  }
#if 1
  for (int64 i = 1; i <= 10000; ++i) {
    int64 u = count_pt_in_circle_q1(i);
    int64 v = count_pt_in_circle_q1_bf(i);
    if (u != v) {
      cerr << i << " " << u << " " << v << endl;
    }
    assert(u == v);
  }
#endif
  // 9999999999999907 7853981733966909 7853981733966913
  for (int64 i = 10000; i <= 100000000000000000; i = i * 10) {
    for (int64 j = -3; j <= 3; ++j) {
      int64 target = i + j;
      int64 u = count_pt_in_circle_q1(target);
      int64 v = count_pt_in_circle_q1_bf(target);
      if (u != v) {
        cerr << target << " " << u << " " << v << endl;
      }
      assert(u == v);
    }
  }
}

PE_REGISTER_TEST(&count_pt_in_circle_test, "count_pt_in_circle_test", BIG);

#if PE_HAS_INT128
SL void sum_sigma0_test() {
#if 1
  for (int64 i = 1; i <= 10000; ++i) {
    int64 u = sum_sigma0(i);
    int64 v = sum_sigma0_bf(i);
    if (u != v) {
      cerr << i << " " << u << " " << v << endl;
    }
    assert(u == v);
  }
#endif

  // 9999999999999907 7853981733966909 7853981733966913
  for (int64 i = 10000; i <= 100000000000000000; i = i * 10) {
    for (int64 j = -3; j <= 3; ++j) {
      int64 target = i + j;
      int64 u = sum_sigma0(target);
      int64 v = sum_sigma0_bf(target);
      if (u != v) {
        cerr << target << " " << u << " " << v << endl;
      }
      assert(u == v);
    }
  }
}

PE_REGISTER_TEST(&sum_sigma0_test, "sum_sigma0_test", BIG);
#endif
}  // namespace misc_test
