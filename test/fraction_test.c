#include "pe_test.h"

namespace fraction_test {

template <typename T>
SL void TestFractionType() {
  using F = Fraction<T>;
  // The canonical form: gcd(a, b) == 1 and b > 0.
  auto canonical = [](const F& f) {
    return f.b > 0 && Gcd(Abs(f.a), f.b) == 1;
  };
  // p / q with q != 0 against exact cross multiplication.
  auto is = [](const F& f, int64 p, int64 q) {
    return f.a * T(q) == f.b * T(p);
  };
  for (int64 a = -6; a <= 6; ++a) {
    for (int64 b = -6; b <= 6; ++b) {
      if (b == 0) continue;
      const F x{T(a), T(b)};
      assert(canonical(x) && is(x, a, b));
      assert(is(-x, -a, b) && is(+x, a, b));
      for (int64 c = -4; c <= 4; ++c) {
        for (int64 d = 1; d <= 4; ++d) {
          const F y{T(c), T(d)};
          const F sum = x + y, diff = x - y, prod = x * y;
          assert(canonical(sum) && is(sum, a * d + c * b, b * d));
          assert(canonical(diff) && is(diff, a * d - c * b, b * d));
          assert(canonical(prod) && is(prod, a * c, b * d));
          if (c != 0) {
            const F quot = x / y;
            assert(canonical(quot) && is(quot, a * d, b * c));
            F z = x;
            z /= y;
            assert(z == quot);
          }
          F z = x;
          z += y;
          assert(z == sum);
          z = x;
          z -= y;
          assert(z == diff);
          z = x;
          z *= y;
          assert(z == prod);

          // Comparisons: a / b op c / d with positive denominators.
          const int64 l = a * (b < 0 ? -1 : 1) * d;
          const int64 r = c * (b < 0 ? -b : b);
          assert((x < y) == (l < r) && (x > y) == (l > r));
          assert((x <= y) == (l <= r) && (x >= y) == (l >= r));
          assert((x == y) == (l == r) && (x != y) == (l != r));
        }
        // Mixed with a scalar on either side.
        assert(is(x + T(c), a + c * b, b) && is(T(c) + x, a + c * b, b));
        assert(is(x - T(c), a - c * b, b) && is(T(c) - x, c * b - a, b));
        assert(is(x * T(c), a * c, b) && is(T(c) * x, a * c, b));
        if (c != 0) assert(is(x / T(c), a, b * c));
        if (a != 0) assert(is(T(c) / x, c * b, a));
      }
    }
  }

  // ++ and -- add and subtract 1.
  F x{T(3), T(4)};
  F& ref = ++x;
  assert(&ref == &x && is(x, 7, 4));
  assert(is(x++, 7, 4) && is(x, 11, 4));
  assert(is(--x, 7, 4));
  assert(is(x--, 7, 4) && is(x, 3, 4));

  // Copy and move
  F copy(x);
  F moved(std::move(copy));
  F assigned;
  assigned = moved;
  F move_assigned;
  move_assigned = std::move(assigned);
  assert(is(moved, 3, 4) && is(move_assigned, 3, 4));
  assert(is(F(), 0, 1));

  // Harmonic sum 1 + 1/2 + ... + 1/10 = 7381/2520
  F h;
  for (int i = 1; i <= 10; ++i) h += F(T(1), T(i));
  assert(h.a == T(7381) && h.b == T(2520));

  std::stringstream ss;
  ss << F(T(-4), T(6));
  assert(ss.str() == "-2/3");
  assert(FAbs(F(T(-1), T(4)).ToDouble() + 0.25) < 1e-15);
  assert(FAbs(F(T(22), T(7)).ToLongDouble() - 22.0L / 7.0L) < 1e-18L);
}

SL void FractionTest() {
  TestFractionType<int64>();
#if PE_HAS_INT128
  TestFractionType<int128>();
#endif
  TestFractionType<int128e>();
  TestFractionType<BigInteger>();
#if ENABLE_GMP
  TestFractionType<MpInteger>();
#endif

  // The converting constructor from another integer type.
  Fraction<int64> f(4, 6);
  assert(f.a == 2 && f.b == 3);
  // Values that need more than 64 bits.
  const Fraction<BigInteger> big(Power(10_bi, 30), Power(10_bi, 20) * 4);
  assert(big.a == Power(10_bi, 10) / 4 && big.b == 1);
}

PE_REGISTER_TEST(&FractionTest, "FractionTest", SMALL);

}  // namespace fraction_test
