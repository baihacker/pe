#include "pe_test.h"

namespace init_inv_test {
constexpr int64 mod = 1000000007;

SL void InitInvTest() {
  constexpr int n = 1000000;
  std::vector<int> sresult(n + 1);
  std::vector<int64> lresult(n + 1);
  InitInverse(std::data(sresult), n, mod);
  InitInverse(std::data(lresult), n, mod);
  for (int i = 1; i <= n; ++i) {
    assert((int64)i * sresult[i] % mod == 1);
    assert(sresult[i] == lresult[i]);
  }
}

PE_REGISTER_TEST(&InitInvTest, "InitInvTest", SMALL);
}  // namespace init_inv_test
