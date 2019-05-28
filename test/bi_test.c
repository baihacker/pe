#include <pe.hpp>

namespace bi_test {
template <typename T>
SL void test_constructor_internal() {
  /*cout << typeid(T).name() << endl;
  cout << BigInteger(T()) << endl;
  cout << BigInteger(T(0)) << " " << BigInteger(T(1)) << endl;
  cout << BigInteger(numeric_limits<T>::min()) << " "
       << numeric_limits<T>::min() << endl;
  cout << BigInteger(numeric_limits<T>::max()) << " "
       << numeric_limits<T>::max() << endl;*/
#ifndef COMPILER_MSVC
  assert(to_string(BigInteger(numeric_limits<T>::min()).toInt<T>()) ==
         to_string(numeric_limits<T>::min()));
  assert(to_string(BigInteger(numeric_limits<T>::max()).toInt<T>()) ==
         to_string(numeric_limits<T>::max()));
#endif
  // cout << endl;
}

SL void test_constructor() {
  //cout << BigInteger() << endl;
  BigInteger x;
  test_constructor_internal<bool>();
  test_constructor_internal<char>();
  test_constructor_internal<signed char>();
  test_constructor_internal<unsigned char>();
  test_constructor_internal<short>();
  test_constructor_internal<int>();
  test_constructor_internal<long>();
  test_constructor_internal<long long>();
  test_constructor_internal<unsigned short>();
  test_constructor_internal<unsigned int>();
  test_constructor_internal<unsigned long>();
  test_constructor_internal<unsigned long long>();

#if PE_HAS_INT128
  int128 max_int128 = ((uint128)-1) >> 1;
  //cout << BigInteger(max_int128) << " " << max_int128 << endl;
  assert(BigInteger(max_int128).toInt<int128>() == max_int128);

  int128 min_int128 = -max_int128 - 1;
  //cout << BigInteger(min_int128) << " " << min_int128 << endl;
  assert(BigInteger(min_int128).toInt<int128>() == min_int128);

  uint128 max_uint128 = -1;
  //cout << BigInteger(max_uint128) << " " << max_uint128 << endl;
  assert(BigInteger(max_uint128).toInt<uint128>() == max_uint128);
#endif
}

template <typename T>
SL void test_assignment_internal() {
  BigInteger x;
  x = T();
  assert(x.toInt<T>() == T());
#ifndef COMPILER_MSVC
  x = numeric_limits<T>::max();
  assert(x.toInt<T>() == numeric_limits<T>::max());

  x = numeric_limits<T>::min();
  assert(x.toInt<T>() == numeric_limits<T>::min());
#endif
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
  test_assignment_internal<unsigned short>();
  test_assignment_internal<unsigned int>();
  test_assignment_internal<unsigned long>();
  test_assignment_internal<unsigned long long>();

  string s = "123456789123456789123456789";
  BigInteger x;
  x = s;
  assert(x.toString() == s);
}

template <typename T>
SL void test_asmd_internal() {
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
      assert((BigInteger(a) + BigInteger(b)).toInt<int>() == (a + b));
      assert((BigInteger(a) += BigInteger(b)).toInt<int>() == (a+b));
      assert((BigInteger(a) - BigInteger(b)).toInt<int>() == (a - b));
      assert((BigInteger(a) -= BigInteger(b)).toInt<int>() == (a-b));
      assert((BigInteger(a) * BigInteger(b)).toInt<int>() == (a * b));
      assert((BigInteger(a) *= BigInteger(b)).toInt<int>() == (a*b));
      if (b != 0) {
        assert((BigInteger(a) / BigInteger(b)).toInt<int>() == (a / b));
        assert((BigInteger(a) /= BigInteger(b)).toInt<int>() == (a / b));
        assert((BigInteger(a) % BigInteger(b)).toInt<int>() == (a % b));
        assert((BigInteger(a) %= BigInteger(b)).toInt<int>() == (a % b));
      }
      if (a >= 0 && b >= 0) {
        assert((BigInteger(a) | BigInteger(b)).toInt<int>() == (a | b));
        assert((BigInteger(a) |= BigInteger(b)).toInt<int>() == (a | b));
        assert((BigInteger(a) & BigInteger(b)).toInt<int>() == (a & b));
        assert((BigInteger(a) &= BigInteger(b)).toInt<int>() == (a & b));
        assert((BigInteger(a) ^ BigInteger(b)).toInt<int>() == (a ^ b));
        assert((BigInteger(a) ^= BigInteger(b)).toInt<int>() == (a ^ b));
      }

      assert((BigInteger(a) + b).toInt<int>() == (a + b));
      assert((BigInteger(a) += b).toInt<int>() == (a + b));
      assert((BigInteger(a) - b).toInt<int>() == (a - b));
      assert((BigInteger(a) -= b).toInt<int>() == (a - b));
      assert((BigInteger(a) * b).toInt<int>() == (a * b));
      assert((BigInteger(a) *= b).toInt<int>() == (a * b));
      if (b != 0) {
        assert((BigInteger(a) / b).toInt<int>() == (a / b));
        assert((BigInteger(a) /= b).toInt<int>() == (a / b));
        assert((BigInteger(a) % b).toInt<int>() == (a % b));
        assert((BigInteger(a) %= b).toInt<int>() == (a % b));
      }
      if (a >= 0 && b >= 0) {
        assert((BigInteger(a) | b).toInt<int>() == (a | b));
        assert((BigInteger(a) |= b).toInt<int>() == (a | b));
        assert((BigInteger(a) & b).toInt<int>() == (a & b));
        assert((BigInteger(a) &= b).toInt<int>() == (a & b));
        assert((BigInteger(a) ^ b).toInt<int>() == (a ^ b));
        assert((BigInteger(a) ^= b).toInt<int>() == (a ^ b));
      }

      assert((a + BigInteger(b)).toInt<int>() == (a + b));
      assert((a - BigInteger(b)).toInt<int>() == (a - b));
      assert((a * BigInteger(b)).toInt<int>() == (a * b));
      if (b != 0) {
        assert((a / BigInteger(b)).toInt<int>() == (a / b));
        assert((a % BigInteger(b)).toInt<int>() == (a % b));
      }
      if (a >= 0 && b >= 0) {
        assert((a | BigInteger(b)).toInt<int>() == (a | b));
        assert((a & BigInteger(b)).toInt<int>() == (a & b));
        assert((a ^ BigInteger(b)).toInt<int>() == (a ^ b));
      }
    }
}

template <typename T>
SL void test_compare_operator_internal() {
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
  BigInteger x;
  for (int i = 0; i <= 19; ++i) x.setBit(i);
  assert(x.toInt<int>() == 1048575);
  x.revBit(0);
  assert(x.toInt<int>() == 1048574);
  x.resetBit(1);
  assert(x.toInt<int>() == 1048572);
  assert(x.bitCount() == 18);

  BigInteger y;
  y.setBit(0);

  x = x | y;
  assert(x.toInt<int>() == 1048573);

  x = x & BigInteger(1048575 - 4);
  assert(x.toInt<int>() == 1048573 - 4);

  x = x ^ x;
  assert(x.toInt<int>() == 0);

  x = x ^ y;
  assert(x.toInt<int>() == 1);
}

SL void test_utilities() {
  power_mod(BigInteger(5), 10, BigInteger("123456789"));
  power_mod(BigInteger(5), BigInteger(10), BigInteger("123456789"));

  power(BigInteger(2), 10u);
  power(BigInteger(2), 10);

  gcd(12_bi, 8_bi);
  123456789123456789_bi * 2 * 5_bi * "10"_bi;

  power(BigInteger(2), 20);
  power(BigInteger(2), 20LL);

  TimeRecorder tr;
  BigInteger v(1);
  for (int i = 1; i <= 100000; ++i) v *= i;
  // cout << tr.elapsed().format() << " " << v.bitCount() << endl;
}

SL void bi_test() {
  test_constructor();
  test_assignment_operator();
  test_asmd_operator();
  test_compare_operator();
  test_bit_operator();
  test_utilities();
}
PE_REGISTER_TEST(&bi_test, "bi_test", SMALL);
}