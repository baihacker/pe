#include "pe_test.h"

namespace mpf_test {
#if ENABLE_GMP
// The same checks for Mpf (GMP) and Mpfr (MPFR); the precision is 256 bits
// (about 77 decimal digits).
template <typename F>
SL void TestMpFloat() {
  F::SetDefaultPrec(256);
  assert(F::GetDefaultPrec() >= 256);
  const F eps("1e-70");
  auto close_to = [&](const F& a, const F& b) {
    return Abs(a - b) < eps * (F(1) + Abs(b));
  };

  // Constructors
  assert(IsZero(F()) && F(true) == F(1) && F(false) == F(0));
  assert(F(3) == F(3L) && F(0.5) == F("0.5") && F(std::string("-2.5")) == F(-2.5));
  assert(F(MpInteger("12345678901234567890")) == F("12345678901234567890"));
  assert(F(int64(-123456789012345)) == F("-123456789012345"));
  assert(F(uint64(18446744073709551615ULL)) == F("18446744073709551615"));

  // Arithmetic
  const F third = F(1) / F(3);
  assert(close_to(third * 3, F(1)));
  assert(close_to(Sqrt(F(2)) * Sqrt(F(2)), F(2)) && close_to(F(2).Sqrt(), Sqrt(F(2))));
  // Powers of two are exact in binary.
  assert(Power(F(2), 100) == F(Power(MpInteger(2), 100)));
  assert(F(3).Power(5) == F(243));
  F x(10);
  x += F(5);
  x -= F(3);
  x *= F(4);
  x /= F(8);
  assert(x == F(6));
  assert(F(7) + F(1) == F(8) && F(7) - F(1) == F(6));
  assert(F(7) * F(2) == F(14) && F(7) / F(2) == F("3.5"));
  assert(-F(7) == F(-7));
  F y(5);
  assert(y++ == F(5) && y == F(6) && ++y == F(7));
  assert(y-- == F(7) && y == F(6) && --y == F(5));
  y = 42;
  assert(y == F(42));

  // Comparisons
  assert(F(1) < F(2) && F(2) > F(1) && F(1) <= F(1) && F(1) >= F(1));
  assert(F(1) != F(2) && compare(F(1), F(2)) < 0 && compare(F(2), F(2)) == 0);

  // Rounding, sign and absolute value
  for (auto [v, fl, ce, tr] : std::vector<std::array<const char*, 4>>{
           {"2.5", "2", "3", "2"}, {"-2.5", "-3", "-2", "-2"},
           {"7", "7", "7", "7"}, {"-0.25", "-1", "0", "0"}}) {
    const F f(v);
    assert(Floor(f) == F(fl) && f.Floor() == F(fl));
    assert(Ceil(f) == F(ce) && f.Ceil() == F(ce));
    assert(Trunc(f) == F(tr) && f.Trunc() == F(tr));
  }
  assert(IntSign(F(-3)) == -1 && IntSign(F(0)) == 0 && F(4).Sgn() == 1);
  assert(Abs(F(-3)) == F(3) && FAbs(F(-3)) == F(3) && F(-3).Abs() == F(3));

  // Precision of a single number
  F hp(1);
  hp.SetPrec(1024);
  assert(hp.Prec() >= 1024);

  // Conversions; integral values avoid the different rounding of Mpf and
  // Mpfr (see the TODO(bug) in pe_mpf).
  assert(F(1.5).ToDouble() == 1.5);
  assert(FAbs(third.ToLongDouble() - 1.0L / 3) < 1e-18L);
  assert(F("-123456789012345678901234567890").ToMpInteger() ==
         MpInteger("-123456789012345678901234567890"));
  assert(F(-77).template ToInt<int>() == -77 &&
         F(1000000).template ToInt<int64>() == 1000000);
  // ToString keeps enough digits to read the value back.
  for (const F& f : {third, F(-2.5), F(1000), Sqrt(F(2)) * 1000000}) {
    assert(close_to(F(ToString(f, 75)), f) && close_to(F(f.ToString(75)), f));
    std::stringstream ss;
    ss << f;
    assert(!ss.str().empty());
  }
  assert(ToString(F(0)) == "0");

  // Copy and move; a moved-from object is a valid zero.
  F a(third);
  F b(std::move(a));
  assert(b == third && IsZero(a));
  F c;
  c = b;
  assert(c == third);
}

#endif

SL void MpfTest() {
#if HAS_MPF
  TestMpFloat<Mpf>();
#endif
#if HAS_MPFR
  TestMpFloat<Mpfr>();
#endif
}

PE_REGISTER_TEST(&MpfTest, "MpfTest", SMALL);
}  // namespace mpf_test
