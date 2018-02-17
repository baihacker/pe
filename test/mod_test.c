#include "pe_test.h"

namespace mod_test {
#if PE_HAS_INT128
SL void mod_test() {
  const int mod = 1000000007;
  for (int64 n = 1; n <= 10; ++n) {
    auto v = frac_mod<int64>({n, n+1, 2*n+1}, {2, 3}, mod);
    auto expected = (int128)n*(n+1)*(2*n+1)/6%mod;
    assert(v == expected);
  }

  for (int i = 1; i <= 10; ++i) {
    const int64 n = 100000000000 + i;
    auto v = frac_mod<int64>({n, n+1, 2*n+1}, {2, 3}, mod);
    auto expected = (int128)n*(n+1)*(2*n+1)/6%mod;
    assert(v == expected);
  }
}

PE_REGISTER_TEST(&mod_test, "mod_test", SMALL);
#endif
}
