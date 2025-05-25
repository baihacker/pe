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
    TestT x;
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

    for (int A : {-10000, 0, 10000}) {
      for (int a = A - 10; a <= A + 10; ++a) {
        for (int b = -10; b <= 10; ++b) {
          assert((TestT(a) + TestT(b)).template ToInt<int>() == (a + b));
          assert((TestT(a) += TestT(b)).template ToInt<int>() == (a + b));
          assert((TestT(a) - TestT(b)).template ToInt<int>() == (a - b));
          assert((TestT(a) -= TestT(b)).template ToInt<int>() == (a - b));
          assert((TestT(a) * TestT(b)).template ToInt<int>() == (a * b));
          assert((TestT(a) *= TestT(b)).template ToInt<int>() == (a * b));
          if (b != 0) {
            assert((TestT(a) / TestT(b)).template ToInt<int>() == (a / b));
            assert((TestT(a) /= TestT(b)).template ToInt<int>() == (a / b));
            assert((TestT(a) % TestT(b)).template ToInt<int>() == (a % b));
            assert((TestT(a) %= TestT(b)).template ToInt<int>() == (a % b));
          }
          if (a >= 0 && b >= 0) {
            assert((TestT(a) | TestT(b)).template ToInt<int>() == (a | b));
            assert((TestT(a) |= TestT(b)).template ToInt<int>() == (a | b));
            assert((TestT(a) & TestT(b)).template ToInt<int>() == (a & b));
            assert((TestT(a) &= TestT(b)).template ToInt<int>() == (a & b));
            assert((TestT(a) ^ TestT(b)).template ToInt<int>() == (a ^ b));
            assert((TestT(a) ^= TestT(b)).template ToInt<int>() == (a ^ b));
          }

          assert((TestT(a) + b).template ToInt<int>() == (a + b));
          assert((TestT(a) += b).template ToInt<int>() == (a + b));
          assert((TestT(a) - b).template ToInt<int>() == (a - b));
          assert((TestT(a) -= b).template ToInt<int>() == (a - b));
          assert((TestT(a) * b).template ToInt<int>() == (a * b));
          assert((TestT(a) *= b).template ToInt<int>() == (a * b));
          if (b != 0) {
            assert((TestT(a) / b).template ToInt<int>() == (a / b));
            assert((TestT(a) /= b).template ToInt<int>() == (a / b));
            assert((TestT(a) % b).template ToInt<int>() == (a % b));
            assert((TestT(a) %= b).template ToInt<int>() == (a % b));
          }
          if (a >= 0 && b >= 0) {
            assert((TestT(a) | b).template ToInt<int>() == (a | b));
            assert((TestT(a) |= b).template ToInt<int>() == (a | b));
            assert((TestT(a) & b).template ToInt<int>() == (a & b));
            assert((TestT(a) &= b).template ToInt<int>() == (a & b));
            assert((TestT(a) ^ b).template ToInt<int>() == (a ^ b));
            assert((TestT(a) ^= b).template ToInt<int>() == (a ^ b));
          }

          assert((a + TestT(b)).template ToInt<int>() == (a + b));
          assert((a - TestT(b)).template ToInt<int>() == (a - b));
          assert((a * TestT(b)).template ToInt<int>() == (a * b));
          if (b != 0) {
            assert((a / TestT(b)).template ToInt<int>() == (a / b));
            assert((a % TestT(b)).template ToInt<int>() == (a % b));
          }
          if (a >= 0 && b >= 0) {
            assert((a | TestT(b)).template ToInt<int>() == (a | b));
            assert((a & TestT(b)).template ToInt<int>() == (a & b));
            assert((a ^ TestT(b)).template ToInt<int>() == (a ^ b));
          }
        }
      }
    }

    for (int64 A : {-10000000000LL, -10000LL, 0LL, 10000LL, 10000000000LL}) {
      for (int64 a = A - 10; a <= A + 10; ++a) {
        for (int64 b = -10; b <= 10; ++b) {
          assert((TestT(a) + TestT(b)).template ToInt<int64>() == (a + b));
          assert((TestT(a) += TestT(b)).template ToInt<int64>() == (a + b));
          assert((TestT(a) - TestT(b)).template ToInt<int64>() == (a - b));
          assert((TestT(a) -= TestT(b)).template ToInt<int64>() == (a - b));
          assert((TestT(a) * TestT(b)).template ToInt<int64>() == (a * b));
          assert((TestT(a) *= TestT(b)).template ToInt<int64>() == (a * b));
          if (b != 0) {
            assert((TestT(a) / TestT(b)).template ToInt<int64>() == (a / b));
            assert((TestT(a) /= TestT(b)).template ToInt<int64>() == (a / b));
            assert((TestT(a) % TestT(b)).template ToInt<int64>() == (a % b));
            assert((TestT(a) %= TestT(b)).template ToInt<int64>() == (a % b));
          }
          if (a >= 0 && b >= 0) {
            assert((TestT(a) | TestT(b)).template ToInt<int64>() == (a | b));
            assert((TestT(a) |= TestT(b)).template ToInt<int64>() == (a | b));
            assert((TestT(a) & TestT(b)).template ToInt<int64>() == (a & b));
            assert((TestT(a) &= TestT(b)).template ToInt<int64>() == (a & b));
            assert((TestT(a) ^ TestT(b)).template ToInt<int64>() == (a ^ b));
            assert((TestT(a) ^= TestT(b)).template ToInt<int64>() == (a ^ b));
          }

          assert((TestT(a) + b).template ToInt<int64>() == (a + b));
          assert((TestT(a) += b).template ToInt<int64>() == (a + b));
          assert((TestT(a) - b).template ToInt<int64>() == (a - b));
          assert((TestT(a) -= b).template ToInt<int64>() == (a - b));
          assert((TestT(a) * b).template ToInt<int64>() == (a * b));
          assert((TestT(a) *= b).template ToInt<int64>() == (a * b));
          if (b != 0) {
            assert((TestT(a) / b).template ToInt<int64>() == (a / b));
            assert((TestT(a) /= b).template ToInt<int64>() == (a / b));
            assert((TestT(a) % b).template ToInt<int64>() == (a % b));
            assert((TestT(a) %= b).template ToInt<int64>() == (a % b));
          }
          if (a >= 0 && b >= 0) {
            assert((TestT(a) | b).template ToInt<int64>() == (a | b));
            assert((TestT(a) |= b).template ToInt<int64>() == (a | b));
            assert((TestT(a) & b).template ToInt<int64>() == (a & b));
            assert((TestT(a) &= b).template ToInt<int64>() == (a & b));
            assert((TestT(a) ^ b).template ToInt<int64>() == (a ^ b));
            assert((TestT(a) ^= b).template ToInt<int64>() == (a ^ b));
          }

          assert((a + TestT(b)).template ToInt<int64>() == (a + b));
          assert((a - TestT(b)).template ToInt<int64>() == (a - b));
          assert((a * TestT(b)).template ToInt<int64>() == (a * b));
          if (b != 0) {
            assert((a / TestT(b)).template ToInt<int64>() == (a / b));
            assert((a % TestT(b)).template ToInt<int64>() == (a % b));
          }
          if (a >= 0 && b >= 0) {
            assert((a | TestT(b)).template ToInt<int64>() == (a | b));
            assert((a & TestT(b)).template ToInt<int64>() == (a & b));
            assert((a ^ TestT(b)).template ToInt<int64>() == (a ^ b));
          }
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

    assert((x == x) == 1);
    assert((x > x) == 0);
    assert((x < x) == 0);
    assert((x <= x) == 1);
    assert((x >= x) == 1);
    assert((x != x) == 0);
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
      TestT a;
      SetBit(a, 120);
      assert(BitWidth(a) == 121);
      assert(Popcount(a) == 1);
      for (int i = 0; i < 120; ++i) {
        assert(GetBit(a, i) == 0);
      }
      assert(GetBit(a, 120) == 1);
      RevBit(a, 120);
      assert(IsZero(a));
      RevBit(a, 240);
      assert(BitWidth(a) == 241);
      assert(Popcount(a) == 1);
      for (int i = 0; i < 240; ++i) {
        assert(GetBit(a, i) == 0);
      }
      assert(GetBit(a, 240) == 1);
      ResetBit(a, 240);
      assert(IsZero(a));
    }
  }

  SL void TestCorrectness() {
    for (int i = -100; i <= 100; ++i)
      for (int j = -100; j <= 100; ++j) {
        TestT a(i), b(j);
        assert(i + j == a + b);
        assert(i - j == a - b);
        assert(i * j == a * b);
        if (j != 0) {
          assert(i / j == a / b);
          assert(i % j == a % b);
        }
        if (i >= 0 && j >= 0) {
          assert((i & j) == (a & b));
          assert((i ^ j) == (a ^ b));
          assert((i | j) == (a | b));
        }
        assert((i > j) == (bool)(a > b));
        assert((i < j) == (bool)(a < b));
        assert((i == j) == (bool)(a == b));
        assert((i >= j) == (bool)(a >= b));
        assert((i <= j) == (bool)(a <= b));
      }
  }

  SL void TestUtilities() {
    PowerMod(TestT(5), 10, TestT("123456789"));
    PowerMod(TestT(5), TestT(10), TestT("123456789"));

    Power(TestT(2), 10u);
    Power(TestT(2), 10);

    Power(TestT(2), 20);
    Power(TestT(2), 20LL);
  }
};

SL void GbiTest_BigInteger() {
  gbi_test::GbiTests<BigInteger> tester;
  tester.TestConstructor();
  tester.TestAssignmentOperator();
  tester.TestAsmdOperator();
  tester.TestCompareOperator();
  tester.TestBitOperator();
  tester.TestCorrectness();
  tester.TestUtilities();
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
  tester.TestCorrectness();
  tester.TestUtilities();
  Gcd(12_mpi, 8_mpi);
  123456789123456789_mpi * 2 * 5_mpi * "10"_mpi;
}

PE_REGISTER_TEST(&GbiTest_MpInteger, "GbiTest_MpInteger", SMALL);
#endif
}  // namespace gbi_test