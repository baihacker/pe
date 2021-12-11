#include "pe_test.h"

namespace algo_test {
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
    int64 step = sqrt(mod);
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
    int64 step = sqrt(mod);
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
    now = MulModEx(now, i, mod);
    s = AddMod(s, now, mod);
  }
  assert(ans == s);
}
PE_REGISTER_TEST(&FactSumModTest, "FactSumModTest", BIG);

SL void PowerSumTest() {
  assert(PowerSum(10, 2, 1000000007) == 385);
  assert(PowerSum(100, 100, 1000000007) == 568830579);
  assert(PowerSum(1000, 1000, 1000000007) == 918088852);
  assert(PowerSum(1000, 10000, 1000000007) == 163720385);

  assert(PowerSumSafe(10, 2, 1000000007) == 385);
  assert(PowerSumSafe(100, 100, 1000000007) == 568830579);
  assert(PowerSumSafe(1000, 1000, 1000000007) == 918088852);
  assert(PowerSumSafe(1000, 10000, 1000000007) == 163720385);
}
PE_REGISTER_TEST(&PowerSumTest, "PowerSumTest", SMALL);

SL void GpSumModTest() {
  const int64 mod = 1000000007;
  assert(GpSumMod(0, 1, 0, mod) == 0);
  assert(GpSumMod(0, 1, 1, mod) == 0);
  assert(GpSumMod(0, 1, 2, mod) == 0);
  assert(GpSumMod(0, 1, 3, mod) == 0);
  assert(GpSumMod(0, 0, 0, mod) == 1);
  assert(GpSumMod(0, 0, 0, mod, 0) == 0);
  assert(GpSumMod(0, 0, 0, mod, 100) == 100);

  assert(GpSumMod(1, 1, 0, mod) == 0);
  assert(GpSumMod(1, 1, 1, mod) == 1);
  assert(GpSumMod(1, 1, 2, mod) == 2);
  assert(GpSumMod(1, 1, 3, mod) == 3);
  assert(GpSumMod(1, 0, 0, mod) == 1);
  assert(GpSumMod(1, 0, 0, mod, 0) == 1);
  assert(GpSumMod(1, 0, 0, mod, 100) == 1);

  assert(GpSumMod(2, 1, 0, mod) == 0);
  assert(GpSumMod(2, 1, 1, mod) == 2);
  assert(GpSumMod(2, 1, 2, mod) == 6);
  assert(GpSumMod(2, 1, 3, mod) == 14);
  assert(GpSumMod(2, 0, 0, mod) == 1);
  assert(GpSumMod(2, 0, 0, mod, 0) == 1);
  assert(GpSumMod(2, 0, 0, mod, 100) == 1);
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
  const int mod = 1000000007;
  MuPhiSumModer moder(mod);

  int64 s0 = 0;
  for (int i = 1; i <= 10000000; ++i) s0 += CalPhi(i) % mod;
  s0 %= mod;

  int64 s1 = moder.CalSumPhi(10000000);
  assert(s0 == s1);
}

PE_REGISTER_TEST(&MuPhiSumModerTest, "MuPhiSumModerTest", MEDIUM);

SL void SquareFreeCounterTest() {
  const int64 n = maxp * 2;
  int64 ans1 = 0;
  for (int i = 1; i <= n; ++i) ans1 += IsSquareFree(i);

  int64 ans2 = SFCounter(maxp).Cal(n);
  assert(ans1 == ans2);
}

PE_REGISTER_TEST(&SquareFreeCounterTest, "SquareFreeCounterTest", SMALL);

SL void MValuesTest() {
  auto compute = [&](int64 /*val*/, int /*imp*/, int64 /*vmp*/,
                     MVVHistory* /*his*/, int /*top*/) -> int64 { return 1; };

  assert(ForMValues<int64>(100000000000LL, compute) == 34113193);
}

PE_REGISTER_TEST(&MValuesTest, "MValuesTest", SMALL);

SL void CountPythagoreanTripleTest() {
  // https://oeis.org/A101930
  const int64 ans[] = {2,       52,       881,       12471,      161436,
                       1980642, 23471475, 271360653, 3080075432, 34465432859};

  int64 n = 1;
  for (int i = 0; i < 10; ++i) {
    n *= 10;
    int64 t = ans[i];
    assert(CountPythagoreanTriple(n) == t);
    assert(CountPythagoreanTripleEx(n) == t);
  }
}

PE_REGISTER_TEST(&CountPythagoreanTripleTest, "CountPythagoreanTripleTest",
                 SMALL);
}  // namespace algo_test
