#include "pe_test.h"

namespace print_int128_test {
#if PE_HAS_INT128
SL void print_int128_test() {
  int128 x = 1;
  for (int i = 0; i < 127; ++i) {
    assert(to_string(x) == bi(x).toString());
    assert(to_string(-x) == bi(-x).toString());
    x <<= 1;
  }
  x = 0;
  assert(to_string(x) == bi(x).toString());
  assert(to_string(-x) == bi(-x).toString());
}

PE_REGISTER_TEST(&print_int128_test, "print_int128_test", SMALL);

SL void int128_literal_test() {
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

PE_REGISTER_TEST(&int128_literal_test, "int128_literal_test", SMALL);
#endif
}  // namespace print_int128_test
