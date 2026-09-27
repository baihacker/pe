#include "pe_test.h"

namespace geometry_test {
SL bool Near(double a, double b, double eps = 1e-12) {
  return std::fabs(a - b) <= eps;
}

SL void Point2DTest() {
  using P = Point2D<int64>;
  const P o;
  assert(o.x == 0 && o.y == 0);
  const P a(3, 4), b(1, -2);
  const P c(3, 5), d(4, 0);

  // Comparisons: lexicographic by (x, y).
  assert(a == P(3, 4) && !(a == c) && a != c && !(a != P(3, 4)));
  assert(a < c && !(c < a) && !(a < a) && b < a && a < d);
  assert(c > a && !(a > c) && !(a > a) && d > c);
  assert(a <= a && a <= c && !(c <= a));
  assert(a >= a && c >= a && !(a >= c));

  // Arithmetic.
  assert(a + b == P(4, 2) && a - b == P(2, 6) && b - a == P(-2, -6));
  assert(a + int64(10) == P(13, 14) && a - int64(3) == P(0, 1));

  // Dot / Cross.
  assert(Dot(a, b) == 3 - 8 && Cross(a, b) == -6 - 4);
  assert(Dot(a, a) == 25 && Cross(a, a) == 0);
  assert(Dot(o, a, b) == Dot(a, b) && Cross(o, a, b) == Cross(a, b));
  // Relative to a different origin: (b - c) . (d - c) and (b - c) x (d - c).
  assert(Dot(c, b, d) == (-2) * 1 + (-7) * (-5));
  assert(Cross(c, b, d) == (-2) * (-5) - (-7) * 1);
  // Counter-clockwise is positive.
  assert(Cross(P(0, 0), P(1, 0), P(0, 1)) == 1);
  assert(Cross(P(0, 0), P(0, 1), P(1, 0)) == -1);
  assert(Cross(P(0, 0), P(1, 1), P(2, 2)) == 0);

  // Norms.
  assert(a.Norm2() == 25 && Norm2(a) == 25 && Norm2(o) == 0);
  assert(Near(a.Norm(), 5.0) && Near(Norm(a), 5.0) && Near(Norm(o), 0.0));
  assert(Near(Norm(P(1, 1)), std::sqrt(2.0)));

  // Conversion to Point3D and back.
  const Point3D<int64> a3 = a;
  assert(a3.x == 3 && a3.y == 4 && a3.z == 0);
  const P back = Point3D<int64>(7, 8, 9);
  assert(back == P(7, 8));

  // Stream output.
  std::stringstream ss;
  ss << P(-1, 2);
  assert(ss.str() == "-1, 2");

  // Sorting uses operator<.
  std::vector<P> pts{P(2, 1), P(1, 5), P(1, -1), P(0, 0)};
  std::sort(std::begin(pts), std::end(pts));
  assert((pts == std::vector<P>{P(0, 0), P(1, -1), P(1, 5), P(2, 1)}));

  // Floating point coordinates, including scaling.
  using PD = Point2D<double>;
  const PD f(1.5, -2.0);
  const PD g = f * 2.0;
  assert(Near(g.x, 3.0) && Near(g.y, -4.0));
  assert(Near(g.Norm(), 5.0) && Near(Dot(f, g), 12.5));
  assert(Near(Cross(PD(1, 0), PD(0.5, 0.5)), 0.5));
  const PD h = f * 0.0;
  assert(h == PD(0, 0));
}

SL void Point3DTest() {
  using P = Point3D<int64>;
  const P o;
  assert(o.x == 0 && o.y == 0 && o.z == 0);
  const P a(1, 2, 3), b(4, -5, 6);

  assert(a == P(1, 2, 3) && a != b && !(a != a));
  assert(a < b && P(1, 2, 2) < a && P(1, 1, 9) < a && !(a < a));
  assert(b > a && a > P(1, 2, 2) && !(a > a));
  assert(a <= a && a <= b && !(b <= a));
  assert(a >= a && b >= a && !(a >= b));

  assert(a + b == P(5, -3, 9) && a - b == P(-3, 7, -3));
  assert(a + int64(1) == P(2, 3, 4) && a - int64(1) == P(0, 1, 2));

  assert(Dot(a, b) == 4 - 10 + 18);
  assert(Dot(o, a, b) == Dot(a, b));
  assert(Dot(a, b, P(0, 0, 0)) == Dot(b - a, o - a));
  assert(a.Norm2() == 14 && Norm2(a) == 14);
  assert(Near(a.Norm(), std::sqrt(14.0)) && Near(Norm(P(2, 3, 6)), 7.0));

  // Three point Cross is the determinant of the rows.
  const P ex(1, 0, 0), ey(0, 1, 0), ez(0, 0, 1);
  assert(Cross(ex, ey, ez) == 1);
  assert(Cross(ey, ex, ez) == -1);
  assert(Cross(ex, ex, ez) == 0);
  assert(Cross(P(2, 0, 1), P(1, 3, 2), P(1, 1, 1)) ==
         2 * (3 * 1 - 2 * 1) - 1 * (0 * 1 - 1 * 1) + 1 * (0 * 2 - 1 * 3));
  // Brute force against the Leibniz formula.
  for (int64 i = -2; i <= 2; ++i) {
    const P u(i, 2 - i, i * i), v(1 - i, 3, -i), w(i, -1, 2);
    const int64 det = u.x * v.y * w.z + u.y * v.z * w.x + u.z * v.x * w.y -
                      u.z * v.y * w.x - u.y * v.x * w.z - u.x * v.z * w.y;
    assert(Cross(u, v, w) == det);
  }
  // Four point Cross: 6 times the signed volume.
  assert(Cross(o, ex, ey, ez) == 1);
  assert(Cross(o, ey, ex, ez) == -1);
  assert(Cross(P(1, 1, 1), P(3, 1, 1), P(1, 4, 1), P(1, 1, 6)) == 2 * 3 * 5);
  assert(Cross(o, ex, ey, P(1, 1, 0)) == 0);

  std::stringstream ss;
  ss << P(-1, 0, 7);
  assert(ss.str() == "-1, 0, 7");

  using PD = Point3D<double>;
  const PD f = PD(1, 2, 2) * 0.5;
  assert(Near(f.x, 0.5) && Near(f.y, 1.0) && Near(f.z, 1.0));
  assert(Near(f.Norm(), 1.5));
}

SL void Line2DTest() {
  using L = Line2D<int64>;
  using P = Point2D<int64>;
  // Default: all zero, normalization is skipped when A == B == 0.
  const L zero;
  assert(zero.A == 0 && zero.B == 0 && zero.C == 0);
  const L only_c(0, 0, 5);
  assert(only_c.A == 0 && only_c.B == 0 && only_c.C == 5);

  // Normalized: divided by gcd, first non-zero of (A, B) positive.
  assert(L(2, 4, 6) == L(1, 2, 3));
  const L l1(2, 4, 6);
  assert(l1.A == 1 && l1.B == 2 && l1.C == 3);
  const L l2(-2, 4, 6);
  assert(l2.A == 1 && l2.B == -2 && l2.C == -3);
  const L l3(0, -3, 6);
  assert(l3.A == 0 && l3.B == 1 && l3.C == -2);
  const L l4(-4, -6, 0);
  assert(l4.A == 2 && l4.B == 3 && l4.C == 0);
  const L l5(5, 0, -10);
  assert(l5.A == 1 && l5.B == 0 && l5.C == -2);

  // Through two points: the same line regardless of the chosen points.
  const L p1(P(1, 2), P(3, 5));
  assert(p1.A == 3 && p1.B == -2 && p1.C == 1);
  assert(p1 == L(P(3, 5), P(1, 2)));
  assert(p1 == L(P(-1, -1), P(5, 8)));
  assert(L(P(0, 0), P(2, 2)) == L(1, -1, 0));
  assert(L(P(0, 7), P(0, -3)) == L(1, 0, 0));
  assert(L(P(4, 7), P(-3, 7)) == L(0, 1, -7));
  // Every point on the segment satisfies A x + B y + C == 0.
  for (int64 t = -3; t <= 3; ++t) {
    const P q(1 + 2 * t, 2 + 3 * t);
    assert(p1.A * q.x + p1.B * q.y + p1.C == 0);
  }
  // Degenerate: two identical points.
  const L deg(P(2, 3), P(2, 3));
  assert(deg.A == 0 && deg.B == 0 && deg.C == 0);

  // Ordering: lexicographic by (A, B, C).
  assert(L(1, 2, 3) < L(1, 2, 4) && L(1, 2, 3) < L(1, 3, 0) &&
         L(1, 2, 3) < L(2, 1, 1));
  assert(L(2, 1, 1) > L(1, 5, 5) && L(1, 2, 4) > L(1, 2, 3));
  assert(L(1, 2, 3) <= L(1, 2, 3) && L(1, 2, 3) >= L(1, 2, 3));
  assert(!(L(1, 2, 4) <= L(1, 2, 3)) && !(L(1, 2, 3) >= L(1, 2, 4)));
  assert(L(1, 2, 3) != L(1, 2, 4) && !(L(1, 2, 3) != L(2, 4, 6)));

  std::stringstream ss;
  ss << L(-2, 4, 6);
  assert(ss.str() == "1, -2, -3");

  // Floating point lines are not normalized.
  const Line2D<double> fl(2, 4, 6);
  assert(fl.A == 2 && fl.B == 4 && fl.C == 6);
  const Line2D<double> fp(Point2D<double>(0, 0), Point2D<double>(1, 2));
  assert(fp.A == 2 && fp.B == -1 && fp.C == 0);
}

SL void DotCrossScalarTest() {
  assert(Dot(3, 4, 5, 6) == 39 && Cross(1, 2, 3, 4) == -2);
  assert(Dot(int64(0), int64(0), int64(7), int64(8)) == 0);
  assert(Cross(int64(2), int64(3), int64(4), int64(6)) == 0);
  using PI = std::pair<int64, int64>;
  assert(Dot(PI{3, 4}, PI{5, 6}) == 39 && Cross(PI{1, 2}, PI{3, 4}) == -2);
  // Three pair forms are relative to the first pair.
  assert(Dot(PI{1, 1}, PI{4, 5}, PI{6, 7}) == 3 * 5 + 4 * 6);
  assert(Cross(PI{1, 1}, PI{4, 5}, PI{6, 7}) == 3 * 6 - 4 * 5);
  assert(Cross(PI{0, 0}, PI{1, 0}, PI{0, 1}) == 1);
  // Agrees with Point2D.
  for (int64 i = -3; i <= 3; ++i) {
    for (int64 j = -3; j <= 3; ++j) {
      const Point2D<int64> a(i, j), b(j - 1, 2 * i), c(1, -j);
      assert(Dot(PI{i, j}, PI{j - 1, 2 * i}) == Dot(a, b));
      assert(Cross(PI{i, j}, PI{j - 1, 2 * i}) == Cross(a, b));
      assert(Dot(PI{i, j}, PI{j - 1, 2 * i}, PI{1, -j}) == Dot(a, b, c));
      assert(Cross(PI{i, j}, PI{j - 1, 2 * i}, PI{1, -j}) == Cross(a, b, c));
    }
  }
  using PD = std::pair<double, double>;
  assert(Near(Dot(PD{0.5, 0.25}, PD{2, 4}), 2.0));
  assert(Near(Cross(PD{0.5, 0.25}, PD{2, 4}), 1.5));
}

SL void AngleAndRotationTest() {
  const double pi = GeoConstant::PI;
  assert(Near(pi, std::acos(-1.0)));
  assert(Near(GeoConstant::RAD_TO_DEGREE_COE * GeoConstant::DEGREE_TO_RAD_COE,
              1.0));
  assert(Near(RadToDegree(pi), 180.0) && Near(RadToDegree(0), 0.0));
  assert(Near(RadToDegree(-pi / 2), -90.0));
  assert(Near(DegreeToRad(180), pi) && Near(DegreeToRad(90), pi / 2));
  assert(Near(DegreeToRad(-45), -pi / 4) && Near(DegreeToRad(0), 0.0));
  for (int deg = -360; deg <= 360; deg += 15) {
    assert(Near(RadToDegree(DegreeToRad(deg)), deg, 1e-9));
  }

  auto near_pair = [](std::pair<double, double> p, double x, double y) {
    return Near(p.first, x, 1e-12) && Near(p.second, y, 1e-12);
  };

  // General rotation.
  const Rotation<> r90(pi / 2);
  assert(Near(r90.rad_, pi / 2) && Near(r90.cos_, 0) && Near(r90.sin_, 1));
  assert(near_pair(r90(1, 0), 0, 1) && near_pair(r90(0, 1), -1, 0));
  assert(near_pair(r90.Rotate(2, 3), -3, 2));
  assert(near_pair(r90.RotateAntiClock(2, 3), -3, 2));
  assert(near_pair(r90.RotateClock(2, 3), 3, -2));
  double xo = 0, yo = 0;
  r90(1, 0, xo, yo);
  assert(Near(xo, 0) && Near(yo, 1));
  r90.Rotate(0, 1, xo, yo);
  assert(Near(xo, -1) && Near(yo, 0));
  r90.RotateAntiClock(5, 0, xo, yo);
  assert(Near(xo, 0) && Near(yo, 5));
  r90.RotateClock(5, 0, xo, yo);
  assert(Near(xo, 0) && Near(yo, -5));

  const Rotation<double> r0(0);
  assert(near_pair(r0(3, -4), 3, -4) &&
         near_pair(r0.RotateClock(3, -4), 3, -4));

  // Rotations by an arbitrary angle keep the norm, and clock undoes
  // anticlock.
  for (int deg = 0; deg < 360; deg += 30) {
    const Rotation<double> r(DegreeToRad(deg));
    const auto [x1, y1] = r(3, 4);
    assert(Near(x1 * x1 + y1 * y1, 25, 1e-9));
    const auto [x2, y2] = r.RotateClock(x1, y1);
    assert(Near(x2, 3, 1e-12) && Near(y2, 4, 1e-12));
  }
  const Rotation<double> r60(DegreeToRad(60));
  assert(near_pair(r60(2, 0), 1, std::sqrt(3.0)));

  // Long double rotation.
  const Rotation<long double> rl(pi);
  const auto pl = rl(1, 2);
  assert(std::fabs(pl.first + 1) < 1e-12 && std::fabs(pl.second + 2) < 1e-12);

  // Exact quarter turn.
  const RotationHalfPi<int64> h;
  assert(Near(h.rad_, pi / 2) && h.cos_ == 0 && h.sin_ == 1);
  assert(h(3, 4) == std::make_pair(int64(-4), int64(3)));
  assert(h.Rotate(3, 4) == std::make_pair(int64(-4), int64(3)));
  assert(h.RotateAntiClock(3, 4) == std::make_pair(int64(-4), int64(3)));
  assert(h.RotateClock(3, 4) == std::make_pair(int64(4), int64(-3)));
  int64 ix = 0, iy = 0;
  h(1, 0, ix, iy);
  assert(ix == 0 && iy == 1);
  h.Rotate(0, 1, ix, iy);
  assert(ix == -1 && iy == 0);
  h.RotateAntiClock(-1, 0, ix, iy);
  assert(ix == 0 && iy == -1);
  h.RotateClock(0, 1, ix, iy);
  assert(ix == 1 && iy == 0);
  // Four quarter turns are the identity.
  int64 x = 5, y = -7;
  for (int i = 0; i < 4; ++i) h(x, y, x, y);
  assert(x == 5 && y == -7);
  // Agrees with the general rotation.
  const RotationHalfPi<double> hd;
  assert(near_pair(hd(2.5, -1), r90(2.5, -1).first, r90(2.5, -1).second));

  // Exact half turn.
  const RotationPi<int64> p;
  assert(Near(p.rad_, pi) && p.cos_ == -1 && p.sin_ == 0);
  assert(p(3, -4) == std::make_pair(int64(-3), int64(4)));
  assert(p.Rotate(3, -4) == std::make_pair(int64(-3), int64(4)));
  assert(p.RotateAntiClock(3, -4) == std::make_pair(int64(-3), int64(4)));
  assert(p.RotateClock(3, -4) == std::make_pair(int64(-3), int64(4)));
  p(1, 2, ix, iy);
  assert(ix == -1 && iy == -2);
  p.Rotate(0, 0, ix, iy);
  assert(ix == 0 && iy == 0);
  p.RotateAntiClock(-5, 6, ix, iy);
  assert(ix == 5 && iy == -6);
  p.RotateClock(7, 8, ix, iy);
  assert(ix == -7 && iy == -8);
  const Rotation<double> r180(pi);
  const RotationPi<double> pd;
  assert(near_pair(pd(1.5, 2), r180(1.5, 2).first, r180(1.5, 2).second));
}

SL void VectorTest() {
  using V = std::vector<int64>;
  const V a{1, 2, 3, -4}, b{5, -6, 7, 8};
  assert(VectorAdd(a, b) == (V{6, -4, 10, 4}));
  assert(VectorSub(a, b) == (V{-4, 8, -4, -12}));
  assert(VectorSub(b, a) == (V{4, -8, 4, 12}));
  assert(VectorSub(a, a) == (V{0, 0, 0, 0}));
  assert(VectorScale(int64(3), a) == (V{3, 6, 9, -12}));
  assert(VectorScale(a, int64(-2)) == (V{-2, -4, -6, 8}));
  assert(VectorScale(int64(0), a) == (V{0, 0, 0, 0}));
  assert(VectorDotProduct(a, b) == 5 - 12 + 21 - 32);
  assert(VectorDotProduct(a, a) == 1 + 4 + 9 + 16);

  // Empty vectors.
  const V e;
  assert(VectorAdd(e, e).empty() && VectorSub(e, e).empty());
  assert(VectorScale(int64(5), e).empty() &&
         VectorScale(e, int64(5)).empty());
  assert(VectorDotProduct(e, e) == 0);

  // Single element.
  assert(VectorAdd(V{7}, V{-7}) == V{0} &&
         VectorDotProduct(V{7}, V{-3}) == -21);

  // Floating point.
  using VD = std::vector<double>;
  const VD x{0.5, 1.5}, y{2.0, -1.0};
  const VD s = VectorAdd(x, y);
  assert(Near(s[0], 2.5) && Near(s[1], 0.5));
  const VD t = VectorScale(0.5, y);
  assert(Near(t[0], 1.0) && Near(t[1], -0.5));
  assert(Near(VectorDotProduct(x, y), -0.5));

  // Linear identities checked on a small range of vectors.
  for (int64 i = -3; i <= 3; ++i) {
    const V u{i, i * i, 1 - i}, v{2 * i, -1, i + 5};
    assert(VectorSub(VectorAdd(u, v), v) == u);
    assert(VectorScale(int64(2), u) == VectorAdd(u, u));
    assert(VectorScale(u, i) == VectorScale(i, u));
    assert(VectorDotProduct(u, v) == VectorDotProduct(v, u));
    assert(VectorDotProduct(VectorScale(i, u), v) ==
           i * VectorDotProduct(u, v));
  }
}

SL void GeometryTest() {
  Point2DTest();
  Point3DTest();
  Line2DTest();
  DotCrossScalarTest();
  AngleAndRotationTest();
  VectorTest();
}

PE_REGISTER_TEST(&GeometryTest, "GeometryTest", SMALL);
}  // namespace geometry_test
