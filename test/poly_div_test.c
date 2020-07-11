#include "pe_test.h"

namespace poly_div_test {
typedef vector<uint64> (*poly_div_t)(const vector<uint64>& X,
                                     const vector<uint64>& Y, int64 mod);
struct DivImpl {
  poly_div_t impl;
  int size;  // 0:small, 1:large
  const char* name;
};

DivImpl divImpl[] = {
    {&poly_div_dc<uint64>, 1, "dc"},
    {&poly_div_normal<uint64>, 0, "normal"},
#if HAS_POLY_FLINT
    {&poly_flint::poly_div<uint64>, 1, "flint"},
#endif
};

const char* dataPolicy[3] = {
    "random",
    "min mod",
    "max mod",
};

SL void test_impl(int dp, int size, int n, int64 mod) {
  fprintf(stderr, "%-8s : data = %s, size = %d, n = %d, mod = %lld\n",
          "config", dataPolicy[dp], size, n, (long long)mod);

  vector<uint64> x, y;
  srand(123456789);
  if (dp == 0) {
    for (int i = 0; i < n; ++i) x.push_back((uint64)crand63() % mod);
    for (int i = 0; i < n / 2; ++i) y.push_back((uint64)crand63() % mod);
    x[n - 1] = y[n / 2 - 1] = 1;
  } else {
    for (int i = 0; i < n; ++i) x.push_back(dp == 1 ? 0 : mod - 1);
    for (int i = 0; i < n / 2; ++i) y.push_back(dp == 1 ? 0 : mod - 1);
    x[n - 1] = y[n / 2 - 1] = 1;
  }

  const int M = sizeof(divImpl) / sizeof(divImpl[0]);

  vector<uint64> expected;
  for (int i = 0; i < M; ++i) {
    auto who = divImpl[i];
    if (i > 0) {
      if (who.size < size) {
        continue;
      }
    }
    auto start = clock();
    auto result = who.impl(x, y, mod);
    auto end = clock();
    fprintf(stderr, "%-8s : %.3f\n", who.name,
            1. * (end - start) / CLOCKS_PER_SEC);
    if (i == 0) {
      expected = result;
    } else {
      assert(expected == result);
    }
  }
}

SL void poly_div_test() {
  for (int dp = 0; dp < 3; ++dp)
    for (int n : {128, 2048, 1000000, 1479725})
      for (int64 mod : {100019LL, 100000000003LL, 316227766016779LL}) {
        test_impl(dp, n > 2048, n, mod);
      }
}
PE_REGISTER_TEST(&poly_div_test, "poly_div_test", SUPER);

SL void poly_div_performance_test() {
  uint64 mods[5] = {100019, 1000003, 1000000007, 100000000003, 316227766016779};

  for (int level = 0; level <= 4; ++level) {
    printf("mod = %llu\n", (unsigned long long)mods[level]);
    const auto mod = mods[level];

    printf("log2(n)  ");

    for (int n = 10; n <= 20; ++n) {
      printf("%-6d ", n);
    }

    puts("");

    const int M = sizeof(divImpl) / sizeof(divImpl[0]);

    vector<uint64> expected;
    for (int i = 0; i < M; ++i) {
      auto who = divImpl[i];

      printf("%-8s ", who.name);
      srand(314159);
      for (int n = 10; n <= 20; ++n) {
        if (who.size == 0 && n > 14) {
          printf("%-6s ", "-");
          continue;
        }
        const int size = 1 << n;
        vector<uint64> x, y;
        for (int i = 0; i < size; ++i) x.push_back((uint64)crand63() % mod);
        for (int i = 0; i < size / 2; ++i) y.push_back((uint64)crand63() % mod);
        x[size - 1] = y[size / 2 - 1] = 1;

        auto start = clock();
        who.impl(x, y, mod);
        auto end = clock();
#if 1
        printf("%-6.3f ", 1. * (end - start) / CLOCKS_PER_SEC);
#else
        uint64 a = n * (1 << n);
        uint64 b = end - start;
        printf("%-6.3f ", 1e5 * b / a);
#endif
      }
      puts("");
    }
  }
}

PE_REGISTER_TEST(&poly_div_performance_test, "poly_div_performance_test",
                 SUPER);
}  // namespace poly_div_test
