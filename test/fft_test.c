#include "pe_test.h"

namespace fft_test {
#if HAS_POLY_MUL_FLINT
SL void random_test() {
  srand(123456789);
  {
    // 8e13
    // 1e5+19
    const int64 mod = 100019;
    vector<uint64> x, y;
    for (int i = 0; i < 7000; ++i)
      x.push_back((uint64)crand63() % mod),
          y.push_back((uint64)crand63() % mod);

    int t0 = clock();
    auto ans0 = ntt_flint::poly_mul_flint(x, y, mod);
    int t1 = clock();
    auto ans1 = fft::poly_mul_fft(x, y, mod);
    int t2 = clock();
    auto ans2 = fft::poly_mul_fft_small(x, y, mod);
    int t3 = clock();
    // cerr << (t1 - t0)*1e-3 << " " << (t2-t1)*1e-3 << " " << (t3-t1)*1e-3 <<
    // endl;

    assert(ans0 == ans1);
    assert(ans0 == ans2);
  }
  {
    // 1e15
    // 1e9+7
    const int64 mod = 1000000007;
    vector<uint64> x, y;
    for (int i = 0; i < 1020000; ++i)
      x.push_back((uint64)crand63() % mod),
          y.push_back((uint64)crand63() % mod);

    int t0 = clock();
    auto ans0 = ntt_flint::poly_mul_flint(x, y, mod);
    int t1 = clock();
    auto ans1 = fft::poly_mul_fft(x, y, mod);
    int t2 = clock();
    // cerr << (t1 - t0)*1e-3 << " " << (t2-t1)*1e-3 << endl;

    assert(ans0 == ans1);
  }
  {
    // 8e14
    // 1e10+19
    const int64 mod = 10000000019;
    vector<uint64> x, y;
    for (int i = 0; i < 80000; ++i)
      x.push_back((uint64)crand63() % mod),
          y.push_back((uint64)crand63() % mod);

    int t0 = clock();
    auto ans0 = ntt_flint::poly_mul_flint(x, y, mod);
    int t1 = clock();
    auto ans1 = fft::poly_mul_fft(x, y, mod);
    int t2 = clock();
    // cerr << (t1 - t0)*1e-3 << " " << (t2-t1)*1e-3 << endl;

    assert(ans0 == ans1);
  }
}

SL void limit_test() {
  {
    // 10018*10018*2048=205537943552 2.06e11
    // 1e5+19
    const int64 mod = 100019;
    vector<uint64> x, y;
    for (int i = 0; i < 2048; ++i) x.push_back(mod - 1), y.push_back(mod - 1);

    int t0 = clock();
    auto ans0 = ntt_flint::poly_mul_flint(x, y, mod);
    int t1 = clock();
    auto ans1 = fft::poly_mul_fft(x, y, mod);
    int t2 = clock();
    auto ans2 = fft::poly_mul_fft_small(x, y, mod);
    int t3 = clock();
    // cerr << (t1 - t0)*1e-3 << " " << (t2-t1)*1e-3 << " " << (t3-t1)*1e-3 <<
    // endl;

    assert(ans0 == ans1);
    assert(ans0 == ans2);
  }
  {
    // 1000000007*339750=339750002378250=3.39e14
    // 1e9+7
    const int64 mod = 1000000007;
    vector<uint64> x, y;
    for (int i = 0; i < 339750; ++i) x.push_back(mod - 1), y.push_back(mod - 1);

    int t0 = clock();
    auto ans0 = ntt_flint::poly_mul_flint(x, y, mod);
    int t1 = clock();
    auto ans1 = fft::poly_mul_fft(x, y, mod);
    int t2 = clock();
    // cerr << (t1 - t0)*1e-3 << " " << (t2-t1)*1e-3 << endl;
    assert(ans0 == ans1);
  }
  {
    // 10000000019*44064=440640000837216=4.4e14
    // 1e10+19
    const int64 mod = 10000000019;
    vector<uint64> x, y;
    for (int i = 0; i < 44064; ++i) x.push_back(mod - 1), y.push_back(mod - 1);

    int t0 = clock();
    auto ans0 = ntt_flint::poly_mul_flint(x, y, mod);
    int t1 = clock();
    auto ans1 = fft::poly_mul_fft(x, y, mod);
    int t2 = clock();
    // cerr << (t1 - t0)*1e-3 << " " << (t2-t1)*1e-3 << endl;
    assert(ans0 == ans1);
  }
}

SL void fft_test() {
  random_test();
  limit_test();
}
PE_REGISTER_TEST(&fft_test, "fft_test", SMALL);
#endif
}  // namespace fft_test
