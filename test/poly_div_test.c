#include "pe_test.h"

namespace poly_div_test {
#if !defined(ONLY_RUN_PE_IMPLEMENTATION)
#define ONLY_RUN_PE_IMPLEMENTATION 0
#endif
using poly_div_t = std::vector<uint64> (*)(const std::vector<uint64>&,
                                           const std::vector<uint64>&, int64);
struct DivImpl {
  poly_div_t impl;
  int size;  // 0:small, 1:large
  const char* name;
};

DivImpl div_impl[] = {
    {&PolyDivDc<uint64>, 1, "dc"},
    {&PolyDivNormal<uint64>, 0, "normal"},
#if HAS_POLY_FLINT && !ONLY_RUN_PE_IMPLEMENTATION
    {&flint::PolyDiv<uint64>, 1, "flint"},
#endif
#if HAS_POLY_NTL && !ONLY_RUN_PE_IMPLEMENTATION
    {&ntl::PolyDivLargeMod<uint64>, 1, "ntl lm"},
    {&ntl::PolyDiv<uint64>, 1, "ntl"},
#endif
};

const char* data_policy[3] = {
    "random",
    "min mod",
    "max mod",
};

SL void TestImpl(int dp, int size, int n, int64 mod) {
  fprintf(stderr, "%-8s : data = %s, size = %d, n = %d, mod = %lld\n", "config",
          data_policy[dp], size, n, (long long)mod);

  std::vector<uint64> x, y;
  srand(123456789);
  if (dp == 0) {
    for (int i = 0; i < n; ++i) x.push_back((uint64)CRand63() % mod);
    for (int i = 0; i < n / 2; ++i) y.push_back((uint64)CRand63() % mod);
    x[n - 1] = y[n / 2 - 1] = 1;
  } else {
    for (int i = 0; i < n; ++i) x.push_back(dp == 1 ? 0 : mod - 1);
    for (int i = 0; i < n / 2; ++i) y.push_back(dp == 1 ? 0 : mod - 1);
    x[n - 1] = y[n / 2 - 1] = 1;
  }

  const int M = std::size(div_impl);

  std::vector<uint64> expected;
  for (int i = 0; i < M; ++i) {
    DivImpl who = div_impl[i];
    if (i > 0) {
      if (who.size < size) {
        continue;
      }
    }
    clock_t start = clock();
    std::vector<uint64> result = who.impl(x, y, mod);
    clock_t end = clock();
    fprintf(stderr, "%-8s : %.3f\n", who.name,
            1. * (end - start) / CLOCKS_PER_SEC);
    if (i == 0) {
      expected = result;
    } else {
      assert(expected == result);
    }
  }
}

SL void PolyDivTest() {
  for (int dp = 0; dp < 3; ++dp) {
    for (int n : {128, 2048, 1000000, 1479725}) {
      for (int64 mod : {100019LL, 100000000003LL, 316227766016779LL}) {
        TestImpl(dp, n > 2048, n, mod);
      }
    }
  }
}
PE_REGISTER_TEST(&PolyDivTest, "PolyDivTest", SUPER);

SL void PolyDivPerformanceTest() {
  constexpr std::array<uint64, 5> mods = {100019, 1000003, 1000000007,
                                          100000000003, 316227766016779};
  constexpr int min_log2 = 10;
  constexpr int max_log2 = 20;
  for (int level = 0; level < mods.size(); ++level) {
    printf("mod = %llu\n", (unsigned long long)mods[level]);
    const uint64 mod = mods[level];

    printf("log2(n)  ");

    for (int n = 10; n <= 20; ++n) {
      printf("%-6d ", n);
    }

    puts("");

    const int M = std::size(div_impl);

    std::vector<uint64> expected;
    for (int i = 0; i < M; ++i) {
      DivImpl who = div_impl[i];

      printf("%-8s ", who.name);
      srand(314159);
      for (int n = min_log2; n <= max_log2; ++n) {
        if (who.size == 0 && n > 14) {
          printf("%-6s ", "-");
          continue;
        }
        const int size = 1 << n;
        std::vector<uint64> x, y;
        for (int i = 0; i < size; ++i) x.push_back((uint64)CRand63() % mod);
        for (int i = 0; i < size / 2; ++i) y.push_back((uint64)CRand63() % mod);
        x[size - 1] = y[size / 2 - 1] = 1;

        clock_t start = clock();
        who.impl(x, y, mod);
        clock_t end = clock();
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

PE_REGISTER_TEST(&PolyDivPerformanceTest, "PolyDivPerformanceTest", SUPER);
}  // namespace poly_div_test
