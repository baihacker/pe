#include "pe_test.h"

namespace mp_test {
SL void mp_test() {
  const int64 P = 1000000009;
  PolyM<P> s{0,1,1,2,3,5};
  auto v = find_minimal_poly(s);
  assert(v[0] == P - 1);
  assert(v[1] == P - 1);
  assert(v[2] == 1);
  const int n = v.data.size();
  int64 ans = 0;
  for (int i = 0; i < n; ++i)
    ans += v.at(i) * s.at(i);
  assert(ans == P);
  
  ans = nth_element(s, 38, v);
  assert(ans == 39088169LL);
  
  auto t = find_linear_recurrence<31>({0,1,1,2,3,5,8,13});
  assert(t[0] == 30);
  assert(t[1] == 30);
  assert(t[2] == 1);
  assert(nth_element<1000000009>({0,1,1,2,3,5,8,13}, 38) == 39088169);
}

PE_REGISTER_TEST(&mp_test, "minimal_polynomial_test");
}
