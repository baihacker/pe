#include "pe_test.h"

#if ENABLE_GMP
namespace mpi_test {

template <typename T>
SL void TestConstructorImpl() {
  assert(MpInteger(std::numeric_limits<T>::min()).ToInt<T>() ==
         std::numeric_limits<T>::min());
  assert(MpInteger(std::numeric_limits<T>::max()).ToInt<T>() ==
         std::numeric_limits<T>::max());
}

SL void TestConstructor() {
  MpInteger x;
  // TestConstructorImp<bool>();
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
  MpInteger x;
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
  MpInteger x;
  x = s;
  assert(x.ToString() == s);
}

template <typename T>
SL void TestAsmdImpl() {
  MpInteger x;
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

  for (int a = -10; a <= 10; ++a) {
    for (int b = -10; b <= 10; ++b) {
      assert((MpInteger(a) + MpInteger(b)).ToInt<int>() == (a + b));
      assert((MpInteger(a) += MpInteger(b)).ToInt<int>() == (a + b));
      assert((MpInteger(a) - MpInteger(b)).ToInt<int>() == (a - b));
      assert((MpInteger(a) -= MpInteger(b)).ToInt<int>() == (a - b));
      assert((MpInteger(a) * MpInteger(b)).ToInt<int>() == (a * b));
      assert((MpInteger(a) *= MpInteger(b)).ToInt<int>() == (a * b));
      if (b != 0) {
        assert((MpInteger(a) / MpInteger(b)).ToInt<int>() == (a / b));
        assert((MpInteger(a) /= MpInteger(b)).ToInt<int>() == (a / b));
        assert((MpInteger(a) % MpInteger(b)).ToInt<int>() == (a % b));
        assert((MpInteger(a) %= MpInteger(b)).ToInt<int>() == (a % b));
      }
      if (a >= 0 && b >= 0) {
        assert((MpInteger(a) | MpInteger(b)).ToInt<int>() == (a | b));
        assert((MpInteger(a) |= MpInteger(b)).ToInt<int>() == (a | b));
        assert((MpInteger(a) & MpInteger(b)).ToInt<int>() == (a & b));
        assert((MpInteger(a) &= MpInteger(b)).ToInt<int>() == (a & b));
        assert((MpInteger(a) ^ MpInteger(b)).ToInt<int>() == (a ^ b));
        assert((MpInteger(a) ^= MpInteger(b)).ToInt<int>() == (a ^ b));
      }

      assert((MpInteger(a) + b).ToInt<int>() == (a + b));
      assert((MpInteger(a) += b).ToInt<int>() == (a + b));
      assert((MpInteger(a) - b).ToInt<int>() == (a - b));
      assert((MpInteger(a) -= b).ToInt<int>() == (a - b));
      assert((MpInteger(a) * b).ToInt<int>() == (a * b));
      assert((MpInteger(a) *= b).ToInt<int>() == (a * b));
      if (b != 0) {
        assert((MpInteger(a) / b).ToInt<int>() == (a / b));
        assert((MpInteger(a) /= b).ToInt<int>() == (a / b));
        assert((MpInteger(a) % b).ToInt<int>() == (a % b));
        assert((MpInteger(a) %= b).ToInt<int>() == (a % b));
      }
      if (a >= 0 && b >= 0) {
        assert((MpInteger(a) | b).ToInt<int>() == (a | b));
        assert((MpInteger(a) |= b).ToInt<int>() == (a | b));
        assert((MpInteger(a) & b).ToInt<int>() == (a & b));
        assert((MpInteger(a) &= b).ToInt<int>() == (a & b));
        assert((MpInteger(a) ^ b).ToInt<int>() == (a ^ b));
        assert((MpInteger(a) ^= b).ToInt<int>() == (a ^ b));
      }

      assert((a + MpInteger(b)).ToInt<int>() == (a + b));
      assert((a - MpInteger(b)).ToInt<int>() == (a - b));
      assert((a * MpInteger(b)).ToInt<int>() == (a * b));
      if (b != 0) {
        assert((a / MpInteger(b)).ToInt<int>() == (a / b));
        assert((a % MpInteger(b)).ToInt<int>() == (a % b));
      }
      if (a >= 0 && b >= 0) {
        assert((a | MpInteger(b)).ToInt<int>() == (a | b));
        assert((a & MpInteger(b)).ToInt<int>() == (a & b));
        assert((a ^ MpInteger(b)).ToInt<int>() == (a ^ b));
      }
    }
  }

  for (int64 A : {-10000000000LL, -10000LL, 0LL, 10000LL, 10000000000LL}) {
    for (int64 a = A - 10; a <= A + 10; ++a) {
      for (int64 b = -10; b <= 10; ++b) {
        assert((MpInteger(a) + MpInteger(b)).ToInt<int64>() == (a + b));
        assert((MpInteger(a) += MpInteger(b)).ToInt<int64>() == (a + b));
        assert((MpInteger(a) - MpInteger(b)).ToInt<int64>() == (a - b));
        assert((MpInteger(a) -= MpInteger(b)).ToInt<int64>() == (a - b));
        assert((MpInteger(a) * MpInteger(b)).ToInt<int64>() == (a * b));
        assert((MpInteger(a) *= MpInteger(b)).ToInt<int64>() == (a * b));
        if (b != 0) {
          assert((MpInteger(a) / MpInteger(b)).ToInt<int64>() == (a / b));
          assert((MpInteger(a) /= MpInteger(b)).ToInt<int64>() == (a / b));
          assert((MpInteger(a) % MpInteger(b)).ToInt<int64>() == (a % b));
          assert((MpInteger(a) %= MpInteger(b)).ToInt<int64>() == (a % b));
        }
        if (a >= 0 && b >= 0) {
          assert((MpInteger(a) | MpInteger(b)).ToInt<int64>() == (a | b));
          assert((MpInteger(a) |= MpInteger(b)).ToInt<int64>() == (a | b));
          assert((MpInteger(a) & MpInteger(b)).ToInt<int64>() == (a & b));
          assert((MpInteger(a) &= MpInteger(b)).ToInt<int64>() == (a & b));
          assert((MpInteger(a) ^ MpInteger(b)).ToInt<int64>() == (a ^ b));
          assert((MpInteger(a) ^= MpInteger(b)).ToInt<int64>() == (a ^ b));
        }

        assert((MpInteger(a) + b).ToInt<int64>() == (a + b));
        assert((MpInteger(a) += b).ToInt<int64>() == (a + b));
        assert((MpInteger(a) - b).ToInt<int64>() == (a - b));
        assert((MpInteger(a) -= b).ToInt<int64>() == (a - b));
        assert((MpInteger(a) * b).ToInt<int64>() == (a * b));
        assert((MpInteger(a) *= b).ToInt<int64>() == (a * b));
        if (b != 0) {
          assert((MpInteger(a) / b).ToInt<int64>() == (a / b));
          assert((MpInteger(a) /= b).ToInt<int64>() == (a / b));
          assert((MpInteger(a) % b).ToInt<int64>() == (a % b));
          assert((MpInteger(a) %= b).ToInt<int64>() == (a % b));
        }
        if (a >= 0 && b >= 0) {
          assert((MpInteger(a) | b).ToInt<int64>() == (a | b));
          assert((MpInteger(a) |= b).ToInt<int64>() == (a | b));
          assert((MpInteger(a) & b).ToInt<int64>() == (a & b));
          assert((MpInteger(a) &= b).ToInt<int64>() == (a & b));
          assert((MpInteger(a) ^ b).ToInt<int64>() == (a ^ b));
          assert((MpInteger(a) ^= b).ToInt<int64>() == (a ^ b));
        }

        assert((a + MpInteger(b)).ToInt<int64>() == (a + b));
        assert((a - MpInteger(b)).ToInt<int64>() == (a - b));
        assert((a * MpInteger(b)).ToInt<int64>() == (a * b));
        if (b != 0) {
          assert((a / MpInteger(b)).ToInt<int64>() == (a / b));
          assert((a % MpInteger(b)).ToInt<int64>() == (a % b));
        }
        if (a >= 0 && b >= 0) {
          assert((a | MpInteger(b)).ToInt<int64>() == (a | b));
          assert((a & MpInteger(b)).ToInt<int64>() == (a & b));
          assert((a ^ MpInteger(b)).ToInt<int64>() == (a ^ b));
        }
      }
    }
  }
}

template <typename T>
SL void TestCompareOperatorImpl() {
  MpInteger x;
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
  MpInteger x;
  for (int i = 0; i <= 19; ++i) x.SetBit(i);
  assert(x.ToInt<int>() == 1048575);
  x.RevBit(0);
  assert(x.ToInt<int>() == 1048574);
  x.ResetBit(1);
  assert(x.ToInt<int>() == 1048572);
  assert(x.Popcount() == 18);

  MpInteger y;
  y.SetBit(0);

  x = x | y;
  assert(x.ToInt<int>() == 1048573);

  x = x & MpInteger(1048575 - 4);
  assert(x.ToInt<int>() == 1048573 - 4);

  x = x ^ x;
  assert(x.ToInt<int>() == 0);

  x = x ^ y;
  assert(x.ToInt<int>() == 1);

  {
    MpInteger z;
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
    MpInteger a;
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

SL void TestUtilities() {
  PowerMod(MpInteger(5), 10, MpInteger("123456789"));
  PowerMod(MpInteger(5), MpInteger(10), MpInteger("123456789"));

  Power(MpInteger(2), 10u);
  Power(MpInteger(2), 10);

  Gcd(12_mpi, 8_mpi);
  123456789123456789_mpi * 2 * 5_mpi * "10"_mpi;

  Power(MpInteger(2), 20);
  Power(MpInteger(2), 20LL);

  TimeRecorder tr;
  MpInteger v(1);
  for (int i = 1; i <= 100000; ++i) v *= i;
  // std::cout << tr.Elapsed().Format() << " " << v.bitCount() << std::endl;
}

SL void MpiTest() {
  TestConstructor();
  TestAssignmentOperator();
  TestAsmdOperator();
  TestCompareOperator();
  TestBitOperator();
  TestUtilities();
}

PE_REGISTER_TEST(&MpiTest, "MpiTest", SMALL);
}  // namespace mpi_test
#endif