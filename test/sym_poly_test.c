#include "pe_test.h"

namespace sym_poly_test {

using P = SymPoly64;

// Evaluates p by replacing every variable with the given value.
SL int64 Eval(const P& p, const std::map<std::string, int64>& values) {
  P t = p;
  for (const auto& [var, value] : values) {
    t = t.Replace(var, P(value));
  }
  auto ret = t.AsNumber();
  assert(ret.has_value());
  return *ret;
}

SL bool Same(const P& a, const P& b) { return a.terms() == b.terms(); }

SL std::string Str(const P& p) {
  std::stringstream ss;
  ss << p;
  return ss.str();
}

SL void SymPolyConstructTest() {
  // Zero.
  P zero;
  assert(std::empty(zero.terms()));
  assert(zero.AsNumber() == 0);
  assert(std::empty(P(0).terms()));
  assert(Str(zero) == "0");

  // Constant.
  P five(5);
  assert(std::size(five.terms()) == 1);
  assert(five.terms().begin()->first.empty());
  assert(five.AsNumber() == 5);
  assert(P(-7).AsNumber() == -7);

  // Zero coefficients are dropped by the map constructor.
  std::map<TermKey, int64> terms;
  terms[{}] = 0;
  terms[{{"x", 1}}] = 3;
  terms[{{"y", 2}}] = 0;
  P p(terms);
  assert(std::size(p.terms()) == 1);
  assert(p.terms().at({{"x", 1}}) == 3);
  assert(!p.AsNumber().has_value());
  P q(std::move(terms));
  assert(Same(p, q));

  // Copy / move.
  P c(p);
  assert(Same(c, p));
  P m(std::move(c));
  assert(Same(m, p));
  P a;
  a = p;
  assert(Same(a, p));
  P b;
  b = std::move(a);
  assert(Same(b, p));

  // From string.
  P s("3 x");
  assert(Same(s, p));
  P s2(std::string("x + 1"));
  assert(std::size(s2.terms()) == 2);
}

SL void SymPolyArithmeticTest() {
  const P x("x"), y("y"), one(1);
  assert(std::empty((x - x).terms()));
  assert(Same(x + x, P("2x")));
  assert(Same(x + y, y + x));
  assert(Same(x * y, y * x));
  assert(Same(x * x, P("x^2")));
  assert(Same((x + one) * (x - one), P("x^2 - 1")));
  assert(Same((x + y) * (x + y), P("x^2 + 2 x y + y^2")));
  assert(Same(x * P(0), P()));
  assert(Same(x * one, x));
  assert(Same(P(3) * P(-4), P(-12)));
  assert(Same(P(2) + P(-2), P()));
  // Exponents merge in MergeTermKey.
  assert(Same(P("x^2 y") * P("x y^3 z"), P("x^3 y^4 z")));
  assert((P("x^2 y") * P("x y^3")).terms().begin()->first ==
         TermKey({{"x", 3}, {"y", 4}}));
  assert(P::MergeTermKey({}, {{"a", 1}}) == TermKey({{"a", 1}}));
  assert(P::MergeTermKey({{"a", 1}}, {}) == TermKey({{"a", 1}}));
  assert(P::MergeTermKey({{"b", 2}}, {{"a", 1}, {"b", 3}}) ==
         TermKey({{"a", 1}, {"b", 5}}));

  // Power.
  assert(Same(x.Power(0), one));
  assert(Same(P().Power(0), one));
  assert(Same(P().Power(3), P()));
  assert(Same(x.Power(1), x));
  assert(Same(P(2).Power(10), P(1024)));
  assert(Same(P(-3).Power(3), P(-27)));
  P expected("1");
  const P xp1 = x + one;
  for (int n = 1; n <= 10; ++n) {
    expected = expected * xp1;
    assert(Same(xp1.Power(n), expected));
  }
  // Binomial coefficients of (x + 1)^10.
  const P p10 = xp1.Power(10);
  for (int k = 0; k <= 10; ++k) {
    TermKey key;
    if (k > 0) key.emplace_back("x", k);
    int64 c = 1;
    for (int i = 0; i < k; ++i) c = c * (10 - i) / (i + 1);
    assert(p10.terms().at(key) == c);
  }

  // MakeNeg.
  P n("x - 2 y + 3");
  n.MakeNeg();
  assert(Same(n, P("-x + 2 y - 3")));
  P z;
  z.MakeNeg();
  assert(std::empty(z.terms()));
}

SL void SymPolyParseTest() {
  // Numbers and precedence.
  assert(P("0").AsNumber() == 0);
  assert(P("42").AsNumber() == 42);
  assert(P("  12  ").AsNumber() == 12);
  assert(P("1 + 2 * 3").AsNumber() == 7);
  assert(P("(1 + 2) * 3").AsNumber() == 9);
  assert(P("2 3").AsNumber() == 6);
  assert(P("2(3)").AsNumber() == 6);
  assert(P("10 - 3 - 2").AsNumber() == 5);
  assert(P("10 - (3 - 2)").AsNumber() == 9);
  assert(P("2^10").AsNumber() == 1024);
  assert(P("2^3^2").AsNumber() == 512);  // right associative
  assert(P("(2^3)^2").AsNumber() == 64);
  assert(P("2^(1+2)").AsNumber() == 8);
  assert(P("-2^2").AsNumber() == -4);
  assert(P("(-2)^2").AsNumber() == 4);
  assert(P("-(3)").AsNumber() == -3);
  assert(P("+5").AsNumber() == 5);
  assert(P("--5").AsNumber() == 5);
  assert(P("2*-3").AsNumber() == -6);
  assert(P("2 * +3").AsNumber() == 6);
  assert(P("7^0").AsNumber() == 1);
  assert(P("((((1))))").AsNumber() == 1);
  assert(P("2 - -3").AsNumber() == 5);

  // Variables.
  assert(Same(P("x_1 + _y + ab2"),
              P("x_1") + P("_y") + P("ab2")));
  assert(P("x_1").terms().begin()->first == TermKey({{"x_1", 1}}));
  assert(Same(P("2x"), P("2 * x")));
  assert(Same(P("x y"), P("x*y")));
  assert(Same(P("x(y+1)"), P("x y + x")));
  assert(Same(P("(x+1)(x-1)"), P("x^2-1")));
  assert(Same(P("x^2 y"), P("x x y")));
  assert(Same(P("-x y"), P("-(x y)")));
  assert(Same(P("a*-b"), P("-a b")));
  assert(Same(P("x^2^2"), P("x^4")));
  assert(Same(P("(x + y)^0"), P(1)));
  assert(Same(P("(x - y)^3"), P("x^3 - 3x^2 y + 3 x y^2 - y^3")));
  assert(Same(P("x - x + y - y"), P()));
  assert(Same(P("x\t+\n1"), P("x+1")));

  // Parsing agrees with direct evaluation on a grid.
  const P e("(x + 2y)^3 - x(y - 1) + 5 - 3 x^2 y^2");
  for (int64 x = -3; x <= 3; ++x) {
    for (int64 y = -3; y <= 3; ++y) {
      const int64 expected = (x + 2 * y) * (x + 2 * y) * (x + 2 * y) -
                             x * (y - 1) + 5 - 3 * x * x * y * y;
      assert(Eval(e, {{"x", x}, {"y", y}}) == expected);
    }
  }

  // Syntax errors give zero and print a message to std::cerr (muted here).
  std::stringstream err;
  auto* old = std::cerr.rdbuf(err.rdbuf());
  assert(std::empty(ParseSymPoly<int64>("").terms()));
  assert(std::empty(ParseSymPoly<int64>("(x").terms()));
  assert(std::empty(ParseSymPoly<int64>("x)").terms()));
  assert(std::empty(ParseSymPoly<int64>("a/b").terms()));
  assert(std::empty(ParseSymPoly<int64>("x^-1").terms()));
  assert(std::empty(ParseSymPoly<int64>("x^y").terms()));
  assert(std::empty(ParseSymPoly<int64>("x*").terms()));
  assert(std::empty(ParseSymPoly<int64>("x +").terms()));
  std::cerr.rdbuf(old);
  assert(err.str().find("Syntax error: cannot parse /b") != std::string::npos);
}

SL void SymPolyReplaceTest() {
  const P p("x^2 y + 3 x + y + 7");
  // Replace with a constant.
  assert(Same(p.Replace("x", P(2)), P("4 y + 6 + y + 7")));
  // Replace with a polynomial.
  assert(Same(p.Replace("x", "z + 1"),
              P("(z+1)^2 y + 3(z+1) + y + 7")));
  // Replace with a variable that already appears in the term.
  assert(Same(p.Replace("x", "y"), P("y^3 + 4 y + 7")));
  // Replace a missing variable: unchanged.
  assert(Same(p.Replace("w", "123"), p));
  // Replace with zero kills the terms containing it.
  assert(Same(p.Replace("y", P()), P("3x + 7")));
  // Replace in the zero polynomial.
  assert(Same(P().Replace("x", "y"), P()));
  // x^0 of the replaced variable never appears, a constant stays a constant.
  assert(Same(P(9).Replace("x", "y"), P(9)));
  // Chained replaces evaluate the polynomial.
  assert(Eval(p, {{"x", -2}, {"y", 5}}) == 4 * 5 - 6 + 5 + 7);
}

SL void SymPolyAsSingleVarPolyTest() {
  auto v = P("3 + x^2").AsSingleVarPoly();
  assert(v.has_value() && *v == std::vector<int64>({3, 0, 1}));
  v = P("x^3 - 2x").AsSingleVarPoly();
  assert(v.has_value() && *v == std::vector<int64>({0, -2, 0, 1}));
  v = P().AsSingleVarPoly();
  assert(v.has_value() && *v == std::vector<int64>({0}));
  v = P(5).AsSingleVarPoly();
  assert(v.has_value() && *v == std::vector<int64>({5}));
  v = P("y").AsSingleVarPoly();
  assert(v.has_value() && *v == std::vector<int64>({0, 1}));
  assert(!P("x y").AsSingleVarPoly().has_value());
  assert(!P("x + y").AsSingleVarPoly().has_value());
  assert(!P("x^2 + y + 1").AsSingleVarPoly().has_value());

  assert(P("x - x + 4").AsNumber() == 4);
  assert(!P("x").AsNumber().has_value());
  assert(!P("x + 1").AsNumber().has_value());
}

SL void SymPolyToStringTest() {
  // operator<< uses the non C-style format.
  assert(Str(P("(x+1)^2")) == "1 + 2 x + x^2");
  assert(Str(P("x")) == "x");
  assert(Str(P("x y")) == "x y");
  assert(Str(P("3")) == "3");
  assert(Str(P("1")) == "1");
  assert(Str(P("x - 1")) == "- 1 + x");
  assert(Str(P("-5")) == "- 5");
  assert(Str(P("-x")) == "- x");
  assert(Str(P("-2x^3 y + 4")) == "4 - 2 x^3 y");
  assert(Str(P("x^2 - x")) == "- x + x^2");

  // Member ToString defaults to C style, which forces show_mul.
  const P p("-2 x y^2 + 3 y - x*x");
  assert(p.ToString() == "- 2 * x * Power(y, 2) - Power(x, 2) + 3 * y");
  assert(p.ToString(1, 0) == p.ToString(1, 1));
  assert(p.ToString(0, 0) == "- 2 x y^2 - x^2 + 3 y");
  assert(p.ToString(0, 1) == "- 2 * x * y^2 - x^2 + 3 * y");
  assert(P("(x+1)^2").ToString() == "1 + 2 * x + Power(x, 2)");
  assert(P("x y").ToString() == "x * y");
  assert(P().ToString() == "0");
  assert(P().ToString(0, 0) == "0");

  // ToString with an explicit stream and TermKey printing.
  std::stringstream ss;
  pe::ToString(P("a b^2"), ss, 0, 1);
  assert(ss.str() == "a * b^2");
  std::stringstream ks;
  pe::ToString(TermKey{{"a", 1}, {"b", 3}}, ks);
  assert(ks.str() == "a Power(b, 3)");
  std::stringstream ks2;
  pe::ToString(TermKey{{"a", 2}, {"b", 1}}, ks2, 0, 0);
  assert(ks2.str() == "a^2 b");
  std::stringstream ks3;
  pe::ToString(TermKey{}, ks3, 0, 0);
  assert(ks3.str().empty());
}

PE_REGISTER_TEST(&SymPolyConstructTest, "SymPolyConstructTest", SMALL);
PE_REGISTER_TEST(&SymPolyArithmeticTest, "SymPolyArithmeticTest", SMALL);
PE_REGISTER_TEST(&SymPolyParseTest, "SymPolyParseTest", SMALL);
PE_REGISTER_TEST(&SymPolyReplaceTest, "SymPolyReplaceTest", SMALL);
PE_REGISTER_TEST(&SymPolyAsSingleVarPolyTest, "SymPolyAsSingleVarPolyTest",
                 SMALL);
PE_REGISTER_TEST(&SymPolyToStringTest, "SymPolyToStringTest", SMALL);
}  // namespace sym_poly_test
