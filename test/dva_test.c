#include "pe_test.h"

namespace dva_test {
SL void dva_test() {
  auto orz = prime_pi<int64>(10000);
  assert(orz[10000] == 1229LL);
  
  orz = prime_s0_ex<int64>(10000);
  assert(orz[10000] == 1229LL);
  //dbg(orz[10000]); // 5761455LL
  
  orz = prime_pi<int64>(100000000);
  assert(orz[100000000] == 5761455LL);
  
  orz = prime_s0_ex<int64>(100000000);
  assert(orz[100000000] == 5761455LL);
  //dbg(orz[100000000]); // 455052511LL
  
  orz = prime_s1<int64>(10000);
  assert(orz[10000] == 5736396LL);
  // dbg(orz[10000]);
  
  int64 s = 0;
  for (int i = 2; i <= 10000; ++i) if (is_prime(i)) s += i;
  assert(s == 5736396LL);
  // dbg(s);
  
  orz = prime_s1<int64>(1000000);
  assert(orz[1000000] == 37550402023LL);
  // dbg(orz[1000000]);
  
  s = 0;
  for (int i = 2; i <= 1000000; ++i) if (is_prime(i)) s += i;
  assert(s == 37550402023LL);
  // dbg(s);
}

PE_REGISTER_TEST(&dva_test, "dva_test", SMALL);
}
