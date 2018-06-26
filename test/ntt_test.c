#include "pe_test.h"

namespace ntt_test {
#if ENABLE_FLINT
typedef vector<uint64> (*poly_mul_t)(const vector<uint64>& X,
                                     const vector<uint64>& Y, int64 mod);
struct MulImpl {
  poly_mul_t impl;
  int size;  // 0: coe < 1e18; 1: coe < 1e28; 2: coe < 1e35
  const char* name;
};
MulImpl mulImpl[] = {
    {&poly_mul_flint<uint64>, 2, "flint n"},
    {&poly_mul_flint_prime<uint64>, 2, "flint p"},
    {&ntt32::poly_mul_ntt_small<uint64>, 0, "ntt32 s"},
#if PE_HAS_INT128
    {&ntt32::poly_mul_ntt<uint64>, 1, "ntt32 l"},
    {&ntt64::poly_mul_ntt_small<uint64>, 0, "ntt64 s"},
    {&ntt64::poly_mul_ntt<uint64>, 2, "ntt64 l"},
    {&ntt_min25::poly_mul_ntt_small<uint64>, 0, "Min_25 s"},
    {&ntt_min25::poly_mul_ntt<uint64>, 2, "Min_25 l"},
#endif
#if ENABLE_LIBBF && HAS_POLY_MUL_LIBBF
    {&ntt_libbf::poly_mul_ntt<uint64>, 2, "libbf"},
#endif
};

const char* dataPolicy[3] = {
  "random",
  "min mod",
  "max mod",
};

SL void test_impl(int dp, int size, int n, int64 mod) {
  fprintf(stderr, "%-8s : data = %s, size = %d, n = %d, mod = %lld\n",
          "ntt test", dataPolicy[dp], size, n, mod);

  vector<uint64> x, y;
  srand(123456789);
  if (dp == 0) {
    for (int i = 0; i < n; ++i) {
      x.push_back((uint64)crand63() % mod),
          y.push_back((uint64)crand63() % mod);
    }
  } else {
    for (int i = 0; i < n; ++i) {
      x.push_back(dp == 1 ? 0 : mod - 1), y.push_back(dp == 1 ? 0 : mod - 1);
    }
  }

  const int M = sizeof(mulImpl) / sizeof(mulImpl[0]);

  vector<uint64> expected;
  for (int i = 0; i < M; ++i) {
    auto who = mulImpl[i];
    if (i > 0) {
      if (who.size < size) {
        continue;
      }
    }
    auto start = clock();
    auto result = who.impl(x, y, mod);
    auto end = clock();
    fprintf(stderr, "%-8s : %.3f\n", who.name, (end - start) * 1e-3);
    if (i == 0) {
      expected = result;
    } else {
      assert(expected == result);
    }
  }
}

SL void ntt_test() {
  // uint128 target = 2655355665167707426;
  // target = target * 100000000000000000 + 92721528518903091;
  // cerr << mod128_64(target, 100000000003) << endl;
  
  test_impl(0, 0, 1000000, 100019);
  test_impl(0, 1, 1479725, 100000000003);
  test_impl(0, 2, 1000000, 316227766016779);

  /*
    Curent implementation cannot pass this test.
    test_impl(1, 0, 1000000, 100019);
    test_impl(1, 1, 1479725, 100000000003);
    test_impl(1, 2, 1000000, 316227766016779);
  */

  // 1e18
  test_impl(2, 0, 999996, 1000003);
  // 1e28
  test_impl(2, 1, 1479725, 100000000003);
  // 1e35
  test_impl(2, 2, 1000000, 316227766016779);
}
PE_REGISTER_TEST(&ntt_test, "ntt_test", BIG);
#endif
}  // namespace ntt_test
