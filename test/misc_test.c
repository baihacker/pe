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
    assert(fabs(v[i]-1) < 1e-10);
  }
}

PE_REGISTER_TEST(&misc_test, "misc_test", SMALL);

SL void count_pt_in_circle_test() {
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

PE_REGISTER_TEST(&count_pt_in_circle_test, "count_pt_in_circle_test", MEDIUM);

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
}
