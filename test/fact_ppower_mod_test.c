#include "pe_test.h"

namespace fact_ppower_mod_test {
SL void FactPPowerModTest_BigInteger() {
  BigInteger N = 1;
  for (int i = 1; i <= 20; ++i) N *= i;

  BigInteger v = 0;
  for (BigInteger n = N; !IsZero(n); n >>= 1, v += n) {
    ;
  }

  BigInteger mod = Power(BigInteger(2), 48);
  BigInteger ans = 1;
  FactPPowerModer<BigInteger> moder(2, 48);
  int i = 0;
  while (N > 1) {
    ans = ans * moder.Cal(N) % mod;
    N >>= 1;
  }

  ans = ans * (Power(BigInteger(2), v[0] & 3)) % mod;

  assert(ans == 21171469991580LL);
}

PE_REGISTER_TEST(&FactPPowerModTest_BigInteger, "FactPPowerModTest_BigInteger",
                 SMALL);

#if ENABLE_GMP
SL void FactPPowerModTest_MpInteger() {
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

PE_REGISTER_TEST(&FactPPowerModTest_MpInteger, "FactPPowerModTest_MpInteger",
                 SMALL);
#endif
}  // namespace fact_ppower_mod_test