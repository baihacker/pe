#include "pe_test.h"

namespace mod_test {
#if PE_HAS_INT128
template <typename T>
struct ValueHolder {};

template <>
struct ValueHolder<int32> {
  static const int32 values[];
  static const int32 mods[];
};

// we don't consider: -2147483648
const int32 ValueHolder<int32>::values[]{-2147483647, -1073741824, -1,        0,
                                         1,           1073741824,  2147483647};
const int32 ValueHolder<int32>::mods[] = {1, 1073741824, 2147483647};

template <>
struct ValueHolder<uint32> {
  static const uint32 values[];
  static const uint32 mods[];
};
const uint32 ValueHolder<uint32>::values[]{0u, 1u, 2147483648u, 4294967295u};
const uint32 ValueHolder<uint32>::mods[]{1u, 2147483648u, 4294967295u};

template <>
struct ValueHolder<int64> {
  static const int64 values[];
  static const int64 mods[];
};
const int64 ValueHolder<int64>::values[]{-9223372036854775807ll,
                                         -4611686018427387904ll,
                                         -2147483647ll,
                                         -1073741824ll,
                                         -1ll,
                                         0ll,
                                         1ll,
                                         1073741824ll,
                                         2147483647ll,
                                         4611686018427387904ll,
                                         9223372036854775807ll};
const int64 ValueHolder<int64>::mods[]{1ll, 1073741824ll, 2147483647ll,
                                       4611686018427387904ll,
                                       9223372036854775807ll};

template <>
struct ValueHolder<uint64> {
  static const uint64 values[];
  static const uint64 mods[];
};
const uint64 ValueHolder<uint64>::values[]{0u,
                                           1ull,
                                           2147483648ull,
                                           2147483647ull,
                                           9223372036854775807ull,
                                           18446744073709551615ull};
const uint64 ValueHolder<uint64>::mods[]{1ull, 2147483648ull, 2147483647ull,
                                         9223372036854775807ull,
                                         18446744073709551615ull};

SL void mod_test() {
#define REGULATE_MOD_TEST(T1, T2)          \
  for (auto v : ValueHolder<T1>::values)   \
    for (auto m : ValueHolder<T2>::mods) { \
      int128 x = v;                        \
      int128 y = m;                        \
      x %= y;                              \
      if (x < 0) x += y;                   \
      auto ans = regulate_mod(v, m);       \
      if (ans != x) {                      \
        dbg(v);                            \
        dbg(m);                            \
        dbg(ans);                          \
        dbg(x);                            \
      }                                    \
      assert(ans == x);                    \
    }
  REGULATE_MOD_TEST(int32, int32)
  REGULATE_MOD_TEST(uint32, int32)
  REGULATE_MOD_TEST(int64, int32)
  REGULATE_MOD_TEST(uint64, int32)
  REGULATE_MOD_TEST(int32, uint32)
  REGULATE_MOD_TEST(uint32, uint32)
  REGULATE_MOD_TEST(int64, uint32)
  REGULATE_MOD_TEST(uint64, uint32)

  REGULATE_MOD_TEST(int32, int64)
  REGULATE_MOD_TEST(uint32, int64)
  REGULATE_MOD_TEST(int64, int64)
  REGULATE_MOD_TEST(uint64, int64)
  REGULATE_MOD_TEST(int32, uint64)
  REGULATE_MOD_TEST(uint32, uint64)
  REGULATE_MOD_TEST(int64, uint64)
  REGULATE_MOD_TEST(uint64, uint64)
}

PE_REGISTER_TEST(&mod_test, "mod_test", SMALL);
#endif

#if PE_HAS_INT128
SL void frac_mod_test() {
  const int mod = 1000000007;
  for (int64 n = 1; n <= 10; ++n) {
    auto v = frac_mod<int64>({n, n + 1, 2 * n + 1}, {2, 3}, mod);
    auto expected = (int128)n * (n + 1) * (2 * n + 1) / 6 % mod;
    assert(v == expected);
  }

  for (int i = 1; i <= 10; ++i) {
    const int64 n = 100000000000 + i;
    auto v = frac_mod<int64>({n, n + 1, 2 * n + 1}, {2, 3}, mod);
    auto expected = (int128)n * (n + 1) * (2 * n + 1) / 6 % mod;
    assert(v == expected);
  }
}

PE_REGISTER_TEST(&frac_mod_test, "frac_mod_test", SMALL);
#endif

SL void gp_sum_mod_test() {
  const int64 mod = 1000000007;
  assert(gp_sum_mod(0, 1, 2, mod) == 0);
  assert(gp_sum_mod(0, 1, 1, mod) == 0);
  assert(gp_sum_mod(0, 1, 0, mod) == 0);
  assert(gp_sum_mod(0, 0, 0, mod) == 1);
  assert(gp_sum_mod(0, 0, 0, mod, 0) == 0);
  assert(gp_sum_mod(0, 0, 0, mod, 100) == 100);

  assert(gp_sum_mod(1, 1, 2, mod) == 2);
  assert(gp_sum_mod(1, 1, 1, mod) == 1);
  assert(gp_sum_mod(1, 1, 0, mod) == 0);
  assert(gp_sum_mod(1, 0, 0, mod) == 1);
  assert(gp_sum_mod(1, 0, 0, mod, 0) == 1);
  assert(gp_sum_mod(1, 0, 0, mod, 100) == 1);

  assert(gp_sum_mod(2, 1, 2, mod) == 6);
  assert(gp_sum_mod(2, 1, 1, mod) == 2);
  assert(gp_sum_mod(2, 1, 0, mod) == 0);
  assert(gp_sum_mod(2, 0, 0, mod) == 1);
  assert(gp_sum_mod(2, 0, 0, mod, 0) == 1);
  assert(gp_sum_mod(2, 0, 0, mod, 100) == 1);
}

PE_REGISTER_TEST(&gp_sum_mod_test, "gp_sum_mod_test", SMALL);

SL void pk_sum_mod_test() {
  // 4e18 + 37
  const int64 mod = 4000000000000000037;
  PowerSumModerB moder(mod, 7);
  auto p1_impl = [=](int64 n, int64 mod) -> int64 {
    return p1_sum_mod(n, mod);
  };
  function<int64(int64, int64)> them[]{p1_impl,     p1_impl,     &p2_sum_mod,
                                       &p3_sum_mod, &p4_sum_mod, &p5_sum_mod,
                                       &p6_sum_mod};
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

PE_REGISTER_TEST(&pk_sum_mod_test, "pk_sum_mod_test", SPECIFIED);
}  // namespace mod_test
