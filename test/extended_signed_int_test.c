#include "pe_test.h"

namespace extended_signed_int_test {
// Int128 uses ExtendedSignedIntImpl<ET, true> (builtin ET), Int512 uses
// ExtendedSignedIntImpl<ET, false> (nested extended ET).
using Int128 = pe::ExtendedSignedInt<uint64>;
using Int256 = pe::ExtendedSignedInt<Int128>;
using Int512 = pe::ExtendedSignedInt<Int256>;

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
    assert(TestT("-" + s).ToString() == "-" + s);
    assert(TestT("000000000").ToString() == "0");

    // (low, hi) constructor
    assert(TestT(ET(5), ET(0)) == 5);
    assert(TestT(ET(0), ET(1)) == TestT(1) << half_bitsize);
    assert(TestT(ET(0) - 1, ET(0) - 1) == -1);

    assert(TestT(-1).Popcount() == bitsize);
    assert((TestT(-1) >> 10).Popcount() == bitsize);

    // numeric_limits
    const TestT max = std::numeric_limits<TestT>::max();
    const TestT min = std::numeric_limits<TestT>::min();
    assert(IntSign(max) > 0 && IntSign(min) < 0);
    assert(max + 1 == min);
    assert(max == (TestT(1) << (bitsize - 1)) - 1);
    assert(Popcount(max) == bitsize - 1 && Popcount(min) == 1);
    assert(std::numeric_limits<TestT>::lowest() == min);
    static_assert(std::numeric_limits<TestT>::is_signed);
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
    x = -1;
    x = ET(7);
    assert(x == 7);

    TestT y;
    y = x;
    assert(y == 7);
    y = TestT(-9);
    assert(y == -9);
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

    // int and int64 operands take different mixed-type overloads.
    TestValues<int>({0, 10000});
    TestValues<int64>({-10000000000LL, 10000000000LL});
  }

  template <typename VT>
  SL void TestValues(std::initializer_list<VT> centers) {
    for (VT A : centers) {
      for (VT a = A - 10; a <= A + 10; ++a) {
        for (VT b = -10; b <= 10; ++b) {
          assert((TestT(a) + TestT(b)).template ToInt<VT>() == (a + b));
          assert((TestT(a) += TestT(b)).template ToInt<VT>() == (a + b));
          assert((TestT(a) - TestT(b)).template ToInt<VT>() == (a - b));
          assert((TestT(a) -= TestT(b)).template ToInt<VT>() == (a - b));
          assert((TestT(a) * TestT(b)).template ToInt<VT>() == (a * b));
          assert((TestT(a) *= TestT(b)).template ToInt<VT>() == (a * b));
          if (b != 0) {
            assert((TestT(a) / TestT(b)).template ToInt<VT>() == (a / b));
            assert((TestT(a) /= TestT(b)).template ToInt<VT>() == (a / b));
            assert((TestT(a) % TestT(b)).template ToInt<VT>() == (a % b));
            assert((TestT(a) %= TestT(b)).template ToInt<VT>() == (a % b));
          }
          // Two's complement: bitwise operators also work for negatives.
          assert((TestT(a) | TestT(b)).template ToInt<VT>() == (a | b));
          assert((TestT(a) |= TestT(b)).template ToInt<VT>() == (a | b));
          assert((TestT(a) & TestT(b)).template ToInt<VT>() == (a & b));
          assert((TestT(a) &= TestT(b)).template ToInt<VT>() == (a & b));
          assert((TestT(a) ^ TestT(b)).template ToInt<VT>() == (a ^ b));
          assert((TestT(a) ^= TestT(b)).template ToInt<VT>() == (a ^ b));

          assert((TestT(a) + b).template ToInt<VT>() == (a + b));
          assert((TestT(a) += b).template ToInt<VT>() == (a + b));
          assert((TestT(a) - b).template ToInt<VT>() == (a - b));
          assert((TestT(a) -= b).template ToInt<VT>() == (a - b));
          assert((TestT(a) * b).template ToInt<VT>() == (a * b));
          assert((TestT(a) *= b).template ToInt<VT>() == (a * b));
          if (b != 0) {
            assert((TestT(a) / b).template ToInt<VT>() == (a / b));
            assert((TestT(a) /= b).template ToInt<VT>() == (a / b));
            assert((TestT(a) % b).template ToInt<VT>() == (a % b));
            assert((TestT(a) %= b).template ToInt<VT>() == (a % b));
          }
          assert((TestT(a) | b).template ToInt<VT>() == (a | b));
          assert((TestT(a) |= b).template ToInt<VT>() == (a | b));
          assert((TestT(a) & b).template ToInt<VT>() == (a & b));
          assert((TestT(a) &= b).template ToInt<VT>() == (a & b));
          assert((TestT(a) ^ b).template ToInt<VT>() == (a ^ b));
          assert((TestT(a) ^= b).template ToInt<VT>() == (a ^ b));

          assert((a + TestT(b)).template ToInt<VT>() == (a + b));
          assert((a - TestT(b)).template ToInt<VT>() == (a - b));
          assert((a * TestT(b)).template ToInt<VT>() == (a * b));
          if (b != 0) {
            assert((a / TestT(b)).template ToInt<VT>() == (a / b));
            assert((a % TestT(b)).template ToInt<VT>() == (a % b));
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

          // Difference is |a - b|, also for operands with different signs.
          assert(TestT(a).Difference(TestT(b)) == (a > b ? a - b : b - a));
        }
      }
    }

    // ET operands (the compound operators have dedicated ET overloads)
    for (int a = -20; a <= 20; ++a) {
      for (int b = 1; b <= 5; ++b) {
        TestT x;
        x = a, x += ET(b);
        assert(x == a + b);
        x = a, x -= ET(b);
        assert(x == a - b);
        x = a, x *= ET(b);
        assert(x == a * b);
        x = a, x /= ET(b);
        assert(x == a / b);
        x = a, x %= ET(b);
        assert(x == a % b);
        if (a >= 0) {
          x = a, x &= ET(b);
          assert(x == (a & b));
          x = a, x |= ET(b);
          assert(x == (a | b));
          x = a, x ^= ET(b);
          assert(x == (a ^ b));
        }
      }
    }
  }

  // Cross check with BigInteger on values wider than 64 bits.
  SL void TestBigValues() {
    // a * b must not overflow: |a|, |b| < 10^max_e.
    constexpr int max_e = bitsize * 3 / 10 / 2 - 1;
    for (int s1 : {-1, 1}) {
      for (int s2 : {-1, 1}) {
        for (int e1 : {max_e / 2, max_e}) {
          for (int e2 : {max_e / 3, max_e}) {
            const BigInteger A = s1 * Power("10"_bi, e1);
            const BigInteger B = s2 * Power("10"_bi, e2);
            for (int64 i = -3; i <= 3; ++i) {
              for (int64 j = -3; j <= 3; ++j) {
                const BigInteger a = A + i;
                const BigInteger b = B + j;
                const TestT x = a.ToString();
                const TestT y = b.ToString();
                assert(x.ToString() == a.ToString());
                assert((x + y).ToString() == (a + b).ToString());
                assert((x - y).ToString() == (a - b).ToString());
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
    const TestT one = 1;
    const TestT half = one << half_bitsize;  // low == 0, hi == 1
    for (TestT x : {TestT(0), TestT(1), TestT(-7), half, -half, half + 12345,
                    -(half + 12345)}) {
      assert(-(-x) == x);
      assert(x + (-x) == 0);
      assert(+x == x);
      assert(~x == -x - 1);

      // ++ and -- carry and borrow across the low/high boundary.
      TestT y = x;
      assert(y++ == x && y == x + 1);
      assert(y-- == x + 1 && y == x);
      assert(++y == x + 1);
      assert(--y == x);

      // Shifts: << multiplies, >> is an arithmetic (flooring) shift.
      for (int k : {0, 1, 7, half_bitsize - 1, half_bitsize,
                    half_bitsize + 1}) {
        // Keep x << k in range.
        if (BitWidth(Abs(x)) + k >= bitsize - 1) continue;
        const TestT p = Power(TestT(2), k);
        assert((x << k) == x * p);
        assert((x >> k) == FloorDiv(x, p));
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
    assert((TestT(-1) >> (bitsize - 1)) == -1);
    assert((TestT(-5) >> 1) == -3);
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
      for (int i = 10; i < bitsize - 1; i += 10) {
        assert(BitWidth(z << i) == i + 1);
        assert(Popcount((z << i) - 1) == i);
      }
    }
    {
      const int k = bitsize - 8;
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

  SL void TestIntegerUtil() {
    for (int a = -12; a <= 12; ++a) {
      const TestT x(a);
      assert(IntSign(x) == (a > 0) - (a < 0));
      assert(x.sign() == IntSign(x));
      assert(IsZero(x) == (a == 0));
      assert(IsEven(x) == (a % 2 == 0));
      assert(IsOdd(x) == (a % 2 != 0));
      assert(SameParity(x, TestT(a + 2)) && !SameParity(x, TestT(a + 1)));
      assert(Abs(x) == std::abs(a));
      assert(FAbs(x) == std::abs(a));
      assert(LowerBits(x) == static_cast<uint32>(a));
      assert(ToFloat<double>(x) == a);
      for (int b = -5; b <= 5; ++b) {
        if (b == 0) continue;
        const TestT y(b);
        auto [q, r] = Div(x, y);
        assert(q == a / b && r == a % b);
        assert(FloorDiv(x, y) == FloorDiv(a, b));
        assert(CeilDiv(x, y) == CeilDiv(a, b));
        if (b > 0) {
          const int expected = (a % b + b) % b;
          assert(Mod(x, b) == expected);
          assert(Mod(x, y) == expected);
        }
      }
    }

    const int m = 7;
    for (int a = 0; a < m; ++a) {
      for (int b = 0; b < m; ++b) {
        assert(AddMod(TestT(a), TestT(b), m) == (a + b) % m);
        assert(SubMod(TestT(a), TestT(b), m) == (a - b + m) % m);
        assert(MulMod(TestT(a), TestT(b), m) == a * b % m);
      }
    }

    // All PowerMod overloads, negative base and mod == 1.
    for (int a = -10; a <= 10; ++a) {
      for (int n = 0; n <= 6; ++n) {
        const int64 expected = PowerMod(int64(a % m + m), int64(n), int64(m));
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
    for (int k : {64, 100, bitsize - 2}) {
      const TestT p = TestT(1) << k;
      assert(ToFloat<double>(p) == std::ldexp(1.0, k));
      assert(ToFloat<double>(-p) == -std::ldexp(1.0, k));
      assert(ToString(p) == (1_bi << k).ToString());
      assert(ToString(-p) == (-(1_bi << k)).ToString());
      std::stringstream ss;
      ss << -p;
      assert(ss.str() == ToString(-p));
    }
  }

  // Compares with the builtin int64 overloads.
  SL void TestNumberTheory() {
    for (int64 a = -15; a <= 15; ++a) {
      for (int64 b = -15; b <= 15; ++b) {
        if (a == 0 && b == 0) continue;
        TestT x, y;
        const TestT d = ExGcd(TestT(a), TestT(b), x, y);
        assert(TestT(a) * x + TestT(b) * y == d);
        assert(Abs(d) == Gcd(Abs(a), Abs(b)));
      }
    }
    for (int64 m = 1; m <= 12; ++m) {
      for (int64 a = 0; a < m; ++a) {
        for (int64 b = 0; b < m; ++b) {
          const auto e = SolveLinearEquation(a, b, m);
          const auto ans = SolveLinearEquation<TestT>(a, b, m);
          assert(ans.ok() == e.ok());
          if (e.ok()) assert(ans.value == e.value && ans.mod == e.mod);
        }
        if (Gcd(a, m) == 1 && m > 1) {
          assert(ModInv<TestT>(a, m) == ModInv(a, m));
        }
      }
    }
    for (int64 m1 = 1; m1 <= 6; ++m1) {
      for (int64 m2 = 1; m2 <= 6; ++m2) {
        for (int64 a = 0; a < m1; ++a) {
          for (int64 b = 0; b < m2; ++b) {
            const auto e = Crt2(a, m1, b, m2);
            const auto ans = Crt2<TestT>(a, m1, b, m2);
            assert(ans.ok() == e.ok());
            if (e.ok()) assert(ans.value == e.value && ans.mod == e.mod);
          }
        }
      }
    }

    // Moduli close to the type width.
    TestT p1 = (TestT(1) << (bitsize / 4 - 3)) - 1;
    TestT p2 = (TestT(1) << (bitsize / 4 - 1)) - 1;
    {
      const TestT inv = ModInv<TestT>(123456789, p2);
      assert(Mod(inv * 123456789, p2) == 1);
      const ModValue<TestT> ans =
          SolveLinearEquation<TestT>(123456789, 987654321, p2);
      assert(Mod(ans.value * 123456789, p2) == 987654321);
    }
    {
      const ModValue<TestT> ans = CrtN<TestT>({123, 456}, {p1, p2});
      assert(ans.ok());
      assert(Mod(ans.value, p1) == 123);
      assert(Mod(ans.value, p2) == 456);
    }
    {
      TestT x = 6, y = 10, z = 15;
      assert(Gcd(x) == 6);
      assert(Gcd(x, y) == 2);
      assert(Gcd(x, y, z) == 1);
      assert(Lcm(x) == 6);
      assert(Lcm(x, y) == 30);
      assert(Lcm(x, y, z) == 30);
    }
    {
      std::vector<int> cf = {1};
      for (int i = 0; i < bitsize / 4 - 2; ++i) cf.push_back(2);
      std::vector<Fraction<TestT>> x = FromContinuedFractionN<TestT, int>(cf);
      Fraction<TestT> ans = FromContinuedFraction<TestT, int>(cf);
      assert(x.back() == ans);
      // Convergents of sqrt(2): a^2 - 2 b^2 = +-1.
      assert(Abs(ans.a * ans.a - 2 * ans.b * ans.b) == 1);
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
    TestIntegerUtil();
    TestNumberTheory();
  }
};

SL void ExtendedSignedIntTest() {
  Tests<Int128>::Run();
  Tests<Int512>::Run();
#if PE_HAS_INT128
  // Builtin uint128 as ET.
  Tests<pe::ExtendedSignedInt<uint128>>::Run();
#endif
}
PE_REGISTER_TEST(&ExtendedSignedIntTest, "ExtendedSignedIntTest", MEDIUM);
}  // namespace extended_signed_int_test
