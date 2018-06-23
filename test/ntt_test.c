#include "pe_test.h"

namespace ntt_test {
#if PE_HAS_INT128 && ENABLE_FLINT
typedef vector<uint64> (*poly_mul_t)(const vector<uint64>& X, const vector<uint64>& Y, int64 mod);
struct MulImpl {
  poly_mul_t impl;
  int isSmall;
  const char* name;
};
MulImpl mulImpl[] = {
  {&poly_mul_flint<uint64>, 0, "flint   "},
  {&ntt32::poly_mul_ntt_small<uint64>, 1, "32 small"},
  {&ntt32::poly_mul_ntt<uint64>, 0, "32 large"},
  {&ntt64::poly_mul_ntt_small<uint64>, 1, "64 small"},
  {&ntt64::poly_mul_ntt<uint64>, 0, "64 large"},
  // {&ntt_min25::product_mod, 0, "min_25  "},
};

SL void test_impl(int isRandom, int largeOnly, int n, int64 mod) {
  fprintf(stderr, "ntt test: isRandom = %d, largeOnly = %d, n = %d, mod = %lld\n", isRandom, largeOnly, n, mod);
  
  vector<uint64> x, y;
  srand(123456789);
  if (isRandom) {
    for (int i = 0; i < n; ++i) {
      x.push_back((uint64)crand63()%mod),
      y.push_back((uint64)crand63()%mod);
    }
  } else {
    for (int i = 0; i < n; ++i) {
      x.push_back(mod - 1),
      y.push_back(mod - 1);
    }
  }
  
  const int M = sizeof(mulImpl) / sizeof(mulImpl[0]);

  vector<uint64> expected;
  for (int i = 0; i < M; ++i) {
    auto who = mulImpl[i];
    if (i > 0) {
      if (largeOnly && who.isSmall) {
        continue;
      }
    }
    auto start = clock();
    auto result = who.impl(x, y, mod);
    auto end = clock();
    fprintf(stderr, "%s : %.3f\n", who.name, (end-start)*1e-3);
    if (i == 0) {
      expected = result;
    } else {
      assert(expected == result);
    }
  }
}

SL void ntt_test() {
  test_impl(1, 0, 1000000, 100019);
  test_impl(1, 1, 1479725, 100000000003);
  test_impl(0, 0, 999996, 1000003);
  test_impl(0, 1, 1479725, 100000000003);
}
PE_REGISTER_TEST(&ntt_test, "ntt_test", BIG);
#endif
}
