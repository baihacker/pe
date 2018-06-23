#include "pe_test.h"

namespace nt_test {
SL void nt_test() {
  const int mod = 1000000007;
  MiuPhiSumModer moder(mod);
  moder.init();
  
  int64 s0 = 0;
  for (int i = 1; i <= 10000000; ++i)
    s0 += cal_phi(i) % mod;
  s0 %= mod;
  
  int64 s1 = moder.get_sum_phi(10000000);
  assert(s0 == s1);
}

PE_REGISTER_TEST(&nt_test, "nt_test", MEDIUM);
}
