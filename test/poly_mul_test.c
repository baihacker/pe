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

  const int M = std::size(mul_impl);

  std::vector<uint64> expected;
  for (int i = 0; i < M; ++i) {
    MulImpl who = mul_impl[i];
    if (i > 0) {
      if (!PolyMulAcceptLengthAndMod(who.size, n, mod)) {
        continue;
      }
    }
    clock_t start = clock();
    std::vector<uint64> result = who.impl(x, y, mod);
    clock_t end = clock();
    fprintf(stderr, "%-8s : %.3f\n", who.name,
            1. * (end - start) / CLOCKS_PER_SEC);
    if (std::empty(expected)) {
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
    const uint64 mod = mods[level];
    printf("mod = %llu\n", (unsigned long long)mod);

    printf("log2(n)  ");

    for (int n = min_log2; n <= max_log2; ++n) {
      printf("%-6d ", n);
    }

    puts("");

    const int M = std::size(mul_impl);

    std::vector<uint64> expected;
    for (int i = 0; i < M; ++i) {
      MulImpl who = mul_impl[i];
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
        x.reserve(size);
        y.reserve(size);
        for (int i = 0; i < size; ++i) {
          x.push_back((uint64)CRand63() % mod);
          y.push_back((uint64)CRand63() % mod);
        }

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

// Correctness tests on small inputs against a naive reference. The timing
// tests above cross-check the implementations on large inputs.
using PolyMulFn = std::vector<uint64> (*)(const std::vector<uint64>&,
                                          const std::vector<uint64>&, int64);

struct Backend {
  PolyMulFn impl;
  PolyMulCoeType capacity;  // 0: unlimited
  const char* name;
  bool zero_mod;       // mod == 0 (exact product) is supported
  bool prime_mod_only;
};

SL std::vector<uint64> PolyMulParallelSmallBlock(const std::vector<uint64>& X,
                                                 const std::vector<uint64>& Y,
                                                 int64 mod) {
  // A small block size splits the inputs into many tasks.
  return PolyMulParallel(X, Y, mod, 4, 7);
}

SL std::vector<uint64> PolyMulDcMod(const std::vector<uint64>& X,
                                    const std::vector<uint64>& Y, int64 mod) {
  return PolyMulDc(X, Y, mod);
}

SL std::vector<uint64> PolyMulDcPointer(const std::vector<uint64>& X,
                                        const std::vector<uint64>& Y,
                                        int64 mod) {
  std::vector<uint64> result(std::size(X) + std::size(Y) - 1);
  PolyMulDc(X.data(), std::size(X), Y.data(), std::size(Y), result.data(),
            mod);
  return result;
}

SL std::vector<uint64> PolyMulPointer(const std::vector<uint64>& X,
                                      const std::vector<uint64>& Y, int64 mod) {
  std::vector<uint64> result(std::size(X) + std::size(Y) - 1);
  PolyMul(X.data(), std::size(X), Y.data(), std::size(Y), result.data(), mod);
  return result;
}

// pe::PolyMul sends inputs with max(n, m) >= 50 to this implementation without
// checking its capacity (see the TODO(bug) in pe_poly_base).
#if HAS_POLY_MUL_FLINT
constexpr PolyMulCoeType kDispatchCapacity = flint::kPolyMulMod;
#elif HAS_POLY_MUL_MIN25
constexpr PolyMulCoeType kDispatchCapacity = min25::kPolyMulMod;
#elif HAS_POLY_MUL_NTT32
constexpr PolyMulCoeType kDispatchCapacity = ntt::kPolyMulMod;
#else
constexpr PolyMulCoeType kDispatchCapacity = 0;
#endif

SL std::vector<Backend> PolyMulBackends() {
  std::vector<Backend> ret = {
      {&PolyMul<uint64>, kDispatchCapacity, "PolyMul", false, false},
      {&PolyMulPointer, kDispatchCapacity, "PolyMul ptr", false, false},
      {&PolyMulDcMod, 0, "PolyMulDc", false, false},
      {&PolyMulDcPointer, 0, "PolyMulDc ptr", false, false},
      {&PolyMulParallelSmallBlock, kDispatchCapacity, "PolyMulParallel", false,
       false},
#if HAS_POLY_MUL_NTT32
      {&ntt32::PolyMulSmall<uint64>, ntt32::kPolyMulSmallMod, "ntt32 s", true,
       false},
      {&ntt32::PolyMulMedium<uint64>, ntt32::kPolyMulMediumMod, "ntt32 m", true,
       false},
      // Without int128 the 3- and 4-prime runners don't support mod == 0 (see
      // the TODO(bug) in pe_poly_base_ntt).
      {&ntt32::PolyMulLarge<uint64>, ntt32::kPolyMulLargeMod, "ntt32 l",
       PE_HAS_INT128, false},
      {&ntt32::PolyMulEnormous<uint64>, ntt32::kPolyMulEnormousMod, "ntt32 e",
       PE_HAS_INT128, false},
      {&ntt32::PolyMul<uint64>, ntt32::kPolyMulMod, "ntt32", PE_HAS_INT128,
       false},
      {&ntt::PolyMul<uint64>, ntt::kPolyMulMod, "ntt", false, false},
#endif
#if HAS_POLY_MUL_NTT64
      {&ntt64::PolyMulSmall<uint64>, ntt64::kPolyMulSmallMod, "ntt64 s", true,
       false},
      {&ntt64::PolyMulLarge<uint64>, ntt64::kPolyMulLargeMod, "ntt64 l",
       PE_HAS_INT128, false},
      {&ntt64::PolyMul<uint64>, ntt64::kPolyMulMod, "ntt64", PE_HAS_INT128,
       false},
#endif
#if HAS_POLY_MUL_MIN25_SMALL
      {&min25::PolyMulSmall<uint64>, min25::kPolyMulSmallMod, "min25 s", true,
       false},
#endif
#if HAS_POLY_MUL_MIN25
      {&min25::PolyMulLarge<uint64>, min25::kPolyMulLargeMod, "min25 l", true,
       false},
      {&min25::PolyMul<uint64>, min25::kPolyMulMod, "min25", true, false},
#endif
#if HAS_POLY_MUL_LIBBF
      {&libbf::PolyMul<uint64>, libbf::kPolyMulMod, "libbf", true, false},
#endif
#if HAS_POLY_MUL_GMP
      {&gmp::bn_poly_mul::PolyMul<uint64>, gmp::bn_poly_mul::kPolyMulMod,
       "gmp bn", true, false},
#endif
#if HAS_POLY_MUL_FLINT
      {&flint::PolyMul<uint64>, flint::kPolyMulMod, "flint", false, false},
      {&flint::bn_poly_mul::PolyMul<uint64>, flint::bn_poly_mul::kPolyMulMod,
       "flint bn", true, false},
      {&flint::pmod::PolyMul<uint64>, flint::pmod::kPolyMulMod, "flint p",
       false, true},
#endif
#if HAS_POLY_MUL_NTL
      {&ntl::PolyMulSmall<uint64>, ntl::kPolyMulSmallMod, "ntl s", false,
       false},
      {&ntl::PolyMulLarge<uint64>, ntl::kPolyMulLargeMod, "ntl l", false,
       false},
      {&ntl::PolyMul<uint64>, ntl::kPolyMulLargeMod, "ntl", false, false},
#endif
  };
  return ret;
}

// mod == 0 means the exact product.
SL std::vector<uint64> NaivePolyMul(const std::vector<uint64>& x,
                                    const std::vector<uint64>& y, int64 mod) {
  std::vector<uint64> ret(std::size(x) + std::size(y) - 1);
  for (size_t i = 0; i < std::size(x); ++i) {
    for (size_t j = 0; j < std::size(y); ++j) {
      if (mod) {
        ret[i + j] = AddMod<uint64>(ret[i + j], MulMod<uint64>(x[i], y[j], mod),
                                    mod);
      } else {
        ret[i + j] += x[i] * y[j];
      }
    }
  }
  return ret;
}

// policy 0: random, 1: all max, 2: zeros at both ends, 3: all zeros
SL std::vector<uint64> MakePoly(int n, uint64 max_value, int policy,
                                uint64 seed) {
  std::vector<uint64> ret(n);
  for (int i = 0; i < n; ++i) {
    seed = seed * 6364136223846793005ULL + 1442695040888963407ULL;
    switch (policy) {
      case 0:
        ret[i] = (seed >> 1) % (max_value + 1);
        break;
      case 1:
        ret[i] = max_value;
        break;
      case 2:
        ret[i] = i < n / 3 || i >= n - n / 4 ? 0 : (seed >> 1) % (max_value + 1);
        break;
      default:
        ret[i] = 0;
    }
  }
  return ret;
}

SL void PolyMulBackendTest() {
  const std::vector<Backend> backends = PolyMulBackends();
  const std::vector<std::pair<int, int>> sizes = {
      {1, 1}, {1, 5}, {7, 3}, {49, 49}, {50, 50}, {51, 120}, {130, 60},
      {300, 300}};
  for (int64 mod : {int64(2), int64(97), int64(998244353), int64(1000000007),
                    int64(1000000000000037), int64(4611686018427387847)}) {
    const bool is_prime = IsPrimeEx(mod);
    for (auto [n, m] : sizes) {
      for (int policy = 0; policy < 4; ++policy) {
        const std::vector<uint64> x = MakePoly(n, mod - 1, policy, n * 31 + m);
        const std::vector<uint64> y = MakePoly(m, mod - 1, policy, m * 17 + n);
        const std::vector<uint64> expected = NaivePolyMul(x, y, mod);
        for (const Backend& b : backends) {
          if (!PolyMulAcceptLengthAndMod(b.capacity, n, m, mod)) continue;
          if (b.prime_mod_only && !is_prime) continue;
          assert(b.impl(x, y, mod) == expected);
        }
      }
    }
  }

  // mod == 0: the exact product.
  for (auto [n, m] : sizes) {
    for (int policy = 0; policy < 4; ++policy) {
      const uint64 max_value = 1000;
      const std::vector<uint64> x = MakePoly(n, max_value, policy, n + 7 * m);
      const std::vector<uint64> y = MakePoly(m, max_value, policy, m + 3 * n);
      const std::vector<uint64> expected = NaivePolyMul(x, y, 0);
      for (const Backend& b : backends) {
        if (!b.zero_mod) continue;
        if (!PolyMulAcceptLengthAndValue(b.capacity, n, max_value, m,
                                         max_value)) {
          continue;
        }
        assert(b.impl(x, y, 0) == expected);
      }
    }
  }
}

PE_REGISTER_TEST(&PolyMulBackendTest, "PolyMulBackendTest", SMALL);

SL void PolyMulOtherTypesTest() {
  // Signed and 32-bit element types go through the unsigned implementation.
  const int64 mod = 998244353;
  const std::vector<uint64> x = MakePoly(100, mod - 1, 0, 1);
  const std::vector<uint64> y = MakePoly(70, mod - 1, 0, 2);
  const std::vector<uint64> expected = NaivePolyMul(x, y, mod);
  {
    std::vector<int64> xs(std::begin(x), std::end(x));
    std::vector<int64> ys(std::begin(y), std::end(y));
    const std::vector<int64> expected_s(std::begin(expected),
                                        std::end(expected));
    assert(PolyMul(xs, ys, mod) == expected_s);
    assert(ntt32::PolyMul(xs, ys, mod) == expected_s);
    assert(ntt64::PolyMul(xs, ys, mod) == expected_s);
    std::vector<int64> result(std::size(xs) + std::size(ys) - 1);
    ntt64::PolyMul(xs.data(), std::size(xs), ys.data(), std::size(ys),
                   result.data(), mod);
    assert(result == expected_s);
  }
  {
    std::vector<uint32> xs(std::begin(x), std::end(x));
    std::vector<uint32> ys(std::begin(y), std::end(y));
    const std::vector<uint32> expected_s(std::begin(expected),
                                         std::end(expected));
    assert(PolyMul(xs, ys, mod) == expected_s);
    assert(ntt32::PolyMul(xs, ys, mod) == expected_s);
  }

  // Without a modulus (generic T): negative coefficients, the naive branch
  // (n <= 49) and the divide and conquer branches (n == m, n > m, n < m).
  for (auto [n, m] : std::vector<std::pair<int, int>>{
           {1, 1}, {3, 40}, {49, 49}, {60, 60}, {200, 70}, {70, 200}}) {
    std::vector<int64> a(n), b(m);
    for (int i = 0; i < n; ++i) a[i] = (i * 37 + 11) % 201 - 100;
    for (int i = 0; i < m; ++i) b[i] = (i * 53 + 7) % 199 - 99;
    std::vector<int64> expected_s(n + m - 1);
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < m; ++j) expected_s[i + j] += a[i] * b[j];
    assert(PolyMul(a, b) == expected_s);
    assert(PolyMulDc(a, b) == expected_s);
    std::vector<int64> result(n + m - 1);
    PolyMul(a.data(), n, b.data(), m, result.data());
    assert(result == expected_s);
    PolyMulDc(a.data(), n, b.data(), m, result.data());
    assert(result == expected_s);
  }
}

PE_REGISTER_TEST(&PolyMulOtherTypesTest, "PolyMulOtherTypesTest", SMALL);

SL void PolyMulHelperTest() {
  // PolyMulAcceptLengthAndValue: max_value1 * max_value2 * n < capacity, and
  // capacity 0 means unlimited.
  const PolyMulCoeType capacity = 1000;
  for (int64 n = 0; n <= 30; ++n) {
    for (uint64 v1 = 0; v1 <= 40; ++v1) {
      for (uint64 v2 = 0; v2 <= 40; v2 += 3) {
        const int expected = n <= 0 || v1 == 0 || v2 == 0 || v1 * v2 * n < 1000;
        assert(PolyMulAcceptLengthAndValue(capacity, n, v1, v2) == expected);
        assert(PolyMulAcceptLengthAndValue(capacity, n, v1, n + 5, v2) ==
               expected);
        assert(PolyMulAcceptLengthAndValue(capacity, n + 5, v1, n, v2) ==
               expected);
        assert(PolyMulAcceptLengthAndValue(PolyMulCoeType(0), n, v1, v2));
      }
      if (v1 > 0) {
        const int expected = n <= 0 || v1 == 1 || (v1 - 1) * (v1 - 1) * n < 1000;
        assert(PolyMulAcceptLengthAndMod(capacity, n, v1) == expected);
        assert(PolyMulAcceptLengthAndMod(capacity, n, n + 1, v1) == expected);
      }
    }
  }
  // Large capacities
  assert(PolyMulAcceptLengthAndMod(ntt32::kPolyMulSmallMod, 1000, 1972));
  assert(!PolyMulAcceptLengthAndMod(ntt32::kPolyMulSmallMod, 1000, 1974));
  assert(PolyMulAcceptLengthAndMod(ntt64::kPolyMulLargeMod, 1 << 20,
                                   int64(100000000000000)));

  // AdjustPolyLeadingZero keeps at least one coefficient.
  for (auto [in, out] : std::vector<std::pair<std::vector<int>, std::vector<int>>>{
           {{1, 2, 0, 0}, {1, 2}}, {{0, 0}, {0}}, {{0}, {0}}, {{5}, {5}},
           {{0, 3}, {0, 3}}}) {
    AdjustPolyLeadingZero(in);
    assert(in == out);
  }
}

PE_REGISTER_TEST(&PolyMulHelperTest, "PolyMulHelperTest", SMALL);
#endif
}  // namespace poly_mul_test
