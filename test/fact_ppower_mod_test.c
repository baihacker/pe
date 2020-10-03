#include "pe_test.h"

namespace fact_ppower_mod_test {
SL void fact_ppower_mod_test() {
  bi N = 1;
  for (int i = 1; i <= 20; ++i) N *= i;

  bi v = 0;
  for (bi n = N; !IsZero(n); n >>= 1, v += n)
    ;

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

PE_REGISTER_TEST(&fact_ppower_mod_test, "fact_ppower_mod_test", SMALL);

#if HAS_MPZ && 0
SL void mpz_fact_ppower_mod_test() {
  Mpz N = 1;
  for (int i = 1; i <= 20; ++i) N *= i;

  Mpz v = 0;
  for (Mpz n = N; !IsZero(n); n >>= 1, v += n)
    ;

  Mpz mod = Power(Mpz(2), 48);
  Mpz ans = 1;
  FactPPowerModer<Mpz> moder(2, 48);
  int i = 0;
  while (N > 1) {
    ans = ans * moder.cal(N) % mod;
    N >>= 1;
  }

  ans = ans * (Power(Mpz(2), LowerBits(v) % 4)) % mod;

  assert(ToInt<int64>(ans) == 21171469991580LL);
}

PE_REGISTER_TEST(&mpz_fact_ppower_mod_test, "mpz_fact_ppower_mod_test", SMALL);
#endif

#if ENABLE_GMP
SL void mp_integer_fact_ppower_mod_test() {
  MpInteger N = 1;
  for (int i = 1; i <= 20; ++i) N *= i;

  MpInteger v = 0;
  for (MpInteger n = N; !IsZero(n); n >>= 1, v += n)
    ;

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

PE_REGISTER_TEST(&mp_integer_fact_ppower_mod_test,
                 "mp_integer_fact_ppower_mod_test", SMALL);
#endif
}  // namespace fact_ppower_mod_test