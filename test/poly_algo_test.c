#include "pe_test.h"

namespace poly_algo_test {
const int64 mod = 1000000007;

SL void PolyMultiPointEvaluationTest() {
  srand(123456789);
  std::vector<int64> data;
  // The reference PolyEvaluate is O(n) per point.
  int n = 2000;
  const int64 mod = 1000000007;
  for (int i = 1; i <= n; ++i) data.push_back(i);
  std::vector<int64> v;
  for (int i = 1; i <= n; ++i) v.push_back(i % 10007);
  {
    TimeRecorder tr;
    std::vector<int64> result = PolyMultipointEvaluateNormal(data, v, mod);
    // std::cout << tr.Elapsed().Format() << std::endl;
    for (int i = 1; i <= n; ++i) {
      int64 value = PolyEvaluate<int64>(data, i % 10007, mod);
      assert(value == result[i - 1]);
    }
  }
  {
    TimeRecorder tr;
    std::vector<int64> result = PolyMultipointEvaluateBls(data, v, mod);
    // std::cout << tr.Elapsed().Format() << std::endl;
    for (int i = 1; i <= n; ++i) {
      int64 value = PolyEvaluate<int64>(data, i % 10007, mod);
      assert(value == result[i - 1]);
    }
  }
#if HAS_POLY_FLINT
  {
    TimeRecorder tr;
    std::vector<int64> result = flint::PolyMultipointEvaluate(data, v, mod);
    // std::cout << tr.Elapsed().Format() << std::endl;
    for (int i = 1; i <= n; ++i) {
      int64 value = PolyEvaluate<int64>(data, i % 10007, mod);
      assert(value == result[i - 1]);
    }
  }
#endif
}
PE_REGISTER_TEST(&PolyMultiPointEvaluationTest, "PolyMultiPointEvaluationTest",
                 SMALL);

SL void PolyBatchMulTest() {
  const int mod = 10007;
  std::vector<int64> data = {1, 1, 2, 1, 3, 1};
  std::vector<int64> result = PolyBatchMul(data, mod);

  std::vector<int64> expected = {6, 11, 6, 1};
  assert(expected == result);
}
PE_REGISTER_TEST(&PolyBatchMulTest, "PolyBatchMulTest", SMALL);

SL void GenBernoulliNumberTest() {
  const int mod = 10007;
  assert((GenBernoulliNumber(7, mod) ==
          std::vector<int64>{1, 5003, 1668, 0, 7672, 0, 4527, 0}));
}
PE_REGISTER_TEST(&GenBernoulliNumberTest, "GenBernoulliNumberTest", SMALL);

SL void GenStirling1ColumnTest() {
  const int mod = 10007;
  assert((pmod::GenStirling1Column(3, 10, mod) ==
          std::vector<int64>{0, 0, 0, 1, 6, 35, 225, 1624, 3125, 8047, 1881}));
}
PE_REGISTER_TEST(&GenStirling1ColumnTest, "GenStirling1ColumnTest", SMALL);

SL void GenStirling1Test() {
  const int mod = 10007;
  assert((GenStirling1(7, mod) ==
          std::vector<int64>{0, 720, 1764, 1624, 735, 175, 21, 1}));
}
PE_REGISTER_TEST(&GenStirling1Test, "GenStirling1Test", SMALL);

SL void GenStirling2Test() {
  const int mod = 10007;
  assert((pmod::GenStirling2(7, mod) ==
          std::vector<int64>{0, 1, 63, 301, 350, 140, 21, 1}));
}
PE_REGISTER_TEST(&GenStirling2Test, "GenStirling2Test", SMALL);

SL void GetGFCoefficientTest() {
  {
    // Fibonacci sequence
    std::vector<int64> A = {1, -1, -1};
    std::vector<int64> B = {0, 1};
    std::vector<int64> result = {0, 1};
    for (int i = 2; i <= 30; ++i) {
      result.push_back(AddMod(result[i - 2], result[i - 1], mod));
    }
    std::vector<int64> x = GetGFCoefficientSeries(A, B, 30, mod);
    for (int i = 0; i <= 30; ++i) {
      assert(result[i] == x[i]);
    }
  }

  {
    // Dollar exchange.
    // Concret math
    // 7 Generating Functions
    // 7.3 Solving Recurrences
    // Example 4: A closed form for change.
    int64 dp[10000 + 1] = {1};
    int64 can[5] = {1, 5, 10, 25, 50};
    for (int64 each : can) {
      for (int j = 0; j + each <= 10000; ++j) {
        if (dp[j]) {
          dp[j + each] = AddMod(dp[j + each], dp[j], mod);
        }
      }
    }

    int64 coe[100] = {0};
    for (int i = 0; i < 1 << 5; ++i) {
      int s = 0;
      int bc = 0;
      for (int j = 0; j < 5; ++j) {
        if (i & (1 << j)) ++bc, s += (int)can[j];
      }
      if (bc & 1) {
        --coe[s];
      } else {
        ++coe[s];
      }
    }
    std::vector<int64> gfresult = GetGFCoefficientSeries(
        std::vector<int64>(coe, coe + 92), {1}, 10000, mod);
    for (int i = 0; i <= 10000; ++i) assert(dp[i] == gfresult[i]);

    std::string mine = ToString(GetGFCoefficientAt(
        std::vector<int64>(coe, coe + 92), {1}, 100000000, mod));
    std::string expected = ToString("66666793333412666685000001"_bi % mod);
    assert(mine == expected);
  }
}
PE_REGISTER_TEST(&GetGFCoefficientTest, "GetGFCoefficientTest", SMALL);

SL void LinearRecurrenceTest() {
  const int64 P = 1000000009;
  std::vector<int64> s = {0, 1, 1, 2, 3, 5};
  std::vector<int64> v = *FindLinearRecurrence(s, P);
  assert(v[0] == P - 1);
  assert(v[1] == P - 1);
  assert(v[2] == 1);
  const int n = static_cast<int>(std::size(v));
  int64 ans = 0;
  for (int i = 0; i < n; ++i) ans += v[i] * s[i];
  assert(ans == P);

  ans = LinearRecurrenceValueAt(v, s, 38, P);
  assert(ans == 39088169LL);

  std::vector<int64> t = *FindLinearRecurrence({0, 1, 1, 2, 3, 5, 8, 13}, 31);
  assert(t[0] == 30);
  assert(t[1] == 30);
  assert(t[2] == 1);
  assert(*FindLinearRecurrenceValueAt({0, 1, 1, 2, 3, 5, 8, 13}, 38, P) ==
         39088169);
}
PE_REGISTER_TEST(&LinearRecurrenceTest, "LinearRecurrenceTest", SMALL);

SL void SeqExprTest() {
  {
    Sequence a;
    (void)a;
    assert((a[1] + a[2]).ValueAt({0, 1}, 20, mod) == 6765);
    assert((a[1] + a[2]).ValueAtWithCharPoly({0, 1}, 20, mod) == 6765);
    assert((a[1] + a[2]).SumAt({0, 1}, 20, mod) == 17710);
    assert((a[1] + a[2]).SumAtWithCharPoly({0, 1}, 20, mod) == 17710);
    assert(((a[1] + a[2]).Generate({0, 1}, 20, mod) ==
            std::vector<int64>{0,   1,   1,   2,    3,    5,    8,
                               13,  21,  34,  55,   89,   144,  233,
                               377, 610, 987, 1597, 2584, 4181, 6765}));
  }
  {
    Sequence a;
    (void)a;
    assert((a[1] + a[2]).ValueAt({0, 1}, 1000, mod) == 517691607);
    assert((a[1] + a[2]).ValueAtWithCharPoly({0, 1}, 1000, mod) == 517691607);
    assert((a[1] + a[2]).SumAt({0, 1}, 1000, mod) == 625271545);
    assert((a[1] + a[2]).SumAtWithCharPoly({0, 1}, 1000, mod) == 625271545);
  }
  {
    using MT = NModCC64<mod>;
    Sequence<MT> a;
    (void)a;
    assert((a[1] + a[2]).ValueAt({0, 1}, 1000).value() == 517691607);
    assert((a[1] + a[2]).SumAt({0, 1}, 1000).value() == 625271545);
  }
}
PE_REGISTER_TEST(&SeqExprTest, "SeqExprTest", SMALL);
SL void BernoulliStirlingTableTest() {
  const int64 p = 1000000007;
  const int N = 25;
  // C(n, k) mod p
  std::vector<std::vector<int64>> c(N + 2, std::vector<int64>(N + 2));
  for (int n = 0; n <= N + 1; ++n) {
    c[n][0] = 1;
    for (int k = 1; k <= n; ++k) c[n][k] = (c[n - 1][k - 1] + c[n - 1][k]) % p;
  }
  // Bernoulli numbers with B1 = -1/2: sum_{k <= m} C(m + 1, k) B_k = 0.
  std::vector<int64> b(N + 1);
  b[0] = 1;
  for (int m = 1; m <= N; ++m) {
    int64 s = 0;
    for (int k = 0; k < m; ++k) s = (s + c[m + 1][k] * b[k]) % p;
    b[m] = MulMod(p - s, ModInv(int64(m + 1), p), p);
  }
  assert(GenBernoulliNumber(N, p) == b);
  {
    std::vector<int64> dest(N + 1);
    InitBernoulliNumber(dest.data(), N, p);
    assert(dest == b);
  }

  // Stirling numbers of the first kind (unsigned and signed) and the second
  // kind by their recurrences.
  std::vector<std::vector<int64>> s1(N + 1, std::vector<int64>(N + 1));
  std::vector<std::vector<int64>> s2(N + 1, std::vector<int64>(N + 1));
  s1[0][0] = s2[0][0] = 1;
  for (int n = 1; n <= N; ++n) {
    for (int k = 1; k <= n; ++k) {
      s1[n][k] = (s1[n - 1][k - 1] + (n - 1) * s1[n - 1][k]) % p;
      s2[n][k] = (s2[n - 1][k - 1] + k * s2[n - 1][k]) % p;
    }
  }
  for (int n = 1; n <= N; ++n) {
    const std::vector<int64> row1(std::begin(s1[n]), std::begin(s1[n]) + n + 1);
    const std::vector<int64> row2(std::begin(s2[n]), std::begin(s2[n]) + n + 1);
    std::vector<int64> signed_row(row1);
    for (int k = 0; k <= n; ++k) {
      if ((n - k) % 2 == 1) signed_row[k] = (p - signed_row[k]) % p;
    }
    assert(GenStirling1(n, p) == row1);
    assert(GenStirling1(n, p, 1) == signed_row);
    assert(pmod::GenStirling2(n, p) == row2);
  }
  for (int n = 1; n <= 6; ++n) {
    std::vector<int64> column(N + 1);
    for (int k = 0; k <= N; ++k) column[k] = s1[k][n];
    assert(pmod::GenStirling1Column(n, N, p) == column);
  }
}

PE_REGISTER_TEST(&BernoulliStirlingTableTest, "BernoulliStirlingTableTest",
                 SMALL);

SL void PowerSumModerBTest() {
  auto brute = [](int64 n, int64 k, int64 mod) {
    int64 s = 0;
    for (int64 i = 1; i <= n; ++i) s = (s + PowerMod(i, k, mod)) % mod;
    return s;
  };
  {
    const int64 p = 1000000007;
    PowerSumModerB<int64> b(p, 12);
    PowerSumModerB1<int64> b1(p, 12);
    for (int64 n = 0; n <= 60; ++n) {
      for (int64 k = 0; k <= 12; ++k) {
        const int64 expected = brute(n, k, p);
        assert(b(n, k) == expected && b.Cal(n, k) == expected);
        assert(b.CalSafe(n, k) == expected);
        assert(b1(n, k) == expected && b1.Cal(n, k) == expected);
        assert(b1.CalSafe(n, k) == expected);
      }
      // PowerSumModBatch computes k = 0..maxk at once.
      std::vector<int64> batch = PowerSumModBatch(n, 12, p);
      std::vector<int64> batch2(13);
      PowerSumModBatch(n, 12, p, batch2.data());
      assert(batch == batch2);
      for (int64 k = 0; k <= 12; ++k) assert(batch[k] == brute(n, k, p));
    }
    // Large n
    const int64 n = 1000000000000000000;
    assert(b(n, 3) == PowerSumMod(n, 3, p));
    assert(b1(n, 5) == b(n, 5));
    assert(PowerSumModBatch(n, 5, p)[5] == b(n, 5));

    // Cal(vec): the coefficients (in n) of sum_{x = 1..n} f(x) where
    // f(x) = sum vec[k] x^k.
    const std::vector<int64> f = {3, 0, 5, 1};
    const std::vector<int64> g = b(f);
    assert(std::size(g) == std::size(f) + 1);
    for (int64 m = 0; m <= 30; ++m) {
      int64 expected = 0, value = 0, pw = 1;
      for (int64 x = 1; x <= m; ++x) {
        expected += 3 + 5 * x * x + x * x * x;
      }
      for (int64 coe : g) {
        value = (value + coe * pw) % p;
        pw = pw * m % p;
      }
      assert(value == expected % p);
    }
    b.Init(4);
    assert(b.Cal(100, 4) == brute(100, 4, p));
    b1.Init(4);
    assert(b1.Cal(100, 4) == brute(100, 4, p));
  }
  {
    // CalSafe for a small prime: some of k + 2 - i and i are divisible by it.
    const int64 p = 13;
    PowerSumModerB<int64> b(p, 12);
    PowerSumModerB1<int64> b1(p, 12);
    for (int64 n = 0; n <= 200; ++n) {
      for (int64 k = 0; k <= 11; ++k) {
        const int64 expected = brute(n, k, p);
        if (n % p != 0) assert(b.CalSafe(n, k) == expected);
        assert(b1.CalSafe(n, k) == expected);
      }
    }
  }
}

PE_REGISTER_TEST(&PowerSumModerBTest, "PowerSumModerBTest", SMALL);

SL void LinearRecurrenceFamilyTest() {
  const int64 p = 1000000007;
  auto check = [&](const std::vector<int64>& terms, int64 n_max) {
    // terms[0..9] are given, the rest is the reference.
    const std::vector<int64> given(std::begin(terms), std::begin(terms) + 10);
    auto cp = FindLinearRecurrence(given, p);
    assert(cp.has_value());
    assert(VerifyLinearRecurrence(*cp, terms, p));
    auto values = FindLinearRecurrenceValues(given, n_max, p);
    assert(values.has_value() && *values == terms);
    int64 sum = 0;
    for (int64 n = 0; n <= n_max; ++n) {
      sum = (sum + terms[n]) % p;
      assert(LinearRecurrenceValueAt(*cp, given, n, p) == terms[n]);
      assert(*FindLinearRecurrenceValueAt(given, n, p) == terms[n]);
      assert(LinearRecurrenceSumAt(*cp, given, n, p) == sum);
    }
    const int order = static_cast<int>(std::size(*cp)) - 1;
    for (int64 n = order; n <= n_max; ++n) {
      std::vector<int64> prev(std::begin(terms) + n - order,
                              std::begin(terms) + n);
      assert(LinearRecurrenceValueNext(*cp, prev, p) == terms[n]);
    }
    std::vector<int64> wrong = terms;
    wrong[n_max] = (wrong[n_max] + 1) % p;
    assert(!VerifyLinearRecurrence(*cp, wrong, p));
  };
  const int64 n_max = 40;
  std::vector<int64> fib = {0, 1}, trib = {0, 0, 1}, sq, mix;
  for (int64 n = 2; n <= n_max; ++n) fib.push_back((fib[n - 1] + fib[n - 2]) % p);
  for (int64 n = 3; n <= n_max; ++n)
    trib.push_back((trib[n - 1] + trib[n - 2] + trib[n - 3]) % p);
  for (int64 n = 0; n <= n_max; ++n) {
    sq.push_back(n * n % p);  // (x - 1)^3
    mix.push_back((PowerMod(int64(2), n, p) + 3 * PowerMod(int64(3), n, p) +
                   p - n % p) % p);
  }
  check(fib, n_max);
  check(trib, n_max);
  check(sq, n_max);
  check(mix, n_max);

  // min_use: at least that many terms are used to find the relation.
  const std::vector<int64> twos = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512};
  assert(std::size(*FindLinearRecurrence(twos, p)) == 2);
  assert(std::size(*FindLinearRecurrence(twos, p, 6)) == 2);
  // No relation fits every given term.
  const std::vector<int64> bad = {1, 2, 4, 8, 16, 33};
  assert(!FindLinearRecurrence(bad, p).has_value());
  assert(!FindLinearRecurrenceValueAt(bad, 10, p).has_value());
  assert(!FindLinearRecurrenceValues(bad, 10, p).has_value());
}

PE_REGISTER_TEST(&LinearRecurrenceFamilyTest, "LinearRecurrenceFamilyTest",
                 SMALL);

SL void SeqExprFormsTest() {
  const int64 p = 1000000007;
  Sequence a;
  // a[n] = expr in a[n-1], a[n-2], ... and constants; the reference is a
  // direct iteration.
  auto check = [&](const SeqExpr<int64>& e, const std::vector<int64>& init,
                   const std::function<int64(const std::vector<int64>&)>& next,
                   bool homogeneous) {
    const int64 n_max = 30;
    std::vector<int64> ref = init;
    while (static_cast<int64>(std::size(ref)) <= n_max) ref.push_back(next(ref));
    std::vector<int64> ref_mod(ref);
    for (auto& x : ref_mod) x = (x % p + p) % p;
    std::vector<int64> init_mod(std::begin(ref_mod),
                                std::begin(ref_mod) + std::size(init));

    assert(e.Generate(init, n_max) == ref);
    assert(e.Generate(init_mod, n_max, p) == ref_mod);
    int64 sum = 0, sum_mod = 0;
    for (int64 n = 0; n <= n_max; ++n) {
      sum += ref[n];
      sum_mod = (sum_mod + ref_mod[n]) % p;
      assert(e.ValueAt(init, n) == ref[n]);
      assert(e.SumAt(init, n) == sum);
      assert(e.ValueAt(init_mod, n, p) == ref_mod[n]);
      assert(e.SumAt(init_mod, n, p) == sum_mod);
      if (homogeneous) {
        assert(e.ValueAtWithCharPoly(init_mod, n, p) == ref_mod[n]);
        assert(e.SumAtWithCharPoly(init_mod, n, p) == sum_mod);
      }
    }
  };
  // Homogeneous relations, including negative coefficients.
  check(a[1] + a[2], {0, 1},
        [](const auto& v) { return v[v.size() - 1] + v[v.size() - 2]; }, true);
  check(2 * a[1] - a(2), {1, 3},
        [](const auto& v) { return 2 * v[v.size() - 1] - v[v.size() - 2]; },
        true);
  check(a[1] * 3 - a[3] * 2, {1, -1, 2},
        [](const auto& v) { return 3 * v[v.size() - 1] - 2 * v[v.size() - 3]; },
        true);
  // Constant terms in every position.
  check(2 * a[1] + 3, {1},
        [](const auto& v) { return 2 * v.back() + 3; }, false);
  check(5 + a[1] - a[2], {1, 2},
        [](const auto& v) { return 5 + v[v.size() - 1] - v[v.size() - 2]; },
        false);
  check(a[1] - 4, {100},
        [](const auto& v) { return v.back() - 4; }, false);
  check(7 - a[1], {2},
        [](const auto& v) { return 7 - v.back(); }, false);

  // Characteristic polynomial of a[n] = a[n-1] + 2 a[n-2]: x^2 - x - 2.
  const auto rel = (a[1] + 2 * a[2]).Evaluate();
  assert((rel.ToCharPoly() == std::vector<int64>{-2, -1, 1}));
  assert((rel.ToCharPoly(p) == std::vector<int64>{p - 2, p - 1, 1}));
  assert(((a[1] + 2 * a[2]).Evaluate(p).ToCharPoly(p) ==
          std::vector<int64>{p - 2, p - 1, 1}));
}

PE_REGISTER_TEST(&SeqExprFormsTest, "SeqExprFormsTest", SMALL);
}  // namespace poly_algo_test
