#include "pe_test.h"

namespace poly_mul_test {
#if HAS_POLY_MUL_FLINT || HAS_POLY_MUL_NTT64
#if !defined(ONLY_RUN_PE_IMPLEMENTATION)
#define ONLY_RUN_PE_IMPLEMENTATION 0
#endif

#if HAS_POLY_MUL_FLINT && !ONLY_RUN_PE_IMPLEMENTATION
SL std::vector<uint64> PolyMulParallelFlintForTest(const std::vector<uint64>& X,
                                                   const std::vector<uint64>& Y,
                                                   int64 mod) {
  return pe::internal::PolyMulParallel<
      uint64, pe::PolyMulType<uint64>::CStyleFunctionPointer>(
      X, Y, mod, &flint::PolyMul<uint64>, 8, X.size() / 4);
}
#endif

using poly_mul_t = pe::PolyMulType<uint64>::CppStyleFunctionPointer;

struct MulImpl {
  poly_mul_t impl;
  PolyMulCoeType size;
  const char* name;
};
MulImpl mul_impl[] = {
#if HAS_POLY_MUL_FLINT && !ONLY_RUN_PE_IMPLEMENTATION
    {&flint::PolyMul<uint64>, flint::kPolyMulMod, "flint n"},
    {&flint::bn_poly_mul::PolyMul<uint64>, flint::bn_poly_mul::kPolyMulMod,
     "flint bn"},
    {&flint::pmod::PolyMul<uint64>, flint::pmod::kPolyMulMod, "flint p"},
#if ENABLE_OPENMP
    {&PolyMulParallelFlintForTest, flint::kPolyMulMod, "flint pn"},
#endif
#else
    {&ntt64::PolyMulLarge<uint64>, ntt64::kPolyMulLargeMod, "ntt64 l"},
#endif
#if HAS_POLY_MUL_NTT32
    {&ntt32::PolyMulSmall<uint64>, ntt32::kPolyMulSmallMod, "ntt32 s"},
    {&ntt32::PolyMulMedium<uint64>, ntt32::kPolyMulMediumMod, "ntt32 m"},
    {&ntt32::PolyMulLarge<uint64>, ntt32::kPolyMulLargeMod, "ntt32 l"},
    {&ntt32::PolyMulEnormous<uint64>, ntt32::kPolyMulEnormousMod, "ntt32 e"},
#endif
#if HAS_POLY_MUL_NTT64
    {&ntt64::PolyMulSmall<uint64>, ntt64::kPolyMulSmallMod, "ntt64 s"},
#endif
#if HAS_POLY_MUL_FLINT && HAS_POLY_MUL_NTT64 && !ONLY_RUN_PE_IMPLEMENTATION
    {&ntt64::PolyMulLarge<uint64>, ntt64::kPolyMulLargeMod, "ntt64 l"},
#endif
#if HAS_POLY_MUL_MIN25_SMALL && !ONLY_RUN_PE_IMPLEMENTATION
    {&min25::PolyMulSmall<uint64>, min25::kPolyMulSmallMod, "Min_25 s"},
#endif
#if HAS_POLY_MUL_MIN25 && !ONLY_RUN_PE_IMPLEMENTATION
    {&min25::PolyMulLarge<uint64>, min25::kPolyMulLargeMod, "Min_25 l"},
#endif
#if HAS_POLY_MUL_GMP && !ONLY_RUN_PE_IMPLEMENTATION
    {&gmp::bn_poly_mul::PolyMul<uint64>, gmp::bn_poly_mul::kPolyMulMod,
     "gmp bn"},
#endif
#if HAS_POLY_MUL_LIBBF && !ONLY_RUN_PE_IMPLEMENTATION
    {&libbf::PolyMul<uint64>, libbf::kPolyMulMod, "libbf"},
#endif
#if HAS_POLY_MUL_NTL && !ONLY_RUN_PE_IMPLEMENTATION
    {&ntl::PolyMulSmall<uint64>, ntl::kPolyMulSmallMod, "ntl s"},
    {&ntl::PolyMulLarge<uint64>, ntl::kPolyMulLargeMod, "ntl l"},
#endif
    //  {&PolyMul<uint64>, 4, "default"},
};

const char* data_policy[3] = {
    "random",
    "min mod",
    "max mod",
};

SL void TestImpl(int dp, int n, int64 mod) {
  fprintf(stderr, "%-8s : data = %s, n = %d, mod = %lld\n", "config",
          data_policy[dp], n, (long long)mod);

  std::vector<uint64> x, y;
  x.reserve(n);
  y.reserve(n);
  srand(123456789);
  if (dp == 0) {
    for (int i = 0; i < n; ++i) {
      x.push_back((uint64)CRand63() % mod);
      y.push_back((uint64)CRand63() % mod);
    }
  } else {
    for (int i = 0; i < n; ++i) {
      x.push_back(dp == 1 ? 0 : mod - 1);
      y.push_back(dp == 1 ? 0 : mod - 1);
    }
  }

  const int M = sizeof(mul_impl) / sizeof(mul_impl[0]);

  std::vector<uint64> expected;
  for (int i = 0; i < M; ++i) {
    auto who = mul_impl[i];
    if (i > 0) {
      if (!PolyMulAcceptLengthAndMod(who.size, n, mod)) {
        continue;
      }
    }
    auto start = clock();
    auto result = who.impl(x, y, mod);
    auto end = clock();
    fprintf(stderr, "%-8s : %.3f\n", who.name,
            1. * (end - start) / CLOCKS_PER_SEC);
    if (expected.empty()) {
      expected = result;
    } else {
      assert(expected == result);
    }
  }
}

SL void PolyMulTest() {
  // uint128 target = 2655355665167707426;
  // target = target * 100000000000000000 + 92721528518903091;
  // std::cerr << Uint128ModUint64(target, 100000000003) << std::endl;

  TestImpl(0, 1000000, 100019);
  TestImpl(0, 1479725, 100000000003);
  TestImpl(0, 1000000, 316227766016779);

  // TestImpl(1, 0, 1000000, 100019);
  // TestImpl(1, 1, 1479725, 100000000003);
  // TestImpl(1, 2, 1000000, 316227766016779);

  // 1e18
  TestImpl(2, 999996, 1000003);
  // 1e28
  TestImpl(2, 1479725, 100000000003);
  // 1e35
  TestImpl(2, 1000000, 316227766016779);
  // 2e43
  TestImpl(2, 1000000, 4611686018427387847);
}
PE_REGISTER_TEST(&PolyMulTest, "PolyMulTest", SUPER);

SL void PolyMulPerformanceTest() {
  std::array<uint64, 7> mods = {97,
                                100019,
                                1000003,
                                1000000007,
                                100000000003,
                                316227766016779,
                                4611686018427387847LL};
  constexpr int min_log2 = 10;
  constexpr int max_log2 = 20;
  for (int level = 0; level < mods.size(); ++level) {
    printf("mod = %llu\n", (unsigned long long)mods[level]);
    const auto mod = mods[level];

    printf("log2(n)  ");

    for (int n = 10; n <= max_log2; ++n) {
      printf("%-6d ", n);
    }

    puts("");

    const int M = sizeof(mul_impl) / sizeof(mul_impl[0]);

    std::vector<uint64> expected;
    for (int i = 0; i < M; ++i) {
      auto who = mul_impl[i];
      if (!PolyMulAcceptLengthAndMod(who.size, 1 << min_log2, mod)) continue;

      printf("%-8s ", who.name);
      srand(314159);
      for (int n = min_log2; n <= max_log2; ++n) {
        const int size = 1 << n;
        if (!PolyMulAcceptLengthAndMod(who.size, size, mod)) {
          printf("%-6s ", "-");
          continue;
        }

        std::vector<uint64> x, y;
        for (int i = 0; i < size; ++i) {
          x.push_back((uint64)CRand63() % mod);
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

void PolyMulZeroModTest() {
  std::vector<int64> a = {1, 2, 3};
  std::vector<int64> expected = {1, 4, 10, 12, 9};
  {
    std::vector<int64> actual = ntt32::PolyMul(a, a, 0);
    assert(actual == expected);
  }
  {
    std::vector<int64> actual = ntt64::PolyMul(a, a, 0);
    assert(actual == expected);
  }
#if HAS_POLY_MUL_MIN25 && !ONLY_RUN_PE_IMPLEMENTATION
  {
    std::vector<int64> actual = min25::PolyMul(a, a, 0);
    assert(actual == expected);
  }
#endif
#if HAS_POLY_MUL_LIBBF && !ONLY_RUN_PE_IMPLEMENTATION
  {
    std::vector<int64> actual = libbf::PolyMul(a, a, 0);
    assert(actual == expected);
  }
#endif
#if HAS_POLY_MUL_FLINT && !ONLY_RUN_PE_IMPLEMENTATION
  {
    std::vector<int64> actual = flint::bn_poly_mul::PolyMul(a, a, 0);
    assert(actual == expected);
  }
#endif
}
PE_REGISTER_TEST(&PolyMulZeroModTest, "PolyMulZeroModTest", SMALL);

void PolyMulExtendedInt() {
  constexpr int64 mod = 97;
  std::vector<uint1024e> a = {1, 2, 3};
  std::vector<uint1024e> expected = {1, 4, 10, 12, 9};
  {
    std::vector<uint1024e> actual = ntt32::PolyMul(a, a, mod);
    assert(actual == expected);
  }
  {
    std::vector<uint1024e> actual = ntt64::PolyMul(a, a, mod);
    assert(actual == expected);
  }
#if HAS_POLY_MUL_MIN25 && !ONLY_RUN_PE_IMPLEMENTATION
  {
    std::vector<uint1024e> actual = min25::PolyMul(a, a, mod);
    assert(actual == expected);
  }
#endif
#if HAS_POLY_MUL_LIBBF && !ONLY_RUN_PE_IMPLEMENTATION
  {
    std::vector<uint1024e> actual = libbf::PolyMul(a, a, mod);
    assert(actual == expected);
  }
#endif
#if HAS_POLY_MUL_NTL && !ONLY_RUN_PE_IMPLEMENTATION
  {
    std::vector<uint1024e> actual = ntl::PolyMul(a, a, mod);
    assert(actual == expected);
  }
#endif
#if HAS_POLY_MUL_FLINT && !ONLY_RUN_PE_IMPLEMENTATION
  {
    std::vector<uint1024e> actual = flint::PolyMul(a, a, mod);
    assert(actual == expected);
  }
  {
    std::vector<uint1024e> actual = flint::bn_poly_mul::PolyMul(a, a, mod);
    assert(actual == expected);
  }
  {
    std::vector<uint1024e> actual = flint::pmod::PolyMul(a, a, mod);
    assert(actual == expected);
  }
#endif
}
PE_REGISTER_TEST(&PolyMulExtendedInt, "PolyMulExtendedInt", SMALL);
#endif
}  // namespace poly_mul_test
