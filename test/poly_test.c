#include "pe_test.h"

namespace poly_test {
SL void poly_mod_test() {
  const int64 P = 10007;
  Poly a({0,1,2,0,0,123}, P);
  Poly b({0,1,17}, P);
  auto m1 = poly_mod_normal(a, b);
  auto m2 = poly_mod_dc(a, b);

  assert(m1.data == m2.data);
}
PE_REGISTER_TEST(&poly_mod_test, "poly_mod_test", SMALL);
}
