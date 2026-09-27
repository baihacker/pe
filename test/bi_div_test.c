#include "pe_test.h"

namespace bi_test {
// BigInteger division: q * b + r == a with |r| < |b| and r having the sign
// of a (truncating division), and the quotient and remainder of MpInteger
// (GMP) parsed from the same decimal strings.
SL void CheckDiv(const std::string& sa, const std::string& sb) {
  const BigInteger a(sa), b(sb);
  if (IsZero(b)) return;
  auto [q, r] = Div(a, b);
  assert(q * b + r == a);
  assert(Abs(r) < Abs(b));
  assert(IsZero(r) || IntSign(r) == IntSign(a));
  assert(a / b == q && a % b == r);
  BigInteger rem;
  assert(Divide(a, b, rem) == q && rem == r);
  assert(Divide(a, b) == q);
  BigInteger t = a;
  t /= b;
  assert(t == q);
  t = a;
  t %= b;
  assert(t == r);
#if ENABLE_GMP
  const MpInteger ma(sa), mb(sb);
  assert(ToString(q) == ToString(ma / mb));
  assert(ToString(r) == ToString(ma % mb));
#endif
}

// A random decimal string with the given number of digits (0 < digits).
SL std::string RandomDecimalForDiv(int digits, uint64& seed, bool negative) {
  std::string s = negative ? "-" : "";
  for (int i = 0; i < digits; ++i) {
    seed = seed * 6364136223846793005ULL + 1442695040888963407ULL;
    const int d = static_cast<int>((seed >> 33) % 10);
    s.push_back(static_cast<char>('0' + (i == 0 && d == 0 ? 1 : d)));
  }
  return s;
}

SL void BiDivTest() {
  uint64 seed = 2;
  // Divisors of 1 and 2 limbs take a separate path; longer ones use the long
  // division.
  const int digits[] = {1, 5, 9, 10, 19, 20, 40, 100, 1000};
  for (int x : digits) {
    for (int y : digits) {
      for (int sign = 0; sign < 4; ++sign) {
        const std::string sa = RandomDecimalForDiv(x, seed, sign & 1);
        const std::string sb = RandomDecimalForDiv(y, seed, sign & 2);
        CheckDiv(sa, sb);
        // An exact multiple: the remainder is 0.
        const BigInteger k(RandomDecimalForDiv(x, seed, false));
        CheckDiv(ToString(BigInteger(sb) * k), sb);
      }
    }
  }
  CheckDiv("0", RandomDecimalForDiv(30, seed, true));
  for (auto [x, y] : std::vector<std::pair<int, int>>{{6000, 3000}, {3000, 40}}) {
    CheckDiv(RandomDecimalForDiv(x, seed, true), RandomDecimalForDiv(y, seed, false));
  }

  // Builtin divisors, including values wider than 32 bits.
  for (int x : {1, 10, 20, 300}) {
    for (int sign = 0; sign < 2; ++sign) {
      const BigInteger a(RandomDecimalForDiv(x, seed, sign));
      for (int64 v : {int64(1), int64(-1), int64(3), int64(-7),
                      int64(4294967296), int64(-4294967311),
                      int64(9223372036854775807)}) {
        const BigInteger q = a / v, r = a % v;
        assert(q * v + r == a);
        assert(Abs(r) < Abs(BigInteger(v)));
        assert(IsZero(r) || IntSign(r) == IntSign(a));
        assert(q == a / BigInteger(v) && r == a % BigInteger(v));
      }
    }
  }
}

PE_REGISTER_TEST(&BiDivTest, "BiDivTest", SMALL);

// Products of many small factors, the original large scale test.
// strategy 0: B divides A up to the sign; strategy 1: independent factors.
SL void BiDivTestImpl(int x, int y) {
  for (int strategy = 0; strategy < 2; ++strategy)
    for (int s1 = -1; s1 <= 1; ++s1)
      for (int s2 = -1; s2 <= 1; s2 += 2)
        for (int id = 0; id < x; ++id) {
          BigInteger a = s1;
          BigInteger b = s2;
          for (int i = 0; i < y; ++i) {
            const int t = rand() + 1;
            a *= t;
            if (i & 1) b *= strategy == 0 ? t : rand() + 1;
          }
          CheckDiv(ToString(a), ToString(b));
        }
}

SL void BiDivTestMedium() { BiDivTestImpl(20, 500); }

#if !defined(CONTINUOUS_INTEGRATION_TEST)
PE_REGISTER_TEST(&BiDivTestMedium, "BiDivTestMedium", MEDIUM);
#endif

SL void BiDivTestBig() { BiDivTestImpl(4, 2000); }

#if !defined(CONTINUOUS_INTEGRATION_TEST)
PE_REGISTER_TEST(&BiDivTestBig, "BiDivTestBig", BIG);
#endif
}  // namespace bi_test
