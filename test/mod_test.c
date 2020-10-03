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
      auto ans = RegulateMod(v, m);        \
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
    auto v = FracMod<int64>({n, n + 1, 2 * n + 1}, {2, 3}, mod);
    auto expected = (int128)n * (n + 1) * (2 * n + 1) / 6 % mod;
    assert(v == expected);
  }

  for (int i = 1; i <= 10; ++i) {
    const int64 n = 100000000000 + i;
    auto v = FracMod<int64>({n, n + 1, 2 * n + 1}, {2, 3}, mod);
    auto expected = (int128)n * (n + 1) * (2 * n + 1) / 6 % mod;
    assert(v == expected);
  }
}

PE_REGISTER_TEST(&frac_mod_test, "frac_mod_test", SMALL);
#endif
}  // namespace mod_test
