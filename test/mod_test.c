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
const int32 ValueHolder<int32>::values[] = {-2147483647, -1073741824, -1, 0, 1,
                                            1073741824,  2147483647};
const int32 ValueHolder<int32>::mods[] = {1, 1073741824, 2147483647};

template <>
struct ValueHolder<uint32> {
  static const uint32 values[];
  static const uint32 mods[];
};
const uint32 ValueHolder<uint32>::values[] = {0u, 1u, 2147483648u, 4294967295u};
const uint32 ValueHolder<uint32>::mods[] = {1u, 2147483648u, 4294967295u};

template <>
struct ValueHolder<int64> {
  static const int64 values[];
  static const int64 mods[];
};
const int64 ValueHolder<int64>::values[] = {-9223372036854775807ll,
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
const int64 ValueHolder<int64>::mods[] = {1ll, 1073741824ll, 2147483647ll,
                                          4611686018427387904ll,
                                          9223372036854775807ll};

template <>
struct ValueHolder<uint64> {
  static const uint64 values[];
  static const uint64 mods[];
};
const uint64 ValueHolder<uint64>::values[] = {0u,
                                              1ULL,
                                              2147483648ULL,
                                              2147483647ULL,
                                              9223372036854775807ULL,
                                              18446744073709551615ULL};
const uint64 ValueHolder<uint64>::mods[] = {1ULL, 2147483648ULL, 2147483647ULL,
                                            9223372036854775807ULL,
                                            18446744073709551615ULL};

SL void ModTest() {
#define REGULATE_MOD_TEST(T1, T2)        \
  for (T1 v : ValueHolder<T1>::values)   \
    for (T2 m : ValueHolder<T2>::mods) { \
      int128 x = v;                      \
      int128 y = m;                      \
      x %= y;                            \
      if (x < 0) x += y;                 \
      auto ans = Mod(v, m);              \
      if (ans != x) {                    \
        dbg(v);                          \
        dbg(m);                          \
        dbg(ans);                        \
        dbg(x);                          \
      }                                  \
      assert(ans == x);                  \
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

PE_REGISTER_TEST(&ModTest, "ModTest", SMALL);
#endif

#if PE_HAS_INT128
SL void FracModTest() {
  const int mod = 1000000007;
  for (int64 n = 1; n <= 10; ++n) {
    int64 v = FracMod<int64, int64>({n, n + 1, 2 * n + 1}, {2, 3}, mod);
    int128 expected = (int128)n * (n + 1) * (2 * n + 1) / 6 % mod;
    assert(v == expected);
  }

  for (int i = 1; i <= 10; ++i) {
    int64 n = 100000000000 + i;
    int64 v = FracMod<int64, int64>({n, n + 1, 2 * n + 1}, {2, 3}, mod);
    int128 expected = (int128)n * (n + 1) * (2 * n + 1) / 6 % mod;
    assert(v == expected);
  }
}

PE_REGISTER_TEST(&FracModTest, "FracModTest", SMALL);
#endif
#if PE_HAS_INT128
// Reduced operands against int128 arithmetic (see the TODO(bug)s in pe_mod
// for unreduced operands).
SL void ModArithmeticTest() {
  uint64 seed = 7;
  auto next = [&]() {
    seed = seed * 6364136223846793005ULL + 1442695040888963407ULL;
    return seed >> 1;
  };
  for (int64 mod : {int64(1), int64(2), int64(7), int64(1000000007),
                    int64(4294967291), int64(4294967311),
                    int64(4611686018427387847)}) {
    const uint64 umod = mod;
    for (int it = 0; it < 300; ++it) {
      const int64 a = next() % mod, b = next() % mod;
      const int128 A = a, B = b;
      assert(AddMod(a, b, mod) == (A + B) % mod);
      assert(SubMod(a, b, mod) == ((A - B) % mod + mod) % mod);
      assert(MulMod(a, b, mod) == A * B % mod);
      const uint64 ua = a, ub = b;
      assert(AddMod(ua, ub, umod) == (A + B) % mod);
      assert(SubMod(ua, ub, umod) == ((A - B) % mod + mod) % mod);
      assert(MulMod(ua, ub, umod) == A * B % mod);
      if (mod <= 3037000499) assert(MulModDirectly(a, b, mod) == A * B % mod);
      if (mod < 2147483648) {
        assert(MulModInt32(int32(a), int32(b), int32(mod)) == A * B % mod);
        assert(MulModUint32(uint32(a), uint32(b), uint32(mod)) == A * B % mod);
        assert(AddMod(int32(a), int32(b), int32(mod)) == (A + B) % mod);
        assert(MulMod(uint32(a), uint32(b), uint32(mod)) == A * B % mod);
      }

      // PowerMod, also with a negative base.
      const int64 n = next() % 70;
      int128 p = 1 % mod;
      for (int64 i = 0; i < n; ++i) p = p * A % mod;
      assert(PowerMod(a, n, mod) == p);
      assert(PowerMod(a - mod, n, mod) == p);
      assert(PowerMod(ua, n, umod) == p);

      assert(SumMod(mod, a, b, a) == (2 * A + B) % mod);
      assert(SumMod(mod, {a, b, a}) == (2 * A + B) % mod);
      assert(ProdMod(mod, a, b, a) == A * B % mod * A % mod);
      assert(ProdMod(mod, {a, b, a}) == A * B % mod * A % mod);

      const uint128 w = static_cast<uint128>(ua) * ub + 12345;
      assert(Uint128ModUint64(w, umod) == w % umod);
    }
  }
  assert(PowerMod(int64(5), 0, int64(1)) == 0);
  assert(ProdMod(int64(1), {int64(0), int64(0)}) == 0);
}

PE_REGISTER_TEST(&ModArithmeticTest, "ModArithmeticTest", SMALL);
#endif

SL void FracModCombTest() {
  // C(n, k) = n (n-1) ... (n-k+1) / k!
  const int64 p = 1000000007;
  std::vector<std::vector<int64>> c(61, std::vector<int64>(61));
  for (int n = 0; n <= 60; ++n) {
    c[n][0] = 1;
    for (int k = 1; k <= n; ++k) c[n][k] = (c[n - 1][k - 1] + c[n - 1][k]) % p;
  }
  for (int64 n = 1; n <= 60; ++n) {
    for (int64 k = 1; k <= n; ++k) {
      std::vector<int64> a, b;
      for (int64 i = 0; i < k; ++i) a.push_back(n - i), b.push_back(i + 1);
      assert(FracMod(a, b, p) == c[n][k]);
    }
  }
  // Large n against BigInteger
  for (int64 n : {int64(100000000003), int64(999999999999)}) {
    for (int64 k = 1; k <= 8; ++k) {
      std::vector<int64> a, b;
      BigInteger num = 1, den = 1;
      for (int64 i = 0; i < k; ++i) {
        a.push_back(n - i), b.push_back(i + 1);
        num *= n - i;
        den *= i + 1;
      }
      assert(FracMod(a, b, p) == (num / den % p).ToInt<int64>());
    }
  }
}

PE_REGISTER_TEST(&FracModCombTest, "FracModCombTest", SMALL);

SL void ModValueTest() {
  for (int64 m = 1; m <= 7; ++m) {
    for (int64 v = 0; v < m; ++v) {
      const ModValue<int64> mv(v, m);
      assert(mv.ok());
      auto match = [&](int64 x) { return ((x - v) % m + m) % m == 0; };
      for (int64 n = -20; n <= 20; ++n) {
        int64 below = n, above = n;
        while (!match(below)) --below;
        while (!match(above)) ++above;
        assert(mv.GetNoMoreThan(n) == below);
        assert(mv.GetAtLeast(n) == above);
        int64 positive = 0;
        for (int64 x = 1; x <= n; ++x) positive += match(x);
        assert(mv.GetPositiveCountNoMoreThan(n) == positive);
        for (int64 hi = n - 1; hi <= 20; ++hi) {
          int64 cnt = 0;
          for (int64 x = n; x <= hi; ++x) cnt += match(x);
          assert(mv.GetCountInRange(n, hi) == cnt);
        }
        // ModNoMore / ModAtLeast (v may be outside [0, m)).
        for (int64 vv : {v, v + 3 * m, v - 2 * m}) {
          assert(ModNoMore(n, vv, m) == below);
          assert(ModAtLeast(n, vv, m) == above);
        }
      }
    }
  }
  assert(!ModValue<int64>::Absent().ok());
  std::stringstream ss;
  ss << ModValue<int64>(3, 7) << "|" << ModValue<int64>::Absent();
  assert(ss.str() == "ok 3 7|fail 0 0");

  using M7 = NModCC64<7>;
  assert(ModNoMore(int64(20), M7(3)) == 17);
  assert(ModAtLeast(int64(20), M7(3)) == 24);
}

PE_REGISTER_TEST(&ModValueTest, "ModValueTest", SMALL);

#if PE_HAS_INT128
using int128_or_int64 = int128;
#else
using int128_or_int64 = int64;
#endif

// make(v) builds the number v modulo m. Power is only checked up to
// max_power: with the lazy APSBL policy larger powers overflow (see the
// TODO(bug) in pe_mod).
template <typename NT, typename Make>
SL void TestNModNumber(int64 m, const Make& make, int max_power = 20) {
  static_assert(IsNModNumberV<NT>);
  auto r = [m](int128_or_int64 v) {
    return static_cast<int64>((v % m + m) % m);
  };
  const int64 values[] = {0, 1, 2, m - 1, m / 2, -1, -m,
                          m < 1000000000000 ? 3 * m + 5 : m + 5};
  for (int64 a : values) {
    const NT x = make(a);
    assert(ExtractValue(x) == r(a));
    assert(ExtractValue(+x) == r(a) && ExtractValue(-x) == r(-a));
    for (int64 b : values) {
      const NT y = make(b);
      const int64 ra = r(a), rb = r(b);
      assert(ExtractValue(x + y) == r(int128_or_int64(ra) + rb));
      assert(ExtractValue(x - y) == r(int128_or_int64(ra) - rb));
      assert(ExtractValue(x * y) ==
             static_cast<int64>(MulMod(uint64(ra), uint64(rb), uint64(m))));
      NT z = x;
      z += y;
      assert(z == x + y);
      z = x;
      z -= y;
      assert(z == x - y);
      z = x;
      z *= y;
      assert(z == x * y);
      assert((x == y) == (ra == rb) && (x != y) == (ra != rb));
      assert((x < y) == (ra < rb) && (x > y) == (ra > rb));
      assert((x <= y) == (ra <= rb) && (x >= y) == (ra >= rb));
    }
    // Mixed with builtin integers
    assert(ExtractValue(x + 5) == r(int128_or_int64(r(a)) + 5));
    assert(ExtractValue(5 + x) == r(int128_or_int64(r(a)) + 5));
    assert(ExtractValue(x - 5) == r(int128_or_int64(r(a)) - 5));
    assert(ExtractValue(5 - x) == r(5 - int128_or_int64(r(a))));
    assert(ExtractValue(x * 3) ==
           static_cast<int64>(MulMod(uint64(r(a)), uint64(3 % m), uint64(m))));
    assert(ExtractValue(3 * x) == ExtractValue(x * 3));

    NT y = x;
    assert(ExtractValue(y++) == r(a) && ExtractValue(y) == r(int128_or_int64(r(a)) + 1));
    assert(ExtractValue(y--) == r(int128_or_int64(r(a)) + 1) && y == x);
    assert(ExtractValue(++y) == r(int128_or_int64(r(a)) + 1));
    assert(--y == x);

    for (int n = 0; n <= max_power; ++n) {
      const int64 expected = PowerMod(uint64(r(a)), n, uint64(m));
      assert(ExtractValue(x.Power(n)) == expected);
      assert(ExtractValue(Power(x, n)) == expected);
      assert(ExtractValue(PowerMod(x, n)) == expected);
      assert(ExtractValue(PowerMod(x, n, m)) == expected);
    }
    std::stringstream ss;
    ss << x.FixValue();
    assert(ss.str() == std::to_string(r(a)));
  }
}

SL void NModNumberTest() {
  constexpr int64 p = 1000000007;
  constexpr int64 big = 4611686018427387847;
  TestNModNumber<NModCC64<p>>(p, [](int64 v) { return NModCC64<p>(v); });
  TestNModNumber<NModCC64<big>>(big, [](int64 v) { return NModCC64<big>(v); });
  TestNModNumber<NModNumber<CCMod64<97>>>(
      97, [](int64 v) { return NModNumber<CCMod64<97>>(v); });
#if PE_HAS_INT128
  using W = NModNumber<CCMod64<big>, APSB<int64, int128>>;
  TestNModNumber<W>(big, [](int64 v) { return W(v); });
  using L = NModNumber<CCMod64<p>, APSBL<int128>>;
  TestNModNumber<L>(p, [](int64 v) { return L(v); }, 3);
#endif
  GlobalMod64::Set(p);
  TestNModNumber<NModNumber<GlobalMod64>>(
      p, [](int64 v) { return NModNumber<GlobalMod64>(v); });
  TLMod64::Set(big);
  TestNModNumber<NModTL64<>>(big, [](int64 v) { return NModTL64<>(v); });
  // NModNumberM: the modulus lives in the instance for MemMod.
  using MM = NModNumberM<MemMod64>;
  TestNModNumber<MM>(1000003, [](int64 v) { return MM(v, MemMod64(1000003)); });
  using MC = NModNumberM<CCMod64<p>, APSB<int64, fake_int128>>;
  TestNModNumber<MC>(p, [](int64 v) { return MC(v); });

  // OfValue / SetValue
  NModCC64<p> x = NModCC64<p>::OfValue(5);
  x.SetValue(6);
  assert(x.value() == 6 && NModCC64<p>::Mod() == p);

  static_assert(!IsNModNumberV<int64>);
  static_assert(IsNModNumberCCModOrGlobalModV<NModCC64<p>>);
  static_assert(IsNModNumberCCModOrGlobalModV<NModNumber<GlobalMod64>>);
  static_assert(IsNModNumberCCModOrGlobalModV<MC>);
  static_assert(!IsNModNumberCCModOrGlobalModV<NModTL64<>>);
  static_assert(!IsNModNumberCCModOrGlobalModV<MM>);
  assert(ExtractValue(int64(-3)) == -3);
#if PE_HAS_INT128
  // ModValueFixer reduces the lazy APSBL values.
  using L = NModNumber<CCMod64<p>, APSBL<int128>>;
  L lazy = L(p - 1) + L(p - 1);
  ModValueFixer<L>::Fix(lazy);
  assert(lazy.value() == p - 2);
#endif
}

PE_REGISTER_TEST(&NModNumberTest, "NModNumberTest", SMALL);
}  // namespace mod_test
