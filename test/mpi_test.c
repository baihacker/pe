#include <pe.hpp>

#if ENABLE_GMP
namespace mpi_test {

template <typename T>
SL void test_constructor_internal() {
  /*cout << typeid(T).name() << endl;
  cout << MpInteger(T()) << endl;
  cout << MpInteger(T(0)) << " " << MpInteger(T(1)) << endl;
  cout << MpInteger(numeric_limits<T>::min()) << " " << numeric_limits<T>::min()
       << endl;
  cout << MpInteger(numeric_limits<T>::max()) << " " << numeric_limits<T>::max()
       << endl;*/

  assert(MpInteger(numeric_limits<T>::min()).ToInt<T>() ==
         numeric_limits<T>::min());
  assert(MpInteger(numeric_limits<T>::max()).ToInt<T>() ==
         numeric_limits<T>::max());

  // cout << endl;
}

SL void test_constructor() {
  // cout << MpInteger() << endl;
  MpInteger x;
  // test_constructor_internal<bool>();
  test_constructor_internal<char>();
  test_constructor_internal<signed char>();
  test_constructor_internal<unsigned char>();
  test_constructor_internal<short>();
  test_constructor_internal<int>();
  test_constructor_internal<long>();
  test_constructor_internal<long long>();
#if PE_HAS_INT128
  test_constructor_internal<int128>();
#endif
  test_constructor_internal<unsigned short>();
  test_constructor_internal<unsigned int>();
  test_constructor_internal<unsigned long>();
  test_constructor_internal<unsigned long long>();
#if PE_HAS_INT128
  test_constructor_internal<uint128>();
#endif
}

template <typename T>
SL void test_assignment_internal() {
  MpInteger x;
  x = T();
  assert(x.ToInt<T>() == T());

  x = numeric_limits<T>::max();
  assert(x.ToInt<T>() == numeric_limits<T>::max());

  x = numeric_limits<T>::min();
  assert(x.ToInt<T>() == numeric_limits<T>::min());
}

SL void test_assignment_operator() {
  // test_assignment_internal<bool>();
  test_assignment_internal<char>();
  test_assignment_internal<signed char>();
  test_assignment_internal<unsigned char>();
  test_assignment_internal<short>();
  test_assignment_internal<int>();
  test_assignment_internal<long>();
  test_assignment_internal<long long>();
#if PE_HAS_INT128
  test_assignment_internal<int128>();
#endif
  test_assignment_internal<unsigned short>();
  test_assignment_internal<unsigned int>();
  test_assignment_internal<unsigned long>();
  test_assignment_internal<unsigned long long>();
#if PE_HAS_INT128
  test_assignment_internal<uint128>();
#endif

  string s = "123456789123456789123456789";
  MpInteger x;
  x = s;
  assert(x.ToString() == s);
}

template <typename T>
SL void test_asmd_internal() {
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

SL void test_asmd_operator() {
  // test_asmd_internal<bool>();
  test_asmd_internal<char>();
  test_asmd_internal<signed char>();
  test_asmd_internal<unsigned char>();
  test_asmd_internal<short>();
  test_asmd_internal<int>();
  test_asmd_internal<long>();
  test_asmd_internal<long long>();
#if PE_HAS_INT128
  test_asmd_internal<int128>();
#endif
  test_asmd_internal<unsigned short>();
  test_asmd_internal<unsigned int>();
  test_asmd_internal<unsigned long>();
  test_asmd_internal<unsigned long long>();
#if PE_HAS_INT128
  test_asmd_internal<uint128>();
#endif

  for (int a = -10; a <= 10; ++a)
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

  for (int64 A : {-10000000000LL, -10000LL, 0LL, 10000LL, 10000000000LL})
    for (int64 a = A - 10; a <= A + 10; ++a)
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

template <typename T>
SL void test_compare_operator_internal() {
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

SL void test_compare_operator() {
  // test_compare_operator_internal<bool>();
  test_compare_operator_internal<char>();
  test_compare_operator_internal<signed char>();
  test_compare_operator_internal<unsigned char>();
  test_compare_operator_internal<short>();
  test_compare_operator_internal<int>();
  test_compare_operator_internal<long>();
  test_compare_operator_internal<long long>();
#if PE_HAS_INT128
  test_compare_operator_internal<int128>();
#endif
  test_compare_operator_internal<unsigned short>();
  test_compare_operator_internal<unsigned int>();
  test_compare_operator_internal<unsigned long>();
  test_compare_operator_internal<unsigned long long>();
#if PE_HAS_INT128
  test_compare_operator_internal<uint128>();
#endif
}

SL void test_bit_operator() {
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

SL void test_utilities() {
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
  // cout << tr.elapsed().format() << " " << v.bitCount() << endl;
}

SL void mpi_test() {
  test_constructor();
  test_assignment_operator();
  test_asmd_operator();
  test_compare_operator();
  test_bit_operator();
  test_utilities();
}

PE_REGISTER_TEST(&mpi_test, "mpi_test", SMALL);
}  // namespace mpi_test
#endif