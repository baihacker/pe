#include "pe_test.h"

const int64 mod = 1000000007;

const int n = 1000000;
int sresult[n+1];
int64 lresult[n+1];

SL void init_inv_test() {
  init_inv(sresult, n, mod);
  init_inv(lresult, n, mod);
  for (int i = 1; i < n; ++i)
  {
    assert((int64)i * sresult[i] % mod == 1);
    assert(sresult[i] == lresult[i]);
  }
}

PE_REGISTER_TEST(&init_inv_test, "init_inv_test");