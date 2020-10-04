#include "pe_test.h"

namespace init_inv_test {
const int64 mod = 1000000007;

const int n = 1000000;
int sresult[n + 1];
int64 lresult[n + 1];

SL void InitInvTest() {
  InitInv(sresult, n, mod);
  InitInv(lresult, n, mod);
  for (int i = 1; i <= n; ++i) {
    assert((int64)i * sresult[i] % mod == 1);
    assert(sresult[i] == lresult[i]);
  }
}

PE_REGISTER_TEST(&InitInvTest, "InitInvTest", SMALL);
}  // namespace init_inv_test
