#include "pe_test.h"

namespace bi_test {
template <typename T>
SL void TestConstructorInternal() {
  // cout << typeid(T).name() << endl;
  // cout << BigInteger(T()) << endl;
  // cout << BigInteger(T(0)) << " " << BigInteger(T(1)) << endl;
  // cout << BigInteger(std::numeric_limits<T>::min()) << " "
  //      << std::numeric_limits<T>::min() << endl;
  // cout << BigInteger(std::numeric_limits<T>::max()) << " "
  //      << std::numeric_limits<T>::max() << endl;

  assert(BigInteger(std::numeric_limits<T>::min()).ToInt<T>() ==
         std::numeric_limits<T>::min());
  assert(BigInteger(std::numeric_limits<T>::max()).ToInt<T>() ==
         std::numeric_limits<T>::max());

  // cout << endl;
}

SL void TestConstructor() {
  // cout << BigInteger() << endl;
  BigInteger x;
  // TestConstructorInternal<bool>();
  TestConstructorInternal<char>();
  TestConstructorInternal<signed char>();
  TestConstructorInternal<unsigned char>();
  TestConstructorInternal<short>();
  TestConstructorInternal<int>();
  TestConstructorInternal<long>();
  TestConstructorInternal<long long>();
#if PE_HAS_INT128
  TestConstructorInternal<int128>();
#endif
  TestConstructorInternal<unsigned short>();
  TestConstructorInternal<unsigned int>();
  TestConstructorInternal<unsigned long>();
  TestConstructorInternal<unsigned long long>();
#if PE_HAS_INT128
  TestConstructorInternal<uint128>();
#endif
}

template <typename T>
SL void TestAssignmentInternal() {
  BigInteger x;
  x = T();
  assert(x.ToInt<T>() == T());

  x = std::numeric_limits<T>::max();
  assert(x.ToInt<T>() == std::numeric_limits<T>::max());

  x = std::numeric_limits<T>::min();
  assert(x.ToInt<T>() == std::numeric_limits<T>::min());
}

SL void TestAssignmentOperator() {
  // TestAssignmentInternal<bool>();
  TestAssignmentInternal<char>();
  TestAssignmentInternal<signed char>();
  TestAssignmentInternal<unsigned char>();
  TestAssignmentInternal<short>();
  TestAssignmentInternal<int>();
  TestAssignmentInternal<long>();
  TestAssignmentInternal<long long>();
#if PE_HAS_INT128
  TestAssignmentInternal<int128>();
#endif
  TestAssignmentInternal<unsigned short>();
  TestAssignmentInternal<unsigned int>();
  TestAssignmentInternal<unsigned long>();
  TestAssignmentInternal<unsigned long long>();
#if PE_HAS_INT128
  TestAssignmentInternal<uint128>();
#endif

  string s = "123456789123456789123456789";
  BigInteger x;
  x = s;
  assert(x.ToString() == s);
}

template <typename T>
SL void TestAsmdInternal() {
  BigInteger x;
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
}

SL void TestAsmdOperator() {
  // TestAsmdInternal<bool>();
  TestAsmdInternal<char>();
  TestAsmdInternal<signed char>();
  TestAsmdInternal<unsigned char>();
  TestAsmdInternal<short>();
  TestAsmdInternal<int>();
  TestAsmdInternal<long>();
  TestAsmdInternal<long long>();
#if PE_HAS_INT128
  TestAsmdInternal<int128>();
#endif
  TestAsmdInternal<unsigned short>();
  TestAsmdInternal<unsigned int>();
  TestAsmdInternal<unsigned long>();
  TestAsmdInternal<unsigned long long>();
#if PE_HAS_INT128
  TestAsmdInternal<uint128>();
#endif

  for (int A : {-10000, 0, 10000}) {
    for (int a = A - 10; a <= A + 10; ++a) {
      for (int b = -10; b <= 10; ++b) {
        assert((BigInteger(a) + BigInteger(b)).ToInt<int>() == (a + b));
        assert((BigInteger(a) += BigInteger(b)).ToInt<int>() == (a + b));
        assert((BigInteger(a) - BigInteger(b)).ToInt<int>() == (a - b));
        assert((BigInteger(a) -= BigInteger(b)).ToInt<int>() == (a - b));
        assert((BigInteger(a) * BigInteger(b)).ToInt<int>() == (a * b));
        assert((BigInteger(a) *= BigInteger(b)).ToInt<int>() == (a * b));
        if (b != 0) {
          assert((BigInteger(a) / BigInteger(b)).ToInt<int>() == (a / b));
          assert((BigInteger(a) /= BigInteger(b)).ToInt<int>() == (a / b));
          assert((BigInteger(a) % BigInteger(b)).ToInt<int>() == (a % b));
          assert((BigInteger(a) %= BigInteger(b)).ToInt<int>() == (a % b));
        }
        if (a >= 0 && b >= 0) {
          assert((BigInteger(a) | BigInteger(b)).ToInt<int>() == (a | b));
          assert((BigInteger(a) |= BigInteger(b)).ToInt<int>() == (a | b));
          assert((BigInteger(a) & BigInteger(b)).ToInt<int>() == (a & b));
          assert((BigInteger(a) &= BigInteger(b)).ToInt<int>() == (a & b));
          assert((BigInteger(a) ^ BigInteger(b)).ToInt<int>() == (a ^ b));
          assert((BigInteger(a) ^= BigInteger(b)).ToInt<int>() == (a ^ b));
        }

        assert((BigInteger(a) + b).ToInt<int>() == (a + b));
        assert((BigInteger(a) += b).ToInt<int>() == (a + b));
        assert((BigInteger(a) - b).ToInt<int>() == (a - b));
        assert((BigInteger(a) -= b).ToInt<int>() == (a - b));
        assert((BigInteger(a) * b).ToInt<int>() == (a * b));
        assert((BigInteger(a) *= b).ToInt<int>() == (a * b));
        if (b != 0) {
          assert((BigInteger(a) / b).ToInt<int>() == (a / b));
          assert((BigInteger(a) /= b).ToInt<int>() == (a / b));
          assert((BigInteger(a) % b).ToInt<int>() == (a % b));
          assert((BigInteger(a) %= b).ToInt<int>() == (a % b));
        }
        if (a >= 0 && b >= 0) {
          assert((BigInteger(a) | b).ToInt<int>() == (a | b));
          assert((BigInteger(a) |= b).ToInt<int>() == (a | b));
          assert((BigInteger(a) & b).ToInt<int>() == (a & b));
          assert((BigInteger(a) &= b).ToInt<int>() == (a & b));
          assert((BigInteger(a) ^ b).ToInt<int>() == (a ^ b));
          assert((BigInteger(a) ^= b).ToInt<int>() == (a ^ b));
        }

        assert((a + BigInteger(b)).ToInt<int>() == (a + b));
        assert((a - BigInteger(b)).ToInt<int>() == (a - b));
        assert((a * BigInteger(b)).ToInt<int>() == (a * b));
        if (b != 0) {
          assert((a / BigInteger(b)).ToInt<int>() == (a / b));
          assert((a % BigInteger(b)).ToInt<int>() == (a % b));
        }
        if (a >= 0 && b >= 0) {
          assert((a | BigInteger(b)).ToInt<int>() == (a | b));
          assert((a & BigInteger(b)).ToInt<int>() == (a & b));
          assert((a ^ BigInteger(b)).ToInt<int>() == (a ^ b));
        }
      }
    }
  }

  for (int64 A : {-10000000000LL, -10000LL, 0LL, 10000LL, 10000000000LL}) {
    for (int64 a = A - 10; a <= A + 10; ++a) {
      for (int64 b = -10; b <= 10; ++b) {
        assert((BigInteger(a) + BigInteger(b)).ToInt<int64>() == (a + b));
        assert((BigInteger(a) += BigInteger(b)).ToInt<int64>() == (a + b));
        assert((BigInteger(a) - BigInteger(b)).ToInt<int64>() == (a - b));
        assert((BigInteger(a) -= BigInteger(b)).ToInt<int64>() == (a - b));
        assert((BigInteger(a) * BigInteger(b)).ToInt<int64>() == (a * b));
        assert((BigInteger(a) *= BigInteger(b)).ToInt<int64>() == (a * b));
        if (b != 0) {
          assert((BigInteger(a) / BigInteger(b)).ToInt<int64>() == (a / b));
          assert((BigInteger(a) /= BigInteger(b)).ToInt<int64>() == (a / b));
          assert((BigInteger(a) % BigInteger(b)).ToInt<int64>() == (a % b));
          assert((BigInteger(a) %= BigInteger(b)).ToInt<int64>() == (a % b));
        }
        if (a >= 0 && b >= 0) {
          assert((BigInteger(a) | BigInteger(b)).ToInt<int64>() == (a | b));
          assert((BigInteger(a) |= BigInteger(b)).ToInt<int64>() == (a | b));
          assert((BigInteger(a) & BigInteger(b)).ToInt<int64>() == (a & b));
          assert((BigInteger(a) &= BigInteger(b)).ToInt<int64>() == (a & b));
          assert((BigInteger(a) ^ BigInteger(b)).ToInt<int64>() == (a ^ b));
          assert((BigInteger(a) ^= BigInteger(b)).ToInt<int64>() == (a ^ b));
        }

        assert((BigInteger(a) + b).ToInt<int64>() == (a + b));
        assert((BigInteger(a) += b).ToInt<int64>() == (a + b));
        assert((BigInteger(a) - b).ToInt<int64>() == (a - b));
        assert((BigInteger(a) -= b).ToInt<int64>() == (a - b));
        assert((BigInteger(a) * b).ToInt<int64>() == (a * b));
        assert((BigInteger(a) *= b).ToInt<int64>() == (a * b));
        if (b != 0) {
          assert((BigInteger(a) / b).ToInt<int64>() == (a / b));
          assert((BigInteger(a) /= b).ToInt<int64>() == (a / b));
          assert((BigInteger(a) % b).ToInt<int64>() == (a % b));
          assert((BigInteger(a) %= b).ToInt<int64>() == (a % b));
        }
        if (a >= 0 && b >= 0) {
          assert((BigInteger(a) | b).ToInt<int64>() == (a | b));
          assert((BigInteger(a) |= b).ToInt<int64>() == (a | b));
          assert((BigInteger(a) & b).ToInt<int64>() == (a & b));
          assert((BigInteger(a) &= b).ToInt<int64>() == (a & b));
          assert((BigInteger(a) ^ b).ToInt<int64>() == (a ^ b));
          assert((BigInteger(a) ^= b).ToInt<int64>() == (a ^ b));
        }

        assert((a + BigInteger(b)).ToInt<int64>() == (a + b));
        assert((a - BigInteger(b)).ToInt<int64>() == (a - b));
        assert((a * BigInteger(b)).ToInt<int64>() == (a * b));
        if (b != 0) {
          assert((a / BigInteger(b)).ToInt<int64>() == (a / b));
          assert((a % BigInteger(b)).ToInt<int64>() == (a % b));
        }
        if (a >= 0 && b >= 0) {
          assert((a | BigInteger(b)).ToInt<int64>() == (a | b));
          assert((a & BigInteger(b)).ToInt<int64>() == (a & b));
          assert((a ^ BigInteger(b)).ToInt<int64>() == (a ^ b));
        }
      }
    }
  }
}

template <typename T>
SL void TestCompareOperatorInternal() {
  BigInteger x;
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
  // TestCompareOperatorInternal<bool>();
  TestCompareOperatorInternal<char>();
  TestCompareOperatorInternal<signed char>();
  TestCompareOperatorInternal<unsigned char>();
  TestCompareOperatorInternal<short>();
  TestCompareOperatorInternal<int>();
  TestCompareOperatorInternal<long>();
  TestCompareOperatorInternal<long long>();
#if PE_HAS_INT128
  TestCompareOperatorInternal<int128>();
#endif
  TestCompareOperatorInternal<unsigned short>();
  TestCompareOperatorInternal<unsigned int>();
  TestCompareOperatorInternal<unsigned long>();
  TestCompareOperatorInternal<unsigned long long>();
#if PE_HAS_INT128
  TestCompareOperatorInternal<uint128>();
#endif
}

SL void TestBitOperator() {
  BigInteger x;
  for (int i = 0; i <= 19; ++i) x.SetBit(i);
  assert(x.ToInt<int>() == 1048575);
  x.RevBit(0);
  assert(x.ToInt<int>() == 1048574);
  x.ResetBit(1);
  assert(x.ToInt<int>() == 1048572);
  assert(x.BitCount() == 18);

  BigInteger y;
  y.SetBit(0);

  x = x | y;
  assert(x.ToInt<int>() == 1048573);

  x = x & BigInteger(1048575 - 4);
  assert(x.ToInt<int>() == 1048573 - 4);

  x = x ^ x;
  assert(x.ToInt<int>() == 0);

  x = x ^ y;
  assert(x.ToInt<int>() == 1);
}

SL void TestUtilities() {
  PowerMod(BigInteger(5), 10, BigInteger("123456789"));
  PowerMod(BigInteger(5), BigInteger(10), BigInteger("123456789"));

  Power(BigInteger(2), 10u);
  Power(BigInteger(2), 10);

  Gcd(12_bi, 8_bi);
  123456789123456789_bi * 2 * 5_bi * "10"_bi;

  Power(BigInteger(2), 20);
  Power(BigInteger(2), 20LL);

  TimeRecorder tr;
  BigInteger v(1);
  for (int i = 1; i <= 100000; ++i) v *= i;
  // cout << tr.Elapsed().Format() << " " << v.bitCount() << endl;
}

SL void BiTest() {
  TestConstructor();
  TestAssignmentOperator();
  TestAsmdOperator();
  TestCompareOperator();
  TestBitOperator();
  TestUtilities();
}
PE_REGISTER_TEST(&BiTest, "BiTest", SMALL);
}  // namespace bi_test