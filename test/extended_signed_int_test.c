#include "pe_test.h"

namespace extended_signed_int_test {
#if PE_HAS_INT128 && 0
using Int256 = pe::ExtendedSignedInt<uint128>;
using Int512 = pe::ExtendedSignedInt<Int256>;
using Int1024 = pe::ExtendedSignedInt<Int512>;
#else
using Int128 = pe::ExtendedSignedInt<uint64>;
using Int256 = pe::ExtendedSignedInt<Int128>;
using Int512 = pe::ExtendedSignedInt<Int256>;
#endif
using TestT = Int512;

template <typename T>
SL void TestConstructorImpl() {
  assert(TestT(std::numeric_limits<T>::min()).ToInt<T>() ==
         std::numeric_limits<T>::min());
  assert(TestT(std::numeric_limits<T>::max()).ToInt<T>() ==
         std::numeric_limits<T>::max());
}

SL void TestConstructor() {
  TestT x;
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
  std::string s = "123456789123456789123456789";
  assert(TestT(s).ToString() == s);
  s = "000000000";
  assert(TestT(s).ToString() == "0");

  s = "-123456789123456789123456789";
  assert(TestT(s).ToString() == s);

  assert(TestT(-1).Popcount() == sizeof(TestT) * 8);

  assert((TestT(-1) >> 10).Popcount() == sizeof(TestT) * 8);
}

template <typename T>
SL void TestAssignmentImpl() {
  TestT x;
  x = T();
  assert(x.ToInt<T>() == T());

  x = std::numeric_limits<T>::max();
  assert(x.ToInt<T>() == std::numeric_limits<T>::max());

  x = std::numeric_limits<T>::min();
  assert(x.ToInt<T>() == std::numeric_limits<T>::min());
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

  std::string s = "123456789123456789123456789";
  TestT x;
  x = s;
  assert(x.ToString() == s);
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

  for (int A : {-10000, 0, 10000}) {
    for (int i = A - 10; i <= A + 10; ++i) {
      for (int j = -10; j <= 10; ++j) {
        int a = i;
        int b = j;
        assert((TestT(a) + TestT(b)).ToInt<int>() == (a + b));
        assert((TestT(a) += TestT(b)).ToInt<int>() == (a + b));
        assert((TestT(a) - TestT(b)).ToInt<int>() == (a - b));
        assert((TestT(a) -= TestT(b)).ToInt<int>() == (a - b));
        assert((TestT(a) * TestT(b)).ToInt<int>() == (a * b));
        assert((TestT(a) *= TestT(b)).ToInt<int>() == (a * b));
        if (b != 0) {
          assert((TestT(a) / TestT(b)).ToInt<int>() == (a / b));
          assert((TestT(a) /= TestT(b)).ToInt<int>() == (a / b));
          assert((TestT(a) % TestT(b)).ToInt<int>() == (a % b));
          assert((TestT(a) %= TestT(b)).ToInt<int>() == (a % b));
        }
        if (a >= 0 && b >= 0) {
          assert((TestT(a) | TestT(b)).ToInt<int>() == (a | b));
          assert((TestT(a) |= TestT(b)).ToInt<int>() == (a | b));
          assert((TestT(a) & TestT(b)).ToInt<int>() == (a & b));
          assert((TestT(a) &= TestT(b)).ToInt<int>() == (a & b));
          assert((TestT(a) ^ TestT(b)).ToInt<int>() == (a ^ b));
          assert((TestT(a) ^= TestT(b)).ToInt<int>() == (a ^ b));
        }

        assert((TestT(a) + b).ToInt<int>() == (a + b));
        assert((TestT(a) += b).ToInt<int>() == (a + b));
        assert((TestT(a) - b).ToInt<int>() == (a - b));
        assert((TestT(a) -= b).ToInt<int>() == (a - b));
        assert((TestT(a) * b).ToInt<int>() == (a * b));
        assert((TestT(a) *= b).ToInt<int>() == (a * b));
        if (b != 0) {
          assert((TestT(a) / b).ToInt<int>() == (a / b));
          assert((TestT(a) /= b).ToInt<int>() == (a / b));
          assert((TestT(a) % b).ToInt<int>() == (a % b));
          assert((TestT(a) %= b).ToInt<int>() == (a % b));
        }
        if (a >= 0 && b >= 0) {
          assert((TestT(a) | b).ToInt<int>() == (a | b));
          assert((TestT(a) |= b).ToInt<int>() == (a | b));
          assert((TestT(a) & b).ToInt<int>() == (a & b));
          assert((TestT(a) &= b).ToInt<int>() == (a & b));
          assert((TestT(a) ^ b).ToInt<int>() == (a ^ b));
          assert((TestT(a) ^= b).ToInt<int>() == (a ^ b));
        }

        assert((a + TestT(b)).ToInt<int>() == (a + b));
        assert((a - TestT(b)).ToInt<int>() == (a - b));
        assert((a * TestT(b)).ToInt<int>() == (a * b));
        if (b != 0) {
          assert((a / TestT(b)).ToInt<int>() == (a / b));
          assert((a % TestT(b)).ToInt<int>() == (a % b));
        }
        if (a >= 0 && b >= 0) {
          assert((a | TestT(b)).ToInt<int>() == (a | b));
          assert((a & TestT(b)).ToInt<int>() == (a & b));
          assert((a ^ TestT(b)).ToInt<int>() == (a ^ b));
        }
      }
    }
  }

  for (int64 A : {-10000000000LL, -10000LL, 0LL, 10000LL, 10000000000LL}) {
    for (int64 i = A - 10; i <= A + 10; ++i) {
      for (int64 j = -10; j <= 10; ++j) {
        int64 a = i;
        int64 b = j;
        assert((TestT(a) + TestT(b)).ToInt<int64>() == (a + b));
        assert((TestT(a) += TestT(b)).ToInt<int64>() == (a + b));
        assert((TestT(a) - TestT(b)).ToInt<int64>() == (a - b));
        assert((TestT(a) -= TestT(b)).ToInt<int64>() == (a - b));
        assert((TestT(a) * TestT(b)).ToInt<int64>() == (a * b));
        assert((TestT(a) *= TestT(b)).ToInt<int64>() == (a * b));
        if (b != 0) {
          assert((TestT(a) / TestT(b)).ToInt<int64>() == (a / b));
          assert((TestT(a) /= TestT(b)).ToInt<int64>() == (a / b));
          assert((TestT(a) % TestT(b)).ToInt<int64>() == (a % b));
          assert((TestT(a) %= TestT(b)).ToInt<int64>() == (a % b));
        }
        if (a >= 0 && b >= 0) {
          assert((TestT(a) | TestT(b)).ToInt<int64>() == (a | b));
          assert((TestT(a) |= TestT(b)).ToInt<int64>() == (a | b));
          assert((TestT(a) & TestT(b)).ToInt<int64>() == (a & b));
          assert((TestT(a) &= TestT(b)).ToInt<int64>() == (a & b));
          assert((TestT(a) ^ TestT(b)).ToInt<int64>() == (a ^ b));
          assert((TestT(a) ^= TestT(b)).ToInt<int64>() == (a ^ b));
        }

        assert((TestT(a) + b).ToInt<int64>() == (a + b));
        assert((TestT(a) += b).ToInt<int64>() == (a + b));
        assert((TestT(a) - b).ToInt<int64>() == (a - b));
        assert((TestT(a) -= b).ToInt<int64>() == (a - b));
        assert((TestT(a) * b).ToInt<int64>() == (a * b));
        assert((TestT(a) *= b).ToInt<int64>() == (a * b));
        if (b != 0) {
          assert((TestT(a) / b).ToInt<int64>() == (a / b));
          assert((TestT(a) /= b).ToInt<int64>() == (a / b));
          assert((TestT(a) % b).ToInt<int64>() == (a % b));
          assert((TestT(a) %= b).ToInt<int64>() == (a % b));
        }
        if (a >= 0 && b >= 0) {
          assert((TestT(a) | b).ToInt<int64>() == (a | b));
          assert((TestT(a) |= b).ToInt<int64>() == (a | b));
          assert((TestT(a) & b).ToInt<int64>() == (a & b));
          assert((TestT(a) &= b).ToInt<int64>() == (a & b));
          assert((TestT(a) ^ b).ToInt<int64>() == (a ^ b));
          assert((TestT(a) ^= b).ToInt<int64>() == (a ^ b));
        }

        assert((a + TestT(b)).ToInt<int64>() == (a + b));
        assert((a - TestT(b)).ToInt<int64>() == (a - b));
        assert((a * TestT(b)).ToInt<int64>() == (a * b));
        if (b != 0) {
          assert((a / TestT(b)).ToInt<int64>() == (a / b));
          assert((a % TestT(b)).ToInt<int64>() == (a % b));
        }
        if (a >= 0 && b >= 0) {
          assert((a | TestT(b)).ToInt<int64>() == (a | b));
          assert((a & TestT(b)).ToInt<int64>() == (a & b));
          assert((a ^ TestT(b)).ToInt<int64>() == (a ^ b));
        }
      }
    }
  }

  for (int s1 = -1; s1 <= 1; s1 += 2)
    for (int s2 = -1; s2 <= 1; s2 += 2)
      for (int e1 = 10; e1 <= 30; e1 += 4) {
        BigInteger A = s1 * Power("10"_bi, e1);
        for (int e2 = 10; e2 <= 30; e2 += 4) {
          BigInteger B = s2 * Power("10"_bi, e2);
          for (int64 i = -10; i <= 10; ++i) {
            for (int64 j = -10; j <= 10; ++j) {
              BigInteger a = A + i;
              BigInteger b = B + j;
              TestT x = a.ToString();
              TestT y = b.ToString();
              assert((x + y).ToString() == (a + b).ToString());
              assert((x - y).ToString() == (a - b).ToString());
              assert((x * y).ToString() == (a * b).ToString());
              assert((x / y).ToString() == (a / b).ToString());
              assert((x % y).ToString() == (a % b).ToString());
              auto [aa, bb] = Div(a, b);
              auto [xx, yy] = Div(x, y);
              assert(xx.ToString() == aa.ToString());
              assert(yy.ToString() == bb.ToString());
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
}

SL void TestBitOperator() {
  TestT x;
  for (int i = 0; i <= 19; ++i) x.SetBit(i);
  assert(x.ToInt<int>() == 1048575);
  x.RevBit(0);
  assert(x.ToInt<int>() == 1048574);
  x.ResetBit(1);
  assert(x.ToInt<int>() == 1048572);
  assert(x.Popcount() == 18);

  TestT y;
  y.SetBit(0);

  x = x | y;
  assert(x.ToInt<int>() == 1048573);

  x = x & TestT(1048575 - 4);
  assert(x.ToInt<int>() == 1048573 - 4);

  x = x ^ x;
  assert(x.ToInt<int>() == 0);

  x = x ^ y;
  assert(x.ToInt<int>() == 1);
}

SL void TestUtilities() {
  Power(TestT(2), 10u);
  Power(TestT(2), 10);

  Power(TestT(2), 20);
  Power(TestT(2), 20LL);

  TimeRecorder tr;
  TestT v(1);
  for (int i = 1; i <= 100000; ++i) v *= i;
  // std::cout << tr.Elapsed().Format() << " " << v.bitCount() << std::endl;

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

SL void ExtendedSignedIntTest() {
  TestConstructor();
  TestAssignmentOperator();
  TestAsmdOperator();
  TestCompareOperator();
  TestBitOperator();
  TestUtilities();
}
PE_REGISTER_TEST(&ExtendedSignedIntTest, "ExtendedSignedIntTest", MEDIUM);
}  // namespace extended_signed_int_test
