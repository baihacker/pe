#include "pe_test.h"

namespace dva_test {
SL void TestS0() {
  auto orz = PrimePi<int64>(10000);
  assert(orz[10000] == 1229LL);

  orz = PrimeS0Ex<int64>(10000);
  assert(orz[10000] == 1229LL);

  orz = PrimePi<int64>(100000000);
  assert(orz[100000000] == 5761455LL);

  orz = PrimeS0Ex<int64>(100000000);
  assert(orz[100000000] == 5761455LL);
}

SL void TestS1() {
  int64 s = 0;
  for (int i = 2; i <= 10000; ++i) {
    if (IsPrime(i)) s += i;
  }
  assert(s == 5736396LL);

  auto orz = PrimeS1<int64>(10000);
  assert(orz[10000] == 5736396LL);

  orz = PrimeS1Ex<int64>(10000);
  assert(orz[10000] == 5736396LL);

  s = 0;
  for (int i = 2; i <= 1000000; ++i) {
    if (IsPrime(i)) s += i;
  }
  assert(s == 37550402023LL);

  orz = PrimeS1<int64>(1000000);
  assert(orz[1000000] == 37550402023LL);

  orz = PrimeS1Ex<int64>(1000000);
  assert(orz[1000000] == 37550402023LL);
}

SL void DvaTest() {
  TestS0();
  TestS1();
}

PE_REGISTER_TEST(&DvaTest, "DvaTest", SMALL);
}  // namespace dva_test
