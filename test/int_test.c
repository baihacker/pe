#include "pe_test.h"

namespace int_test {
SL void IntConversionTest() {
  // ToInt32 / ToUInt32 keep the lower 32 bits.
  assert(ToInt32(int64(5)) == 5 && ToInt32(-7) == -7);
  assert(ToInt32((int64(1) << 32) | 5) == 5);
  assert(ToInt32(uint32(0xFFFFFFFFu)) == -1);
  assert(ToInt32(uint64(0x80000000u)) == std::numeric_limits<int32>::min());
  assert(ToUInt32(-1) == 0xFFFFFFFFu && ToUInt32(int64(-2)) == 0xFFFFFFFEu);
  assert(ToUInt32(uint64(0x1234567890ULL)) == 0x34567890u);
  assert(ToUInt32(0) == 0u);

  // ToInt64 / ToUInt64.
  assert(ToInt64(-1) == -1 && ToInt64(uint32(0xFFFFFFFFu)) == 0xFFFFFFFFLL);
  assert(ToInt64(std::numeric_limits<uint64>::max()) == -1);
  assert(ToInt64(uint64(1) << 63) == std::numeric_limits<int64>::min());
  assert(ToUInt64(-1) == std::numeric_limits<uint64>::max());
  assert(ToUInt64(int64(-2)) == std::numeric_limits<uint64>::max() - 1);
  assert(ToUInt64(uint32(7)) == 7u);
  assert(ToUInt64('A') == 65u);

  // Return types.
  static_assert(std::is_same_v<decltype(ToInt32(int64(1))), int32>);
  static_assert(std::is_same_v<decltype(ToUInt32(1)), uint32>);
  static_assert(std::is_same_v<decltype(ToInt64(1)), int64>);
  static_assert(std::is_same_v<decltype(ToUInt64(1)), uint64>);
  // constexpr.
  static_assert(ToInt32(int64(0x100000003LL)) == 3);
  static_assert(ToUInt64(-1) == ~uint64(0));

  // ToInt with other targets.
  assert(ToInt<std::int8_t>(300) == 44 && ToInt<std::uint8_t>(-1) == 255);
  assert(ToInt<std::int16_t>(0x18000) ==
         std::numeric_limits<std::int16_t>::min());
  assert(ToInt<std::uint16_t>(-1) == 65535);

  // ToFloat.
  assert(ToFloat<double>(3) == 3.0 && ToFloat<float>(-2) == -2.0f);
  assert(ToFloat<long double>(int64(1) << 60) == std::ldexp(1.0L, 60));
  assert(ToFloat<double>(uint64(1) << 63) == std::ldexp(1.0, 63));
  static_assert(std::is_same_v<decltype(ToFloat<float>(1)), float>);

#if PE_HAS_INT128
  const int128 i128_max = std::numeric_limits<int128>::max();
  const int128 i128_min = std::numeric_limits<int128>::min();
  const uint128 u128_max = std::numeric_limits<uint128>::max();
  assert(ToInt128(-1) == -1 && ToInt128(int64(-5)) == -5);
  assert(ToInt128(std::numeric_limits<uint64>::max()) ==
         (int128(1) << 64) - 1);
  assert(ToInt128(u128_max) == -1);
  assert(ToInt128(uint128(1) << 127) == i128_min);
  assert(ToUInt128(-1) == u128_max && ToUInt128(i128_max) == u128_max >> 1);
  assert(ToUInt128(int64(-2)) == u128_max - 1);
  assert(ToInt64(i128_min) == 0 && ToInt64((int128(3) << 64) + 9) == 9);
  assert(ToUInt64(u128_max) == std::numeric_limits<uint64>::max());
  assert(ToInt32(int128(-1)) == -1 && ToUInt32(u128_max) == 0xFFFFFFFFu);
  static_assert(std::is_same_v<decltype(ToInt128(1)), int128>);
  static_assert(std::is_same_v<decltype(ToUInt128(1)), uint128>);
  static_assert(ToUInt128(-1) == ~uint128(0));
  assert(ToFloat<double>(int128(1) << 100) == std::ldexp(1.0, 100));
#endif
}

SL void IntPredicateTest() {
  assert(IsZero(0) && !IsZero(1) && !IsZero(-1) && IsZero(uint64(0)));
  assert(IntSign(0) == 0 && IntSign(5) == 1 && IntSign(-5) == -1);
  assert(IntSign(std::numeric_limits<int64>::min()) == -1);
  assert(IntSign(std::numeric_limits<uint64>::max()) == 1);
  for (int64 i = -10; i <= 10; ++i) {
    assert(IsEven(i) == (i % 2 == 0));
    assert(IsOdd(i) == (i % 2 != 0));
    assert(IsEven(i) + IsOdd(i) == 1);
    for (int64 j = -4; j <= 4; ++j) {
      assert(SameParity(i, j) == ((i - j) % 2 == 0));
    }
  }
  assert(SameParity(std::int8_t(-3), uint64(5)) && !SameParity(2, uint32(7)));
  assert(IsOdd(std::numeric_limits<int64>::min() + 1));
  assert(IsEven(std::numeric_limits<uint64>::max() - 1));

  assert(LowerBits(int64(0x123456789LL)) == 0x23456789u);
  assert(LowerBits(-1) == 0xFFFFFFFFu &&
         LowerBits(std::uint8_t(200)) == 200u);
  static_assert(std::is_same_v<decltype(LowerBits(int64(1))), uint32>);

  assert(Abs(-5) == 5 && Abs(5) == 5 && Abs(0) == 0);
  assert(Abs(int64(-1) << 62) == int64(1) << 62);
  assert(FAbs(-7) == 7 && FAbs(uint32(7)) == 7u);
  static_assert(Abs(-3) == 3 && IntSign(-3) == -1 && IsZero(0));
  static_assert(IsEven(4) == 1);
  static_assert(IsOdd(-1) == 1);
  static_assert(SameParity(3, -1) == 1);

#if PE_HAS_INT128
  const int128 big = int128(1) << 100;
  assert(IsZero(int128(0)) && !IsZero(big));
  assert(IntSign(-big) == -1 && IntSign(big) == 1);
  assert(IsEven(big) && IsOdd(big + 1) && SameParity(big, uint128(2)));
  assert(LowerBits(big + 7) == 7u);
  assert(Abs(-big) == big && FAbs(-big) == big);
#endif
}

SL void SumProdPowerTest() {
  assert(Sum<int64>() == 0 && Sum<int64>(5) == 5);
  assert(Sum<int64>(1, 2, 3) == 6 && Sum<int64>(-1, 1) == 0);
  // The accumulation happens in T, so no int overflow.
  assert(Sum<int64>(2000000000, 2000000000) == 4000000000LL);
  assert(Prod<int64>() == 1 && Prod<int64>(7) == 7);
  assert(Prod<int64>(2, 3, 4) == 24 && Prod<int64>(-2, 3) == -6);
  assert(Prod<int64>(5, 0, 9) == 0);
  assert(Prod<int64>(100000, 100000) == 10000000000LL);
  assert(Prod<double>(0.5, 4) == 2.0 && Sum<double>(0.25, 0.5) == 0.75);
  static_assert(Sum<int>(1, 2, 3, 4) == 10 && Prod<int>(1, 2, 3, 4) == 24);

  assert(Power(3, 4) == 81 && Power(2, 0) == 1 && Power(0, 0) == 1);
  assert(Power(0, 5) == 0 && Power(1, 1000000) == 1);
  assert(Power(-2, 3) == -8 && Power(-2, 4) == 16 && Power(-1, 99) == -1);
  // The result type is at least 64 bits wide.
  assert(Power(2, 62) == int64(1) << 62);
  assert(Power(10, 18) == 1000000000000000000LL);
  assert(Power(uint64(2), 63) == uint64(1) << 63);
  assert(Power(3, int64(5)) == 243);
  static_assert(std::is_same_v<decltype(Power(2, 3)), int64>);
  static_assert(std::is_same_v<decltype(Power(uint64(2), 3)), uint64>);
  static_assert(Power(7, 3) == 343);
  for (int64 b = -5; b <= 5; ++b) {
    int64 expected = 1;
    for (int e = 0; e <= 15; ++e) {
      assert(Power(b, e) == expected);
      expected *= b;
    }
  }
#if PE_HAS_INT128
  assert(Power(int128(2), 126) == int128(1) << 126);
  assert(Power(uint128(3), 80) ==
         Power(uint128(3), 40) * Power(uint128(3), 40));
  static_assert(std::is_same_v<decltype(Power(int128(2), 3)), int128>);
#endif
}

SL void DivTest() {
  assert(Div(7, 2) == std::make_tuple(3, 1));
  assert(Div(-7, 2) == std::make_tuple(-3, -1));
  assert(Div(7, -2) == std::make_tuple(-3, 1));
  assert(Div(uint64(17), uint64(5)) == std::make_tuple(uint64(3), uint64(2)));

  for (int64 a = -25; a <= 25; ++a) {
    for (int64 b = -7; b <= 7; ++b) {
      if (b == 0) continue;
      const auto [q, r] = Div(a, b);
      assert(q * b + r == a);
      const int64 fl = static_cast<int64>(std::floor(double(a) / double(b)));
      const int64 ce = static_cast<int64>(std::ceil(double(a) / double(b)));
      assert(FloorDiv(a, b) == fl);
      assert(CeilDiv(a, b) == ce);
      assert(FloorDiv(int(a), int(b)) == fl && CeilDiv(int(a), int(b)) == ce);
#if PE_HAS_INT128
      assert(FloorDiv(int128(a), int128(b)) == fl);
      assert(CeilDiv(int128(a), int128(b)) == ce);
#endif
    }
  }
  for (uint64 a = 0; a <= 30; ++a) {
    for (uint64 b = 1; b <= 7; ++b) {
      assert(FloorDiv(a, b) == a / b);
      assert(CeilDiv(a, b) == (a + b - 1) / b);
    }
  }
  // Values near the limits.
  const int64 mx = std::numeric_limits<int64>::max();
  assert(FloorDiv(mx, int64(2)) == mx / 2 &&
         CeilDiv(mx, int64(2)) == mx / 2 + 1);
  assert(FloorDiv(-mx, int64(2)) == -(mx / 2) - 1);
  assert(CeilDiv(-mx, int64(2)) == -(mx / 2));
  assert(CeilDiv(-mx, int64(-2)) == mx / 2 + 1);
  assert(CeilDiv(std::numeric_limits<uint64>::max(), uint64(2)) ==
         (uint64(1) << 63));
}

SL void BinHexStringTest() {
  assert(ToBinString(5, 4) == "0101" && ToBinString(5, 1) == "1");
  assert(ToBinString(std::uint8_t(5)) == "00000101");
  assert(ToBinString(std::int8_t(-1)) == "11111111");
  assert(ToBinString(0) == std::string(32, '0'));
  assert(ToBinString(-1) == std::string(32, '1'));
  assert(ToBinString(int64(-2)) == std::string(63, '1') + "0");
  assert(ToBinString(uint64(1) << 63) == "1" + std::string(63, '0'));
  // A width larger than the type is padded with zeros.
  assert(ToBinString(std::uint8_t(3), 12) == "000000000011");
  assert(ToBinString(std::uint16_t(0xA5)).size() == 16);

  assert(ToHexString(255) == "000000FF" && ToHexString(0) == "00000000");
  assert(ToHexString(0xABC, 3) == "ABC" && ToHexString(0xABC, 2) == "BC");
  assert(ToHexString(int64(-1)) == std::string(16, 'F'));
  assert(ToHexString(std::uint8_t(0x3C)) == "3C" &&
         ToHexString(std::int16_t(-2)) == "FFFE");
  assert(ToHexString(uint64(0x0123456789ABCDEFULL)) == "0123456789ABCDEF");
  assert(ToHexString(std::uint8_t(0xF), 5) == "0000F");

  // Consistent with each other.
  for (int v = 0; v < 256; ++v) {
    const std::string bin = ToBinString(std::uint8_t(v));
    const std::string hex = ToHexString(std::uint8_t(v));
    assert(std::stoi(bin, nullptr, 2) == v &&
           std::stoi(hex, nullptr, 16) == v);
  }
#if PE_HAS_INT128
  assert(ToBinString(int128(-1)) == std::string(128, '1'));
  assert(ToHexString(uint128(1) << 124) == "1" + std::string(31, '0'));
#endif
}

#if PE_HAS_INT128
SL void Int128UtilTest() {
  const int128 i128_max = std::numeric_limits<int128>::max();
  const int128 i128_min = std::numeric_limits<int128>::min();
  const uint128 u128_max = std::numeric_limits<uint128>::max();
  assert(ToString(int128(0)) == "0" && ToString(uint128(0)) == "0");
  assert(ToString(int128(-1)) == "-1" && ToString(uint128(10)) == "10");
  assert(ToString(i128_max) == "170141183460469231731687303715884105727");
  assert(ToString(i128_min) == "-170141183460469231731687303715884105728");
  assert(ToString(u128_max) == "340282366920938463463374607431768211455");
  std::stringstream ss;
  ss << int128(-123456789) << " " << uint128(987654321);
  assert(ss.str() == "-123456789 987654321");

  // gcd / lcm overloads in std, results non-negative.
  assert(std::gcd(int128(12), int128(18)) == 6);
  assert(std::gcd(int128(-12), int128(18)) == 6);
  assert(std::gcd(int128(-12), int128(-18)) == 6);
  assert(std::gcd(int128(0), int128(-5)) == 5);
  assert(std::gcd(int128(0), int128(0)) == 0);
  assert(std::gcd(uint128(1) << 100, uint128(3) << 90) == uint128(1) << 90);
  assert(std::gcd(int128(-8), uint128(12)) == 4u);
  assert(std::gcd(uint128(12), int128(-8)) == 4u);
  assert(std::lcm(int128(4), int128(6)) == 12);
  assert(std::lcm(int128(-4), int128(6)) == 12);
  assert(std::lcm(int128(0), int128(5)) == 0);
  assert(std::lcm(uint128(5), uint128(0)) == 0u);
  assert(std::lcm(int128(-3), uint128(5)) == 15u);
  assert(std::lcm(uint128(1) << 64, int128(-6)) == uint128(3) << 64);
  // gcd(min, 0) is 2^127 as an unsigned value.
  assert(std::gcd(uint128(0), i128_min) == uint128(1) << 127);
  for (int a = -12; a <= 12; ++a) {
    for (int b = -12; b <= 12; ++b) {
      assert(std::gcd(int128(a), int128(b)) == std::gcd(a, b));
      assert(std::lcm(int128(a), int128(b)) == std::lcm(a, b));
    }
  }
}
#endif

// Digits little-endian in base 2^k, normalized like internal::FixSize.
SL std::vector<int> ToDigits(int64 v, int k) {
  std::vector<int> ret;
  do {
    ret.push_back(static_cast<int>(v & ((1 << k) - 1)));
    v >>= k;
  } while (v > 0);
  return ret;
}

SL void FixSizeTest() {
  using V = std::vector<int>;
  V a;
  internal::FixSize(a);
  assert(a == V{0});
  a = {0, 0, 0};
  internal::FixSize(a);
  assert(a == V{0});
  a = {1, 2, 0, 0};
  internal::FixSize(a);
  assert((a == V{1, 2}));
  a = {0, 0, 3};
  internal::FixSize(a);
  assert((a == V{0, 0, 3}));
  a = {5};
  internal::FixSize(a);
  assert(a == V{5});
  std::vector<int64> b{7, 0};
  internal::FixSize(b);
  assert(b == std::vector<int64>{7});
}

SL void AbsDivideTest() {
  using V = std::vector<int>;
  // Base 2: 13 / 3 = 4 remainder 1.
  V remain;
  assert((internal::AbsDivide(V{1, 0, 1, 1}, V{1, 1}, remain) == V{0, 0, 1}));
  assert(remain == V{1});
  // Dividend shorter than divisor.
  assert(internal::AbsDivide(V{1}, V{0, 1}, remain) == V{0});
  assert(remain == V{1});
  // Zero dividend.
  assert(internal::AbsDivide(V{0}, V{1, 1}, remain) == V{0});
  assert(remain == V{0});
  assert(internal::AbsDivide(V{0}, V{1}, remain) == V{0});
  assert(remain == V{0});

  // Brute force for base 2 and base 2^k with k = 1..4.
  for (int k = 1; k <= 4; ++k) {
    for (int64 l = 0; l <= 300; ++l) {
      for (int64 r = 1; r <= 40; ++r) {
        const V ld = ToDigits(l, k), rd = ToDigits(r, k);
        V rem;
        const V q = internal::AbsDivide(k, ld, rd, rem);
        assert(q == ToDigits(l / r, k));
        assert(rem == ToDigits(l % r, k));
        if (k == 1) {
          V rem1;
          assert(internal::AbsDivide(ld, rd, rem1) == q && rem1 == rem);
        }
      }
    }
  }
  // Larger values in base 16.
  const int64 big = 0x123456789ABCDEFLL;
  for (int64 r : std::initializer_list<int64>{1, 15, 16, 255, 4097, 0x10000000,
                                              big}) {
    V rem;
    const V q = internal::AbsDivide(4, ToDigits(big, 4), ToDigits(r, 4), rem);
    assert(q == ToDigits(big / r, 4) && rem == ToDigits(big % r, 4));
  }
}

SL void FloatTest() {
  const double nan = std::numeric_limits<double>::quiet_NaN();
  const double inf = std::numeric_limits<double>::infinity();
  assert(IsNAN(nan) && !IsNAN(0.0) && !IsNAN(inf) && !IsNAN(-inf));
  assert(IsNAN(std::numeric_limits<float>::quiet_NaN()) && !IsNAN(1.5f));
  assert(IsNAN(std::numeric_limits<long double>::quiet_NaN()));
  assert(!IsNAN(1.0L));
  assert(IsNAN(inf - inf) && IsNAN(0.0 * inf));
  // Non floating point values are never NaN.
  assert(!IsNAN(0) && !IsNAN(int64(-5)) && !IsNAN(std::uint8_t(3)));

  assert(Abs(-2.5) == 2.5 && FAbs(-2.5f) == 2.5f && Abs(3.0L) == 3.0L);
  assert(FAbs(-0.0) == 0.0 && !std::signbit(FAbs(-0.0)));
  assert(Floor(2.5) == 2.0 && Floor(-2.5) == -3.0 && Floor(-2.0) == -2.0);
  assert(Ceil(2.5) == 3.0 && Ceil(-2.5) == -2.0 && Ceil(2.0f) == 2.0f);
  assert(Trunc(2.7) == 2.0 && Trunc(-2.7) == -2.0 && Trunc(0.3L) == 0.0L);
  assert(Power(2.0, 10) == 1024.0 && Power(2.0, -2) == 0.25);
  assert(Power(5.0f, 0) == 1.0f && Power(-3.0L, 3) == -27.0L);
  assert(Sqrt(16.0) == 4.0 && Sqrt(2.25f) == 1.5f && Sqrt(0.0L) == 0.0L);
  assert(IsNAN(Sqrt(-1.0)));
  static_assert(std::is_same_v<decltype(Floor(1.0f)), float>);
  static_assert(std::is_same_v<decltype(Sqrt(1.0L)), long double>);

  const double pi = std::acos(-1.0);
  assert(Cos(0.0) == 1.0 && Sin(0.0) == 0.0);
  assert(std::fabs(Cos(pi) + 1) < 1e-15 &&
         std::fabs(Sin(pi / 2) - 1) < 1e-15);
  assert(std::fabs(Sin(pi / 6) - 0.5) < 1e-15);
  assert(std::fabs(Cos(pi / 3.0f) - 0.5f) < 1e-6f);
  assert(Exp(0.0) == 1.0 && std::fabs(Exp(1.0) - 2.718281828459045) < 1e-15);
  assert(Log(1.0) == 0.0 && std::fabs(Log(Exp(3.0)) - 3.0) < 1e-14);
  assert(Log10(1000.0) == 3.0 && Log10(1.0f) == 0.0f);
  assert(std::fabs(Log10(2.0L) - 0.30102999566398119521L) < 1e-18L);
  assert(Log(0.0) == -inf && IsNAN(Log(-1.0)));
  static_assert(std::is_same_v<decltype(Exp(1.0f)), float>);
  static_assert(std::is_same_v<decltype(Log(1.0L)), long double>);
}

SL void FloatStringTest() {
  // Scientific with the given digits after the point.
  assert(ToString(1.5, 3) == "1.500e+00");
  assert(ToString(-123.456, 2) == "-1.23e+02");
  assert(ToString(0.0, 0) == "0.e+00");  // '#' keeps the decimal point.
  assert(ToString(0.25f, 2) == "2.50e-01");
  assert(ToString(1.5L, 3) == "1.500e+00");
  assert(ToString(1.0).size() == std::string("1.").size() + 20 + 4);
  assert(ToString(std::numeric_limits<double>::infinity(), 3) == "inf");

  // Fixed point.
  assert(ToStringF(1.5, 2) == "1.50" && ToStringF(-2.0, 0) == "-2.");
  assert(ToStringF(0.125, 3) == "0.125" && ToStringF(0.125f, 1) == "0.1");
  assert(ToStringF(2.5L, 1) == "2.5" &&
         ToStringF(1.0) == "1." + std::string(20, '0'));
  // A result longer than the internal buffer.
  const std::string long_str = ToStringF(1e300, 0);
  assert(long_str.size() == 302 && long_str.back() == '.');
  assert(long_str.substr(0, 20) == "10000000000000000525");
  const std::string long_str2 = ToStringF(1.0, 300);
  assert(long_str2.size() == 302 && long_str2.substr(0, 3) == "1.0");
  assert(long_str2.find_first_not_of('0', 2) == std::string::npos);

  assert(internal::ToStringFloat<double>(3.0, "%.*f", 1) == "3.0");
  assert(internal::ToStringFloat<double>(-0.5, "%.*e", 1) == "-5.0e-01");
  assert(internal::ToStringFloat<long double>(0.75L, "%.*Lf", 2) == "0.75");
}

// The float128 overloads are not covered: they need libquadmath, which the
// test build does not link.

SL void IntTest() {
  IntConversionTest();
  IntPredicateTest();
  SumProdPowerTest();
  DivTest();
  BinHexStringTest();
#if PE_HAS_INT128
  Int128UtilTest();
#endif
  FixSizeTest();
  AbsDivideTest();
}

PE_REGISTER_TEST(&IntTest, "IntTest", SMALL);

SL void FloatUtilTest() {
  FloatTest();
  FloatStringTest();
}

PE_REGISTER_TEST(&FloatUtilTest, "FloatUtilTest", SMALL);
}  // namespace int_test
