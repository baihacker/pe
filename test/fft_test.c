#include "pe_test.h"

namespace fft_test {
SL void fft_test() {
  fft::init_fft();
  {
    // 1e5+19
    const int64 mod = 100019;
    vector<uint64> x, y;
    for (int i = 0; i < 8000; ++i)
      x.push_back((uint64)rand()*rand()*rand()%mod),
      y.push_back((uint64)rand()*rand()*rand()%mod);

    int t0 = clock();
    auto ans0 = poly_mul(x, y, mod);
    int t1 = clock();
    auto ans1 = fft::fft_mul_mod(x, y, mod);
    int t2 = clock();
    auto ans2 = fft::fft_mul_mod_small(x, y, mod);
    int t3 = clock();
    // cerr << (t1 - t0)*1e-3 << " " << (t2-t1)*1e-3 << " " << (t3-t1)*1e-3 << endl;

    assert(ans0 == ans1);
    assert(ans0 == ans2);
  }
  {
    // 1e9+7
    const int64 mod = 1000000007;
    vector<uint64> x, y;
    for (int i = 0; i < 1020000; ++i)
      x.push_back((uint64)rand()*rand()*rand()%mod),
      y.push_back((uint64)rand()*rand()*rand()%mod);

    int t0 = clock();
    auto ans0 = poly_mul(x, y, mod);
    int t1 = clock();
    auto ans1 = fft::fft_mul_mod(x, y, mod);
    int t2 = clock();
    // cerr << (t1 - t0)*1e-3 << " " << (t2-t1)*1e-3 << endl;

    assert(ans0 == ans1);
  }
  {
    // 1e10+19
    const int64 mod = 10000000019;
    vector<uint64> x, y;
    for (int i = 0; i < 120000; ++i)
      x.push_back((uint64)rand()*rand()*rand()%mod),
      y.push_back((uint64)rand()*rand()*rand()%mod);

    int t0 = clock();
    auto ans0 = poly_mul(x, y, mod);
    int t1 = clock();
    auto ans1 = fft::fft_mul_mod(x, y, mod);
    int t2 = clock();
    // cerr << (t1 - t0)*1e-3 << " " << (t2-t1)*1e-3 << endl;

    assert(ans0 == ans1);
  }
}
PE_REGISTER_TEST(&fft_test, "fft_test", SMALL);
}
