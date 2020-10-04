#include "pe_test.h"

namespace poly_test {
SL void PolyGeneralTest() {
  NModPolyT<31> p{1, 2, 3, 31, 32};
  assert(p[3] == 0);
  assert(p[4] == 1);

  NModPolyT<31> q{1, 1};
  auto r = q * q;
  assert(r.deg() == 2);
  assert(r[0] == 1);
  assert(r[1] == 2);
  assert(r[2] == 1);

  r = q % q;
  assert(r.IsZero());

  r = q / q;
  assert(r[0] == 1);

  r = q - q;
  assert(r.IsZero());

  r = q + q;
  assert(r.deg() == 1);
  assert(r[0] == 2);
  assert(r[1] == 2);

  assert((q * NModPoly({0}, 31)).IsZero());
  assert(q * NModPoly({1}, 31) == q);

  assert(NModPoly({0, 0, 0}, 31).IsZero());
  assert(NModPoly({0, 1, 0}, 31).deg() == 1);

  assert(q / NModPoly({1}, 31) == q);
}
PE_REGISTER_TEST(&PolyGeneralTest, "PolyGeneralTest", SMALL);

SL void PolyModTest() {
  const int64 P = 10007;
  NModPoly a({0, 1, 2, 0, 0, 123}, P);
  NModPoly b({0, 1, 17}, P);
  auto m1 = PolyModNormal(a, b);
  auto m2 = PolyModDc(a, b);

  assert(m1 == m2);
}
PE_REGISTER_TEST(&PolyModTest, "PolyModTest", SMALL);
}  // namespace poly_test
