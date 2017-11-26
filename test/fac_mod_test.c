#include "pe_test.h"

namespace fac_mod_test {
SL void fac_mod_test()
{
  bn N = 1;
  for (int i = 1; i <= 20; ++i) N *= i;

  bn v = 0;
  for (bn n = N; !n.isZero(); n = n / 2, v += n);

  bn mod = power(bn(2), 48);
  bn ans = 1;
  FacModer moder(2, 48);
  int i = 0;
  while (N > 1) {
    ans = ans * moder.cal(N) % mod;
    N = N / 2;
  }
  v = v % 4;
  
  ans = ans * (power(bn(2), v[0])) % mod;

  assert(ans == 21171469991580LL);
}

PE_REGISTER_TEST(&fac_mod_test, "fac_mod_test", MEDIUM);
}