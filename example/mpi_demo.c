#include <pe.hpp>

void mpi_test0() {
  MpInteger x("123456");
  // add
  x += 1;
  x += 1U;
  x += 1L;
  x += 1LU;
  x += 1LL;
  x += "1"_i128;
  x = x + 1;
  x = x + 1U;
  x = x + 1L;
  x = x + 1LU;
  x = x + 1LL;
  x = 1 + x;
  x = 1U + x;
  x = 1L + x;
  x = 1LU + x;
  x = 1LL + x;

  // sub
  x -= 1;
  x -= 1U;
  x -= 1L;
  x -= 1LU;
  x -= 1LL;
  x -= "1"_i128;
  x = x - 1;
  x = x - 1U;
  x = x - 1L;
  x = x - 1LU;
  x = x - 1LL;
  x = x - 1LL;
  x -= 1U;
  x -= 1L;
  x -= 1LU;
  x -= 1LL;
  x = 1 - x;
  x = 1U - x;
  x = 1L - x;
  x = 1LU - x;

  x = 123456;
  x = 123456U;
  x = 123456LU;
  x = 123456L;
  x = 123456LL;

  // mul
  x *= 2;
  x *= 2U;
  x *= 2L;
  x *= 2LU;
  x = x * 2;
  x = x * 2U;
  x = x * 2L;
  x = x * 2LU;
  x = 2 * x;
  x = 2U * x;
  x = 2L * x;
  x = 2LU * x;

  x /= 2;
  x /= 2U;
  x /= 2L;
  x /= 2LU;
  x = x / 2;
  x = x / 2U;
  x = x / 2L;
  x = x / 2LU;

  //  cout << (x>>4) << endl;
}

void mpi_test1() {
  MpInteger x = 1;
  x <<= 20;
  x = x << 20;
  x |= 1;
  x &= 1;
  x ^= 1;
  x = x | 1;
  x = x & 1;
  x = x ^ 1;
  x = 1 | x;
  x = 1 & x;
  x = 1 ^ x;
  x = 1;
  x = ~x;
}

void mpi_test2() {
  MpInteger x = 1;
  cout << x.toInt<int>() << endl;
  cout << x.toInt<int64>() << endl;
  cout << x.toInt<int128>() << endl;
  cout << x.toInt<unsigned int>() << endl;
  cout << x.toInt<uint64>() << endl;
  cout << x.toInt<uint128>() << endl;
  x = -1;
  cout << x.toInt<int>() << endl;
  cout << x.toInt<int64>() << endl;
  cout << x.toInt<int128>() << endl;
  cout << x.toInt<unsigned int>() << endl;
  cout << x.toInt<uint64>() << endl;
  cout << x.toInt<uint128>() << endl;

  x = 1;
  x <<= 52;
  printf("%.16f\n", x.toFloat<double>());

  cout << power_mod(MpInteger(5), 10, MpInteger("123456789")) << endl;
  cout << power_mod(MpInteger(5), MpInteger(10), MpInteger("123456789"))
       << endl;
  cout << power(MpInteger(2), 10u) << endl;
  cout << power(MpInteger(2), 10) << endl;

  cout << gcd(12_mpi, 8_mpi) << endl;
  cout << 123456789123456789_mpi * 2 * 5_mpi * "10"_mpi << endl;
}

int main() {
  mpi_test0();
  mpi_test1();
  mpi_test2();
  dbg(typeid(2L).name());
  dbg(typeid(2).name());
  dbg(typeid(2U).name());
  dbg(typeid(2UL).name());
  return 0;
}