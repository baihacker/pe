#include "pe_test.h"

namespace poly_mul_test {
#if HAS_POLY_MUL_FLINT || HAS_POLY_MUL_NTT64
using poly_mul_t = std::vector<uint64> (*)(const std::vector<uint64>&,
                                           const std::vector<uint64>&, int64);
struct MulImpl {
  poly_mul_t impl;
  int size;  // 1: coe < 1e18; 3: coe < 1e28; 4: coe < 1e35
  const char* name;
};
MulImpl mul_impl[] = {
#if HAS_POLY_MUL_FLINT
    {&poly_flint::PolyMulNMod<uint64>, 4, "flint n"},
    {&poly_flint::PolyMulPrime<uint64>, 4, "flint p"},
#else
    {&ntt64::PolyMulLarge<uint64>, 4, "ntt64 l"},
#endif
#if HAS_POLY_MUL_NTT32
    {&ntt32::PolyMulSmall<uint64>, 1, "ntt32 s"},
    {&ntt32::PolyMulLarge<uint64>, 3, "ntt32 l"},
#endif
#if HAS_POLY_MUL_NTT64
    {&ntt64::PolyMulSmall<uint64>, 1, "ntt64 s"},
#endif
#if HAS_POLY_MUL_FLINT && HAS_POLY_MUL_NTT64
    {&ntt64::PolyMulLarge<uint64>, 4, "ntt64 l"},
#endif
#if HAS_POLY_MUL_MIN25_SMALL
    {&poly_min25::PolyMulSmall<uint64>, 1, "Min_25 s"},
#endif
#if HAS_POLY_MUL_MIN25
    {&poly_min25::PolyMulLarge<uint64>, 4, "Min_25 l"},
#endif
#if HAS_POLY_MUL_LIBBF
    {&poly_libbf::PolyMul<uint64>, 4, "libbf"},
#endif
#if HAS_POLY_MUL_NTL
    {&poly_ntl::PolyMulLargeMod<uint64>, 4, "ntl lm"},
    {&poly_ntl::PolyMul<uint64>, 4, "ntl"},
#endif
    //    {&PolyMul<uint64>, 4, "default"},
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
    for (int i = 0; i < n; ++i) {
      x.push_back((uint64)CRand63() % mod),
          y.push_back((uint64)CRand63() % mod);
    }
  } else {
    for (int i = 0; i < n; ++i) {
      x.push_back(dp == 1 ? 0 : mod - 1), y.push_back(dp == 1 ? 0 : mod - 1);
    }
  }

  const int M = sizeof(mul_impl) / sizeof(mul_impl[0]);

  std::vector<uint64> expected;
  for (int i = 0; i < M; ++i) {
    auto who = mul_impl[i];
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

SL void PolyMulTest() {
  // uint128 target = 2655355665167707426;
  // target = target * 100000000000000000 + 92721528518903091;
  // std::cerr << Mod128And64(target, 100000000003) << std::endl;

  TestImpl(0, 1, 1000000, 100019);
  TestImpl(0, 3, 1479725, 100000000003);
  TestImpl(0, 4, 1000000, 316227766016779);

  // TestImpl(1, 0, 1000000, 100019);
  // TestImpl(1, 1, 1479725, 100000000003);
  // TestImpl(1, 2, 1000000, 316227766016779);

  // 1e18
  TestImpl(2, 1, 999996, 1000003);
  // 1e28
  TestImpl(2, 3, 1479725, 100000000003);
  // 1e35
  TestImpl(2, 4, 1000000, 316227766016779);
}
PE_REGISTER_TEST(&PolyMulTest, "PolyMulTest", SUPER);

SL void PolyMulPerformanceTest() {
  uint64 mods[5] = {100019, 1000003, 1000000007, 100000000003, 316227766016779};

  for (int level = 0; level <= 4; ++level) {
    printf("mod = %llu\n", (unsigned long long)mods[level]);
    const auto mod = mods[level];

    printf("log2(n)  ");

    for (int n = 10; n <= 20; ++n) {
      printf("%-6d ", n);
    }

    puts("");

    const int M = sizeof(mul_impl) / sizeof(mul_impl[0]);

    std::vector<uint64> expected;
    for (int i = 0; i < M; ++i) {
      auto who = mul_impl[i];
      if (who.size < level) continue;

      printf("%-8s ", who.name);
      srand(314159);
      for (int n = 10; n <= 20; ++n) {
        const int size = 1 << n;
        std::vector<uint64> x, y;
        for (int i = 0; i < size; ++i) {
          x.push_back((uint64)CRand63() % mod),
              y.push_back((uint64)CRand63() % mod);
        }

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

PE_REGISTER_TEST(&PolyMulPerformanceTest, "PolyMulPerformanceTest", SUPER);
#endif
}  // namespace poly_mul_test
