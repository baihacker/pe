#include "pe_test.h"

namespace nt_test {
SL void parity_test() {
  assert(is_even(0) == 1);
  assert(is_even(1) == 0);
  assert(is_even(2) == 1);
  assert(is_even(3) == 0);

  assert(is_odd(0) == 0);
  assert(is_odd(1) == 1);
  assert(is_odd(2) == 0);
  assert(is_odd(3) == 1);

  assert(same_parity(0, 0) == 1);
  assert(same_parity(0, 1) == 0);
  assert(same_parity(0, 2) == 1);
  assert(same_parity(0, 3) == 0);

  assert(same_parity(1, 0) == 0);
  assert(same_parity(1, 1) == 1);
  assert(same_parity(1, 2) == 0);
  assert(same_parity(1, 3) == 1);

  assert(same_parity(2, 0) == 1);
  assert(same_parity(2, 1) == 0);
  assert(same_parity(2, 2) == 1);
  assert(same_parity(2, 3) == 0);

  assert(same_parity(3, 0) == 0);
  assert(same_parity(3, 1) == 1);
  assert(same_parity(3, 2) == 0);
  assert(same_parity(3, 3) == 1);

  for (int i = -100; i <= 100; ++i) assert(is_even(i) + is_odd(i) == 1);
}
PE_REGISTER_TEST(&parity_test, "parity_test", SMALL);

SL void gcd_test() {
  assert(gcd({2, 4, 6}) == 2);
  assert(gcd(2, 4, 6) == 2);
  assert(gcd(2L, 4LL, 6ULL) == 2);
}
PE_REGISTER_TEST(&gcd_test, "gcd_test", SMALL);

SL void get_factors_test() {
  auto result = get_factors(1);
  assert(result == vector<int64>{1});

  result = get_factors(2);
  assert(result == vector<int64>({1, 2}));

  result = get_factors(3);
  assert(result == vector<int64>({1, 3}));

  result = get_factors(4);
  assert(result == vector<int64>({1, 2, 4}));

  result = get_factors(12);
  sort(all(result));
  assert(result == vector<int64>({1, 2, 3, 4, 6, 12}));

  for (int64 limit = -1; limit <= 20; ++limit) {
    auto result = get_factors(12, limit);
    sort(all(result));
    vector<int64> expected;
    for (auto iter : {1, 2, 3, 4, 6, 12})
      if (limit < 0 || iter <= limit) {
        expected.push_back(iter);
      }
    assert(result == expected);
  }
}

PE_REGISTER_TEST(&get_factors_test, "get_factors_test", SMALL);

SL void is_square_free_test() {
  const int64 n = maxp * 2;
  int64 ans1 = 0;
  {
    TimeRecorder tr;
    for (int i = 1; i <= n; ++i) ans1 += is_square_free(i);
    // cerr << tr.elapsed().format() << endl;
  }
  int64 ans2 = 0;
  {
    TimeRecorder tr;
    for (int i = 1; i <= n; ++i) ans2 += is_square_free_normal(i);
    // cerr << tr.elapsed().format() << endl;
  }
  assert(ans1 == ans2);
}

PE_REGISTER_TEST(&is_square_free_test, "is_square_free_test", SMALL);

SL void square_root_mod_test() {
  for (int i = 0; i < 1000; ++i) {
    const int64 p = plist[i];
    int cnt = 0;
    for (int n = 0; n < p; ++n) {
      vector<int64> ans = square_root_mod(n, p);
      for (auto x : ans) {
        assert(x * x % p == n);
      }
      cnt += sz(ans) > 0;
    }
    if (p > 2)
      assert(cnt * 2 == p + 1);
    else
      assert(cnt == 2);
  }
}

PE_REGISTER_TEST(&square_root_mod_test, "square_root_mod_test", SMALL);

#if PE_HAS_INT128
SL void test_n_two_squares(int64 n, int64 expected) {
  int64 real = 0;
  auto f = two_squares(n);
  for (auto& iter : f) {
    if (iter.first == 0) {
      real += 4;
    } else {
      real += iter.first == iter.second ? 4 : 8;
    }
  }
  assert(expected == real);
  for (auto& iter : f) assert(sq(iter.first) + sq(iter.second) == n);
}

SL void two_squares_test() {
  auto num_solutions = [=](int64 n) -> int64 {
    int64 ret = 1;
    for (auto& iter : factorize(n)) {
      int mod4 = iter.first & 3;
      if (mod4 == 3) {
        if (is_odd(iter.second)) return 0;
      } else if (mod4 == 1) {
        ret *= iter.second + 1;
      }
    }
    return ret * 4;
  };
  int64 offset = 0;
  for (int64 n = 2; n <= 10000; ++n) {
    test_n_two_squares(offset + n, num_solutions(offset + n));
  }
  // 1e12
  offset = 1000000000000;
  for (int64 n = 2; n <= 10000; ++n) {
    test_n_two_squares(offset + n, num_solutions(offset + n));
  }
}

PE_REGISTER_TEST(&two_squares_test, "two_squares_test", SMALL);
#endif

SL void basek_converstion_test() {
  for (int64 n = 0; n <= 100; ++n)
    for (int k = -200; k <= 200; ++k)
      if (abs(k) >= 2) {
        assert(from_basek<int64>(to_basek(n, k), k) == n);
      }
  for (int64 n = 0; n <= 10000; ++n)
    for (int k = -16; k <= 16; ++k)
      if (abs(k) >= 2) {
        assert(from_basek<int64>(to_basek(n, k), k) == n);
      }
  for (int64 n = -100; n <= 100; ++n)
    for (int k = -200; k <= -2; ++k)
      if (abs(k) >= 2) {
        assert(from_basek<int64>(to_basek(n, k), k) == n);
      }
}
PE_REGISTER_TEST(&basek_converstion_test, "basek_converstion_test", SMALL);
}  // namespace nt_test
