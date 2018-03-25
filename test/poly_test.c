#include "pe_test.h"

namespace poly_test {
SL void poly_general_test() {
  PolyM<31> p{1,2,3,31,32};
  assert(p[3] == 0);
  assert(p[4] == 1);
  
  PolyM<31> q{1, 1};
  auto r = q * q;
  assert(r.deg() == 2);
  assert(r[0] == 1);
  assert(r[1] == 2);
  assert(r[2] == 1);
  
  r = q % q;
  assert(r.isZero());
  
  r = q / q;
  assert(r[0] == 1);
  
  r = q - q;
  assert(r.isZero());
  
  r = q + q;
  assert(r.deg() == 1);
  assert(r[0] == 2);
  assert(r[1] == 2);
  
  assert((q * Poly({0}, 31)).isZero());
  assert(q * Poly({1}, 31) == q);
  
  assert(Poly({0, 0, 0}, 31).isZero());
  assert(Poly({0, 1, 0}, 31).deg() == 1);
  
  assert(q / Poly({1}, 31) == q);
}
PE_REGISTER_TEST(&poly_general_test, "poly_general_test", SMALL);

SL void poly_mod_test() {
  const int64 P = 10007;
  Poly a({0,1,2,0,0,123}, P);
  Poly b({0,1,17}, P);
  auto m1 = poly_mod_normal(a, b);
  auto m2 = poly_mod_dc(a, b);

  assert(m1 == m2);
}
PE_REGISTER_TEST(&poly_mod_test, "poly_mod_test", SMALL);
}
