#include "pe_test.h"

namespace nt_test {
SL void nt_test() {
  const int mod = 1000000007;
  MiuPhiSumModer moder(mod);

  int64 s0 = 0;
  for (int i = 1; i <= 10000000; ++i)
    s0 += cal_phi(i) % mod;
  s0 %= mod;

  int64 s1 = moder.get_sum_phi(10000000);
  assert(s0 == s1);
}

PE_REGISTER_TEST(&nt_test, "nt_test", MEDIUM);

SL void get_factors_test() {
  auto result = get_factors(1);
  assert(result == vector<int64>{1});

  result = get_factors(2);
  assert(result == vector<int64>({1,2}));

  result = get_factors(3);
  assert(result == vector<int64>({1,3}));

  result = get_factors(4);
  assert(result == vector<int64>({1,2,4}));

  result = get_factors(12);
  sort(all(result));
  assert(result == vector<int64>({1,2,3,4,6,12}));

  for (int64 limit = -1; limit <= 20; ++limit) {
    auto result = get_factors(12, limit);
    sort(all(result));
    vector<int64> expected;
    for (auto iter: {1,2,3,4,6,12}) if (limit < 0 || iter <= limit) {
      expected.push_back(iter);
    }
    assert(result == expected);
  }
}

PE_REGISTER_TEST(&get_factors_test, "get_factors_test", SMALL);

SL void is_square_free_test() {
  const int64 n = maxp*2;
  int64 ans1 = 0;
  {
    TimeRecorder tr;
    for (int i = 1; i <= n; ++i)
      ans1 += is_square_free(i);
    // cerr << tr.elapsed().format() << endl;
  }
  int64 ans2 = 0;
  {
    TimeRecorder tr;
    for (int i = 1; i <= n; ++i)
      ans2 += is_square_free_normal(i);
    // cerr << tr.elapsed().format() << endl;
  }
  assert(ans1 == ans2);
}

PE_REGISTER_TEST(&is_square_free_test, "is_square_free_test", SMALL);

SL void square_free_counter_test() {
  const int64 n = maxp*2;
  int64 ans1 = 0;
  for (int i = 1; i <= n; ++i)
    ans1 += is_square_free(i);

  int64 ans2 = SFCounter(maxp).get(n);
  assert(ans1 == ans2);
}

PE_REGISTER_TEST(&square_free_counter_test, "square_free_counter_test", SMALL);
}
