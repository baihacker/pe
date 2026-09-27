#include "pe_test.h"

namespace extended_unsigned_int_test {
// UInt128 uses ExtendedUnsignedIntImpl<ET, true> (builtin ET), UInt512 uses
// ExtendedUnsignedIntImpl<ET, false> (nested extended ET).
using UInt128 = pe::ExtendedUnsignedInt<uint64>;
using UInt256 = pe::ExtendedUnsignedInt<UInt128>;
using UInt512 = pe::ExtendedUnsignedInt<UInt256>;

template <typename TestT>
class Tests {
 public:
  using ET = std::decay_t<decltype(std::declval<TestT>().low())>;
  static constexpr int bitsize = sizeof(TestT) * 8;
  static constexpr int half_bitsize = bitsize / 2;

  template <typename T>
  SL void TestConstructorImpl() {
    assert(TestT(std::numeric_limits<T>::min()).template ToInt<T>() ==
           std::numeric_limits<T>::min());
    assert(TestT(std::numeric_limits<T>::max()).template ToInt<T>() ==
           std::numeric_limits<T>::max());
  }

  SL void TestConstructor() {
    TestConstructorImpl<bool>();
    TestConstructorImpl<char>();
    TestConstructorImpl<short>();
    TestConstructorImpl<int>();
    TestConstructorImpl<long>();
    TestConstructorImpl<long long>();
#if PE_HAS_INT128
    TestConstructorImpl<int128>();
#endif
    TestConstructorImpl<unsigned char>();
    TestConstructorImpl<unsigned short>();
    TestConstructorImpl<unsigned int>();
    TestConstructorImpl<unsigned long>();
    TestConstructorImpl<unsigned long long>();
#if PE_HAS_INT128
    TestConstructorImpl<uint128>();
#endif

    std::string s = "12345678912345678912345678";
    assert(TestT(s).ToString() == s);
    assert(TestT("000000000").ToString() == "0");

    const TestT max = std::numeric_limits<TestT>::max();
    // A negative builtin value is sign-extended (wraps modulo 2^bitsize).
    assert(TestT(-1) == max);
    assert(TestT(int64(-2)) == max - 1);

    // (low, hi) constructor
    assert(TestT(ET(5), ET(0)) == 5);
    assert(TestT(ET(0), ET(1)) == TestT(1) << half_bitsize);
    assert(TestT(ET(0) - 1, ET(0) - 1) == max);

    assert(max.Popcount() == bitsize);
    assert((max >> 10).Popcount() == bitsize - 10);

    // numeric_limits
    assert(std::numeric_limits<TestT>::min() == 0);
    assert(std::numeric_limits<TestT>::lowest() == 0);
    assert(max + 1 == 0);
    assert(TestT(0) - 1 == max);
    assert(max * max == 1);
    static_assert(std::numeric_limits<TestT>::is_unsigned);
    static_assert(std::numeric_limits<TestT>::is_integer);
  }

  template <typename T>
  SL void TestAssignmentImpl() {
    TestT x;
    x = T();
    assert(x.template ToInt<T>() == T());
    x = std::numeric_limits<T>::max();
    assert(x.template ToInt<T>() == std::numeric_limits<T>::max());
    x = std::numeric_limits<T>::min();
    assert(x.template ToInt<T>() == std::numeric_limits<T>::min());
  }

  SL void TestAssignmentOperator() {
    TestAssignmentImpl<char>();
    TestAssignmentImpl<short>();
    TestAssignmentImpl<int>();
    TestAssignmentImpl<long>();
    TestAssignmentImpl<long long>();
#if PE_HAS_INT128
    TestAssignmentImpl<int128>();
#endif
    TestAssignmentImpl<unsigned char>();
    TestAssignmentImpl<unsigned short>();
    TestAssignmentImpl<unsigned int>();
    TestAssignmentImpl<unsigned long>();
    TestAssignmentImpl<unsigned long long>();
#if PE_HAS_INT128
    TestAssignmentImpl<uint128>();
#endif

    std::string s = "12345678912345678912345678";
    TestT x;
    x = s;
    assert(x.ToString() == s);

    // operator=(ET) clears the high half.
    x = std::numeric_limits<TestT>::max();
    x = ET(7);
    assert(x == 7);

    TestT y;
    y = x;
    assert(y == 7);
  }

  template <typename T>
  SL void TestAsmdImpl() {
    TestT x;
    x += T(1);
    x = x + T(1);
    x = T(1) + x;
    x = x + x;

    x -= T(1);
    x = x - T(1);
    x = T(1) - x;
    x = x - x;

    x *= T(1);
    x = x * T(1);
    x = T(1) * x;
    x = x * x;

    x = 1;
    x /= T(1);
    x = x / T(1);
    x = T(1) / x;
    x = 1;
    x = x / x;

    x = 1;
    x %= T(2);
    x = x % T(2);
    x = 1;
    x = x % x;

    ++x;
    x++;
    --x;
    x--;
  }

  SL void TestAsmdOperator() {
    TestAsmdImpl<char>();
    TestAsmdImpl<short>();
    TestAsmdImpl<int>();
    TestAsmdImpl<long>();
    TestAsmdImpl<long long>();
#if PE_HAS_INT128
    TestAsmdImpl<int128>();
#endif
    TestAsmdImpl<unsigned char>();
    TestAsmdImpl<unsigned short>();
    TestAsmdImpl<unsigned int>();
    TestAsmdImpl<unsigned long>();
    TestAsmdImpl<unsigned long long>();
#if PE_HAS_INT128
    TestAsmdImpl<uint128>();
#endif

    // Builtin operands are unsigned: a negative builtin operand is
    // zero-extended by the builtin-ET compound operators (see the TODO(bug)
    // in pe_extended_unsigned_int).
    TestValues<unsigned int>({0, 10000, -10000});
    TestValues<uint64>({10000000000LL, -10000000000LL});
  }

  // Centers are converted to VT, so negative centers give values close to
  // VT's maximum.
  template <typename VT>
  SL void TestValues(std::initializer_list<int64> centers) {
    for (int64 A : centers) {
      for (int64 i = A - 10; i <= A + 10; ++i) {
        for (int64 j = -10; j <= 10; ++j) {
          const VT a = static_cast<VT>(i);
          const VT b = static_cast<VT>(j);
          assert((TestT(a) + TestT(b)).template ToInt<VT>() == VT(a + b));
          assert((TestT(a) += TestT(b)).template ToInt<VT>() == VT(a + b));
          assert((TestT(a) - TestT(b)).template ToInt<VT>() == VT(a - b));
          assert((TestT(a) -= TestT(b)).template ToInt<VT>() == VT(a - b));
          assert((TestT(a) * TestT(b)).template ToInt<VT>() == VT(a * b));
          assert((TestT(a) *= TestT(b)).template ToInt<VT>() == VT(a * b));
          if (b != 0) {
            assert((TestT(a) / TestT(b)).template ToInt<VT>() == a / b);
            assert((TestT(a) /= TestT(b)).template ToInt<VT>() == a / b);
            assert((TestT(a) % TestT(b)).template ToInt<VT>() == a % b);
            assert((TestT(a) %= TestT(b)).template ToInt<VT>() == a % b);
          }
          assert((TestT(a) | TestT(b)).template ToInt<VT>() == (a | b));
          assert((TestT(a) |= TestT(b)).template ToInt<VT>() == (a | b));
          assert((TestT(a) & TestT(b)).template ToInt<VT>() == (a & b));
          assert((TestT(a) &= TestT(b)).template ToInt<VT>() == (a & b));
          assert((TestT(a) ^ TestT(b)).template ToInt<VT>() == (a ^ b));
          assert((TestT(a) ^= TestT(b)).template ToInt<VT>() == (a ^ b));

          assert((TestT(a) + b).template ToInt<VT>() == VT(a + b));
          assert((TestT(a) += b).template ToInt<VT>() == VT(a + b));
          assert((TestT(a) - b).template ToInt<VT>() == VT(a - b));
          assert((TestT(a) -= b).template ToInt<VT>() == VT(a - b));
          assert((TestT(a) * b).template ToInt<VT>() == VT(a * b));
          assert((TestT(a) *= b).template ToInt<VT>() == VT(a * b));
          if (b != 0) {
            assert((TestT(a) / b).template ToInt<VT>() == a / b);
            assert((TestT(a) /= b).template ToInt<VT>() == a / b);
            assert((TestT(a) % b).template ToInt<VT>() == a % b);
            assert((TestT(a) %= b).template ToInt<VT>() == a % b);
          }
          assert((TestT(a) | b).template ToInt<VT>() == (a | b));
          assert((TestT(a) |= b).template ToInt<VT>() == (a | b));
          assert((TestT(a) & b).template ToInt<VT>() == (a & b));
          assert((TestT(a) &= b).template ToInt<VT>() == (a & b));
          assert((TestT(a) ^ b).template ToInt<VT>() == (a ^ b));
          assert((TestT(a) ^= b).template ToInt<VT>() == (a ^ b));

          assert((a + TestT(b)).template ToInt<VT>() == VT(a + b));
          assert((a - TestT(b)).template ToInt<VT>() == VT(a - b));
          assert((a * TestT(b)).template ToInt<VT>() == VT(a * b));
          if (b != 0) {
            assert((a / TestT(b)).template ToInt<VT>() == a / b);
            assert((a % TestT(b)).template ToInt<VT>() == a % b);
          }
          assert((a | TestT(b)).template ToInt<VT>() == (a | b));
          assert((a & TestT(b)).template ToInt<VT>() == (a & b));
          assert((a ^ TestT(b)).template ToInt<VT>() == (a ^ b));

          // Comparison: extended vs extended, extended vs builtin, builtin vs
          // extended
          assert((a < b) == (TestT(a) < TestT(b)));
          assert((a > b) == (TestT(a) > TestT(b)));
          assert((a <= b) == (TestT(a) <= TestT(b)));
          assert((a >= b) == (TestT(a) >= TestT(b)));
          assert((a == b) == (TestT(a) == TestT(b)));
          assert((a != b) == (TestT(a) != TestT(b)));
          assert((a < b) == (TestT(a) < b));
          assert((a == b) == (TestT(a) == b));
          assert((a > b) == (a > TestT(b)));
          assert((a == b) == (a == TestT(b)));

          assert(TestT(a).Difference(TestT(b)) == (a > b ? a - b : b - a));
        }
      }
    }

    // ET operands. With a nested ET the binary operators are ambiguous (see
    // the TODO(bug) in pe_extended_unsigned_int), only the compound ones work.
    for (int a = 0; a <= 20; ++a) {
      for (int b = 1; b <= 5; ++b) {
        if constexpr (is_builtin_integer_v<ET>) {
          assert((TestT(a) + ET(b)) == a + b);
          assert((TestT(a) * ET(b)) == a * b);
          assert((TestT(a) / ET(b)) == a / b);
          assert((TestT(a) % ET(b)) == a % b);
          assert((TestT(a) & ET(b)) == (a & b));
          assert((TestT(a) | ET(b)) == (a | b));
          assert((TestT(a) ^ ET(b)) == (a ^ b));
        }
        TestT x;
        x = a, x += ET(b);
        assert(x == a + b);
        x = a + b, x -= ET(b);
        assert(x == a);
        x = a, x *= ET(b);
        assert(x == a * b);
        x = a, x /= ET(b);
        assert(x == a / b);
        x = a, x %= ET(b);
        assert(x == a % b);
        x = a, x &= ET(b);
        assert(x == (a & b));
        x = a, x |= ET(b);
        assert(x == (a | b));
        x = a, x ^= ET(b);
        assert(x == (a ^ b));
      }
    }
  }

  // Cross check with BigInteger on values wider than 64 bits.
  SL void TestBigValues() {
    // a * b must not overflow: a, b < 10^max_e.
    constexpr int max_e = bitsize * 3 / 10 / 2 - 1;
    const BigInteger M = 1_bi << bitsize;
    for (int e1 : {max_e / 2, max_e}) {
      for (int e2 : {max_e / 3, max_e}) {
        const BigInteger A = Power("10"_bi, e1);
        const BigInteger B = Power("10"_bi, e2);
        for (int64 i = -3; i <= 3; ++i) {
          for (int64 j = -3; j <= 3; ++j) {
            const BigInteger a = A + i;
            const BigInteger b = B + j;
            const TestT x = a.ToString();
            const TestT y = b.ToString();
            assert(x.ToString() == a.ToString());
            assert((x + y).ToString() == (a + b).ToString());
            // Subtraction wraps modulo 2^bitsize.
            assert((x - y).ToString() == (a >= b ? a - b : a - b + M).ToString());
            assert((x * y).ToString() == (a * b).ToString());
            auto [aa, bb] = Div(a, b);
            auto [xx, yy] = Div(x, y);
            assert(xx.ToString() == aa.ToString());
            assert(yy.ToString() == bb.ToString());
            assert((x / y).ToString() == aa.ToString());
            assert((x % y).ToString() == bb.ToString());
            assert((x < y) == (a < b) && (x > y) == (a > b));
          }
        }
      }
    }
  }

  template <typename T>
  SL void TestCompareOperatorImpl() {
    TestT x;
    assert((x == T(0)) == 1);
    assert((x > T(0)) == 0);
    assert((x < T(0)) == 0);
    assert((x <= T(0)) == 1);
    assert((x >= T(0)) == 1);
    assert((x != T(0)) == 0);

    assert((x == x) == 1);
    assert((x > x) == 0);
    assert((x < x) == 0);
    assert((x <= x) == 1);
    assert((x >= x) == 1);
    assert((x != x) == 0);

    x = 1;
    assert((x == T(1)) == 1);
    assert((x > T(1)) == 0);
    assert((x < T(1)) == 0);
    assert((x <= T(1)) == 1);
    assert((x >= T(1)) == 1);
    assert((x != T(1)) == 0);
  }

  SL void TestCompareOperator() {
    TestCompareOperatorImpl<char>();
    TestCompareOperatorImpl<short>();
    TestCompareOperatorImpl<int>();
    TestCompareOperatorImpl<long>();
    TestCompareOperatorImpl<long long>();
#if PE_HAS_INT128
    TestCompareOperatorImpl<int128>();
#endif
    TestCompareOperatorImpl<unsigned char>();
    TestCompareOperatorImpl<unsigned short>();
    TestCompareOperatorImpl<unsigned int>();
    TestCompareOperatorImpl<unsigned long>();
    TestCompareOperatorImpl<unsigned long long>();
#if PE_HAS_INT128
    TestCompareOperatorImpl<uint128>();
#endif
  }

  SL void TestUnaryAndShift() {
    const TestT max = std::numeric_limits<TestT>::max();
    const TestT half = TestT(1) << half_bitsize;  // low == 0, hi == 1
    for (TestT x : {TestT(0), TestT(1), TestT(7), half, half + 12345, max,
                    max - 12345}) {
      assert(-(-x) == x);
      assert(x + (-x) == 0);
      assert(+x == x);
      assert(~x == max - x);

      // ++ and -- carry and borrow across the low/high boundary and wrap.
      TestT y = x;
      assert(y++ == x && y == x + 1);
      assert(y-- == x + 1 && y == x);
      assert(++y == x + 1);
      assert(--y == x);

      // << multiplies, >> is a logical shift.
      for (int k : {0, 1, 7, half_bitsize - 1, half_bitsize,
                    half_bitsize + 1, bitsize - 1}) {
        const TestT p = Power(TestT(2), k);
        assert((x >> k) == x / p);
        if (BitWidth(x) + k > bitsize) continue;
        assert((x << k) == x * p);
        TestT z = x;
        z <<= k;
        z >>= k;
        assert(z == x);
      }
    }
    TestT x = half;
    --x;
    assert(x.hi() == 0 && x.low() == ET(0) - 1);
    ++x;
    assert(x == half);
    x = 0;
    assert(--x == max);
    assert(++x == 0);
  }

  SL void TestBitOperator() {
    TestT x;
    for (int i = 0; i <= 19; ++i) x.SetBit(i);
    assert(x.template ToInt<int>() == 1048575);
    x.RevBit(0);
    assert(x.template ToInt<int>() == 1048574);
    x.ResetBit(1);
    assert(x.template ToInt<int>() == 1048572);
    assert(x.Popcount() == 18);

    TestT y;
    y.SetBit(0);
    x = x | y;
    assert(x.template ToInt<int>() == 1048573);
    x = x & TestT(1048575 - 4);
    assert(x.template ToInt<int>() == 1048573 - 4);
    x = x ^ x;
    assert(x.template ToInt<int>() == 0);
    x = x ^ y;
    assert(x.template ToInt<int>() == 1);

    {
      TestT z;
      assert(BitWidth(z) == 0);
      assert(Popcount(z) == 0);
      z = 1;
      assert(BitWidth(z) == 1);
      assert(Popcount(z) == 1);
      for (int i = 10; i < bitsize; i += 10) {
        assert(BitWidth(z << i) == i + 1);
        assert(Popcount((z << i) - 1) == i);
      }
    }
    {
      const int k = bitsize - 1;
      auto only_bit_k = [=](const TestT& a) {
        assert(BitWidth(a) == k + 1);
        assert(Popcount(a) == 1);
        for (int i = 0; i < k; ++i) {
          assert(GetBit(a, i) == 0);
        }
        assert(GetBit(a, k) == 1);
      };
      TestT a;
      SetBit(a, k);
      only_bit_k(a);
      RevBit(a, k);
      assert(IsZero(a));
      RevBit(a, k);
      only_bit_k(a);
      ResetBit(a, k);
      assert(IsZero(a));
    }
  }

  // ToBinary / ToKBits / ToUInt32Vector and their inverses.
  SL void TestBitsConversion() {
    const TestT max = std::numeric_limits<TestT>::max();
    const TestT half = TestT(1) << half_bitsize;
    for (TestT x : {TestT(1), TestT(12345), half - 1, half, half + 12345,
                    max}) {
      {
        const std::vector<int> bits = x.template ToBinary<int>();
        assert(static_cast<int>(std::size(bits)) == BitWidth(x));
        for (int i = 0; i < BitWidth(x); ++i) {
          assert(bits[i] == GetBit(x, i));
        }
        assert(static_cast<int>(std::size(x.template ToBinary<int>(0))) ==
               bitsize);
        TestT y;
        y.FromBinary(bits);
        assert(y == x);
      }
      for (int k : {1, 2, 4, 8, 16}) {
        const std::vector<int> blocks = x.template ToKBits<int>(k);
        TestT v = 0;
        for (int i = static_cast<int>(std::size(blocks)) - 1; i >= 0; --i) {
          assert(0 <= blocks[i] && blocks[i] < (1 << k));
          v = (v << k) + blocks[i];
        }
        assert(v == x);
        assert(static_cast<int>(std::size(x.template ToKBits<int>(k, 0))) ==
               bitsize / k);
        TestT y;
        y.FromKBits(k, blocks);
        assert(y == x);
      }
      {
        const std::vector<uint32> limbs = x.ToUInt32Vector();
        TestT v = 0;
        for (int i = static_cast<int>(std::size(limbs)) - 1; i >= 0; --i) {
          v = (v << 32) + limbs[i];
        }
        assert(v == x);
        TestT y;
        y.FromUInt32Vector(limbs);
        assert(y == x);
      }
    }
  }

  SL void TestIntegerUtil() {
    for (int a = 0; a <= 24; ++a) {
      const TestT x(a);
      assert(IntSign(x) == (a > 0));
      assert(x.sign() == IntSign(x));
      assert(IsZero(x) == (a == 0));
      assert(IsEven(x) == (a % 2 == 0));
      assert(IsOdd(x) == (a % 2 != 0));
      assert(SameParity(x, TestT(a + 2)) && !SameParity(x, TestT(a + 1)));
      assert(Abs(x) == a);
      assert(FAbs(x) == a);
      assert(LowerBits(x) == static_cast<uint32>(a));
      assert(ToFloat<double>(x) == a);
      for (int b = 1; b <= 6; ++b) {
        const TestT y(b);
        auto [q, r] = Div(x, y);
        assert(q == a / b && r == a % b);
        assert(FloorDiv(x, y) == a / b);
        assert(CeilDiv(x, y) == (a + b - 1) / b);
        assert(Mod(x, b) == a % b);
        assert(Mod(x, y) == a % b);
      }
    }

    // SubMod takes a separate branch for a < b.
    const int m = 7;
    for (int a = 0; a < m; ++a) {
      for (int b = 0; b < m; ++b) {
        assert(AddMod(TestT(a), TestT(b), m) == (a + b) % m);
        assert(SubMod(TestT(a), TestT(b), m) == (a - b + m) % m);
        assert(MulMod(TestT(a), TestT(b), m) == a * b % m);
      }
    }

    // All PowerMod overloads and mod == 1.
    for (int a = 0; a <= 20; ++a) {
      for (int n = 0; n <= 6; ++n) {
        const int64 expected = PowerMod(int64(a), int64(n), int64(m));
        assert(PowerMod(TestT(a), n, TestT(m)) == expected);
        assert(PowerMod(TestT(a), n, m) == expected);
        assert(PowerMod(TestT(a), TestT(n), TestT(m)) == expected);
      }
    }
    assert(IsZero(PowerMod(TestT(5), 3, TestT(1))));
    assert(IsZero(PowerMod(TestT(5), 3, 1)));
    assert(IsZero(PowerMod(TestT(5), TestT(3), TestT(1))));

    assert(Power(TestT(2), 10u) == 1024);
    assert(Power(TestT(2), 20LL) == 1048576);

    // ToFloat and ToString above 64 bits; powers of two are exact.
    for (int k : {64, 100, bitsize - 1}) {
      const TestT p = TestT(1) << k;
      assert(ToFloat<double>(p) == std::ldexp(1.0, k));
      assert(ToString(p) == (1_bi << k).ToString());
      std::stringstream ss;
      ss << p;
      assert(ss.str() == ToString(p));
    }
    assert(ToString(std::numeric_limits<TestT>::max()) ==
           ((1_bi << bitsize) - 1).ToString());

    // Gcd / Lcm
    for (int64 a = 0; a <= 30; ++a) {
      for (int64 b = 0; b <= 30; ++b) {
        assert(Gcd(TestT(a), TestT(b)) == Gcd(a, b));
      }
    }
    {
      TestT x = 6, y = 10, z = 15;
      assert(Gcd(x, y, z) == 1);
      assert(Lcm(x, y) == 30);
      assert(Lcm(x, y, z) == 30);
    }
  }

  SL void Run() {
    TestConstructor();
    TestAssignmentOperator();
    TestAsmdOperator();
    TestBigValues();
    TestCompareOperator();
    TestUnaryAndShift();
    TestBitOperator();
    TestBitsConversion();
    TestIntegerUtil();
  }
};

// Conversions between different widths.
SL void TestCrossWidth() {
  using Int128 = pe::ExtendedSignedInt<uint64>;
  const UInt512 max512 = std::numeric_limits<UInt512>::max();
  // Widening
  assert(UInt512(UInt128(12345)) == 12345);
  assert(UInt512(std::numeric_limits<UInt128>::max()) ==
         (UInt512(1) << 128) - 1);
  // Narrowing keeps the low bits.
  assert(UInt128((UInt512(1) << 200) + 5) == 5);
  assert(UInt128(max512) == std::numeric_limits<UInt128>::max());
  // A signed value of another width is sign-extended.
  assert(UInt512(Int128(5)) == 5);
  assert(UInt512(Int128(-1)) == max512);
  assert(UInt512(Int128(-7)) == max512 - 6);
}

SL void ExtendedUnsignedIntTest() {
  Tests<UInt128>::Run();
  Tests<UInt512>::Run();
#if PE_HAS_INT128
  // Builtin uint128 as ET.
  Tests<pe::ExtendedUnsignedInt<uint128>>::Run();
#endif
  TestCrossWidth();
}
PE_REGISTER_TEST(&ExtendedUnsignedIntTest, "ExtendedUnsignedIntTest", MEDIUM);
}  // namespace extended_unsigned_int_test
