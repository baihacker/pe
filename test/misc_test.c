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
}
