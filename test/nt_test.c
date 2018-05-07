#include "pe_test.h"

namespace nt_test {
SL void nt_test() {
  const int mod = 1000000007;
  MiuPhiSumModer moder(mod);
  moder.init();
  
  int64 s = 0;
  for (int i = 1; i <= 10000000; ++i)
    s += cal_phi(i) % mod;
  s %= mod;
  assert(s == moder.get_sum_phi(10000000));
}

PE_REGISTER_TEST(&nt_test, "nt_test", MEDIUM);
}
