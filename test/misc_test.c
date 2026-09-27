#include "pe_test.h"

// pe_base doesn't include <bitset>; MSVC doesn't get it transitively.
#include <bitset>

namespace misc_test {
SL void MiscTest() {
  GaussianEliminationSolver solver;
  solver.Init(10, 10);
  for (int i = 0; i < 10; ++i) {
    solver.At(i, 10) = 10 - i;
    for (int j = i; j < 10; ++j) {
      solver.At(i, j) = 1;
    }
  }
  auto v = solver.Solve();
  for (int i = 0; i < 10; ++i) {
    assert(FAbs(v[i] - 1) < 1e-10);
  }

  auto vtos = [=](const std::vector<int64>& vec) {
    std::stringstream ss;
    ss << vec;
    return ss.str();
  };

  std::vector<int64> vec;
  assert(vtos(vec) == "{}");

  vec.push_back(1);
  assert(vtos(vec) == "{1}");

  vec.push_back(2);
  assert(vtos(vec) == "{1, 2}");

  vec.push_back(3);
  assert(vtos(vec) == "{1, 2, 3}");
}

PE_REGISTER_TEST(&MiscTest, "MiscTest", SMALL);

SL void CountPtInCircleTest() {
  for (int64 n = 0; n <= 100; ++n) {
    int64 u = CountPtInCircle(n);
    int64 v = CountPtInCircleBf(n);
    int64 ans = 0;
    const int t = (int)SqrtI(n);
    for (int x = -t; x <= t; ++x) {
      for (int y = -t; y <= t; ++y) ans += sq(x) + sq(y) <= n;
    }
    assert(u == ans);
    assert(v == ans);
  }
#if 1
  for (int64 i = 1; i <= 10000; ++i) {
    int64 u = CountPtInCircleQ1(i);
    int64 v = CountPtInCircleQ1Bf(i);
    if (u != v) {
      std::cerr << i << " " << u << " " << v << std::endl;
    }
    assert(u == v);
  }
#endif

#if !defined(CONTINUOUS_INTEGRATION_TEST)
  // 9999999999999907 7853981733966909 7853981733966913
  // CountPtInCircleQ1Bf is O(sqrt(n)).
  for (int64 i = 10000; i <= 100000000000000; i = i * 10) {
    for (int64 j = -3; j <= 3; ++j) {
      int64 target = i + j;
      int64 u = CountPtInCircleQ1(target);
      int64 v = CountPtInCircleQ1Bf(target);
      if (u != v) {
        std::cerr << target << " " << u << " " << v << std::endl;
      }
      assert(u == v);
    }
  }
#endif
}

PE_REGISTER_TEST(&CountPtInCircleTest, "CountPtInCircleTest", MEDIUM);

#if PE_HAS_INT128
SL void SumSigma0Test() {
#if 1
  for (int64 i = 1; i <= 10000; ++i) {
    int64 u = SumSigma0(i);
    int64 v = SumSigma0Bf(i);
    auto w = min25::sigma0_sum_fast(i);
    if (u != v || v != w || u != w) {
      std::cerr << i << " " << u << " " << v << " " << w << std::endl;
    }
    assert(u == v);
    assert(u == w);
  }
#endif

  // 9999999999999907 7853981733966909 7853981733966913
  for (int64 i = 10000; i <= 100000000000000000; i = i * 10) {
    for (int64 j = -3; j <= 3; ++j) {
      int64 target = i + j;
      int64 u = SumSigma0(target);
      int64 v = SumSigma0Bf(target);
      auto w = min25::sigma0_sum_fast(target);
      if (u != v || v != w || u != w) {
        std::cerr << target << " " << u << " " << v << " " << w << std::endl;
      }
      assert(u == v);
      assert(u == w);
    }
  }
}

PE_REGISTER_TEST(&SumSigma0Test, "SumSigma0Test", SUPER);
#endif

SL int64 IntDivFloor(int64 a, int64 b) {
  if (b < 0) a = -a;
  if (a % b == 0) return a / b;
  if (a >= 0) return a / b;
  return a / b - 1;
}

SL void SolveInequatilityGE2Test() {
  for (int64 x1 = -350; x1 <= 350; ++x1) {
    for (int64 x2 = x1; x2 <= 350; ++x2) {
      // (100 x-x1)(100 x-x2) >= 0
      // 10000 x^2-(100 x1 + 100 x2) x + x1 x2 >= 0
      const int64 A = 10000;
      const int64 B = -(100 * x1 + 100 * x2);
      const int64 C = x1 * x2;
      int64 u = IntDivFloor(x1, 100);
      int64 v = x2 % 100 == 0 ? IntDivFloor(x2, 100) : IntDivFloor(x2, 100) + 1;
      auto ans = SolveInequatilityGE2<int64>(A, B, C);
      if (u == v || u + 1 == v) {
        assert(std::size(ans) == 1);
        assert(ans[0].x1 == -IntegerRange64::inf);
        assert(ans[0].x2 == IntegerRange64::inf);
      } else {
        assert(std::size(ans) == 2);
        assert(ans[0].x1 == -IntegerRange64::inf);
        assert(ans[0].x2 == u);
        assert(ans[1].x1 == v);
        assert(ans[1].x2 == IntegerRange64::inf);
      }
    }
  }
}
PE_REGISTER_TEST(&SolveInequatilityGE2Test, "SolveInequatilityGE2Test", SMALL);

SL void SolveInequatilityG2Test() {
  for (int64 x1 = -350; x1 <= 350; ++x1) {
    for (int64 x2 = x1; x2 <= 350; ++x2) {
      // (100 x-x1)(100 x-x2) > 0
      // 10000 x^2-(100 x1 + 100 x2) x + x1 x2 > 0
      const int64 A = 10000;
      const int64 B = -(100 * x1 + 100 * x2);
      const int64 C = x1 * x2;
      int64 u = x1 % 100 == 0 ? IntDivFloor(x1, 100) - 1 : IntDivFloor(x1, 100);
      int64 v = IntDivFloor(x2, 100) + 1;
      auto ans = SolveInequatilityG2<int64>(A, B, C);
      if (u == v || u + 1 == v) {
        assert(std::size(ans) == 1);
        assert(ans[0].x1 == -IntegerRange64::inf);
        assert(ans[0].x2 == IntegerRange64::inf);
      } else {
        assert(std::size(ans) == 2);
        assert(ans[0].x1 == -IntegerRange64::inf);
        assert(ans[0].x2 == u);
        assert(ans[1].x1 == v);
        assert(ans[1].x2 == IntegerRange64::inf);
      }
    }
  }
}
PE_REGISTER_TEST(&SolveInequatilityG2Test, "SolveInequatilityG2Test", SMALL);

SL void SolveInequatilityLE2Test() {
  for (int64 x1 = -350; x1 <= 350; ++x1) {
    for (int64 x2 = x1; x2 <= 350; ++x2) {
      // (100 x-x1)(100 x-x2) <= 0
      // 10000 x^2-(100 x1 + 100 x2) x + x1 x2 <= 0
      const int64 A = 10000;
      const int64 B = -(100 * x1 + 100 * x2);
      const int64 C = x1 * x2;
      int64 u = x1 % 100 == 0 ? IntDivFloor(x1, 100) : IntDivFloor(x1, 100) + 1;
      int64 v = IntDivFloor(x2, 100);
      auto ans = SolveInequatilityLE2<int64>(A, B, C);
      if (u > v) {
        assert(std::size(ans) == 0);
      } else {
        assert(std::size(ans) == 1);
        assert(ans[0].x1 == u);
        assert(ans[0].x2 == v);
      }
    }
  }
}
PE_REGISTER_TEST(&SolveInequatilityLE2Test, "SolveInequatilityLE2Test", SMALL);

SL void SolveInequatilityL2Test() {
  for (int64 x1 = -350; x1 <= 350; ++x1) {
    for (int64 x2 = x1; x2 <= 350; ++x2) {
      // (100 x-x1)(100 x-x2) < 0
      // 10000 x^2-(100 x1 + 100 x2) x + x1 x2 < 0
      const int64 A = 10000;
      const int64 B = -(100 * x1 + 100 * x2);
      const int64 C = x1 * x2;
      int64 u = IntDivFloor(x1, 100) + 1;
      int64 v = x2 % 100 == 0 ? IntDivFloor(x2, 100) - 1 : IntDivFloor(x2, 100);
      auto ans = SolveInequatilityL2<int64>(A, B, C);
      if (u > v) {
        assert(std::size(ans) == 0);
      } else {
        assert(std::size(ans) == 1);
        assert(ans[0].x1 == u);
        assert(ans[0].x2 == v);
      }
    }
  }
}
PE_REGISTER_TEST(&SolveInequatilityL2Test, "SolveInequatilityL2Test", SMALL);
// Checks the ranges against A x^2 + B x + C op 0 for every small x. The roots
// of these small quadratics are within [-20, 20].
template <typename F, typename P>
SL void CheckInequality(const F& solve, const P& pred) {
  for (int64 A = -4; A <= 4; ++A) {
    for (int64 B = -6; B <= 6; ++B) {
      for (int64 C = -6; C <= 6; ++C) {
        const std::vector<IntegerRange64> ranges = solve(A, B, C);
        for (size_t i = 0; i < std::size(ranges); ++i) {
          assert(ranges[i].x1 <= ranges[i].x2);
          if (i > 0) assert(ranges[i - 1].x2 + 1 < ranges[i].x1);
        }
        for (int64 x = -40; x <= 40; ++x) {
          bool in = false;
          for (const auto& r : ranges) in = in || (r.x1 <= x && x <= r.x2);
          assert(in == pred(A * x * x + B * x + C));
        }
      }
    }
  }
}

SL void SolveInequalityAllCasesTest() {
  // Degree 2 (A != 0), degree 1 (A == 0) and degree 0 (A == B == 0).
  CheckInequality([](int64 A, int64 B, int64 C) {
    return SolveInequatilityGE2(A, B, C);
  }, [](int64 v) { return v >= 0; });
  CheckInequality([](int64 A, int64 B, int64 C) {
    return SolveInequatilityG2(A, B, C);
  }, [](int64 v) { return v > 0; });
  CheckInequality([](int64 A, int64 B, int64 C) {
    return SolveInequatilityLE2(A, B, C);
  }, [](int64 v) { return v <= 0; });
  CheckInequality([](int64 A, int64 B, int64 C) {
    return SolveInequatilityL2(A, B, C);
  }, [](int64 v) { return v < 0; });
  // Degree 1 and 0 directly.
  for (int64 B = -6; B <= 6; ++B) {
    for (int64 C = -6; C <= 6; ++C) {
      auto check = [&](const std::vector<IntegerRange64>& ranges, auto pred) {
        for (int64 x = -40; x <= 40; ++x) {
          bool in = false;
          for (const auto& r : ranges) in = in || (r.x1 <= x && x <= r.x2);
          assert(in == pred(B * x + C));
        }
      };
      check(SolveInequatilityGE1(B, C), [](int64 v) { return v >= 0; });
      check(SolveInequatilityG1(B, C), [](int64 v) { return v > 0; });
      check(SolveInequatilityLE1(B, C), [](int64 v) { return v <= 0; });
      check(SolveInequatilityL1(B, C), [](int64 v) { return v < 0; });
    }
    assert(std::size(SolveInequatilityGE0(B)) == (B >= 0));
    assert(std::size(SolveInequatilityG0(B)) == (B > 0));
    assert(std::size(SolveInequatilityLE0(B)) == (B <= 0));
    assert(std::size(SolveInequatilityL0(B)) == (B < 0));
  }
}

PE_REGISTER_TEST(&SolveInequalityAllCasesTest, "SolveInequalityAllCasesTest",
                 SMALL);

SL void IntegerRangeTest() {
  using R = IntegerRange64;
  const int64 inf = R::inf;
  assert(R(3, 7).Count() == 5);
  assert(R(-inf, 7).Count() == -1 && R(3, inf).Count() == -1);
  assert(CountIntegersInRanges<int64>({{1, 3}, {10, 10}}) == 4);
  assert(CountIntegersInRanges<int64>({{1, 3}, {10, inf}}) == -1);
  assert(R(1, 2) < R(1, 3) && R(1, 3) < R(2, 0) && R(2, 0) > R(1, 3));
  assert(R(1, 2) <= R(1, 2) && R(1, 2) >= R(1, 2) && R(1, 2) != R(1, 3));
  {
    std::stringstream ss;
    ss << R(-1, 5);
    assert(ss.str() == "[-1, 5]");
  }

  // AppendRange merges overlapping and adjacent ranges.
  std::vector<R> v;
  AppendRange(v, R(1, 3));
  AppendRange<int64>(v, 4, 6);
  AppendRange(v, R(5, 5));
  AppendRange<int64>(v, 8, 9);
  assert((v == std::vector<R>{{1, 6}, {8, 9}}));

  // IntersectRanges / UniteRanges against sets of small integers.
  auto to_set = [](const std::vector<R>& rs) {
    std::set<int64> s;
    for (const R& r : rs)
      for (int64 x = r.x1; x <= r.x2; ++x) s.insert(x);
    return s;
  };
  // Disjoint, sorted and non-adjacent lists built from bit masks.
  auto from_mask = [](int mask) {
    std::vector<R> rs;
    for (int i = 0; i < 12; ++i) {
      if (mask >> i & 1) AppendRange<int64>(rs, i, i);
    }
    return rs;
  };
  for (int a = 0; a < (1 << 12); a += 37) {
    for (int b = 0; b < (1 << 12); b += 41) {
      const std::vector<R> ra = from_mask(a), rb = from_mask(b);
      assert(to_set(IntersectRanges(ra, rb)) == to_set(from_mask(a & b)));
      assert(IntersectRanges(ra, rb) == from_mask(a & b));
      assert(UniteRanges(ra, rb) == from_mask(a | b));
    }
  }
  R single(2, 5);
  assert(IntersectRanges<int64>({{0, 3}, {5, 9}}, single) ==
         (std::vector<R>{{2, 3}, {5, 5}}));
  assert(UniteRanges<int64>({{0, 1}, {7, 9}}, single) ==
         (std::vector<R>{{0, 5}, {7, 9}}));
  assert(std::empty(IntersectRanges<int64>({}, {{1, 2}})));
  assert(UniteRanges<int64>({}, {{1, 2}}) == (std::vector<R>{{1, 2}}));
}

PE_REGISTER_TEST(&IntegerRangeTest, "IntegerRangeTest", SMALL);

SL void LinearSystemTest() {
  // GF(2) rank against a brute force elimination on bit masks (70 columns
  // span two words).
  for (int seed = 1; seed <= 30; ++seed) {
    const int r = 5 + seed % 4, c = 70;
    GaussianEliminationMod2 g;
    g.Init(r, c);
    std::vector<std::bitset<70>> rows(r);
    uint64 s = seed * 1234567;
    for (int i = 0; i < r; ++i) {
      for (int j = 0; j < c; ++j) {
        s = s * 6364136223846793005ULL + 1;
        const int bit = (s >> 40) % (seed % 3 == 0 ? 7 : 2) == 0;
        g.Set(i, j, bit);
        rows[i][j] = bit;
      }
    }
    // Make the last row dependent: row[r - 1] ^= row[r - 1] ^ row0 ^ row1,
    // using Change for the bits to flip.
    if (seed % 2 == 0) {
      for (int j = 0; j < c; ++j) {
        if (g.At(r - 1, j) != (g.At(0, j) ^ g.At(1, j))) g.Change(r - 1, j);
      }
      rows[r - 1] = rows[0] ^ rows[1];
    }
    int rank = 0;
    for (int col = 0; col < c && rank < r; ++col) {
      int p = rank;
      while (p < r && !rows[p][col]) ++p;
      if (p == r) continue;
      std::swap(rows[p], rows[rank]);
      for (int i = rank + 1; i < r; ++i)
        if (rows[i][col]) rows[i] ^= rows[rank];
      ++rank;
    }
    assert(g.Reduce() == rank && g.Rank() == rank);
    g.FillZero();
    assert(g.Reduce() == 0);
  }

  {
    // Rank of a singular system (with a larger eps, see the TODO(bug)).
    GaussianEliminationSolver solver;
    solver.Init(3, 3);
    solver.SetEps(1e-9);
    solver.Fill(0);
    const double m[3][3] = {{1, 2, 3}, {2, 4, 6}, {1, 0, 1}};
    for (int i = 0; i < 3; ++i)
      for (int j = 0; j < 3; ++j) solver.At(i, j) = m[i][j];
    assert(solver.reduce() == 2);
    // Without pivoting a zero leading entry is swapped with a lower row.
    solver.Init(2, 2);
    solver.FillZero();
    solver.RowData(0) = {0, 1, 2};
    solver.RowData(1) = {3, 0, 3};
    assert(solver.reduce(0) == 2);
    solver.RowData(0) = {0, 1, 2};
    solver.RowData(1) = {3, 0, 3};
    const auto& x = solver.Solve();
    assert(FAbs(x[0] - 1) < 1e-12 && FAbs(x[1] - 2) < 1e-12);
  }

  // Iterative solvers on a small sparse system; x = {1, 2, ..., n}.
  const int n = 6;
  auto stop = [](const std::vector<double>&, const std::vector<double>& r) {
    return VectorDotProduct(r, r) < 1e-24 ? 1 : 0;
  };
  auto residual_ok = [&](const SparseMatrixAdjVec<double>& a,
                         const std::vector<double>& x,
                         const std::vector<double>& b) {
    const std::vector<double> ax = a.MultiplyVector(x);
    for (int i = 0; i < n; ++i)
      if (FAbs(ax[i] - b[i]) > 1e-8) return false;
    return true;
  };
  std::vector<double> expected(n);
  for (int i = 0; i < n; ++i) expected[i] = i + 1;
  {
    // Symmetric positive definite: tridiagonal (-1, 4, -1).
    SparseMatrixAdjVec<double> a(n);
    for (int i = 0; i < n; ++i) {
      a(i, i) = 4;
      if (i > 0) a(i, i - 1) = -1;
      if (i + 1 < n) a(i, i + 1) = -1;
    }
    const std::vector<double> b = a.MultiplyVector(expected);
    assert(residual_ok(a, CGSolve<double>(a, b, std::vector<double>(n, 0), stop), b));
    assert(residual_ok(
        a, BiCGSTABSolve<double>(a, b, std::vector<double>(n, 0), stop), b));
  }
  {
    // Asymmetric, diagonally dominant.
    SparseMatrixAdjVec<double> a(n);
    for (int i = 0; i < n; ++i) {
      a(i, i) = 5 + i;
      if (i > 0) a(i, i - 1) = 2;
      if (i + 2 < n) a(i, i + 2) = -1;
    }
    const std::vector<double> b = a.MultiplyVector(expected);
    assert(residual_ok(
        a, CGSolveAsymmetry<double>(a, b, std::vector<double>(n, 0), stop), b));
    assert(residual_ok(
        a, BiCGSTABSolve<double>(a, b, std::vector<double>(n, 0), stop), b));
  }
}

PE_REGISTER_TEST(&LinearSystemTest, "LinearSystemTest", SMALL);

SL void PartitionPermTest() {
  // Bell numbers
  const int64 bell[] = {1, 1, 2, 5, 15, 52, 203, 877, 4140};
  const int64 mod = 1000000007;
  PartitionMobius mobius(mod);
  for (int n = 1; n <= 8; ++n) {
    const std::vector<Partition> ps = Partition::GenPartitions(n);
    assert(static_cast<int64>(std::size(ps)) == bell[n]);
    int64 mu_sum = 0;
    for (const Partition& p : ps) {
      // colors is a restricted growth string.
      int maxv = -1;
      for (int c : p.colors) {
        assert(c <= maxv + 1);
        maxv = std::max(maxv, c);
      }
      assert(static_cast<int>(std::size(p.parts)) == maxv + 1);
      assert(std::accumulate(std::begin(p.parts), std::end(p.parts), 0) == n);
      assert(std::is_sorted(std::begin(p.parts), std::end(p.parts)));
      int total = 0;
      for (auto [size, cnt] : p.parts_c) total += size * cnt;
      assert(total == n);
      mu_sum = (mu_sum + mobius.Cal(p)) % mod;
    }
    // The Moebius function of the partition lattice sums to 0 over [0, 1].
    assert(mu_sum == (n == 1 ? 1 : 0));
    // The first partition is one block: mu(0, 1) = (-1)^(n-1) (n-1)!. The
    // last one is n singletons: mu(0, 0) = 1.
    assert(ps.front().colors == std::vector<int>(n, 0));
    int64 f = 1;
    for (int i = 1; i < n; ++i) f = f * i % mod;
    assert(mobius.Cal(ps.front()) == (n % 2 == 1 ? f : (mod - f) % mod));
    assert(mobius.Cal(ps.back()) == 1);
  }
  {
    auto ps = Partition::GenPartitions(3);
    assert(ps[0] < ps[1] && ps[1] > ps[0] && ps[0] == ps[0]);
  }

  PermHashA ha;
  PermHashB hb;
  int64 fac = 1;
  for (int len = 1; len <= 7; ++len) {
    fac *= len;
    std::vector<int> perm(len);
    std::iota(std::begin(perm), std::end(perm), 0);
    std::set<int64> ids_a, ids_b;
    do {
      const int64 a = ha.Encode(perm), b = hb.Encode(perm);
      assert(0 <= a && a < fac && 0 <= b && b < fac);
      ids_a.insert(a);
      ids_b.insert(b);
      assert(ha.Decode(a, len) == perm);
      assert(hb.Decode(b, len) == perm);
    } while (std::next_permutation(std::begin(perm), std::end(perm)));
    assert(static_cast<int64>(std::size(ids_a)) == fac);
    assert(static_cast<int64>(std::size(ids_b)) == fac);
  }
}

PE_REGISTER_TEST(&PartitionPermTest, "PartitionPermTest", SMALL);

#if PE_HAS_INT128
SL void DivisorSumFastTest() {
  // N == 0 is not supported (see the TODO(bug)s).
  int128 s0 = 0, s1 = 0;
  for (int64 n = 1; n <= 3000; ++n) {
    for (int64 d = 1; d * d <= n; ++d) {
      if (n % d == 0) {
        s0 += d * d == n ? 1 : 2;
        s1 += d * d == n ? d : d + n / d;
      }
    }
    assert(SumSigma0(n) == s0);
    assert(min25::sigma0_sum_fast(n) == static_cast<uint128>(s0));
    assert(min25::sigma1_sum_fast(n) == static_cast<uint128>(s1));
  }
  // sum_{d <= n} d * floor(n / d) in O(sqrt(n)) blocks
  for (int64 n : {int64(1000000007), int64(1000000000000)}) {
    uint128 ref = 0;
    for (int64 l = 1; l <= n;) {
      const int64 q = n / l, r = n / q;
      ref += static_cast<uint128>(q) * ((static_cast<uint128>(l) + r) *
                                        (r - l + 1) / 2);
      l = r + 1;
    }
    assert(min25::sigma1_sum_fast(n) == ref);
  }
}

PE_REGISTER_TEST(&DivisorSumFastTest, "DivisorSumFastTest", SMALL);
#endif

SL void SuffixStructureTest() {
  uint64 seed = 42;
  for (int len = 1; len <= 60; ++len) {
    for (int alphabet : {1, 2, 4, 26}) {
      std::string s(len, 'a');
      for (char& c : s) {
        seed = seed * 6364136223846793005ULL + 1;
        c = static_cast<char>('a' + (seed >> 33) % alphabet);
      }
      // Brute force suffix array and LCP
      std::vector<int> sa(len);
      std::iota(std::begin(sa), std::end(sa), 0);
      std::sort(std::begin(sa), std::end(sa), [&](int a, int b) {
        return s.compare(a, std::string::npos, s, b, std::string::npos) < 0;
      });
      std::vector<int> lcp(len, 0);
      for (int i = 1; i < len; ++i) {
        int l = 0;
        while (sa[i] + l < len && sa[i - 1] + l < len &&
               s[sa[i] + l] == s[sa[i - 1] + l])
          ++l;
        lcp[i] = l;
      }

      std::vector<int> SA(len), rank(len), LCP(len);
      dc3::BuildSuffixArray(s, SA.data(), rank.data(), LCP.data());
      assert(SA == sa && LCP == lcp);
      for (int i = 0; i < len; ++i) assert(rank[SA[i]] == i);
      // Without rank (a temporary one is used for the LCP) and without both.
      std::fill(std::begin(LCP), std::end(LCP), -1);
      dc3::BuildSuffixArray(s, SA.data(), nullptr, LCP.data());
      assert(SA == sa && LCP == lcp);
      dc3::BuildSuffixArray(s, SA.data(), nullptr, nullptr);
      assert(SA == sa);
      // Integer alphabet 1..K
      std::vector<int> text(len);
      for (int i = 0; i < len; ++i) text[i] = s[i] - 'a' + 1;
      dc3::BuildSuffixArray(text.data(), len, SA.data(), rank.data(),
                            LCP.data(), 26);
      assert(SA == sa && LCP == lcp);

      // Suffix automaton: the number of distinct substrings.
      std::set<std::string> subs;
      for (int i = 0; i < len; ++i)
        for (int j = 1; i + j <= len; ++j) subs.insert(s.substr(i, j));
      SAM sam(len);
      for (int round = 0; round < 2; ++round) {
        sam.Reset();
        for (char c : s) sam.Extend(c);
        int64 distinct = 0;
        for (int i = 1; i < sam.StateCount(); ++i) {
          distinct += sam[i].len - sam[sam[i].link].len;
        }
        assert(distinct == static_cast<int64>(std::size(subs)));
        assert(sam.StateCount() <= 2 * len && sam.top() == sam.StateCount());
        assert(sam.state(sam.last()).len == len);
        assert(sam.IdxOf(sam[sam.last()]) == sam.last());
      }
    }
  }
}

PE_REGISTER_TEST(&SuffixStructureTest, "SuffixStructureTest", SMALL);

SL void TableFormatterTest() {
  TableFormatter tf;
  tf.Push({"a", "bbb"}).Push({"cccc", "d"});
  tf.AppendLine() = {"ee"};
  assert(tf.AtLine(2)[0] == "ee");
  // Default: left aligned, width = the widest cell + 4.
  assert((tf.Render() == std::vector<std::string>{"a       bbb    ",
                                                   "cccc    d      ",
                                                   "ee      "}));

  TableFormatter::ColumnFormat right;
  right.align = TableFormatter::RIGHT;
  TableFormatter::ColumnFormat middle;
  middle.align = TableFormatter::MIDDLE;
  middle.width_base = 6;
  TableFormatter::ColumnFormat narrow;
  narrow.width_base = 2;  // truncates
  tf.SetDefaultFormat(0, right)
      .SetDefaultFormat(1, middle)
      .SetDefaultFormat(2, narrow)
      .SetSeparator("|");
  tf.AtLine(2).push_back("x");
  tf.AtLine(2).push_back("longer");
  assert((tf.Render() == std::vector<std::string>{"   a| bbb  ", "cccc|  d   ",
                                                   "  ee|  x   |lo"}));
  std::stringstream ss;
  tf.SetDefaultFormat(TableFormatter::NoAlign()).SetDefaultFormat(1, TableFormatter::NoAlign());
  tf.Render(ss);
  assert(ss.str() == "   a|bbb\ncccc|d\n  ee|x|lo\n");
}

PE_REGISTER_TEST(&TableFormatterTest, "TableFormatterTest", SMALL);

SL void PeriodicSequenceTest() {
  // x -> x^2 + 1 mod 1000 enters a cycle after a pre-period.
  auto f = [](int64 x) { return (x * x + 1) % 1000; };
  PeriodicSequence<int64> seq = MakePeriodicSequence1<int64>(2, f);
  assert(0 <= seq.start && seq.start < seq.end);
  int64 x = 2, sum = 0;
  for (int64 i = 0; i < 3000; ++i) {
    assert(seq[i] == x);
    assert(seq.CalSum<int64>(i) == sum);
    sum += x;
    x = f(x);
  }
  // A pure cycle (start == 0).
  PeriodicSequence<int64> cyc =
      MakePeriodicSequence1<int64>(1, [](int64 v) { return v * 3 % 7; });
  assert(cyc.start == 0 && cyc.end == 6);
  assert(cyc[13] == 3 && cyc.CalSum<int64>(12) == 2 * 21);
}

PE_REGISTER_TEST(&PeriodicSequenceTest, "PeriodicSequenceTest", SMALL);

SL void MatrixScannerTest() {
  const int64 mod = 1000003;
  // cell(i, j) = 3 cell(i - 1, j) + 5 cell(i, j - 1) + 7 i + j
  auto cell = [&](int64 up, int64 left, int64 i, int64 j) {
    return ((3 * up + 5 * left + 7 * i + j) % mod + mod) % mod;
  };
  for (auto [rows, cols, rb, cb] : std::vector<std::array<int64, 4>>{
           {1, 1, 1, 1}, {7, 5, 3, 2}, {10, 13, 4, 5}, {6, 6, 100, 100},
           {9, 4, 1, 3}}) {
    const int64 row_start = 10, column_start = -3;
    std::vector<int64> first_row(cols + 1), first_column(rows + 1);
    for (int64 j = 0; j <= cols; ++j) first_row[j] = j * 11 + 1;
    for (int64 i = 0; i <= rows; ++i) first_column[i] = i * 13 + 1;
    // Reference grid, g[i][j] is the cell (row_start + i, column_start + j).
    std::vector<std::vector<int64>> g(rows + 1, std::vector<int64>(cols + 1));
    for (int64 j = 0; j <= cols; ++j) g[0][j] = first_row[j];
    for (int64 i = 0; i <= rows; ++i) g[i][0] = first_column[i];
    for (int64 i = 1; i <= rows; ++i)
      for (int64 j = 1; j <= cols; ++j)
        g[i][j] = cell(g[i - 1][j], g[i][j - 1], row_start + i,
                       column_start + j);

    MatrixScanner scanner;
    scanner.Init(row_start, row_start + rows, column_start,
                 column_start + cols, first_row, first_column, rb, cb);
    for (;;) {
      auto tasks = scanner.GetTasks();
      if (!tasks.has_value()) break;
      for (MatrixTask* task : *tasks) {
        const int64 m = task->column_end - task->column_start + 1;
        std::vector<int64> prev(task->previous_row), cur(m);
        for (int64 i = task->row_start; i <= task->row_end; ++i) {
          for (int64 j = 0; j < m; ++j) {
            const int64 left = j == 0
                ? task->previous_column[i - task->row_start]
                : cur[j - 1];
            cur[j] = cell(prev[j], left, i, task->column_start + j);
          }
          task->current_column[i - task->row_start] = cur[m - 1];
          prev = cur;
        }
        task->current_row = prev;
      }
    }
    auto [curr, total] = scanner.GetProgress();
    assert(curr == total);
    for (int64 j = 0; j <= cols; ++j) assert(scanner.LastRow()[j] == g[rows][j]);
    for (int64 i = 0; i <= rows; ++i)
      assert(scanner.LastColumn()[i] == g[i][cols]);
  }
}

PE_REGISTER_TEST(&MatrixScannerTest, "MatrixScannerTest", SMALL);
}  // namespace misc_test
