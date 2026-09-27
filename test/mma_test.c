#include "pe_test.h"

namespace mma_test {

using namespace pe::mma;

// Exact integer evaluation of a parsed tree. Divisions and square roots must
// be exact for the inputs used.
SL int64 EvalTree(const MmaExpTree* t, const std::map<std::string, int64>& v) {
  switch (t->nodeType) {
    case NUMBER:
      return t->value;
    case VAR:
      return v.at(t->token);
    case NEG:
      return -EvalTree(t->right, v);
    case ADD:
      return EvalTree(t->left, v) + EvalTree(t->right, v);
    case SUB:
      return EvalTree(t->left, v) - EvalTree(t->right, v);
    case MUL:
      return EvalTree(t->left, v) * EvalTree(t->right, v);
    case DIV: {
      const int64 a = EvalTree(t->left, v), b = EvalTree(t->right, v);
      assert(b != 0 && a % b == 0);
      return a / b;
    }
    case POW: {
      const int64 e = EvalTree(t->right, v);
      assert(e >= 0);
      return Power(EvalTree(t->left, v), e);
    }
    case FUN: {
      assert(t->token == "Sqrt");
      const int64 a = EvalTree(t->right, v);
      const int64 r = SqrtI(a);
      assert(r * r == a);
      return r;
    }
  }
  assert(false);
  return 0;
}

SL std::string Tree(const std::string& s) {
  MmaExpParser parser;
  MmaExpTree* t = parser.Parse(s);
  std::stringstream ss;
  if (t) t->Display(ss);
  MmaExpTree::DestroyTree(t);
  return ss.str();
}

SL std::string C(const std::string& s) {
  auto r = Compile(s);
  assert(std::size(r) == 1);
  return r[0];
}

SL void MmaParserTest() {
  // Structure, precedence and associativity.
  assert(Tree("1") == "1");
  assert(Tree("x") == "x");
  assert(Tree("  x  ") == "x");
  assert(Tree("a+b*c") == "(+,a,(*,b,c))");
  assert(Tree("a*b+c") == "(+,(*,a,b),c)");
  assert(Tree("a-b-c") == "(-,(-,a,b),c)");
  assert(Tree("a-b+c") == "(+,(-,a,b),c)");
  assert(Tree("a/b/c") == "(/,(/,a,b),c)");
  assert(Tree("a/b c") == "(*,(/,a,b),c)");
  assert(Tree("a^b^c") == "(^,a,(^,b,c))");
  assert(Tree("a b^2") == "(*,a,(^,b,2))");
  assert(Tree("-x") == "(-,,x)");
  assert(Tree("+x") == "x");
  assert(Tree("-x^2") == "(-,,(^,x,2))");
  assert(Tree("-a b") == "(*,(-,,a),b)");
  assert(Tree("a*-b") == "(*,a,(-,,b))");
  assert(Tree("2 x y") == "(*,(*,2,x),y)");
  assert(Tree("2x") == "(*,2,x)");
  assert(Tree("x2") == "x2");
  assert(Tree("x_1 _y") == "(*,x_1,_y)");
  assert(Tree("(a+b)(a-b)") == "(*,(+,a,b),(-,a,b))");
  assert(Tree("a(b+c)") == "(*,a,(+,b,c))");
  assert(Tree("(x+1)^3") == "(^,(+,x,1),3)");
  assert(Tree("Sqrt[x]") == "(Sqrt,,x)");
  assert(Tree("Sqrt(x+1)") == "(Sqrt,,(+,x,1))");
  assert(Tree("2 Sqrt[x]") == "(*,2,(Sqrt,,x))");
  assert(Tree("Sqrt[x] Sqrt[y]") == "(*,(Sqrt,,x),(Sqrt,,y))");
  assert(Tree("") == "");

  // Collected variables, bracket flags and node values.
  MmaExpParser parser;
  MmaExpTree* t = parser.Parse("(a + b) * c - 12 a");
  assert(parser.vars == (std::map<std::string, int64>{{"a", 0}, {"b", 0},
                                                       {"c", 0}}));
  assert(t->parent == nullptr);
  assert(t->nodeType == SUB && t->token == "-");
  assert(t->left->nodeType == MUL && t->left->left->bracketFlag == 1);
  assert(t->left->left->nodeType == ADD && t->left->left->left->parent ==
                                               t->left->left);
  assert(t->right->nodeType == MUL && t->right->left->nodeType == NUMBER);
  assert(t->right->left->value == 12);
  assert(t->Priority() == priority[SUB]);
  MmaExpTree::DestroyTree(t);
  MmaExpTree::DestroyTree(nullptr);

  // The parser reuses its state across Parse calls.
  t = parser.Parse("x");
  assert(t->nodeType == VAR && t->token == "x");
  assert(parser.vars.count("x") == 1);
  MmaExpTree::DestroyTree(t);

  // Priorities are ordered as expected.
  assert(priority[POW] < priority[NEG] && priority[NEG] < priority[MUL]);
  assert(priority[MUL] == priority[DIV] && priority[MUL] < priority[ADD]);
  assert(priority[ADD] == priority[SUB] && priority[ADD] < priority[ROOT]);

  // Tokenizer.
  MmaExpParser tk;
  tk.expression = "Sqrt x1 23+-*/()[]^";
  tk.curr = 0;
  std::string token;
  const int expected[] = {FUN,
                          VAR,
                          NUMBER,
                          ADD,
                          SUB,
                          MUL,
                          DIV,
                          LEFT_BRACKET,
                          RIGHT_BRACKET,
                          LEFT_SQUARE_BRACKET,
                          RIGHT_SQUARE_BRACKET,
                          POW,
                          EOI};
  const char* expected_token[] = {"Sqrt", "x1", "23", "+", "-", "*", "/",
                                  "(",    ")",  "[",  "]", "^", ""};
  for (int i = 0; i < 13; ++i) {
    assert(tk.NextToken(token) == expected[i]);
    assert(token == expected_token[i]);
  }
  assert(tk.OperatorNumber(ADD) == 2);
}

SL void MmaEvaluateTest() {
  struct Case {
    const char* expr;
    std::function<int64(int64, int64)> f;
  };
  const std::vector<Case> cases = {
      {"x + y", [](int64 x, int64 y) { return x + y; }},
      {"x - y - 3", [](int64 x, int64 y) { return x - y - 3; }},
      {"2 x y - x^2", [](int64 x, int64 y) { return 2 * x * y - x * x; }},
      {"(x + y)^3", [](int64 x, int64 y) { return (x + y) * (x + y) * (x + y); }},
      {"-x^2 + y", [](int64 x, int64 y) { return -x * x + y; }},
      {"(-x)^2 + y", [](int64 x, int64 y) { return x * x + y; }},
      {"x (y - 1) (x + 1)",
       [](int64 x, int64 y) { return x * (y - 1) * (x + 1); }},
      {"2^3^2 - x*-y", [](int64 x, int64 y) { return 512 + x * y; }},
      {"(x^2 y^2 - x y) / (x y)",
       [](int64 x, int64 y) { return x * y - 1; }},
      {"Sqrt[x^2 + 2 x y + y^2] Sqrt[4]",
       [](int64 x, int64 y) { return 2 * Abs(x + y); }},
      {"x - (y - (x - y))", [](int64 x, int64 y) { return 2 * x - 2 * y; }},
      {"12 / 4 / 3 x", [](int64 x, int64) { return x; }},
  };
  for (const auto& c : cases) {
    MmaExpParser parser;
    MmaExpTree* t = parser.Parse(c.expr);
    for (int64 x = -4; x <= 4; ++x) {
      for (int64 y = -4; y <= 4; ++y) {
        if (x == 0 || y == 0) continue;
        assert(EvalTree(t, {{"x", x}, {"y", y}}) == c.f(x, y));
      }
    }
    MmaExpTree::DestroyTree(t);
  }
}

SL void MmaCompileTest() {
  assert(C("1") == "1");
  assert(C("x") == "x");
  assert(C("+x") == "x");
  assert(C("a+b*c") == "a + b * c");
  assert(C("a-b-c") == "a - b - c");
  assert(C("a/b/c") == "a / b / c");
  assert(C("a - (b - c)") == "a - (b - c)");
  assert(C("a/(b c)") == "a / (b * c)");
  assert(C("2 x y") == "2 * x * y");
  assert(C("(a+b)(a-b)") == "(a + b) * (a - b)");
  assert(C("-x") == "-x");
  assert(C("-x^2") == "-(x * x)");
  // Small integer powers of a variable or a number are expanded.
  assert(C("x^1") == "x");
  assert(C("x^2") == "(x * x)");
  assert(C("x^3") == "(x * x * x)");
  assert(C("x^4") == "(x * x * x * x)");
  assert(C("3^2") == "(3 * 3)");
  assert(C("x^(2)") == "(x * x)");
  assert(C("a/x^2") == "a / (x * x)");
  // Other powers use Power(...).
  assert(C("x^5") == "Power(x,5)");
  assert(C("x^n") == "Power(x,n)");
  assert(C("x^-2") == "Power(x,-2)");
  assert(C("(x+1)^3") == "Power((x + 1),3)");
  assert(C("a^b^c") == "Power(a,Power(b,c))");
  // Sqrt.
  assert(C("Sqrt[x]") == "sqrt(x)");
  assert(C("Sqrt(x+1)") == "sqrt(x + 1)");
  assert(C("2 Sqrt[x]") == "2 * sqrt(x)");
  assert(C("Sqrt[x^2 - 2 x y + y^2]/(2 Sqrt[x])") ==
         "sqrt((x * x) - 2 * x * y + (y * y)) / (2 * sqrt(x))");

  // The compiler object can be used directly.
  MmaExpParser parser;
  MmaExpTree* t = parser.Parse("a b + c");
  MmaExpCompiler compiler;
  compiler.Compile(t);
  assert(compiler.result == "a * b + c");
  assert(compiler.CanSkipBracket(t->left->left));
  assert(!compiler.CanSkipBracket(t));
  MmaExpTree::DestroyTree(t);
}

SL void MmaCompileModTest() {
  const std::vector<std::string> r0 = CompileMod("(a^4+a b)*7/b");
  const std::vector<std::string> e0 = {
      "int64 foo(int64 a, int64 b, int64 mod) {",
      "  const int64 t0 = a % mod;",
      "  const int64 t1 = 4;",
      "  const int64 t2 = PowerMod(t0, t1, mod) % mod;",
      "  const int64 t3 = b % mod;",
      "  const int64 t4 = (t0 * t3) % mod;",
      "  const int64 t5 = (t2 + t4) % mod;",
      "  const int64 t6 = 7 % mod;",
      "  const int64 t7 = (t5 * t6) % mod;",
      "  const int64 t8 = t7 * ModInv(t3, mod) % mod;",
      "  return t8;",
      "}"};
  assert(r0 == e0);

  const std::vector<std::string> e1 = {
      "int64 bar(int64 a, int64 b, int64 mod) {",
      "  const int64 t0 = a % mod;", "  const int64 t1 = b % mod;",
      "  const int64 t2 = (mod + (t0 - t1) % mod) % mod;", "  return t2;",
      "}"};
  assert(CompileMod("a-b", "bar") == e1);

  const std::vector<std::string> e2 = {
      "int64 foo(int64 a, int64 mod) {", "  const int64 t0 = a % mod;",
      "  const int64 t1 = (mod - t0) % mod;",
      "  const int64 t2 = (t0 * t0) % mod;",
      "  const int64 t3 = (t1 + t2) % mod;", "  return t3;", "}"};
  assert(CompileMod("-a+a*a") == e2);

  // Commutative subexpressions are shared.
  const std::vector<std::string> e3 = {
      "int64 foo(int64 a, int64 b, int64 mod) {",
      "  const int64 t0 = a % mod;",
      "  const int64 t1 = b % mod;",
      "  const int64 t2 = (t0 * t1) % mod;",
      "  const int64 t3 = (t2 + t2) % mod;",
      "  return t3;",
      "}"};
  assert(CompileMod("a b + b a") == e3);

  // Exponents are computed without reduction.
  const std::vector<std::string> e4 = {
      "int64 foo(int64 x, int64 n, int64 mod) {",
      "  const int64 t0 = x % mod;",
      "  const int64 t1 = n;",
      "  const int64 t2 = 1;",
      "  const int64 t3 = (t1 - t2);",
      "  const int64 t4 = PowerMod(t0, t3, mod) % mod;",
      "  return t4;",
      "}"};
  assert(CompileMod("x^(n-1)") == e4);
  const std::vector<std::string> e5 = {
      "int64 foo(int64 x, int64 n, int64 mod) {",
      "  const int64 t0 = x % mod;",
      "  const int64 t1 = n;",
      "  const int64 t2 = 2;",
      "  const int64 t3 = (t1 / t2);",
      "  const int64 t4 = PowerMod(t0, t3, mod) % mod;",
      "  return t4;",
      "}"};
  assert(CompileMod("x^(n/2)") == e5);
  const std::vector<std::string> e6 = {
      "int64 foo(int64 x, int64 n, int64 mod) {",
      "  const int64 t0 = x % mod;",
      "  const int64 t1 = n;",
      "  const int64 t2 = (-t1);",
      "  const int64 t3 = PowerMod(t0, t2, mod) % mod;",
      "  return t3;",
      "}"};
  assert(CompileMod("x^(-n)") == e6);
  // Note: nested powers inside an exponent are not tested, see the TODO(bug)
  // in MmaModExpCompiler::CompileImpl.

  // A single variable or number.
  const std::vector<std::string> e7 = {"int64 f(int64 z, int64 mod) {",
                                       "  const int64 t0 = z % mod;",
                                       "  return t0;", "}"};
  assert(CompileMod("z", "f") == e7);
  const std::vector<std::string> e8 = {
      "int64 foo(int64 mod) {", "  const int64 t0 = 5 % mod;", "  return t0;",
      "}"};
  assert(CompileMod("5") == e8);
  const std::vector<std::string> e9 = {"int64 foo(int64 a, int64 mod) {",
                                       "  const int64 t0 = a % mod;",
                                       "  return t0;", "}"};
  assert(CompileMod("(a)") == e9);
}

SL void MmaParseSolutionTest() {
  auto s = ParseSolution(
      "{x1p0 -> 0, x1p1 -> -2, x1p2 -> 2, x2p0 -> 0, x2p1 -> 1, x2p2 -> -2, "
      "x2p3 -> 1, x3p0 -> 0}");
  // Zero values are dropped; trailing zero degrees are not stored.
  assert(std::size(s) == 2);
  assert(s[1] == std::vector<int64>({0, -2, 2}));
  assert(s[2] == std::vector<int64>({0, 1, -2, 1}));
  s = ParseSolution("x10p5 -> 1234567890123");
  assert(std::size(s) == 1);
  assert(s[10] == std::vector<int64>({0, 0, 0, 0, 0, 1234567890123LL}));
  s = ParseSolution("x1p0 -> 0");
  assert(s.empty());
  // Note: "x1p0->3" (no spaces) is not tested, see the TODO(bug) in
  // ParseSolution.

  auto l = ParseSolutionList("{{x1p0 -> 1, x2p1 -> -2}, {x1p0 -> 3}}");
  assert(l == std::vector<std::string>(
                  {"{x1p0 -> 1, x2p1 -> -2}", "{x1p0 -> 3}"}));
  assert(ParseSolutionList("{}").empty());
  assert(ParseSolutionList("").empty());
  assert(ParseSolutionList("{{}}") == std::vector<std::string>({"{}"}));
  l = ParseSolutionList("{{x1p0 -> 1}}");
  assert(std::size(l) == 1);
  assert(ParseSolution(l[0])[1] == std::vector<int64>({1}));
}

// Runs f with std::cout redirected and returns the output.
template <typename F>
SL std::string Capture(F f) {
  std::stringstream ss;
  auto* old = std::cout.rdbuf(ss.rdbuf());
  f();
  std::cout.rdbuf(old);
  return ss.str();
}

SL int CountFailed(const std::string& out) {
  int total = 0;
  std::stringstream ss(out);
  std::string line;
  const std::string key = "failed tests ";
  while (std::getline(ss, line)) {
    if (line.rfind(key, 0) == 0) total += std::stoi(line.substr(size(key)));
  }
  return total;
}

SL void MmaValidateTest() {
  // Fibonacci: a[n] = a[n-1] + a[n-2].
  std::vector<int64> fib = {1, 1};
  for (int i = 2; i < 30; ++i) fib.push_back(fib[i - 1] + fib[i - 2]);
  std::string out =
      Capture([&]() { ValidateOne<int64>(fib, "{x1p0 -> 1, x2p0 -> 1}"); });
  assert(out.find("max offset = 2, max degree = 0") != std::string::npos);
  assert(out.find("failed tests 0\n") != std::string::npos);
  assert(out.find("successful tests 28\n") != std::string::npos);

  out = Capture([&]() { ValidateOne<int64>(fib, "{x1p0 -> 2}"); });
  assert(CountFailed(out) > 0);

  // A001499 with leading 2: 2 a[n] = 2 n (n-1) a[n-1] + n (n-1)^2 a[n-2].
  std::vector<int64> a = {1, 0, 1};
  for (int64 n = 3; n < 12; ++n) {
    a.push_back((2 * n * (n - 1) * a[n - 1] + n * (n - 1) * (n - 1) * a[n - 2]) /
                2);
  }
  assert(a[4] == 90 && a[5] == 2040 && a[6] == 67950);
  const std::string sol =
      "{{x1p0 -> 0, x1p1 -> -2, x1p2 -> 2, x1p3 -> 0, x2p0 -> 0, x2p1 -> 1, "
      "x2p2 -> -2, x2p3 -> 1}, {x1p0 -> 1}}";
  out = Capture([&]() { ValidateAll<int64>(a, sol, 2); });
  assert(out.find("max offset = 2, max degree = 3") != std::string::npos);
  assert(out.find("successful tests 10\n") != std::string::npos);
  // The second (wrong) solution fails somewhere.
  assert(CountFailed(out) > 0);
  out = Capture([&]() { ValidateAll<int64>(a, "{{x1p0 -> 0}}", 1); });
  assert(out.find("max offset = 0, max degree = 0") != std::string::npos);

  // FRHelper.
  FRHelper<int64> h;
  h.set_values({1, 1, 2, 3, 5, 8})
      .set_check_points({2, 3})
      .set_offsets({1, 2})
      .set_max_degree(0)
      .set_max_abs_coe(2);
  const std::string expected =
      "FindInstance[1x1p0+1x2p0-2==0&&2x1p0+1x2p0-3==0&&x1p0>=-2&&x1p0<=2&&"
      "x2p0>=-2&&x2p0<=2, {x1p0, x2p0}, Integers, 10]";
  assert(h.ToString() == expected);
  std::stringstream ss;
  ss << h;
  assert(ss.str() == expected);

  // Without max_abs_coe, with degree 1 and leading 3.
  FRHelper<int64> h2;
  h2.set_values({1, 2, 4, 8, 16}).set_check_points({2}).set_offsets({1});
  h2.set_leading(3);
  assert(h2.ToString() ==
         "FindInstance[2x1p0+4x1p1-12==0, {x1p0, x1p1}, Integers, 10]");
  assert(h2.max_degree_ == 1 && h2.leading_ == 3);

  out = Capture([&]() { h.Validate("{{x1p0 -> 1, x2p0 -> 1}}"); });
  assert(out.find("failed tests 0\n") != std::string::npos);
  assert(out.find("successful tests 4\n") != std::string::npos);
  out = Capture([&]() { h.Validate(fib, "{{x1p0 -> 1, x2p0 -> 1}}"); });
  assert(out.find("successful tests 28\n") != std::string::npos);
}

SL void MmaIntPolyTest() {
  // PPrintVec prints a one-element vector without braces.
  IntPoly ip;
  ip.Reset({"x"});
  assert(ip.n == 1);
  for (int64 x = 0; x <= 3; ++x) ip.Add({x}, 1 + 2 * x + 3 * x * x);
  ip.Add({2}, 17);  // duplicated with the same value
  std::stringstream ss;
  ss << ip;
  assert(ss.str() ==
         "InterpolatingPolynomial[{{0, 1}, {1, 6}, {2, 17}, {3, 34}}, x]");

  // Without variable names, a, b, ... are used.
  IntPoly ip2;
  ip2.Add({1, 2}, 3);
  ip2.Add({0, 0}, 1);
  assert(ip2.n == 2);
  std::stringstream ss2;
  ip2.ToStream(ss2);
  assert(ss2.str() == "InterpolatingPolynomial[{{{0, 0}, 1}, {{1, 2}, 3}}, "
                      "{a, b}]");

  IntPoly empty;
  std::stringstream ss3;
  ss3 << empty;
  assert(ss3.str() == "InterpolatingPolynomial[{}, ]");

  // IntPoly2D.
  IntPoly2D p(1);
  assert(p.n1 == 1 && p.n2 == 1 && p.n3 == 1 && p.vcnt == 3);
  assert(p.symbols == std::vector<std::string>({"1", "y", "x"}));
  p.Add({2, 3}, 7);
  p.Add({2, 3}, 7);
  std::stringstream s4;
  s4 << p;
  assert(s4.str() == "Values[Solve[{1 x0 + 3 x1 + 2 x2 == 7}, {x0, x1, x2}]]");
  std::stringstream s5;
  p.Show(s5, {1, -1, -2});
  assert(s5.str() == "1 - y - 2 x");
  std::stringstream s6;
  p.Show(s6, {-1, 0, 1});
  assert(s6.str() == "-1 + x");
  std::stringstream s7;
  p.Show(s7, {0, 0, 0});
  assert(s7.str().empty());

  IntPoly2D q(2, 2, 2);
  assert(q.vcnt == 6);
  assert(q.symbols ==
         std::vector<std::string>({"1", "y", "y^2", "x", "x y", "x^2"}));
  std::stringstream s8;
  q.Show(s8, {1, 3, 6, 2, 4, 5});
  assert(s8.str() == "1 + 3 y + 6 y^2 + 2 x + 4 x y + 5 x^2");
  assert(q.Evaluate(2, 3, 2, 1) == 12);
  assert(q.Evaluate(5, 7, 0, 0) == 1);

  // n2 / n3 default to n1; full rectangle when n3 is large.
  q.Reset(1, 2, 10);
  assert(q.vcnt == 6);
  q.Reset({"u", "v"}, 2, 1, 1);
  assert(q.vcnt == 4);
  assert(q.symbols == std::vector<std::string>({"1", "v", "u", "u^2"}));
  assert(q.Format(3, 2) == "u^3 v^2");
  assert(q.Format(0, 0) == "1");
  assert(q.Format("w", 1) == "w");
  q.Reset(0);
  assert(q.vcnt == 1 && q.symbols == std::vector<std::string>({"1"}));
  q.Add({0, 0}, 4);
  q.Add({1, 0}, 4);
  std::stringstream s9;
  s9 << q;
  assert(s9.str() == "Values[Solve[{1 x0 == 4 && 1 x0 == 4}, x0]]");
}

PE_REGISTER_TEST(&MmaParserTest, "MmaParserTest", SMALL);
PE_REGISTER_TEST(&MmaEvaluateTest, "MmaEvaluateTest", SMALL);
PE_REGISTER_TEST(&MmaCompileTest, "MmaCompileTest", SMALL);
PE_REGISTER_TEST(&MmaCompileModTest, "MmaCompileModTest", SMALL);
PE_REGISTER_TEST(&MmaParseSolutionTest, "MmaParseSolutionTest", SMALL);
PE_REGISTER_TEST(&MmaValidateTest, "MmaValidateTest", SMALL);
PE_REGISTER_TEST(&MmaIntPolyTest, "MmaIntPolyTest", SMALL);
}  // namespace mma_test
