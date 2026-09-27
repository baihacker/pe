#include "pe_test.h"

namespace algo_test {
SL int64 ModRef(int64 x, int64 mod) { return (x % mod + mod) % mod; }

// Pascal's triangle modulo mod: C(n, m) for 0 <= m <= n <= maxn.
SL std::vector<std::vector<int64>> PascalMod(int maxn, int64 mod) {
  std::vector<std::vector<int64>> c(maxn + 1);
  for (int n = 0; n <= maxn; ++n) {
    c[n].resize(n + 1);
    c[n][0] = c[n][n] = 1 % mod;
    for (int m = 1; m < n; ++m) c[n][m] = (c[n - 1][m - 1] + c[n - 1][m]) % mod;
  }
  return c;
}

SL void BinarySearchTest() {
  const int n = 30;
  std::vector<int> vec(n);
  srand(123456789);
  for (int i = 0; i < n; ++i) vec[i] = (rand() % 20) * 2 + 1 - 20;
  for (int i = 0; i < 4; ++i) vec.push_back(vec[i]);
  sort(std::begin(vec), std::end(vec));

  const int start = 0, end = static_cast<int>(std::size(vec)) - 1;

  for (int target = -50; target <= 50; ++target) {
    std::function<int(int)> check_methods[] = {
        [&](int idx) { return vec[idx] > target; },
        [&](int idx) { return vec[idx] >= target; },
    };
    for (auto& f : check_methods) {
      int a = 0;
      while (a <= end && !f(a)) ++a;

      int b = BinarySearchFirst(start, end, f);
      assert(a == b);
    }
  }

  for (int target = -50; target <= 50; ++target) {
    std::function<int(int)> check_methods[] = {
        [&](int idx) { return vec[idx] < target; },
        [&](int idx) { return vec[idx] <= target; },
    };
    for (auto& f : check_methods) {
      int a = -1;
      while (a + 1 <= end && f(a + 1)) ++a;

      int b = BinarySearchLast(start, end, f);
      assert(a == b);
    }
  }
}
PE_REGISTER_TEST(&BinarySearchTest, "BinarySearchTest", SMALL);

SL void FactModTest() {
  {
    const int64 mod = 10007;
    FactModer moder(mod);
    int64 last = 1;
    for (int i = 1; i <= mod; ++i) {
      last = last * i % mod;
      assert(moder.Cal(i) == last);
    }
  }
#if PE_HAS_INT128
  {
    const int64 mod = 1000000007;
    FactModer moder(mod);
    int64 step = std::sqrt(mod);
    int64 n = 500000000;
    int64 last = moder.Cal(n);
    for (int i = n, j = 0; j < 10; ++i, ++j) {
      i += step;
      while (n != i) last = last * ++n % mod;
      assert(moder.Cal(n) == last);
    }
  }
  {
    const int64 mod = 4000000007;
    FactModer moder(mod);
    int64 step = std::sqrt(mod);
    int64 n = 2000000000;
    int64 last = moder.Cal(n);
    for (int64 i = n, j = 0; j < 10; ++j) {
      i += step;
      while (n != i) last = (uint64)last * ++n % mod;
      assert(moder.Cal(n) == last);
    }
  }
  {
    const int64 mod = 99999999907LL;
    FactModer moder(mod);
    assert(moder.Cal(10000000000LL) == 40583077821);
  }
#endif
}
PE_REGISTER_TEST(&FactModTest, "FactModTest", BIG);

SL void FactSumModTest() {
  const int64 mod = 99999999907;
  FactSumModer moder(mod);

  TimeRecorder tr;
  const int64 n = 100000000;
  auto ans = moder.Cal(n);
  std::cerr << tr.Elapsed().Format() << std::endl;

  int64 now = 1;
  int64 s = 1;
  for (int64 i = 1; i <= n; ++i) {
    now = MulMod(now, i, mod);
    s = AddMod(s, now, mod);
  }
  assert(ans == s);
}
PE_REGISTER_TEST(&FactSumModTest, "FactSumModTest", BIG);

SL void CombModerTest() {
  const int mod = 10007;
  CombModer m1(mod);
  CombModerEx m2(mod);
  for (int i = 0; i < 10000; ++i) {
    const int n = rand() % mod;
    const int m = rand() % mod;
    assert(m1.Comb(n, m) == m2.Comb(n, m));
  }
#if PE_HAS_INT128
  {
    const int64 mod = 99999999907;
    CombModerEx moder(mod);
    const int64 n = 66666666604;
    const int64 m = 33333333302;
    auto result = moder.Comb(n, m);

    assert(result == 99999410307);
  }
#endif
}
PE_REGISTER_TEST(&CombModerTest, "CombModerTest", MEDIUM);

SL void PowerSumTest() {
  assert(PowerSumMod(10, 2, 1000000007) == 385);
  assert(PowerSumMod(100, 100, 1000000007) == 568830579);
  assert(PowerSumMod(1000, 1000, 1000000007) == 918088852);
  assert(PowerSumMod(1000, 10000, 1000000007) == 163720385);

  assert(PowerSumModSafe(10, 2, 1000000007) == 385);
  assert(PowerSumModSafe(100, 100, 1000000007) == 568830579);
  assert(PowerSumModSafe(1000, 1000, 1000000007) == 918088852);
  assert(PowerSumModSafe(1000, 10000, 1000000007) == 163720385);
}
PE_REGISTER_TEST(&PowerSumTest, "PowerSumTest", SMALL);

SL void GpSumModTest() {
  const int64 mod = 1000000007;
  // GpSumMod(x, a, b, mod) = x^a + x^(a+1) + ... + x^b, and 0^0 is
  // zero_p_zero (default 1).
  for (int64 x = 0; x <= 3; ++x) {
    for (int64 a = 0; a <= 3; ++a) {
      for (int64 b = 0; b <= 6; ++b) {
        for (int64 zpz : {0, 1, 100}) {
          int64 expected = 0;
          for (int64 i = a; i <= b; ++i) {
            expected += x == 0 && i == 0 ? zpz : PowerMod(x, i, mod);
          }
          assert(GpSumMod(x, a, b, mod, zpz) == expected % mod);
          if (zpz == 1) assert(GpSumMod(x, a, b, mod) == expected % mod);
        }
      }
    }
  }
}

PE_REGISTER_TEST(&GpSumModTest, "GpSumModTest", SMALL);

SL void PkSumModTest() {
  // 4e18 + 37
  const int64 mod = 4000000000000000037;
  PowerSumModerB moder(mod, 7);
  auto p1_impl = [=](int64 n, int64 mod) -> int64 { return P1SumMod(n, mod); };
  std::function<int64(int64, int64)> them[]{p1_impl,   p1_impl,   &P2SumMod,
                                            &P3SumMod, &P4SumMod, &P5SumMod,
                                            &P6SumMod, &P7SumMod};
  for (int k = 1; k <= 7; ++k) {
    for (int offset = -100; offset < 100; ++offset) {
      const int64 n = mod + offset;
      const int64 ans1 = (them[k])(n, mod);
      const int64 ans2 = moder.Cal(n, k);
      if (ans1 != ans2) {
        std::cout << n << " " << k << " " << ans1 << " " << ans2 << std::endl;
      }
      assert(ans1 == ans2);
    }
  }
}

PE_REGISTER_TEST(&PkSumModTest, "PkSumModTest", SMALL);

SL void MuPhiSumModerTest() {
  const int64 mod = 1000000007;
  // A small pivot makes CalSumMu / CalSumPhi recurse above 1000.
  MuPhiSumModer moder(mod, 1000);
  const std::set<int64> checkpoints = {1, 2, 999, 1000, 1001, 12345, maxp};
  int64 sum_mu = 0, sum_phi = 0;
  for (int64 i = 1; i <= maxp; ++i) {
    sum_mu += mu[i];
    sum_phi = (sum_phi + phi[i]) % mod;
    if (checkpoints.count(i)) {
      assert(moder.CalSumMu(i) == ModRef(sum_mu, mod));
      assert(moder.CalSumPhi(i) == sum_phi);
    }
  }
}

PE_REGISTER_TEST(&MuPhiSumModerTest, "MuPhiSumModerTest", SMALL);

SL void SquareFreeCounterTest() {
  // Q(n) = sum_{d <= sqrt(n)} mu(d) floor(n / d^2)
  auto q_ref = [](int64 n) {
    int64 ret = 0;
    for (int64 d = 1; d * d <= n; ++d) ret += mu[d] * (n / (d * d));
    return ret;
  };
  SFCounter small_pivot(1000);
  SFCounter default_pivot;
  int64 cnt = 0;
  for (int64 n = 1; n <= 100000; ++n) {
    cnt += IsSquareFree(n);
    if (n % 997 == 0 || n <= 1001) assert(small_pivot.Cal(n) == cnt);
  }
  for (int64 n : {int64(100000000), int64(1000000000000)}) {
    assert(small_pivot(n) == q_ref(n));
    assert(default_pivot(n) == q_ref(n));
  }
}

PE_REGISTER_TEST(&SquareFreeCounterTest, "SquareFreeCounterTest", SMALL);

SL void MValuesTest() {
  auto compute = [&](int64 /*n*/, int64 /*val*/, int /*imp*/, int64 /*vmp*/,
                     int /*emp*/, MVVHistory* /*his*/,
                     int /*top*/) -> int64 { return 1; };

  // The visited values are {v : v * P(v) <= n} where P(v) is the largest
  // prime factor of v and P(1) = 1.
  for (int64 n : {1, 2, 10, 1000, 1000000}) {
    int64 expected = 0;
    for (int64 v = 1; v <= n; ++v) {
      const int64 pv = v == 1 ? 1 : Factorize(v).back().first;
      if (v * pv <= n) ++expected;
    }
    assert(ForMValues<int64>(n, compute) == expected);
  }
}

PE_REGISTER_TEST(&MValuesTest, "MValuesTest", SMALL);

SL void CountPythagoreanTripleTest() {
  // https://oeis.org/A101930
  const int64 ans[] = {2,       52,       881,       12471,      161436,
                       1980642, 23471475, 271360653, 3080075432, 34465432859};

  int64 n = 1;
  for (int i = 0; i < 9; ++i) {
    n *= 10;
    int64 t = ans[i];
    assert(CountPythagoreanTriple(n) == t);
    assert(CountPythagoreanTripleEx(n) == t);
  }
}

PE_REGISTER_TEST(&CountPythagoreanTripleTest, "CountPythagoreanTripleTest",
                 SMALL);

SL void FindRecurrenceTest() {
  const int64 mod = 1000000007;
  // Ones
  assert(*FindRecurrenceValueAt({1, 1, 1, 1, 1}, 100, mod, 1) == 1);

  // Factorials
  assert(*FindRecurrenceValueAt({1, 1, 2, 6, 24, 120}, 100, mod, 1) ==
         437918130);

  // Catalan numbers
  assert(*FindRecurrenceValueAt({1, 1, 2, 5, 14, 42}, 100, mod, 1) ==
         558488487);

  // Subfactorials
  assert(*FindRecurrenceValueAt({1, 0, 1, 2, 9, 44, 265}, 100, mod, 1) ==
         944828409);

  // Motzkin numbers
  assert(*FindRecurrenceValueAt({1, 1, 2, 4, 9, 21, 51}, 100, mod, 1) ==
         345787718);

  // Large SchrÃ¶der numbers
  assert(*FindRecurrenceValueAt({1, 2, 6, 22, 90, 394, 1806}, 100, mod, 1) ==
         532944014);

  // Hertzsprung's problem: order 4, degree 1
  assert(
      *FindRecurrenceValueAt({1, 1, 0, 0, 2, 14, 90, 646, 5242, 47622, 479306,
                              5296790, 63779034, 831283558, 661506141},
                             100, mod, 1) == 251310489);
}
PE_REGISTER_TEST(&FindRecurrenceTest, "FindRecurrenceTest", SMALL);

SL void InitCombTest() {
  const int64 mod = 1000000007;
  const int MAXN = 30;
  int64 comb[MAXN + 1][MAXN + 1] = {};
  InitComb(comb, MAXN, mod);

  // Pascal's triangle identity
  for (int n = 0; n <= MAXN; ++n) {
    assert(comb[n][0] == 1 % mod);
    assert(comb[n][n] == 1 % mod);
    for (int k = 1; k < n; ++k) {
      assert(comb[n][k] == (comb[n - 1][k - 1] + comb[n - 1][k]) % mod);
    }
  }

  // Known values
  assert(comb[5][2] == 10);
  assert(comb[10][5] == 252);
  assert(comb[20][10] == 184756);
}

PE_REGISTER_TEST(&InitCombTest, "InitCombTest", SMALL);

SL void InitSeqProd2Test() {
  const int64 mod = 1000000007;
  const int n = 100;
  std::vector<int64> fac(n + 1), ifac(n + 1);
  InitSeqProd2<int64>(std::data(fac), std::data(ifac), int64(1), int64(n),
                      mod);

  // fac[0]=1, fac[k]=k! mod p
  assert(fac[0] == 1);
  int64 expected = 1;
  for (int i = 1; i <= n; ++i) {
    expected = expected * i % mod;
    assert(fac[i] == expected);
  }

  // ifac[k] is the modular inverse of fac[k]
  assert(ifac[0] == 1);
  for (int i = 1; i <= n; ++i) {
    assert(fac[i] * ifac[i] % mod == 1);
  }
}

PE_REGISTER_TEST(&InitSeqProd2Test, "InitSeqProd2Test", SMALL);

SL void LinearRecurrenceSumTest() {
  const int64 mod = 1000000007;
  // Fibonacci {1,1,2,3,5,...}: char poly for x^2 - x - 1 (mod p) is
  // {mod-1, mod-1, 1}
  std::vector<int64> char_poly = {mod - 1, mod - 1, 1};
  std::vector<int64> terms = {1, 1};

  // f10 = 89
  assert(LinearRecurrenceValueAt(char_poly, terms, 10, mod) == 89);

  // Sum f0..f10 = 1+1+2+3+5+8+13+21+34+55+89 = 232
  assert(LinearRecurrenceSumAt(char_poly, terms, 10, mod) == 232);

  // Edge cases inside the initial terms
  assert(LinearRecurrenceSumAt(char_poly, terms, 0, mod) == 1);
  assert(LinearRecurrenceSumAt(char_poly, terms, 1, mod) == 2);

  // Identity for this sequence: sum(f0..fn) = f(n+2) - 1
  for (int n = 2; n <= 20; ++n) {
    int64 f_n2 = LinearRecurrenceValueAt(char_poly, terms, n + 2, mod);
    int64 sum_n = LinearRecurrenceSumAt(char_poly, terms, n, mod);
    assert(sum_n == (f_n2 - 1 + mod) % mod);
  }
}

PE_REGISTER_TEST(&LinearRecurrenceSumTest, "LinearRecurrenceSumTest", SMALL);

SL void PowerModerTest() {
  // x must be in [0, mod) (see the TODO(bug) in PowerModer::Init).
  for (int64 mod : {int64(1), int64(1000003), int64(1000000007),
                    int64(4000000000000000037)}) {
    for (int64 x : {int64(0), int64(1), int64(2), mod - 1, mod / 3}) {
      if (x >= mod) continue;
      PowerModer<int64> pm(x, mod);
      PowerModerEx<int64> pme(x, mod, 1000);
      for (int64 n : {int64(0), int64(1), int64(5), int64(511), int64(512),
                      int64(99999), int64(100000), int64(100001),
                      int64(123456789), int64(1000000000000000000)}) {
        const int64 expected = PowerMod(x, n, mod);
        assert(pm(n) == expected && pm.Cal(n) == expected);
        assert(pme(n) == expected && pme.Cal(n) == expected);
      }
    }
  }
  // Init(f) rebuilds the tables with the same x and mod.
  PowerModer<int64> pm(3, 1000000007);
  pm.Init(200000);
  PowerModerEx<int64> pme(3, 1000000007);
  pme.Init(5000);
  for (int64 n : {int64(150000), int64(4097), int64(1) << 40}) {
    assert(pm(n) == PowerMod(int64(3), n, int64(1000000007)));
    assert(pme(n) == PowerMod(int64(3), n, int64(1000000007)));
  }
}

PE_REGISTER_TEST(&PowerModerTest, "PowerModerTest", SMALL);

SL void CombModTest() {
  // CombModer (small prime) and CombModerEx against Pascal's triangle; n
  // beyond p uses Lucas' theorem.
  for (int64 p : {int64(13), int64(10007)}) {
    const auto c = PascalMod(400, p);
    CombModer<int64> cm(p);
    CombModerEx<int64> cme(p);
    for (int n = 0; n <= 400; ++n) {
      for (int m = 0; m <= n; ++m) {
        assert(cm(n, m) == c[n][m] && cm.Comb(n, m) == c[n][m]);
        if (n % 13 == 0 && m % 7 == 0) {
          assert(cme.Comb(n, m) == c[n][m] && cme.Cal(n, m) == c[n][m]);
        }
      }
    }
    assert(cm(5, 7) == 0 && cm(5, -1) == 0 && cm(-1, 0) == 0);
    assert(cme.Cal(5, 7) == 0);
  }
  {
    // A table size f < p only covers n <= f.
    CombModer<int64> cm(10007, 100);
    const auto c = PascalMod(100, 10007);
    for (int n = 0; n <= 100; ++n)
      for (int m = 0; m <= n; ++m) assert(cm(n, m) == c[n][m]);
    cm.Init(10006);
    assert(cm(10006, 5003) == CombModer<int64>(10007)(10006, 5003));
  }

  // ShortCombMod / ShortComb (n < 2^32 and small m, see the TODO(bug)s).
  {
    // ShortCombMod is O(m^2) per call.
    const auto c = PascalMod(60, 1000000007);
    for (int n = 0; n <= 60; ++n) {
      for (int m = -1; m <= n + 1; ++m) {
        const int64 expected = m < 0 || m > n ? 0 : c[n][m];
        assert(ShortCombMod(n, m, 1000000007) == expected);
      }
    }
    for (int n = 0; n <= 50; ++n) {
      int64 row = 1;  // C(n, m) computed exactly
      for (int m = 0; m <= n; ++m) {
        assert(ShortComb<int64>(n, m) == row);
        row = row * (n - m) / (m + 1);
      }
      assert(ShortComb<int64>(n, n + 1) == 0);
    }
  }

  // Prime powers
  for (auto [p, e] :
       std::vector<std::pair<int64, int64>>{{2, 5}, {3, 4}, {5, 2}, {7, 1}}) {
    const int64 mod = Power(p, e);
    const auto c = PascalMod(120, mod);
    CombModerPrimePower<int64> cm(p, e);
    for (int n = 0; n <= 120; ++n) {
      for (int m = 0; m <= n; ++m) {
        assert(cm(n, m) == c[n][m] && cm.Comb(n, m) == c[n][m]);
      }
    }
    assert(cm.Cal(3, 4) == 0);
  }
  for (int64 p : {int64(2), int64(3), int64(7), int64(101)}) {
    const auto c = PascalMod(120, p * p);
    CombModerPrimeSquare<int64> cm(p);
    for (int n = 0; n <= 120; ++n) {
      for (int m = 0; m <= n; ++m) {
        assert(cm(n, m) == c[n][m] && cm.Comb(n, m) == c[n][m]);
      }
    }
    assert(cm.Cal(3, 4) == 0);
  }
}

PE_REGISTER_TEST(&CombModTest, "CombModTest", SMALL);

SL void PowerSumModerTest() {
  auto brute = [](int64 n, int64 k, int64 mod) {
    int64 s = 0;
    for (int64 i = 1; i <= n; ++i) s = (s + PowerMod(i, k, mod)) % mod;
    return s;
  };
  {
    const int64 mod = 1000000007;
    PowerSumModer<int64> moder(mod, 10);
    for (int64 n = 0; n <= 60; ++n) {
      for (int64 k = 0; k <= 10; ++k) {
        assert(moder(n, k) == brute(n, k, mod));
        assert(moder.CalSafe(n, k) == brute(n, k, mod));
      }
    }
  }
  {
    // CalSafe also works when some of n + 1, n, ..., n - k + 1 are divisible
    // by mod.
    const int64 mod = 13;
    PowerSumModer<int64> moder(mod, 10);
    for (int64 n = 0; n <= 200; ++n) {
      for (int64 k = 0; k <= 10; ++k) {
        assert(moder.CalSafe(n, k) == brute(n, k, mod));
      }
    }
    moder.Init(4);
    assert(moder.CalSafe(100, 4) == brute(100, 4, mod));
  }
}

PE_REGISTER_TEST(&PowerSumModerTest, "PowerSumModerTest", SMALL);

#if PE_HAS_INT128
SL void PatternNumberCounterTest() {
  const int64 n = 100000;
  // Prime signatures (exponents in descending order) of 1..n.
  std::map<std::vector<int>, int64> cnt;
  for (int64 x = 2; x <= n; ++x) {
    std::vector<int> sig;
    for (auto [p, e] : Factorize(x)) sig.push_back(e);
    std::sort(std::rbegin(sig), std::rend(sig));
    ++cnt[sig];
  }
  // Only the first prime_cnt primes are tried for all but the last exponent;
  // the first 70 primes (up to 349) cover every such p, which is at most
  // sqrt(n) = 316.
  PatternNumberCounter counter;
  counter.Init(70);
  for (const std::vector<int>& pattern :
       std::vector<std::vector<int>>{{1}, {2}, {3}, {1, 1}, {2, 1}, {1, 2},
                                     {2, 2}, {1, 1, 1}, {3, 2}, {2, 1, 1}}) {
    std::vector<int> sig = pattern;
    std::sort(std::rbegin(sig), std::rend(sig));
    assert(counter.Cal(pattern, n) == cnt[sig]);
    assert(counter(pattern, n) == cnt[sig]);
  }
}

PE_REGISTER_TEST(&PatternNumberCounterTest, "PatternNumberCounterTest", SMALL);
#endif

SL void PerfectPowerCounterTest() {
  // f(x) = Gcd of the exponents of x, f(1) = 0.
  const int64 n = 1000000;
  std::vector<std::vector<int64>> acc(10, std::vector<int64>(n + 1));
  for (int64 x = 1; x <= n; ++x) {
    int d = 0;
    for (auto [p, e] : Factorize(x)) d = Gcd(d, e);
    for (int j = 0; j < 10; ++j) acc[j][x] = acc[j][x - 1] + (d == j);
  }
  // A small pivot makes Cal recurse above 1000.
  PerfectPowerCounter<int64> counter(1000);
  for (int64 m : {int64(1), int64(4), int64(1000), int64(1001), int64(65536),
                  int64(999999), n}) {
    for (int d = 0; d < 10; ++d) {
      assert(counter.Cal(m, d) == acc[d][m]);
    }
  }
  assert(counter.Cal(500, 25) == 0);
  assert(counter.Cal(n, 200) == 0);
}

PE_REGISTER_TEST(&PerfectPowerCounterTest, "PerfectPowerCounterTest", SMALL);

SL void NotDivTest() {
  auto brute = [](int64 n, const std::vector<int64>& L) {
    int64 ret = 0;
    for (int64 i = 1; i <= n; ++i) {
      bool ok = true;
      for (int64 x : L) ok = ok && i % x != 0;
      ret += ok;
    }
    return ret;
  };
  NotDivCounter<int64> counter;
  // Not pairwise coprime, unsorted
  for (const auto& L : std::vector<std::vector<int64>>{
           {}, {4}, {6, 4}, {2, 3, 5}, {10, 6, 15}, {12, 18, 8, 27}, {9, 3}}) {
    for (int64 n : {0, 1, 17, 360, 1000, 2999}) {
      assert(counter.NotDiv(n, L) == brute(n, L));
    }
  }
  // Pairwise coprime
  for (const auto& L : std::vector<std::vector<int64>>{
           {}, {7}, {5, 3}, {4, 9, 25}, {11, 2, 3, 35}}) {
    std::vector<int> bc(1 << std::size(L));
    for (int i = 1; i < static_cast<int>(std::size(bc)); ++i) {
      bc[i] = bc[i >> 1] + (i & 1);
    }
    for (int64 n : {0, 1, 17, 360, 1000, 2999}) {
      const int64 expected = brute(n, L);
      assert(counter.NotDivCoprime(n, L) == expected);
      assert(CountNotDiv(n, L) == expected);
      assert(CountNotDiv(n, L, bc.data()) == expected);
    }
  }
  assert(CountNotDiv(int64(-5), std::vector<int64>{2}) == -5);
}

PE_REGISTER_TEST(&NotDivTest, "NotDivTest", SMALL);

// Counts x <= n: every x has exactly one M-value x / P(x).
struct CountVisitor : public MValueVisitor<CountVisitor> {
  int64 Visit(int64 n, int64 val, int /*imp*/, int64 vmp, int /*emp*/,
              MVVHistory* /*his*/, int top) {
    // x = val * q with q prime, vmp <= q <= n / val; plus x = 1.
    if (top == 0) return dva[n] + 1;
    return dva[n / val] - dva[vmp - 1];
  }
  void Init(int64 n) { dva = PrimeS0Ex<int64>(n); }
  DVA<int64> dva;
};

#if ENABLE_OPENMP
struct CountVisitorParallel
    : public MValueVisitor<CountVisitorParallel, int64, 4> {
  int64 Visit(int64 n, int64 val, int imp, int64 vmp, int emp,
              MVVHistory* his, int top) {
    return impl.Visit(n, val, imp, vmp, emp, his, top);
  }
  void Init(int64 n) { impl.Init(n); }
  CountVisitor impl;
};
#endif

// Sums d(x) for x <= n, d(x) is accumulated along the prime factors.
struct DivisorSumVisitor : public MValueVisitorEx<DivisorSumVisitor> {
  int64 Visit(int64 n, int64 val, int /*imp*/, int64 vmp, int emp,
              MVVHistory* /*his*/, int top, int64 now, int64 now1) {
    if (top == 0) return 1 + 2 * dva[n];
    // q > vmp: d(val * q) = 2 d(val); q = vmp: the exponent of vmp grows.
    return now * 2 * (dva[n / val] - dva[vmp]) + now1 * (emp + 2);
  }
  int64 AccumulateValue(int64 init, int64 /*p*/, int64 e, int64 /*pd*/) {
    return init * (e + 1);
  }
  void Init(int64 n) { dva = PrimeS0Ex<int64>(n); }
  DVA<int64> dva;
};

// Multiplicative functions with MValueBaseLite: F(p, e) only depends on e.
template <bool small_to_large, int TN = 1>
struct DivisorCount
    : public MValueBaseLite<DivisorCount<small_to_large, TN>, int64, TN,
                            small_to_large> {
  int64 F(int64 /*p*/, int64 e) { return e + 1; }
};

template <bool small_to_large>
struct SquareFreeCount
    : public MValueBaseLite<SquareFreeCount<small_to_large>, int64, 1,
                            small_to_large> {
  int64 F(int64 /*p*/, int64 e) { return e == 1 ? 1 : 0; }
};

// f(x) = x with MValueBase and an explicit BatchF (sum of primes).
template <bool small_to_large>
struct IdentitySum
    : public MValueBase<IdentitySum<small_to_large>, int64, 1, small_to_large> {
  int64 F(int64 p, int64 e) { return Power(p, e); }
  int64 BatchF(int /*imp*/, int64 vmp, int64 remain) {
    return dva[remain] - (vmp == 1 ? 0 : dva[vmp]);
  }
  void Init(int64 n) { dva = PrimeS1Ex<int64>(n); }
  DVA<int64> dva;
};

SL void MValueTest() {
  auto divisor_sum = [](int64 n) {
    int64 s = 0;
    for (int64 i = 1; i <= n; ++i) s += n / i;
    return s;
  };
  auto square_free = [](int64 n) {
    int64 ret = 0;
    for (int64 d = 1; d * d <= n; ++d) ret += mu[d] * (n / (d * d));
    return ret;
  };
  for (int64 n : {int64(1), int64(2), int64(10), int64(1000),
                  int64(10000000)}) {
    assert(CountVisitor().Cal(n) == n);
#if ENABLE_OPENMP
    assert(CountVisitorParallel().Cal(n) == n);
#endif
    const int64 ds = divisor_sum(n);
    assert(DivisorSumVisitor().Cal(n) == ds);
    assert(DivisorCount<false>().Cal(n) == ds);
    assert(DivisorCount<true>().Cal(n) == ds);
#if ENABLE_OPENMP
    assert((DivisorCount<false, 4>().Cal(n)) == ds);
    assert((DivisorCount<true, 4>().Cal(n)) == ds);
#endif
    assert(SquareFreeCount<false>().Cal(n) == square_free(n));
    assert(SquareFreeCount<true>().Cal(n) == square_free(n));
    if (n <= 1000000) {
      assert(IdentitySum<false>().Cal(n) == n * (n + 1) / 2);
      assert(IdentitySum<true>().Cal(n) == n * (n + 1) / 2);
    }
  }
}

PE_REGISTER_TEST(&MValueTest, "MValueTest", SMALL);

SL void DfaTest() {
  auto has3 = [](int64 x) {
    for (; x; x /= 10)
      if (x % 10 == 3) return 1;
    return 0;
  };
  // state 0: initial, 1: 3 not seen, 2: 3 seen. Leading zeros stay in 0.
  auto build1 = [](auto& dfa) {
    for (int i = 1; i <= 9; ++i)
      if (i != 3) dfa.AddTrans(0, i, 1);
    dfa.AddTrans(0, 3, 2);
    dfa.AddTrans(0, 0, 0);
    for (int i = 0; i <= 9; ++i)
      if (i != 3) dfa.AddTrans(1, i, 1);
    dfa.AddTrans(1, 3, 2);
    for (int i = 0; i <= 9; ++i) dfa.AddTrans(2, i, 2);
    dfa.MarkTargetState(2);
  };
  // As above, but a leading zero goes to the dead state 3, so
  // set_count_each_len(1) is needed.
  auto build2 = [](auto& dfa) {
    for (int i = 1; i <= 9; ++i)
      if (i != 3) dfa.AddTrans(0, i, 1);
    dfa.AddTrans(0, 3, 2);
    for (int i = 0; i <= 9; ++i)
      if (i != 3) dfa.AddTrans(1, i, 1);
    dfa.AddTrans(1, 3, 2);
    for (int i = 0; i <= 9; ++i) dfa.AddTrans(2, i, 2);
    dfa.MarkTargetState(2);
    dfa.AddTrans(0, 0, 3);
    for (int i = 0; i <= 9; ++i) dfa.AddTrans(3, i, 3);
    dfa.set_count_each_len(1);
  };

  DfaCounter<int64> c1, c2, c3;
  c1.Init(3, 10, 8);
  build1(c1);
  c1.Prepare();
  c2.Init(4, 10, 8);
  build2(c2);
  // c3 is c1 with the states shifted by one: the initial state is 1.
  c3.Init(4, 10, 8, 1);
  for (int i = 0; i <= 9; ++i) c3.AddTrans(0, i, 0);
  for (int i = 1; i <= 9; ++i)
    if (i != 3) c3.AddTrans(1, i, 2);
  c3.AddTrans(1, 3, 3).AddTrans(1, 0, 1);
  for (int i = 0; i <= 9; ++i)
    if (i != 3) c3.AddTrans(2, i, 2);
  c3.AddTrans(2, 3, 3);
  for (int i = 0; i <= 9; ++i) c3.AddTrans(3, i, 3);
  c3.MarkTargetState(3).SetInitState(1);

  std::vector<int64> accepted;
  int64 cnt = 0;
  for (int64 n = 0; n <= 20000; ++n) {
    if (has3(n)) {
      ++cnt;
      accepted.push_back(n);
    }
    if (n % 97 == 0 || n < 100) {
      assert(c1.Cal(n) == cnt && c2.Cal(n) == cnt && c3.Cal(n) == cnt);
    }
  }
  assert(c1.Cal(std::vector<int>{3, 2, 1}) == c1.Cal(123));
  for (int64 k = 1; k <= static_cast<int64>(std::size(accepted)); k += 37) {
    const int64 expected = accepted[k - 1];
    assert(c1.CalKth<int64>(k) == expected);
    assert(c2.CalKth<int64>(k) == expected);
    assert(c1.CalKthEx<int64>(k) == expected);
    assert(c2.CalKthEx<int64>(k) == expected);
  }

  // DfaSummer: sums x^p for accepted x <= n.
  DfaSummer<int64> s1, s2;
  s1.Init(3, 3, 10, 8);
  build1(s1);
  s2.Init(4, 3, 10, 8);
  build2(s2);
  s2.Prepare();
  int64 sum[4] = {};
  for (int64 n = 0; n <= 3000; ++n) {
    if (has3(n)) {
      for (int p = 0; p < 4; ++p) sum[p] += Power(n, p);
    }
    if (n % 101 == 0 || n < 50) {
      for (int p = 1; p <= 3; ++p) {
        assert(s1.Cal(n, p) == sum[p] && s2.Cal(n, p) == sum[p]);
      }
      // target_power == 0 means the max power.
      assert(s1.Cal(n) == sum[3]);
    }
  }
  assert(s1.Cal(std::vector<int>{3, 2, 1}, 2) == s1.Cal(123, 2));
}

PE_REGISTER_TEST(&DfaTest, "DfaTest", SMALL);

SL void CarlitzWordsCounterTest() {
  auto brute = [](const std::vector<int64>& counts) {
    std::vector<int> word;
    for (int i = 0; i < static_cast<int>(std::size(counts)); ++i)
      for (int j = 0; j < counts[i]; ++j) word.push_back(i);
    int64 ret = 0;
    do {
      bool ok = true;
      for (size_t i = 1; i < std::size(word); ++i) {
        ok = ok && word[i] != word[i - 1];
      }
      ret += ok;
    } while (std::next_permutation(std::begin(word), std::end(word)));
    return ret;
  };
  const int64 mod = 1000000007;
  CarlitzWordsCounter counter(mod, 20);
  for (const auto& v : std::vector<std::vector<int64>>{
           {}, {0}, {1}, {2}, {1, 1}, {2, 1}, {2, 2}, {3, 1}, {1, 1, 1},
           {2, 1, 1}, {2, 2, 2}, {3, 2, 1}, {0, 2, 0, 2}, {1, 1, 1, 1, 1},
           {3, 3, 2}, {2, 2, 2, 2}}) {
    assert(counter.Cal(v) == brute(v));
    // Cached
    assert(counter.Cal(v) == brute(v));
  }
}

PE_REGISTER_TEST(&CarlitzWordsCounterTest, "CarlitzWordsCounterTest", SMALL);

SL void RecurrenceTest() {
  const int64 mod = 1000000007;
  // Catalan numbers: (n + 2) C(n + 1) = (4n + 2) C(n).
  std::vector<int64> catalan = {1};
  for (int64 n = 0; n < 40; ++n) {
    catalan.push_back(MulMod(catalan.back() * (4 * n + 2) % mod,
                             ModInv(n + 2, mod), mod));
  }
  const std::vector<int64> terms(std::begin(catalan), std::begin(catalan) + 10);

  // min_deg = 1: with degree 0, 10 terms always fit some linear recurrence
  // with constant coefficients, which is not the Catalan one.
  auto rec = FindRecurrence(terms, mod, 1);
  assert(rec.has_value());
  assert(VerifyRecurrence(*rec, terms, mod));
  // A wrong term is detected.
  std::vector<int64> wrong = terms;
  wrong[9] += 1;
  assert(!VerifyRecurrence(*rec, wrong, mod));
  // RecurrenceValueNext gives the term after the last `order` terms.
  const int order = static_cast<int>(std::size(*rec)) - 1;
  for (int i = order; i < 40; ++i) {
    std::vector<int64> prev(std::begin(catalan) + i - order,
                            std::begin(catalan) + i);
    assert(RecurrenceValueNext(*rec, prev, i, mod) == catalan[i]);
  }

  auto values = FindRecurrenceValues(terms, 40, mod, 1);
  assert(values.has_value() && *values == catalan);
  assert(*FindRecurrenceValueAt(terms, 40, mod, 1) == catalan[40]);
  // n inside the given terms
  assert(*FindRecurrenceValueAt(terms, 3, mod) == catalan[3]);

  // Degree 0 only: Fibonacci numbers need no polynomial coefficients.
  const std::vector<int64> fib = {0, 1, 1, 2, 3, 5, 8, 13};
  assert(*FindRecurrenceValueAt(fib, 30, mod, 0, 0) == 832040);
  // Degree 1 only: n! = n (n - 1)!.
  std::vector<int64> fac = {1};
  for (int64 i = 1; i < 8; ++i) fac.push_back(fac.back() * i);
  assert(*FindRecurrenceValueAt(fac, 12, mod, 1, 1) == 479001600);

  // Too few terms.
  assert(!FindRecurrence(std::vector<int64>{5}, mod).has_value());
  assert(!FindRecurrenceValues(std::vector<int64>{5}, 10, mod).has_value());
  assert(!FindRecurrenceValueAt(std::vector<int64>{5}, 10, mod).has_value());
}

PE_REGISTER_TEST(&RecurrenceTest, "RecurrenceTest", SMALL);

// FindSurrealNumber doesn't compile with clang (see the TODO(bug) in
// pe_fraction).
#if !defined(COMPILER_CLANG)
SL void SurrealNumberTest() {
  using F = Fraction<int64>;
  const std::optional<F> none;
  // The simplest number between the left and the right options.
  assert(*FindSurrealNumber<int64>(none, none) == F(0));
  assert(*FindSurrealNumber<int64>(F(1), none) == F(2));
  assert(*FindSurrealNumber<int64>(F(3, 2), none) == F(2));
  assert(*FindSurrealNumber<int64>(F(-3, 2), none) == F(0));
  assert(*FindSurrealNumber<int64>(none, F(-1)) == F(-2));
  assert(*FindSurrealNumber<int64>(none, F(3)) == F(0));
  assert(*FindSurrealNumber<int64>(F(-1), F(1)) == F(0));
  assert(*FindSurrealNumber<int64>(F(1), F(2)) == F(3, 2));
  assert(*FindSurrealNumber<int64>(F(1, 2), F(1)) == F(3, 4));
  assert(*FindSurrealNumber<int64>(F(-2), F(-1)) == F(-3, 2));
  assert(!FindSurrealNumber<int64>(F(2), F(1)).has_value());
  assert(!FindSurrealNumber<int64>(F(1), F(1)).has_value());
}

PE_REGISTER_TEST(&SurrealNumberTest, "SurrealNumberTest", SMALL);
#endif
}  // namespace algo_test
