#include "pe_test.h"

namespace fact_ppower_mod_test {
SL void fact_ppower_mod_test()
{
  bn N = 1;
  for (int i = 1; i <= 20; ++i) N *= i;

  bn v = 0;
  for (bn n = N; !n.isZero(); n >>= 1, v += n);

  bn mod = power(bn(2), 48);
  bn ans = 1;
  FactPPowerModer moder(2, 48);
  int i = 0;
  while (N > 1) {
    ans = ans * moder.cal(N) % mod;
    N >>= 1;
  }
  
  ans = ans * (power(bn(2), v[0]&3)) % mod;

  assert(ans == 21171469991580LL);
}

PE_REGISTER_TEST(&fact_ppower_mod_test, "fact_ppower_mod_test", MEDIUM);
}