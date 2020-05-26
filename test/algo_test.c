#include "pe_test.h"

namespace algo_test {
SL void fact_mod_test() {
  {
    const int64 mod = 10007;
    FactModer moder(mod);
    int64 last = 1;
    for (int i = 1; i <= mod; ++i) {
      last = last * i % mod;
      assert(moder.cal(i) == last);
    }
  }
#if PE_HAS_INT128
  {
    const int64 mod = 1000000007;
    FactModer moder(mod);
    int64 step = sqrt(mod);
    int64 n = 500000000;
    int64 last = moder.cal(n);
    for (int i = n, j = 0; j < 10; ++i, ++j) {
      i += step;
      while (n != i) last = last * ++n % mod;
      assert(moder.cal(n) == last);
    }
  }
  {
    const int64 mod = 4000000007;
    FactModer moder(mod);
    int64 step = sqrt(mod);
    int64 n = 2000000000;
    int64 last = moder.cal(n);
    for (int64 i = n, j = 0; j < 10; ++j) {
      i += step;
      while (n != i) last = (uint64)last * ++n % mod;
      assert(moder.cal(n) == last);
    }
  }
  {
    const int64 mod = 99999999907LL;
    FactModer moder(mod);
    assert(moder.cal(10000000000LL) == 40583077821);
  }
#endif
}
PE_REGISTER_TEST(&fact_mod_test, "fact_mod_test", BIG);

SL void fact_sum_mod_test() {
  const int64 mod = 99999999907;
  FactSumModer moder(mod);

  TimeRecorder tr;
  const int64 n = 100000000;
  auto ans = moder.cal(n);
  cerr << tr.elapsed().format() << endl;

  int64 now = 1;
  int64 s = 1;
  for (int64 i = 1; i <= n; ++i) {
    now = mul_mod_ex(now, i, mod);
    s = add_mod(s, now, mod);
  }
  assert(ans == s);
}
PE_REGISTER_TEST(&fact_sum_mod_test, "fact_sum_mod_test", BIG);

SL void power_sum_test() {
  assert(power_sum(10, 2, 1000000007) == 385);
  assert(power_sum(100, 100, 1000000007) == 568830579);
  assert(power_sum(1000, 1000, 1000000007) == 918088852);
  assert(power_sum(1000, 10000, 1000000007) == 163720385);

  assert(power_sum_safe(10, 2, 1000000007) == 385);
  assert(power_sum_safe(100, 100, 1000000007) == 568830579);
  assert(power_sum_safe(1000, 1000, 1000000007) == 918088852);
  assert(power_sum_safe(1000, 10000, 1000000007) == 163720385);
}
PE_REGISTER_TEST(&power_sum_test, "power_sum_test", SMALL);

SL void gp_sum_mod_test() {
  const int64 mod = 1000000007;
  assert(gp_sum_mod(0, 1, 0, mod) == 0);
  assert(gp_sum_mod(0, 1, 1, mod) == 0);
  assert(gp_sum_mod(0, 1, 2, mod) == 0);
  assert(gp_sum_mod(0, 1, 3, mod) == 0);
  assert(gp_sum_mod(0, 0, 0, mod) == 1);
  assert(gp_sum_mod(0, 0, 0, mod, 0) == 0);
  assert(gp_sum_mod(0, 0, 0, mod, 100) == 100);

  assert(gp_sum_mod(1, 1, 0, mod) == 0);
  assert(gp_sum_mod(1, 1, 1, mod) == 1);
  assert(gp_sum_mod(1, 1, 2, mod) == 2);
  assert(gp_sum_mod(1, 1, 3, mod) == 3);
  assert(gp_sum_mod(1, 0, 0, mod) == 1);
  assert(gp_sum_mod(1, 0, 0, mod, 0) == 1);
  assert(gp_sum_mod(1, 0, 0, mod, 100) == 1);

  assert(gp_sum_mod(2, 1, 0, mod) == 0);
  assert(gp_sum_mod(2, 1, 1, mod) == 2);
  assert(gp_sum_mod(2, 1, 2, mod) == 6);
  assert(gp_sum_mod(2, 1, 3, mod) == 14);
  assert(gp_sum_mod(2, 0, 0, mod) == 1);
  assert(gp_sum_mod(2, 0, 0, mod, 0) == 1);
  assert(gp_sum_mod(2, 0, 0, mod, 100) == 1);
}

PE_REGISTER_TEST(&gp_sum_mod_test, "gp_sum_mod_test", SPECIFIED);

SL void pk_sum_mod_test() {
  // 4e18 + 37
  const int64 mod = 4000000000000000037;
  PowerSumModerB moder(mod, 7);
  auto p1_impl = [=](int64 n, int64 mod) -> int64 {
    return p1_sum_mod(n, mod);
  };
  function<int64(int64, int64)> them[]{p1_impl,     p1_impl,     &p2_sum_mod,
                                       &p3_sum_mod, &p4_sum_mod, &p5_sum_mod,
                                       &p6_sum_mod, &p7_sum_mod};
  for (int k = 1; k <= 7; ++k)
    for (int offset = -100; offset < 100; ++offset) {
      const int64 n = mod + offset;
      const int64 ans1 = (them[k])(n, mod);
      const int64 ans2 = moder.cal(n, k);
      if (ans1 != ans2) {
        cout << n << " " << k << " " << ans1 << " " << ans2 << endl;
      }
      assert(ans1 == ans2);
    }
}

PE_REGISTER_TEST(&pk_sum_mod_test, "pk_sum_mod_test", SMALL);

SL void mu_phi_sum_moder_test() {
  const int mod = 1000000007;
  MuPhiSumModer moder(mod);

  int64 s0 = 0;
  for (int i = 1; i <= 10000000; ++i) s0 += cal_phi(i) % mod;
  s0 %= mod;

  int64 s1 = moder.get_sum_phi(10000000);
  assert(s0 == s1);
}

PE_REGISTER_TEST(&mu_phi_sum_moder_test, "mu_phi_sum_moder_test", MEDIUM);

SL void square_free_counter_test() {
  const int64 n = maxp * 2;
  int64 ans1 = 0;
  for (int i = 1; i <= n; ++i) ans1 += is_square_free(i);

  int64 ans2 = SFCounter(maxp).get(n);
  assert(ans1 == ans2);
}

PE_REGISTER_TEST(&square_free_counter_test, "square_free_counter_test", SMALL);

SL void mvalues_test() {
  auto compute = [&](int64 val, int imp, int64 vmp, MVVHistory* his,
                     int top) -> int64 { return 1; };

  assert(for_m_values<int64>(100000000000LL, compute) == 34113193);
}

PE_REGISTER_TEST(&mvalues_test, "mvalues_test", SMALL);

SL void count_pythagorean_triple_test() {
  // https://oeis.org/A101930
  const int64 ans[] = {2,       52,       881,       12471,      161436,
                       1980642, 23471475, 271360653, 3080075432, 34465432859};

  int64 n = 1;
  for (int i = 0; i < 10; ++i) {
    n *= 10;
    int64 t = ans[i];
    assert(count_pythagorean_triple(n) == t);
    assert(count_pythagorean_triple_ex(n) == t);
  }
}

PE_REGISTER_TEST(&count_pythagorean_triple_test,
                 "count_pythagorean_triple_test", SMALL);
}  // namespace algo_test
