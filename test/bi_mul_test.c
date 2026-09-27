#include "pe_test.h"

namespace bi_test {
// BigInteger multiplication: identities, and the product of MpInteger (GMP)
// parsed from the same decimal strings.
// check_div also checks (a * b) / b == a, which is slow for huge numbers.
SL void CheckMul(const std::string& sa, const std::string& sb,
                 bool check_div = true) {
  const BigInteger a(sa), b(sb);
  const BigInteger c = a * b;
  assert(c == b * a);
  BigInteger d = a;
  d *= b;
  assert(d == c);
  if (check_div && !IsZero(b)) {
    assert(c / b == a);
    assert(IsZero(c % b));
  }
#if ENABLE_GMP
  assert(ToString(c) == ToString(MpInteger(sa) * MpInteger(sb)));
#endif
}

// A random decimal string with the given number of digits (0 < digits).
SL std::string RandomDecimalForMul(int digits, uint64& seed, bool negative) {
  std::string s = negative ? "-" : "";
  for (int i = 0; i < digits; ++i) {
    seed = seed * 6364136223846793005ULL + 1442695040888963407ULL;
    const int d = static_cast<int>((seed >> 33) % 10);
    s.push_back(static_cast<char>('0' + (i == 0 && d == 0 ? 1 : d)));
  }
  return s;
}

SL void BiMulTest() {
  uint64 seed = 1;
  // From one limb up; the NTT multiplication is used from about 500 limbs
  // (4800 digits) on each side.
  const int digits[] = {1, 5, 9, 10, 19, 20, 40, 100, 1000};
  for (int x : digits) {
    for (int y : digits) {
      for (int sign = 0; sign < 4; ++sign) {
        CheckMul(RandomDecimalForMul(x, seed, sign & 1),
                 RandomDecimalForMul(y, seed, sign & 2));
      }
    }
  }
  CheckMul(RandomDecimalForMul(3000, seed, true),
           RandomDecimalForMul(40, seed, false));
  CheckMul(RandomDecimalForMul(5000, seed, false),
           RandomDecimalForMul(4800, seed, true));
  CheckMul("0", RandomDecimalForMul(100, seed, true));
  CheckMul(RandomDecimalForMul(100, seed, false), "0");

  // Builtin operands, including values wider than 32 bits.
  const BigInteger a(RandomDecimalForMul(200, seed, true));
  for (int64 v : {int64(0), int64(1), int64(-3), int64(4294967295),
                  int64(4294967296), int64(-9223372036854775807)}) {
    const BigInteger expected = a * BigInteger(v);
    assert(a * v == expected && v * a == expected);
    BigInteger t = a;
    t *= v;
    assert(t == expected);
  }
  const uint64 big_u = 18446744073709551615ULL;
  assert(a * big_u == a * BigInteger(big_u));
}

PE_REGISTER_TEST(&BiMulTest, "BiMulTest", SMALL);

// Products of many small factors, the original large scale test.
SL void BiMulTestImpl(int x, int y) {
  for (int s1 = -1; s1 <= 1; ++s1)
    for (int s2 = -1; s2 <= 1; ++s2)
      for (int id = 0; id < x; ++id) {
        BigInteger a = s1;
        BigInteger b = s2;
        for (int i = 0; i < y; ++i) {
          a *= rand();
          b *= rand();
        }
        CheckMul(ToString(a), ToString(b), y <= 500);
      }
}

SL void BiMulTestMedium() { BiMulTestImpl(100, 500); }

#if !defined(CONTINUOUS_INTEGRATION_TEST)
PE_REGISTER_TEST(&BiMulTestMedium, "BiMulTestMedium", MEDIUM);
#endif

SL void BiMulTestBig() { BiMulTestImpl(3, 10000); }

#if !defined(CONTINUOUS_INTEGRATION_TEST)
PE_REGISTER_TEST(&BiMulTestBig, "BiMulTestBig", BIG);
#endif
}  // namespace bi_test
