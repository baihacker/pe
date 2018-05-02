#include "pe_test.h"

namespace comb_moder_test {
SL void comb_moder_test() {
  const int mod = 10007;
  CombModer m1(mod);
  CombModerEx m2(mod);
  for (int i = 0; i < 10000; ++i) {
    const int n = rand() % mod;
    const int m = rand() % mod;
    assert(m1.comb(n, m) == m2.comb(n, m));
  }
}
PE_REGISTER_TEST(&comb_moder_test, "comb_moder_test", MEDIUM);
}