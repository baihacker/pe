#include "pe_test.h"

namespace poly_test {
SL void poly_test() {
  assert(power_sum(10, 2, 1000000007) == 385);
  assert(power_sum(100, 100, 1000000007) == 568830579);
  assert(power_sum(1000, 1000, 1000000007) == 918088852);
  assert(power_sum(1000, 10000, 1000000007) == 163720385);
}

PE_REGISTER_TEST(&poly_test, "poly_test", SMALL);
}
