#include "pe_test.h"

namespace poly_app_test {
const int64 mod = 1000000007;

SL void gf_test() {
  {
    // Fibonacci sequence
    vector<int64> A{1, -1, -1};
    vector<int64> B{0, 1};
    vector<int64> result{0, 1};
    for (int i = 2; i <= 30; ++i) {
      result.push_back(add_mod(result[i-2], result[i-1], mod));
    }
    auto x = gf_first({A, mod}, {B, mod}, 30);
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
    int64 dp[10000+1] = {1};
    int64 can[5] = {1, 5, 10, 25, 50};
    for (auto each: can)
    {
      for (int j = 0; j + each <= 10000; ++j) if (dp[j])
      {
        dp[j+each] = add_mod(dp[j+each], dp[j], mod);
      }
    }

    int64 coe[100] = {0};
    for (int i = 0; i < 1 << 5; ++i)
    {
      int s = 0;
      int bc = 0;
      for (int j = 0; j < 5; ++j)
        if (i & (1<<j)) ++bc, s += (int)can[j];
      if (bc & 1) --coe[s]; else ++coe[s];
    }
    auto gfresult = gf_first({vector<int64>(coe, coe+92), mod}, {{1}, mod}, 10000);
    for (int i = 0; i <= 10000; ++i)
      assert(dp[i] == gfresult[i]);

    string mine = to_string(gf_at({vector<int64>(coe, coe+92), mod}, {{1}, mod}, 100000000));
    string expected = to_string("66666793333412666685000001"_bi % mod);
    assert(mine == expected);
  }
}
PE_REGISTER_TEST(&gf_test, "gf_test", SMALL);

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

SL void minimal_polynomial_test() {
  const int64 P = 1000000009;
  NModPoly s{{0,1,1,2,3,5}, P};
  auto v = find_minimal_poly(s);
  assert(v[0] == P - 1);
  assert(v[1] == P - 1);
  assert(v[2] == 1);
  const int n = (int)v.data.size();
  int64 ans = 0;
  for (int i = 0; i < n; ++i)
    ans += v.at(i) * s.at(i);
  assert(ans == P);
  
  ans = nth_element(s, 38, v);
  assert(ans == 39088169LL);
  
  auto t = find_linear_recurrence({{0,1,1,2,3,5,8,13}, 31});
  assert(t[0] == 30);
  assert(t[1] == 30);
  assert(t[2] == 1);
  assert(nth_element({{0,1,1,2,3,5,8,13}, P}, 38) == 39088169);
}
PE_REGISTER_TEST(&minimal_polynomial_test, "minimal_polynomial_test", SMALL);

SL void poly_multipoint_evaluation_test() {
  srand(123456789);
  vector<int64> data;
  int n = 5000;
  const int64 mod = 10007;
  for (int i = 1; i <= n; ++i) data.push_back(i);
  NModPoly p(data, mod);
  vector<int64> v;
  for (int i = 1; i <= n; ++i)
    v.push_back(i%10007);
  {
    auto result = poly_evaluate_mod_normal(p, v);
    for (int i = 1; i <= n; ++i) {
      assert(p.valueAt(i%10007) == result[i-1]);
    }
  }
  {
    auto result = poly_evaluate_mod_bls(p, v);
    for (int i = 1; i <= n; ++i) {
      assert(p.valueAt(i%10007) == result[i-1]);
    }
  }
}
PE_REGISTER_TEST(&poly_multipoint_evaluation_test, "poly_multipoint_evaluation_test", SMALL);

SL void poly_fact_mod_test() {
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
PE_REGISTER_TEST(&poly_fact_mod_test, "poly_fact_mod_test", BIG);

SL void poly_batch_mul_mod_test() {
  const int mod = 10007;
  vector<int64> data{1,1,2,1,3,1};
  auto result = poly_batch_mul_mod(data, mod);

  vector<int64> expected{6, 11, 6, 1};
  assert(expected == result);
}
PE_REGISTER_TEST(&poly_batch_mul_mod_test, "poly_batch_mul_mod_test", SMALL);


SL void poly_fact_sum_mod_test() {
  const int64 mod = 99999999907;
  FactSumModer moder(mod);
  
  const int64 n = 100000000;
  auto ans = moder.cal(n);

  int64 now = 1;
  int64 s = 1;
  for (int64 i = 1; i <= n; ++i) {
    now = mul_mod_ex(now, i, mod);
    s = add_mod(s, now, mod);
  }
  assert(ans == s);
}
PE_REGISTER_TEST(&poly_fact_sum_mod_test, "poly_fact_sum_mod_test", BIG);
}
