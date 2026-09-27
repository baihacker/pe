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
// Correctness tests of the polynomial operations on small inputs; the tests
// above cross-check the division implementations on large inputs.
constexpr int64 kP = 998244353;

SL std::vector<uint64> RandPoly(int n, uint64& seed, int64 mod = kP) {
  std::vector<uint64> ret(n);
  for (auto& v : ret) {
    seed = seed * 6364136223846793005ULL + 1442695040888963407ULL;
    v = (seed >> 1) % mod;
  }
  return ret;
}

SL std::vector<uint64> NaiveMul(const std::vector<uint64>& x,
                                const std::vector<uint64>& y,
                                int64 mod = kP) {
  std::vector<uint64> ret(std::size(x) + std::size(y) - 1);
  for (size_t i = 0; i < std::size(x); ++i)
    for (size_t j = 0; j < std::size(y); ++j)
      ret[i + j] = (ret[i + j] + x[i] * y[j] % mod) % mod;
  return ret;
}

// x mod x^n, padded with zeros.
SL std::vector<uint64> Trunc(std::vector<uint64> x, int n) {
  x.resize(n);
  return x;
}

SL void PolyBasicOpsTest() {
  const std::vector<int64> p = {1, 2, 3};
  assert((PolyShift(p, 2) == std::vector<int64>{0, 0, 1, 2, 3}));
  assert((PolyShift(p, -1) == std::vector<int64>{2, 3}));
  assert((PolyShift(p, -3) == std::vector<int64>{0}));
  assert(PolyShift(p, 0) == p);
  assert(PolyShiftLeft(p, 1) == PolyShift(p, 1));
  assert(PolyShiftRight(p, 1) == PolyShift(p, -1));

  // Add / Sub with and without a modulus, different lengths.
  const std::vector<int64> a = {5, 6, 7, 8}, b = {3, 9};
  assert((PolyAdd(a, b) == std::vector<int64>{8, 15, 7, 8}));
  assert((PolySub(a, b) == std::vector<int64>{2, -3, 7, 8}));
  assert((PolySub(b, a) == std::vector<int64>{-2, 3, -7, -8}));
  assert((PolyAdd(a, b, int64(10)) == std::vector<int64>{8, 5, 7, 8}));
  assert((PolySub(a, b, int64(10)) == std::vector<int64>{2, 7, 7, 8}));
  assert((PolySub(b, a, int64(10)) == std::vector<int64>{8, 3, 3, 2}));
  std::vector<int64> r(4);
  PolyAdd(a.data(), 4, b.data(), 2, r.data(), int64(10));
  assert((r == std::vector<int64>{8, 5, 7, 8}));
  PolySub(b.data(), 2, a.data(), 4, r.data(), int64(10));
  assert((r == std::vector<int64>{8, 3, 3, 2}));
  PolyAdd(b.data(), 2, a.data(), 4, r.data());
  assert((r == std::vector<int64>{8, 15, 7, 8}));
  PolySub(a.data(), 4, b.data(), 2, r.data());
  assert((r == std::vector<int64>{2, -3, 7, 8}));

  // PartialSumAt: terms[0] + ... + terms[n]
  assert(PartialSumAt(a, 2, 10) == 8);
  assert(PartialSumAt<int64>(a, 3) == 26);

  // Evaluation
  const std::vector<int64> f = {4, 0, 3, 1};  // x^3 + 3x^2 + 4
  for (int64 v = -3; v <= 3; ++v) {
    const int64 expected = v * v * v + 3 * v * v + 4;
    assert(PolyEvaluate<int64>(f, v) == expected);
    assert((PolyEvaluate<int64, int64, int64>(f.data(), 4, v) == expected));
    if (v >= 0) {
      assert(PolyEvaluate<int64>(f, v, 11) == expected % 11);
      assert(PolyEvaluate<int64>(f.data(), 4, v, 11) == expected % 11);
    }
  }
}

PE_REGISTER_TEST(&PolyBasicOpsTest, "PolyBasicOpsTest", SMALL);

SL void PolyConvolutionTest() {
  uint64 seed = 1;
  for (auto [n, m] : std::vector<std::pair<int, int>>{
           {1, 1}, {8, 8}, {5, 3}, {3, 16}, {100, 60}}) {
    const std::vector<uint64> x = RandPoly(n, seed), y = RandPoly(m, seed);
    const int size = static_cast<int>(std::max(BitCeil(n), BitCeil(m)));
    std::vector<uint64> e_or(size), e_and(size), e_xor(size);
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m; ++j) {
        const uint64 v = x[i] * y[j] % kP;
        e_or[i | j] = (e_or[i | j] + v) % kP;
        e_and[i & j] = (e_and[i & j] + v) % kP;
        e_xor[i ^ j] = (e_xor[i ^ j] + v) % kP;
      }
    }
    const uint64 mod = kP;
    assert(PolyConvolutionOr(x, y, mod) == e_or);
    assert(PolyConvolutionAnd(x, y, mod) == e_and);
    assert(PolyConvolutionXor(x, y, mod) == e_xor);
    std::vector<uint64> r(size);
    PolyConvolutionOr(x.data(), n, y.data(), m, r.data(), mod);
    assert(r == e_or);
    PolyConvolutionAnd(x.data(), n, y.data(), m, r.data(), mod);
    assert(r == e_and);
    PolyConvolutionXor(x.data(), n, y.data(), m, r.data(), mod);
    assert(r == e_xor);
  }
}

PE_REGISTER_TEST(&PolyConvolutionTest, "PolyConvolutionTest", SMALL);

SL void PolyInvDivModTest() {
  uint64 seed = 2;
  // Inverse: x * inv(x) = 1 mod x^trunc (trunc >= 1).
  for (int m : {1, 2, 7, 60}) {
    for (int trunc : {1, 2, 5, 64, 150}) {
      std::vector<uint64> x = RandPoly(m, seed);
      if (x[0] == 0) x[0] = 1;
      std::vector<uint64> one(trunc);
      one[0] = 1;
      assert(Trunc(NaiveMul(x, PolyInv(x, trunc, kP)), trunc) == one);
      assert(Trunc(NaiveMul(x, PolyInvDoubling(x, trunc, kP)), trunc) == one);
      assert(Trunc(NaiveMul(x, pmod::PolyInv(x, trunc, kP)), trunc) == one);
      std::vector<uint64> r(trunc);
      PolyInv(x.data(), m, trunc, r.data(), kP);
      assert(Trunc(NaiveMul(x, r), trunc) == one);
    }
  }

  // Division with remainder: X = Q * Y + R, deg R < deg Y.
  using Div = std::tuple<std::vector<uint64>, std::vector<uint64>> (*)(
      const std::vector<uint64>&, const std::vector<uint64>&, int64);
  const Div impls[] = {&PolyDivAndMod<uint64>, &PolyDivAndModDc<uint64>,
                       &PolyDivAndModNormal<uint64>};
  for (auto [n, m] : std::vector<std::pair<int, int>>{
           {1, 1}, {5, 1}, {5, 5}, {10, 3}, {200, 70}, {300, 299}}) {
    std::vector<uint64> x = RandPoly(n, seed), y = RandPoly(m, seed);
    if (y.back() == 0) y.back() = 1;
    for (Div impl : impls) {
      auto [q, r] = impl(x, y, kP);
      assert(static_cast<int>(std::size(q)) == n - m + 1);
      assert(static_cast<int>(std::size(r)) <= std::max(m - 1, 1));
      std::vector<uint64> back = NaiveMul(q, y);
      back.resize(std::max(std::size(back), std::size(r)));
      for (size_t i = 0; i < std::size(r); ++i) back[i] = (back[i] + r[i]) % kP;
      back.resize(n);
      assert(back == x);
      assert(PolyDiv(x, y, kP) == q && PolyDivDc(x, y, kP) == q &&
             PolyDivNormal(x, y, kP) == q);
      assert(PolyMod(x, y, kP) == r && PolyModDc(x, y, kP) == r &&
             PolyModNormal(x, y, kP) == r);
    }
  }

  // Gcd of (x - 1)(x - 2)(x - 3) and (x - 2)(x - 3)(x - 5) is a multiple of
  // (x - 2)(x - 3).
  const std::vector<uint64> f = NaiveMul(NaiveMul({kP - 1, 1}, {kP - 2, 1}),
                                         {kP - 3, 1});
  const std::vector<uint64> g = NaiveMul(NaiveMul({kP - 5, 1}, {kP - 2, 1}),
                                         {kP - 3, 1});
  const std::vector<uint64> h = PolyGcd(f, g, kP);
  assert(std::size(h) == 3);
  for (uint64 root : {2, 3}) assert(PolyEvaluate<uint64>(h, root, kP) == 0);

  // x^n mod poly against repeated multiplication.
  const std::vector<uint64> base = {3, 1, 4, 1, 5}, pm = {9, 2, 6, 5, 3, 1};
  std::vector<uint64> expected = {1};
  for (int64 n = 0; n <= 40; ++n) {
    assert(PolyPowerModPoly(base, n, pm, kP) == PolyMod(expected, pm, kP));
    expected = PolyMod(NaiveMul(expected, base), pm, kP);
  }
}

PE_REGISTER_TEST(&PolyInvDivModTest, "PolyInvDivModTest", SMALL);

SL void PolyLogExpTest() {
  uint64 seed = 3;
  const int n = 80;
  // Derivative and integral (the integral has constant term 1).
  const std::vector<uint64> f = RandPoly(30, seed);
  const std::vector<uint64> df = PolyDerivative(f, kP);
  for (int i = 0; i + 1 < 30; ++i) assert(df[i] == f[i + 1] * (i + 1) % kP);
  const std::vector<uint64> F = pmod::PolyIntegral(df, kP);
  assert(F[0] == 1);
  for (int i = 1; i < 30; ++i) assert(F[i] == f[i]);

  // exp(x) = sum x^k / k!
  std::vector<uint64> ex(n);
  ex[0] = 1;
  for (int k = 1; k < n; ++k) ex[k] = MulMod(ex[k - 1], ModInv<int64>(k, kP), kP);
  assert(pmod::PolyExp(std::vector<uint64>{0, 1}, n, kP) == ex);

  // log and exp are inverse to each other, and log(fg) = log f + log g.
  std::vector<uint64> a = RandPoly(n, seed), b = RandPoly(n, seed);
  a[0] = b[0] = 1;
  const std::vector<uint64> la = pmod::PolyLog(a, n, kP);
  const std::vector<uint64> lb = pmod::PolyLog(b, n, kP);
  assert(la[0] == 0);
  assert(pmod::PolyExp(la, n, kP) == a);
  const std::vector<uint64> lab = pmod::PolyLog(Trunc(NaiveMul(a, b), n), n, kP);
  assert(lab == PolyAdd(la, lb, int64(kP)));
  std::vector<uint64> r(n);
  pmod::PolyLog(a.data(), n, n, r.data(), kP);
  assert(r == la);
  pmod::PolyExp(la.data(), n, n, r.data(), kP);
  assert(r == a);

  // Euler transform: prod_k (1 - y_k x^k)^(-a_k); y_k = 1 is the plain
  // transform.
  const int len = 30;
  const std::vector<uint64> ak = {0, 1, 2, 0, 3, 1, 2};
  const std::vector<uint64> yk = {0, 1, 5, 7, 2, 1, 3};
  auto brute = [&](const std::vector<uint64>& y) {
    std::vector<uint64> ret(len);
    ret[0] = 1;
    for (int k = 1; k < static_cast<int>(std::size(ak)); ++k) {
      // (1 - y x^k)^(-a) = sum_j C(a + j - 1, j) y^j x^(k j)
      std::vector<uint64> t(len);
      uint64 c = 1, yp = 1;
      for (int j = 0; j * k < len; ++j) {
        t[j * k] = c * yp % kP;
        c = MulMod<uint64>(c * ((ak[k] + j) % kP) % kP,
                           ModInv<int64>(j + 1, kP), kP);
        yp = yp * y[k] % kP;
      }
      ret = Trunc(NaiveMul(ret, t), len);
    }
    return ret;
  };
  const std::vector<uint64> ones(std::size(ak), 1);
  assert(pmod::PolyEulerTransform(ak, len, kP) == brute(ones));
  assert(pmod::PolyEulerTransform(ak, yk, len, kP) == brute(yk));
  {
    std::vector<int64> invs(len);
    InitInverse(invs.data(), len - 1, kP);
    assert(pmod::PolyEulerTransform(ak, len, kP, invs.data()) == brute(ones));
    assert(pmod::PolyEulerTransform(ak, yk, len, kP, invs.data()) == brute(yk));
  }
}

PE_REGISTER_TEST(&PolyLogExpTest, "PolyLogExpTest", SMALL);

SL void PolyEvaluationTest() {
  uint64 seed = 4;
  // Multipoint evaluation (all implementations) against Horner.
  for (int n : {1, 2, 17, 300}) {
    const std::vector<uint64> x = RandPoly(n, seed), v = RandPoly(n, seed);
    std::vector<uint64> expected(n);
    for (int i = 0; i < n; ++i) expected[i] = PolyEvaluate<uint64>(x, v[i], kP);
    assert(PolyMultipointEvaluate(x, v, kP) == expected);
    assert(PolyMultipointEvaluateNormal(x, v, kP) == expected);
    assert(PolyMultipointEvaluateBls(x, v, kP) == expected);
    std::vector<uint64> r(n);
    PolyMultipointEvaluate(x.data(), n, v.data(), r.data(), kP);
    assert(r == expected);
  }

  // PolyOffsetEvaluate: f(offset), ..., f(offset + d) from f(0), ..., f(d).
  for (int d : {1, 2, 10, 50}) {
    const std::vector<uint64> f = RandPoly(d + 1, seed);
    std::vector<uint64> values(d + 1), ifac(d + 1);
    for (int i = 0; i <= d; ++i) values[i] = PolyEvaluate<uint64>(f, i, kP);
    ifac[0] = 1;
    for (int i = 1; i <= d; ++i)
      ifac[i] = MulMod<uint64>(ifac[i - 1], ModInv<int64>(i, kP), kP);
    for (int64 offset : {int64(d + 1), int64(1000), int64(123456789)}) {
      std::vector<uint64> expected(d + 1);
      for (int i = 0; i <= d; ++i)
        expected[i] = PolyEvaluate<uint64>(f, offset + i, kP);
      assert(PolyOffsetEvaluate(values, offset, ifac.data(), uint64(kP)) ==
             expected);
    }
  }

  // PolyBatchMul and PolyBatchMulAcc
  for (int n : {1, 2, 7, 64}) {
    const std::vector<uint64> x = RandPoly(2 * n, seed);
    std::vector<uint64> prod = {1};
    std::vector<uint64> acc(n + 1);
    for (int i = 0; i < n; ++i) {
      prod = NaiveMul(prod, {x[2 * i], x[2 * i + 1]});
      for (size_t j = 0; j < std::size(prod); ++j) acc[j] = (acc[j] + prod[j]) % kP;
    }
    assert(PolyBatchMul(x, kP) == prod);
    std::vector<uint64> r(n + 1);
    PolyBatchMul(x.data(), n, r.data(), kP);
    assert(r == prod);
    assert(PolyBatchMulAcc(x, kP) == acc);
    std::vector<uint64> c;
    assert(PolyBatchMulAcc(x, c, kP) == acc);
    assert(c == prod);
  }
}

PE_REGISTER_TEST(&PolyEvaluationTest, "PolyEvaluationTest", SMALL);
}  // namespace poly_div_test
