#include "pe_test.h"

namespace init_inv_test {
const int64 mod = 1000000007;

SL void InitInvTest() {
  const int n = 1000000;
  std::vector<int> sresult(n + 1);
  std::vector<int64> lresult(n + 1);
  InitInverse(&sresult[0], n, mod);
  InitInverse(&lresult[0], n, mod);
  for (int i = 1; i <= n; ++i) {
    assert((int64)i * sresult[i] % mod == 1);
    assert(sresult[i] == lresult[i]);
  }
}

PE_REGISTER_TEST(&InitInvTest, "InitInvTest", SMALL);
}  // namespace init_inv_test
