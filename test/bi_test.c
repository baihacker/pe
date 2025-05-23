#include "pe_test.h"

namespace bi_test {
template <typename T>
SL void TestConstructorImpl() {
  assert(BigInteger(std::numeric_limits<T>::min()).ToInt<T>() ==
         std::numeric_limits<T>::min());
  assert(BigInteger(std::numeric_limits<T>::max()).ToInt<T>() ==
         std::numeric_limits<T>::max());
}

SL void TestConstructor() {
  BigInteger x;
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

template <typename T>
SL void TestAssignmentImpl() {
  BigInteger x;
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
  TestAssignmentImpl<int256e>();
  TestAssignmentImpl<uint256e>();

  std::string s = "123456789123456789123456789";
  BigInteger x;
  x = s;
  assert(x.ToString() == s);
}

template <typename T>
SL void TestAsmdImpl() {
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
SL void TestCompareOperatorImpl() {
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
  BigInteger x;
  for (int i = 0; i <= 19; ++i) x.SetBit(i);
  assert(x.ToInt<int>() == 1048575);
  x.RevBit(0);
  assert(x.ToInt<int>() == 1048574);
  x.ResetBit(1);
  assert(x.ToInt<int>() == 1048572);
  assert(x.Popcount() == 18);

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

  {
    BigInteger z;
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
  // std::cout << tr.Elapsed().Format() << " " << v.bitCount() << std::endl;
}

template <typename T>
SL void BiCorrectnessTestImpl() {
  for (int i = -100; i <= 100; ++i)
    for (int j = -100; j <= 100; ++j) {
      T a(i), b(j);
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

SL void BiTest() {
  TestConstructor();
  TestAssignmentOperator();
  TestAsmdOperator();
  TestCompareOperator();
  TestBitOperator();
  TestUtilities();
  BiCorrectnessTestImpl<BigInteger>();
#if ENABLE_GMP
  BiCorrectnessTestImpl<MpInteger>();
#endif
}
PE_REGISTER_TEST(&BiTest, "BiTest", SMALL);
}  // namespace bi_test