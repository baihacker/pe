#include "pe_test.h"

namespace fact_ppower_mod_test {
SL void fact_ppower_mod_test() {
  bi N = 1;
  for (int i = 1; i <= 20; ++i) N *= i;

  bi v = 0;
  for (bi n = N; !n.isZero(); n >>= 1, v += n)
    ;

  bi mod = power(bi(2), 48);
  bi ans = 1;
  FactPPowerModer moder(2, 48);
  int i = 0;
  while (N > 1) {
    ans = ans * moder.cal(N) % mod;
    N >>= 1;
  }

  ans = ans * (power(bi(2), v[0] & 3)) % mod;

  assert(ans == 21171469991580LL);
}

PE_REGISTER_TEST(&fact_ppower_mod_test, "fact_ppower_mod_test", MEDIUM);
}  // namespace fact_ppower_mod_test