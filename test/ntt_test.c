#include "pe_test.h"

namespace ntt_test {
SL void random_test() {
// This test ensure that the NTT32_SMALL implementation is correct
#if HAS_POLY_MUL_NTT32 && HAS_POLY_MUL_NTT32_SMALL
  {
    srand(123456789);
    const int64 mod = 100019;
    vector<uint64> x, y;
    for (int i = 0; i < 500000; ++i)
      x.push_back((uint64)crand63()%mod),
      y.push_back((uint64)crand63()%mod);

    int t0 = clock();
    auto ans0 = ntt32::poly_mul_ntt(x, y, mod);
    int t1 = clock();
    auto ans1 = ntt32::poly_mul_ntt_small(x, y, mod);
    int t2 = clock();
    // cerr << (t1 - t0)*1e-3 << " " << (t2-t1)*1e-3 << endl;

    assert(ans0 == ans1);
  }
  {
    const int64 mod = 100019;
    vector<uint64> x, y;
    for (int i = 0; i < 100000; ++i)
      x.push_back(mod-1),
      y.push_back(mod-1);

    int t0 = clock();
    auto ans0 = ntt32::poly_mul_ntt(x, y, mod);
    int t1 = clock();
    auto ans1 = ntt32::poly_mul_ntt_small(x, y, mod);
    int t2 = clock();
    // cerr << (t1 - t0)*1e-3 << " " << (t2-t1)*1e-3 << endl;

    assert(ans0 == ans1);
  }
#endif
}


SL void ntt_test() {
  random_test();
}
PE_REGISTER_TEST(&ntt_test, "ntt_test", MEDIUM);
}
