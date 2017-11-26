#include "pe_test.h"

namespace print_int128_test {
SL void print_int128_test() {
  int128 x = 1;
  for (int i = 0; i < 127; ++i)
  {
    assert(to_string(x) == bi(x).toString());
    assert(to_string(-x) == bi(-x).toString());
    x <<= 1;
  }
  x = 0;
  assert(to_string(x) == bi(x).toString());
  assert(to_string(-x) == bi(-x).toString());
}

PE_REGISTER_TEST(&print_int128_test, "print_int128_test", SMALL);
}
