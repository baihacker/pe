#include "pe_test.h"

namespace print_int128_test {
#if PE_HAS_INT128
SL void PrintInt128Test() {
  int128 x = 1;
  for (int i = 0; i < 127; ++i) {
    assert(ToString(x) == bi(x).ToString());
    assert(ToString(-x) == bi(-x).ToString());
    x <<= 1;
  }
  x = 0;
  assert(ToString(x) == bi(x).ToString());
  assert(ToString(-x) == bi(-x).ToString());
}

PE_REGISTER_TEST(&PrintInt128Test, "PrintInt128Test", SMALL);

SL void Int128LiteralTest() {
  const int128 p1 = 10;
  const int128 p2 = p1 * p1;
  const int128 p4 = p2 * p2;
  const int128 p8 = p4 * p4;
  const int128 p16 = p8 * p8;
  const int128 p32 = p16 * p16;
  assert(p32 == "100000000000000000000000000000000"_i128);
  assert(p32 == "+100000000000000000000000000000000"_i128);
  assert(-p32 == "-100000000000000000000000000000000"_i128);
  assert(p32 == "100000000000000000000000000000000"_u128);
  assert(p32 == "+100000000000000000000000000000000"_u128);
  const int128 b62 = 1LL << 62;
  const int128 b63 = b62 << 1;
  const int128 b126 = b63 * b63;
  assert(b63 == "9223372036854775808"_i128);
  assert(b126 == "85070591730234615865843651857942052864"_i128);
  const uint128 b127 = (uint128)b126 << 1;
  assert(b127 == "170141183460469231731687303715884105728"_u128);
}

PE_REGISTER_TEST(&Int128LiteralTest, "Int128LiteralTest", SMALL);
#endif
}  // namespace print_int128_test
