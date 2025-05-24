#include "pe_test.h"

namespace fact_ppower_mod_test {
SL void FactPPowerModTest() {
  bi N = 1;
  for (int i = 1; i <= 20; ++i) N *= i;

  bi v = 0;
  for (bi n = N; !IsZero(n); n >>= 1, v += n) {
    ;
  }

  bi mod = Power(bi(2), 48);
  bi ans = 1;
  FactPPowerModer<bi> moder(2, 48);
  int i = 0;
  while (N > 1) {
    ans = ans * moder.Cal(N) % mod;
    N >>= 1;
  }

  ans = ans * (Power(bi(2), v[0] & 3)) % mod;

  assert(ans == 21171469991580LL);
}

PE_REGISTER_TEST(&FactPPowerModTest, "FactPPowerModTest", SMALL);

#if ENABLE_GMP
SL void MpIntegerFactPPowerModTest() {
  MpInteger N = 1;
  for (int i = 1; i <= 20; ++i) N *= i;

  MpInteger v = 0;
  for (MpInteger n = N; !IsZero(n); n >>= 1, v += n) {
    ;
  }

  MpInteger mod = Power(MpInteger(2), 48);
  MpInteger ans = 1;
  FactPPowerModer<MpInteger> moder(2, 48);
  int i = 0;
  while (N > 1) {
    ans = ans * moder.Cal(N) % mod;
    N >>= 1;
  }

  ans = ans * (Power(MpInteger(2), ToInt<int>(v % 4))) % mod;

  assert(ToInt<int64>(ans) == 21171469991580LL);
}

PE_REGISTER_TEST(&MpIntegerFactPPowerModTest, "MpIntegerFactPPowerModTest",
                 SMALL);
#endif
}  // namespace fact_ppower_mod_test