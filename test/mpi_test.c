#include "pe_test.h"

#if ENABLE_GMP
namespace mpi_test {

template <typename T>
SL void TestConstructorImp() {
  // std::cout << typeid(T).name() << std::endl;
  // std::cout << MpInteger(T()) << std::endl;
  // std::cout << MpInteger(T(0)) << " " << MpInteger(T(1)) << std::endl;
  // std::cout << MpInteger(std::numeric_limits<T>::min()) << " " <<
  // std::numeric_limits<T>::min()
  //      << std::endl;
  // std::cout << MpInteger(std::numeric_limits<T>::max()) << " " <<
  // std::numeric_limits<T>::max()
  //      << std::endl;

  assert(MpInteger(std::numeric_limits<T>::min()).ToInt<T>() ==
         std::numeric_limits<T>::min());
  assert(MpInteger(std::numeric_limits<T>::max()).ToInt<T>() ==
         std::numeric_limits<T>::max());

  // std::cout << std::endl;
}

SL void TestConstructor() {
  // std::cout << MpInteger() << std::endl;
  MpInteger x;
  // TestConstructorImp<bool>();
  TestConstructorImp<char>();
  TestConstructorImp<signed char>();
  TestConstructorImp<unsigned char>();
  TestConstructorImp<short>();
  TestConstructorImp<int>();
  TestConstructorImp<long>();
  TestConstructorImp<long long>();
#if PE_HAS_INT128
  TestConstructorImp<int128>();
#endif
  TestConstructorImp<unsigned short>();
  TestConstructorImp<unsigned int>();
  TestConstructorImp<unsigned long>();
  TestConstructorImp<unsigned long long>();
#if PE_HAS_INT128
  TestConstructorImp<uint128>();
#endif
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
  TestAssignmentImpl<signed char>();
  TestAssignmentImpl<unsigned char>();
  TestAssignmentImpl<short>();
  TestAssignmentImpl<int>();
  TestAssignmentImpl<long>();
  TestAssignmentImpl<long long>();
#if PE_HAS_INT128
  TestAssignmentImpl<int128>();
#endif
  TestAssignmentImpl<unsigned short>();
  TestAssignmentImpl<unsigned int>();
  TestAssignmentImpl<unsigned long>();
  TestAssignmentImpl<unsigned long long>();
#if PE_HAS_INT128
  TestAssignmentImpl<uint128>();
#endif

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
}

SL void TestAsmdOperator() {
  // TestAsmdImpl<bool>();
  TestAsmdImpl<char>();
  TestAsmdImpl<signed char>();
  TestAsmdImpl<unsigned char>();
  TestAsmdImpl<short>();
  TestAsmdImpl<int>();
  TestAsmdImpl<long>();
  TestAsmdImpl<long long>();
#if PE_HAS_INT128
  TestAsmdImpl<int128>();
#endif
  TestAsmdImpl<unsigned short>();
  TestAsmdImpl<unsigned int>();
  TestAsmdImpl<unsigned long>();
  TestAsmdImpl<unsigned long long>();
#if PE_HAS_INT128
  TestAsmdImpl<uint128>();
#endif

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
  TestCompareOperatorImpl<signed char>();
  TestCompareOperatorImpl<unsigned char>();
  TestCompareOperatorImpl<short>();
  TestCompareOperatorImpl<int>();
  TestCompareOperatorImpl<long>();
  TestCompareOperatorImpl<long long>();
#if PE_HAS_INT128
  TestCompareOperatorImpl<int128>();
#endif
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
  assert(x.BitCount() == 18);

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