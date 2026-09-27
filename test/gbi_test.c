#include "pe_test.h"

namespace gbi_test {
template <typename TestT>
class GbiTests {
 public:
  template <typename NT>
  SL void TestConstructorImpl() {
    assert(TestT(std::numeric_limits<NT>::min()).template ToInt<NT>() ==
           std::numeric_limits<NT>::min());
    assert(TestT(std::numeric_limits<NT>::max()).template ToInt<NT>() ==
           std::numeric_limits<NT>::max());
  }

  SL void TestConstructor() {
    // TestConstructorImpl<bool>();
    TestConstructorImpl<char>();
    TestConstructorImpl<signed char>();
    TestConstructorImpl<unsigned char>();
    TestConstructorImpl<short>();
    TestConstructorImpl<int>();
    TestConstructorImpl<long>();
    TestConstructorImpl<long long>();
#if PE_HAS_INT128
    TestConstructorImpl<int128>();
#endif
    TestConstructorImpl<unsigned short>();
    TestConstructorImpl<unsigned int>();
    TestConstructorImpl<unsigned long>();
    TestConstructorImpl<unsigned long long>();
#if PE_HAS_INT128
    TestConstructorImpl<uint128>();
#endif
    TestConstructorImpl<int256e>();
    TestConstructorImpl<uint256e>();
  }

  template <typename NT>
  SL void TestAssignmentImpl() {
    TestT x;
    x = NT();
    assert(x.template ToInt<NT>() == NT());

    x = std::numeric_limits<NT>::max();
    assert(x.template ToInt<NT>() == std::numeric_limits<NT>::max());

    x = std::numeric_limits<NT>::min();
    assert(x.template ToInt<NT>() == std::numeric_limits<NT>::min());
  }

  SL void TestAssignmentOperator() {
    // TestAssignmentImpl<bool>();
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
    TestAssignmentImpl<int256e>();
    TestAssignmentImpl<uint256e>();

    std::string s = "123456789123456789123456789";
    TestT x;
    x = s;
    assert(x.ToString() == s);
  }

  template <typename NT>
  SL void TestAsmdImpl() {
    TestT x;
    x += NT(1);
    x = x + NT(1);
    x = NT(1) + x;
    x = x + x;

    x -= NT(1);
    x = x - NT(1);
    x = NT(1) - x;
    x = x - x;

    x *= NT(1);
    x = x * NT(1);
    x = NT(1) * x;
    x = x * x;

    x = 1;
    x /= NT(1);
    x = x / NT(1);
    x = NT(1) / x;
    x = 1;
    x = x / x;

    x = 1;
    x %= NT(2);
    x = x % NT(2);
    x = 1;
    x = x % x;

    ++x;
    x++;
    --x;
    x--;
  }

  SL void TestAsmdOperator() {
    // TestAsmdImpl<bool>();
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
    TestAsmdImpl<int256e>();
    TestAsmdImpl<uint256e>();

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
          if (a >= 0 && b >= 0) {
            assert((TestT(a) | TestT(b)).template ToInt<VT>() == (a | b));
            assert((TestT(a) |= TestT(b)).template ToInt<VT>() == (a | b));
            assert((TestT(a) & TestT(b)).template ToInt<VT>() == (a & b));
            assert((TestT(a) &= TestT(b)).template ToInt<VT>() == (a & b));
            assert((TestT(a) ^ TestT(b)).template ToInt<VT>() == (a ^ b));
            assert((TestT(a) ^= TestT(b)).template ToInt<VT>() == (a ^ b));
          }

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
          if (a >= 0 && b >= 0) {
            assert((TestT(a) | b).template ToInt<VT>() == (a | b));
            assert((TestT(a) |= b).template ToInt<VT>() == (a | b));
            assert((TestT(a) & b).template ToInt<VT>() == (a & b));
            assert((TestT(a) &= b).template ToInt<VT>() == (a & b));
            assert((TestT(a) ^ b).template ToInt<VT>() == (a ^ b));
            assert((TestT(a) ^= b).template ToInt<VT>() == (a ^ b));
          }

          assert((a + TestT(b)).template ToInt<VT>() == (a + b));
          assert((a - TestT(b)).template ToInt<VT>() == (a - b));
          assert((a * TestT(b)).template ToInt<VT>() == (a * b));
          if (b != 0) {
            assert((a / TestT(b)).template ToInt<VT>() == (a / b));
            assert((a % TestT(b)).template ToInt<VT>() == (a % b));
          }
          if (a >= 0 && b >= 0) {
            assert((a | TestT(b)).template ToInt<VT>() == (a | b));
            assert((a & TestT(b)).template ToInt<VT>() == (a & b));
            assert((a ^ TestT(b)).template ToInt<VT>() == (a ^ b));
          }

          // Comparison: big vs big, big vs builtin, builtin vs big
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
        }
      }
    }
  }

  template <typename NT>
  SL void TestCompareOperatorImpl() {
    TestT x;
    assert((x == NT(0)) == 1);
    assert((x > NT(0)) == 0);
    assert((x < NT(0)) == 0);
    assert((x <= NT(0)) == 1);
    assert((x >= NT(0)) == 1);
    assert((x != NT(0)) == 0);

    assert((x == x) == 1);
    assert((x > x) == 0);
    assert((x < x) == 0);
    assert((x <= x) == 1);
    assert((x >= x) == 1);
    assert((x != x) == 0);

    x = 1;
    assert((x == NT(1)) == 1);
    assert((x > NT(1)) == 0);
    assert((x < NT(1)) == 0);
    assert((x <= NT(1)) == 1);
    assert((x >= NT(1)) == 1);
    assert((x != NT(1)) == 0);
  }

  SL void TestCompareOperator() {
    // TestCompareOperatorImpl<bool>();
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
    TestCompareOperatorImpl<int256e>();
    TestCompareOperatorImpl<uint256e>();
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
      for (int i = 10; i <= 100; i += 10) {
        assert(BitWidth(z << i) == i + 1);
        assert(Popcount((z << i) - 1) == i);
      }
    }
    {
      auto only_bit_120 = [](const TestT& a) {
        assert(BitWidth(a) == 121);
        assert(Popcount(a) == 1);
        for (int i = 0; i < 120; ++i) {
          assert(GetBit(a, i) == 0);
        }
        assert(GetBit(a, 120) == 1);
      };
      TestT a;
      SetBit(a, 120);
      only_bit_120(a);
      RevBit(a, 120);
      assert(IsZero(a));
      RevBit(a, 120);
      only_bit_120(a);
      ResetBit(a, 120);
      assert(IsZero(a));
    }
  }

  SL void TestUtilities() {
    assert(PowerMod(TestT(5), 10, TestT("123456789")) == 9765625);
    assert(PowerMod(TestT(5), TestT(10), TestT("123456789")) == 9765625);

    assert(Power(TestT(2), 10u) == 1024);
    assert(Power(TestT(2), 10) == 1024);
    assert(Power(TestT(2), 20) == 1048576);
    assert(Power(TestT(2), 20LL) == 1048576);

    {
      TestT x(2);
      Div(x, x);
      IntSign(x);
      IsZero(x);
      IsEven(x);
      IsOdd(x);
      LowerBits(x);
      Abs(x);
      FAbs(x);
      ToInt<int>(x);
      ToFloat<float>(x);
      ToFloat<double>(x);
    }
    {
      TestT p;
      SetBit(p, 127);
      --p;
      ModValue<TestT> ans = SolveLinearEquation<TestT>(123456789, 987654321, p);
      assert(Mod(ans.value * 123456789, p) == 987654321);
    }
    {
      TestT p;
      SetBit(p, 127);
      --p;
      TestT ans = ModInv<TestT>(123456789, p);
      assert(Mod(ans * 123456789, p) == 1);
    }
    {
      TestT p1;
      SetBit(p1, 89);
      --p1;
      TestT p2;
      SetBit(p2, 107);
      --p2;
      TestT p3;
      SetBit(p3, 127);
      --p3;
      ModValue<TestT> ans = CrtN<TestT>({123, 456, 789}, {p1, p2, p3});
      assert(Mod(ans.value, p1) == 123);
      assert(Mod(ans.value, p2) == 456);
      assert(Mod(ans.value, p3) == 789);
    }
    {
      TestT x = 1;
      assert(Gcd(x) == 1);
      assert(Gcd(x, x) == 1);
      assert(Gcd(x, x, x) == 1);
      assert(Lcm(x) == 1);
      assert(Lcm(x, x) == 1);
      assert(Lcm(x, x, x) == 1);
      assert(std::get<0>(ExGcd(x, x)) == 1);
    }
    {
      std::vector<int> cf = {1};
      for (int i = 0; i < 128; ++i) {
        cf.push_back(2);
      }
      std::vector<Fraction<TestT>> x = FromContinuedFractionN<TestT, int>(cf);
      Fraction<TestT> ans = FromContinuedFraction<TestT, int>(cf);
      assert(x.back() == ans);
      assert(ans.a ==
             TestT("11940799687771084222816790191714476900294896923393"));
      assert(ans.b ==
             TestT("8443420432013143050795938339643913980856932710785"));
    }
    {
      TestT N = 1;
      for (int i = 1; i <= 20; ++i) N *= i;

      TestT v = 0;
      for (TestT n = N; !IsZero(n); n >>= 1, v += n) {
        ;
      }

      TestT mod = Power(TestT(2), 48);
      TestT ans = 1;
      FactPPowerModer<TestT> moder(2, 48);
      while (N > 1) {
        ans = ans * moder.Cal(N) % mod;
        N >>= 1;
      }

      ans = ans * (Power(TestT(2), v[0] & 3)) % mod;

      assert(ans == 21171469991580LL);
    }
    {
      for (int i = 2; i <= 16; ++i) {
        for (int base : {2, 10}) {
          for (int64 k = 1; k <= 100; ++k) {
            TestT n = Power<TestT>(base, k);
            int ans = LogI(i, n);
            TestT val1 = Power<TestT>(i, ans);
            TestT val2 = Power<TestT>(i, ans + 1);
            assert(val1 <= n);
            assert(val2 > n);
          }
        }
      }
    }
  }

  SL void TestIntegerUtil() {
    for (int a = -12; a <= 12; ++a) {
      const TestT x(a);
      assert(IntSign(x) == (a > 0) - (a < 0));
      assert(IsZero(x) == (a == 0));
      assert(IsEven(x) == (a % 2 == 0));
      assert(IsOdd(x) == (a % 2 != 0));
      assert(SameParity(x, TestT(a + 2)) && !SameParity(x, TestT(a + 1)));
      assert(Abs(x) == std::abs(a));
      assert(FAbs(x) == std::abs(a));
      if (a > 0) {
        assert(LowerBits(x) == static_cast<uint32>(a));
      }
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

    // All three PowerMod overloads, negative base and mod == 1.
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
  }

  SL void TestBigValues() {
    // Deterministic multi-limb values of both signs.
    std::vector<TestT> values;
    TestT v = 1;
    for (int i = 0; i < 12; ++i) {
      v = v * 1000000007 + i * 12345;
      values.push_back(v);
      values.push_back(-v);
    }
    for (const TestT& a : values) {
      assert(TestT(ToString(a)) == a);
      for (int k : {1, 31, 32, 33, 100}) {
        assert((a << k) == a * Power(TestT(2), k));
        assert(((a << k) >> k) == a);
      }
      for (const TestT& b : values) {
        auto [q, r] = Div(a, b);
        assert(q * b + r == a);
        assert(Abs(r) < Abs(b));
        assert(IsZero(r) || IntSign(r) == IntSign(a));
        assert((a + b) - b == a);
        assert(a * b / b == a);
        assert(IsZero(a * b % b));

        const TestT g = Gcd(Abs(a), Abs(b));
        assert(IsZero(a % g) && IsZero(b % g));
        assert(Gcd(Abs(a) / g, Abs(b) / g) == 1);
        TestT x, y;
        const TestT d = ExGcd(a, b, x, y);
        assert(a * x + b * y == d && Abs(d) == g);
      }
    }
    // Truncating right shift of a negative value.
    assert((TestT(-5) >> 1) == -2);

    // Powers of two convert to double exactly.
    for (int k : {0, 50, 100, 500}) {
      assert(ToFloat<double>(TestT(1) << k) == std::ldexp(1.0, k));
      assert(ToFloat<double>(-(TestT(1) << k)) == -std::ldexp(1.0, k));
    }
  }

  // Compares the gbi overloads with the builtin int64 ones.
  SL void TestNumberTheory() {
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
  }
};

SL void GbiTest_BigInteger() {
  gbi_test::GbiTests<BigInteger> tester;
  tester.TestConstructor();
  tester.TestAssignmentOperator();
  tester.TestAsmdOperator();
  tester.TestCompareOperator();
  tester.TestBitOperator();
  tester.TestUtilities();
  tester.TestIntegerUtil();
  tester.TestBigValues();
  tester.TestNumberTheory();
  Gcd(12_bi, 8_bi);
  123456789123456789_bi * 2 * 5_bi * "10"_bi;
}
PE_REGISTER_TEST(&GbiTest_BigInteger, "GbiTest_BigInteger", SMALL);

#if ENABLE_GMP
SL void GbiTest_MpInteger() {
  gbi_test::GbiTests<MpInteger> tester;
  tester.TestConstructor();
  tester.TestAssignmentOperator();
  tester.TestAsmdOperator();
  tester.TestCompareOperator();
  tester.TestBitOperator();
  tester.TestUtilities();
  tester.TestIntegerUtil();
  tester.TestBigValues();
  tester.TestNumberTheory();
  Gcd(12_mpi, 8_mpi);
  123456789123456789_mpi * 2 * 5_mpi * "10"_mpi;
}

PE_REGISTER_TEST(&GbiTest_MpInteger, "GbiTest_MpInteger", SMALL);
#endif
}  // namespace gbi_test