#include "pe_test.h"

namespace mat_mul_test {
// Correctness tests on small matrices against a naive reference. The timing
// test MatMulTest is at the end of this file.
constexpr int64 kMod = 1000003;
using Dense = std::vector<std::vector<int64>>;

SL Dense MakeDense(int r, int c, int seed, int64 lo, int64 hi) {
  Dense ret(r, std::vector<int64>(c));
  for (int i = 0; i < r; ++i) {
    for (int j = 0; j < c; ++j) {
      ret[i][j] = lo + (seed * 37 + i * 11 + j * 7 + i * j * 5) % (hi - lo + 1);
    }
  }
  return ret;
}

SL int64 ModRef(int64 x, int64 mod) { return (x % mod + mod) % mod; }

SL Dense ModRef(Dense a, int64 mod) {
  for (auto& row : a) {
    for (auto& x : row) x = ModRef(x, mod);
  }
  return a;
}

SL std::vector<int64> ModRef(std::vector<int64> v, int64 mod) {
  for (auto& x : v) x = ModRef(x, mod);
  return v;
}

// mod == 0 means no modulus.
SL Dense NaiveMul(const Dense& a, const Dense& b, int64 mod = 0) {
  Dense ret(std::size(a), std::vector<int64>(std::size(b[0])));
  for (size_t i = 0; i < std::size(a); ++i) {
    for (size_t j = 0; j < std::size(b[0]); ++j) {
      int64 s = 0;
      for (size_t k = 0; k < std::size(b); ++k) {
        s += a[i][k] * b[k][j];
        if (mod) s %= mod;
      }
      ret[i][j] = s;
    }
  }
  return ret;
}

SL std::vector<int64> NaiveMulVec(const Dense& a, const std::vector<int64>& v,
                                  int64 mod = 0) {
  std::vector<int64> ret(std::size(a));
  for (size_t i = 0; i < std::size(a); ++i) {
    int64 s = 0;
    for (size_t k = 0; k < std::size(v); ++k) {
      s += a[i][k] * v[k];
      if (mod) s %= mod;
    }
    ret[i] = s;
  }
  return ret;
}

SL Dense NaivePow(const Dense& a, int n, int64 mod = 0) {
  const int d = static_cast<int>(std::size(a));
  Dense ret(d, std::vector<int64>(d));
  for (int i = 0; i < d; ++i) ret[i][i] = 1;
  for (int i = 0; i < n; ++i) ret = NaiveMul(a, ret, mod);
  return ret;
}

SL Dense Transpose(const Dense& a) {
  Dense ret(std::size(a[0]), std::vector<int64>(std::size(a)));
  for (size_t i = 0; i < std::size(a); ++i) {
    for (size_t j = 0; j < std::size(a[0]); ++j) ret[j][i] = a[i][j];
  }
  return ret;
}

template <typename T>
SL int64 AsInt64(const T& x) {
  if constexpr (IsNModNumberV<T>) {
    return static_cast<int64>(ExtractValue(x));
  } else if constexpr (is_builtin_integer_v<T>) {
    return static_cast<int64>(x);
  } else {
    return x.template ToInt<int64>();
  }
}

template <typename T, int C = -1>
SL PeMatrix<T, C> ToPe(const Dense& a) {
  PeMatrix<T, C> ret(static_cast<int>(std::size(a)),
                     static_cast<int>(std::size(a[0])));
  for (int i = 0; i < ret.row(); ++i) {
    for (int j = 0; j < ret.col(); ++j) ret(i, j) = T(a[i][j]);
  }
  return ret;
}

template <typename T, int C>
SL Dense FromPe(const PeMatrix<T, C>& m) {
  Dense ret(m.row(), std::vector<int64>(m.col()));
  for (int i = 0; i < m.row(); ++i) {
    for (int j = 0; j < m.col(); ++j) ret[i][j] = AsInt64(m(i, j));
  }
  return ret;
}

template <typename T>
SL std::vector<T> ToVec(const std::vector<int64>& v) {
  return std::vector<T>(std::begin(v), std::end(v));
}

template <typename T>
SL std::vector<int64> FromVec(const std::vector<T>& v) {
  std::vector<int64> ret;
  for (const T& x : v) ret.push_back(AsInt64(x));
  return ret;
}

// Fibonacci numbers modulo kMod.
SL int64 FibRef(int64 n) {
  int64 a = 0, b = 1;
  for (int64 i = 0; i < n; ++i) {
    const int64 c = (a + b) % kMod;
    a = b;
    b = c;
  }
  return a;
}

SL void RawArrayTest() {
  constexpr int D = 4;
  const Dense A = MakeDense(D, D, 1, -9, 9);
  const Dense B = MakeDense(D, D, 2, -9, 9);
  const Dense AM = MakeDense(D, D, 3, 0, kMod - 1);
  const Dense BM = MakeDense(D, D, 4, 0, kMod - 1);
  // n < D multiplies the top-left block; the 2D-array overloads keep the row
  // stride D.
  for (int n : {D, 3}) {
    auto block = [n](const Dense& x) {
      Dense ret(n, std::vector<int64>(n));
      for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) ret[i][j] = x[i][j];
      return ret;
    };
    for (int64 mod : {int64(0), kMod}) {
      const Dense a = block(mod ? AM : A);
      const Dense b = block(mod ? BM : B);
      const Dense expected = NaiveMul(a, b, mod);
      std::vector<int64> v(n);
      for (int i = 0; i < n; ++i) v[i] = b[i][0];
      const std::vector<int64> expected_v = NaiveMulVec(a, v, mod);

      int64 a2[D][D], b2[D][D], c2[D][D];
      std::vector<int64> a1(n * n), b1(n * n), c1(n * n);
      for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
          a2[i][j] = a1[i * n + j] = a[i][j];
          b2[i][j] = b1[i * n + j] = b[i][j];
        }
      }
      std::vector<int64> out(n);

      mod ? MatMulMatMod(a2, b2, c2, mod, n) : MatMulMat(a2, b2, c2, n);
      for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) assert(c2[i][j] == expected[i][j]);

      mod ? MatMulMatMod(a1.data(), b1.data(), c1.data(), mod, n)
          : MatMulMat(a1.data(), b1.data(), c1.data(), n);
      for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) assert(c1[i * n + j] == expected[i][j]);

      // Column form: the vector is column 0 of b2.
      mod ? MatMulVecMod(a2, b2, c2, mod, n) : MatMulVec(a2, b2, c2, n);
      for (int i = 0; i < n; ++i) assert(c2[i][0] == expected_v[i]);

      mod ? MatMulVecMod(a2, v.data(), out.data(), mod, n)
          : MatMulVec(a2, v.data(), out.data(), n);
      assert(out == expected_v);

      mod ? MatMulVecMod(a1.data(), v.data(), out.data(), mod, n)
          : MatMulVec(a1.data(), v.data(), out.data(), n);
      assert(out == expected_v);
    }
  }
}

SL void PeMatrixTest() {
  {
    PeMatrix<int64> m(2, 3, 7);
    assert(m.row() == 2 && m.col() == 3);
    assert(FromPe(m) == Dense(2, std::vector<int64>(3, 7)));
    m.FillValue(5);
    assert(FromPe(m) == Dense(2, std::vector<int64>(3, 5)));
    PeMatrix<int64> n(2, 3, matrix_no_init);
    assert(n.row() == 2 && n.col() == 3);
  }
  {
    // A view on external data writes through.
    int64 buf[6] = {1, 2, 3, 4, 5, 6};
    PeMatrix<int64> v(buf, 2, 3);
    assert(v(1, 0) == 4 && v.at(0, 2) == 3);
    v(1, 2) = 9;
    assert(buf[5] == 9);
    PeMatrix<int64, 3> fixed_view(buf, 2, 3);
    assert(fixed_view(1, 2) == 9);

    // A copy owns its data.
    PeMatrix<int64> copy(v);
    copy(0, 0) = 100;
    assert(buf[0] == 1 && v(0, 0) == 1);

    // A 2D array keeps its row stride 4 even when only 3 columns are used.
    int64 arr[2][4] = {{1, 2, 3, 0}, {4, 5, 6, 0}};
    PeMatrix<int64> from_array(arr, 2, 3);
    assert(from_array.col() == 3 && from_array(1, 1) == 5);
    PeMatrix<int64, 4> fixed_array(arr, 2, 4);
    assert(fixed_array(1, 2) == 6);
  }
  {
    const Dense d = MakeDense(3, 4, 5, -9, 9);
    PeMatrix<int64> dyn = ToPe<int64>(d);
    PeMatrix<int64, 4> fixed = ToPe<int64, 4>(d);
    assert(FromPe(fixed) == d);

    // Moves, also across fixed and dynamic column counts. (Copying an lvalue
    // across column counts does not compile, see the TODO(bug) in pe_mat.)
    PeMatrix<int64> copy(dyn);
    PeMatrix<int64> moved(std::move(copy));
    assert(FromPe(moved) == d);
    PeMatrix<int64> moved2(ToPe<int64, 4>(d));
    assert(FromPe(moved2) == d);
    PeMatrix<int64, 4> moved3(ToPe<int64>(d));
    assert(FromPe(moved3) == d);

    // Assignments, including self assignment and across column counts.
    PeMatrix<int64> x(1, 1);
    x = dyn;
    assert(FromPe(x) == d);
    PeMatrix<int64>& x_ref = x;  // Avoids -Wself-assign-overloaded.
    x = x_ref;
    assert(FromPe(x) == d);
    PeMatrix<int64, 4> y(1, 4);
    y = fixed;
    assert(FromPe(y) == d);
    PeMatrix<int64> z(1, 1);
    z = ToPe<int64>(d);
    assert(FromPe(z) == d);
    z = ToPe<int64, 4>(MakeDense(3, 4, 6, 0, 3));
    assert(FromPe(z) == MakeDense(3, 4, 6, 0, 3));
  }
  {
    std::stringstream ss;
    ss << ToPe<int64>({{1, 2}, {3, -4}});
    assert(ss.str() == "{{1, 2}\n{3, -4}}");
  }
}

template <typename T>
SL void TestMatrixMulNMod() {
  const Dense a = MakeDense(2, 3, 7, 0, kMod - 1);
  const Dense b = MakeDense(3, 4, 8, 0, kMod - 1);
  const std::vector<int64> v = MakeDense(1, 3, 9, 0, kMod - 1)[0];
  assert(FromPe(MatrixMul(ToPe<T>(a), ToPe<T>(b))) == NaiveMul(a, b, kMod));
  assert(FromVec(MatrixMulVec(ToPe<T>(a), ToVec<T>(v))) ==
         NaiveMulVec(a, v, kMod));
}

SL void MatrixMulTest() {
  {
    // No modulus, negative entries, non-square, fixed and dynamic columns.
    const Dense a = MakeDense(2, 3, 1, -9, 9);
    const Dense b = MakeDense(3, 4, 2, -9, 9);
    const std::vector<int64> v = {3, -1, 2};
    assert(FromPe(MatrixMul(ToPe<int64>(a), ToPe<int64>(b))) == NaiveMul(a, b));
    assert(FromPe(MatrixMul(ToPe<int64, 3>(a), ToPe<int64, 4>(b))) ==
           NaiveMul(a, b));
    assert(MatrixMulVec(ToPe<int64>(a), v) == NaiveMulVec(a, v));
    assert(MatrixMulVec(ToPe<int64, 3>(a), v) == NaiveMulVec(a, v));
  }
  {
    // Runtime modulus: builtin and non-builtin element types.
    const Dense a = MakeDense(2, 3, 3, 0, kMod - 1);
    const Dense b = MakeDense(3, 4, 4, 0, kMod - 1);
    const std::vector<int64> v = MakeDense(1, 3, 5, 0, kMod - 1)[0];
    const Dense expected = NaiveMul(a, b, kMod);
    const std::vector<int64> expected_v = NaiveMulVec(a, v, kMod);
    assert(FromPe(MatrixMul(ToPe<int64>(a), ToPe<int64>(b), kMod)) == expected);
    assert(MatrixMulVec(ToPe<int64>(a), v, kMod) == expected_v);
    assert(FromPe(MatrixMul(ToPe<int128e>(a), ToPe<int128e>(b),
                            int128e(kMod))) == expected);
    assert(FromVec(MatrixMulVec(ToPe<int128e>(a), ToVec<int128e>(v),
                                int128e(kMod))) == expected_v);
  }
  // NModNumber: compile-time modulus, thread-local modulus (not CC/Global),
  // NModNumberM, and the lazy APSBL policy (ModValueFixer).
  TestMatrixMulNMod<NModCC64<kMod>>();
  TLMod64::Set(kMod);
  TestMatrixMulNMod<NModTL64<>>();
  TestMatrixMulNMod<NModNumberM<CCMod64<kMod>, APSB<int64, fake_int128>>>();
#if PE_HAS_INT128
  TestMatrixMulNMod<NModNumber<CCMod64<kMod>, APSBL<int128>>>();
#endif
}

SL void MatrixPowerPeTest() {
  const Dense m = MakeDense(3, 3, 5, -2, 2);
  const Dense mr = ModRef(m, kMod);
  const std::vector<int64> v = {1, -2, 3};
  const std::vector<int64> vr = ModRef(v, kMod);
  using CC = NModCC64<kMod>;

  const std::function<void(PeMatrix<int64>&)> init = [&](PeMatrix<int64>& a) {
    for (int i = 0; i < 3; ++i)
      for (int j = 0; j < 3; ++j) a(i, j) = m[i][j];
  };
  const std::function<void(PeMatrix<int64>&, std::vector<int64>&)> init_v =
      [&](PeMatrix<int64>& a, std::vector<int64>& u) {
        init(a);
        u = v;
      };

  for (int n = 0; n <= 10; ++n) {
    const Dense p = NaivePow(m, n);
    const Dense pm = NaivePow(mr, n, kMod);
    const std::vector<int64> pv = NaiveMulVec(p, v);
    const std::vector<int64> pmv = NaiveMulVec(pm, vr, kMod);

    assert(FromPe(MatrixPowerPe(ToPe<int64>(m), n)) == p);
    assert(FromPe(MatrixPowerPe(ToPe<int64, 3>(m), n)) == p);
    // Negative entries are reduced first.
    assert(FromPe(MatrixPowerPe(ToPe<int64>(m), n, kMod)) == pm);
    assert(MatrixPowerPe(ToPe<int64>(m), n, v) == pv);
    assert(MatrixPowerPe(ToPe<int64>(m), n, v, kMod) == pmv);

    assert(FromPe(MatrixPowerPe(ToPe<CC>(mr), n)) == pm);
    assert(FromVec(MatrixPowerPe(ToPe<CC>(mr), n, ToVec<CC>(vr))) == pmv);

    assert(FromPe(MatrixPowerPe<int64>(3, init, n)) == p);
    assert(FromPe(MatrixPowerPe<int64>(3, init, n, kMod)) == pm);
    assert(MatrixPowerPe<int64>(3, init_v, n) == pv);
    assert(MatrixPowerPe<int64>(3, init_v, n, kMod) == pmv);
  }

  // Convenience versions: Fibonacci numbers.
  auto fib = [](auto& a) {
    a(0, 0) = 1;
    a(0, 1) = 1;
    a(1, 0) = 1;
  };
  auto fib_v = [](auto& a, auto& u) {
    a(0, 0) = 1;
    a(0, 1) = 1;
    a(1, 0) = 1;
    u[0] = 1;
  };
  for (int64 n : {0, 1, 2, 10, 100000}) {
    const int64 f = FibRef(n);
    const int64 f1 = FibRef(n + 1);
    assert(MatrixPowerPe<kMod>(2, fib, n)(0, 1) == f);
    assert(MatrixPowerPe<kMod>(2, fib_v, n) == std::vector<int64>({f1, f}));
    assert(MatrixPowerPe(2, fib, n, kMod)(0, 1) == f);
    assert(MatrixPowerPe(2, fib_v, n, kMod) == std::vector<int64>({f1, f}));
  }
}

// MatrixPower forwards to MatrixPowerEigen or MatrixPowerPe.
SL void MatrixPowerTest() {
  const Dense m = MakeDense(3, 3, 6, -2, 2);
  const Dense mr = ModRef(m, kMod);
  const std::vector<int64> v = {2, 0, -1};
  const std::vector<int64> vr = ModRef(v, kMod);
#if ENABLE_EIGEN
  using M = EigenMatrix<int64>;
  auto to_mat = [](const Dense& a) {
    M ret = M::Zero(std::size(a), std::size(a[0]));
    for (size_t i = 0; i < std::size(a); ++i)
      for (size_t j = 0; j < std::size(a[0]); ++j) ret(i, j) = a[i][j];
    return ret;
  };
  auto from_mat = [](const auto& a) {
    Dense ret(a.rows(), std::vector<int64>(a.cols()));
    for (int i = 0; i < a.rows(); ++i)
      for (int j = 0; j < a.cols(); ++j) ret[i][j] = AsInt64(a(i, j));
    return ret;
  };
#else
  using M = PeMatrix<int64>;
  auto to_mat = [](const Dense& a) { return ToPe<int64>(a); };
  auto from_mat = [](const auto& a) { return FromPe(a); };
#endif
  const std::function<void(M&)> init = [&](M& a) { a = to_mat(m); };
  const std::function<void(M&, std::vector<int64>&)> init_v =
      [&](M& a, std::vector<int64>& u) {
        a = to_mat(m);
        u = v;
      };
  for (int n = 0; n <= 8; ++n) {
    const Dense p = NaivePow(m, n);
    const Dense pm = NaivePow(mr, n, kMod);
    assert(from_mat(MatrixPower(to_mat(m), n)) == p);
    assert(from_mat(MatrixPower(to_mat(m), n, kMod)) == pm);
    assert(MatrixPower(to_mat(m), n, v) == NaiveMulVec(p, v));
    assert(MatrixPower(to_mat(m), n, v, kMod) == NaiveMulVec(pm, vr, kMod));
    assert(from_mat(MatrixPower<int64>(3, init, n)) == p);
    assert(from_mat(MatrixPower<int64>(3, init, n, kMod)) == pm);
    assert(MatrixPower<int64>(3, init_v, n) == NaiveMulVec(p, v));
    assert(MatrixPower<int64>(3, init_v, n, kMod) == NaiveMulVec(pm, vr, kMod));
  }

  auto fib = [](auto& a) {
    a(0, 0) = 1;
    a(0, 1) = 1;
    a(1, 0) = 1;
  };
  auto fib_v = [](auto& a, auto& u) {
    a(0, 0) = 1;
    a(0, 1) = 1;
    a(1, 0) = 1;
    u[0] = 1;
  };
  for (int64 n : {0, 1, 10, 100000}) {
    const int64 f = FibRef(n);
    const int64 f1 = FibRef(n + 1);
    assert(MatrixPower<kMod>(2, fib, n)(0, 1) == f);
    assert(MatrixPower<kMod>(2, fib_v, n) == std::vector<int64>({f1, f}));
    assert(MatrixPower(2, fib, n, kMod)(0, 1) == f);
    assert(MatrixPower(2, fib_v, n, kMod) == std::vector<int64>({f1, f}));
  }
}

#if ENABLE_EIGEN
template <typename T>
SL void TestMatrixPowerEigenNMod() {
  const Dense m = MakeDense(3, 3, 7, 0, kMod - 1);
  const std::vector<int64> v = {5, 6, 7};
  EigenMatrix<T> e = EigenMatrix<T>::Zero(3, 3);
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j) e(i, j) = m[i][j];
  for (int n = 0; n <= 6; ++n) {
    const Dense p = NaivePow(m, n, kMod);
    const EigenMatrix<T> r = MatrixPowerEigen(e, n);
    for (int i = 0; i < 3; ++i)
      for (int j = 0; j < 3; ++j) assert(AsInt64(r(i, j)) == p[i][j]);
    assert(FromVec(MatrixPowerEigen(e, n, ToVec<T>(v))) ==
           NaiveMulVec(p, v, kMod));
  }
}

SL void MatrixPowerEigenTest() {
  const int threads = GetEigenNbThreads();
  SetEigenNbThreads(1);
  assert(GetEigenNbThreads() == 1);

  TestMatrixPowerEigenNMod<NModCC64<kMod>>();
  // A modulus that is not CC/Global requires a single Eigen thread.
  TLMod64::Set(kMod);
  TestMatrixPowerEigenNMod<NModTL64<>>();
  TestMatrixPowerEigenNMod<NModNumberM<CCMod64<kMod>, APSB<int64, fake_int128>>>();
#if PE_HAS_INT128
  // APSBL uses NModEigenMatrixModFixer.
  TestMatrixPowerEigenNMod<NModNumber<CCMod64<kMod>, APSBL<int128>>>();
  TestMatrixPowerEigenNMod<NModNumberM<CCMod64<kMod>, APSBL<int128>>>();
#endif

  SetEigenNbThreads(threads);
}
#endif

SL void SparseMatrixTest() {
  const Dense d = {{1, 0, 2, 0}, {0, 0, 0, 0}, {0, -3, 0, 4}};
  const std::vector<int64> v = {1, 2, 3, 4};
  const std::vector<int64> u = {5, -6, 7};

  SparseMatrixAdjMap<int64> sm(3, 4);
  sm(0, 0) = 1;
  sm(0, 2) = 2;
  sm(2, 1) = -3;
  sm(2, 3) = 4;
  const auto& csm = sm;
  assert(csm.Row() == 3 && csm.Column() == 4);
  assert(csm(0, 2) == 2 && csm(1, 1) == 0 && csm(2, 3) == 4);
  assert(std::empty(csm.Row(1)) && std::size(csm.Row(2)) == 2);
  assert(csm.MultiplyVector(v) == NaiveMulVec(d, v));
  {
    const SparseMatrixAdjMap<int64> t = sm.Transpose();
    assert(t.Row() == 4 && t.Column() == 3);
    assert(t(2, 0) == 2 && t(1, 2) == -3);
    assert(t.MultiplyVector(u) == NaiveMulVec(Transpose(d), u));
  }
  assert(SparseMatrixAdjMap<int64>(2).Column() == 2);

  SparseMatrixAdjVec<int64> sv(sm);
  const auto& csv = sv;
  assert(csv.Row() == 3 && csv.Column() == 4);
  assert(csv(0, 2) == 2 && csv(1, 1) == 0);
  assert(csv.MultiplyVector(v) == NaiveMulVec(d, v));
  {
    const SparseMatrixAdjVec<int64> t = sv.Transpose();
    assert(t.Row() == 4 && t.Column() == 3);
    assert(t(3, 2) == 4);
    assert(t.MultiplyVector(u) == NaiveMulVec(Transpose(d), u));
  }
  // operator() inserts a missing entry once and then updates it.
  sv(1, 3) = 5;
  sv(1, 3) += 1;
  assert(std::size(csv.Row(1)) == 1 && csv(1, 3) == 6);
  assert(SparseMatrixAdjVec<int64>(2).Column() == 2);
}

SL void MatrixCorrectnessTest() {
  RawArrayTest();
  PeMatrixTest();
  MatrixMulTest();
  MatrixPowerPeTest();
  MatrixPowerTest();
#if ENABLE_EIGEN
  MatrixPowerEigenTest();
#endif
  SparseMatrixTest();
}

PE_REGISTER_TEST(&MatrixCorrectnessTest, "MatrixCorrectnessTest", SMALL);

#if ENABLE_EIGEN && PE_HAS_INT128
constexpr int K = 500;
constexpr int64 mod = 1000000007;
constexpr int show = 1;

template <typename E>
void TestEigen(const std::vector<int>& data, const std::vector<int>& V) {
  std::vector<E> v(K, 0);
  EigenMatrix<E> m = EigenMatrix<E>::Zero(K, K);

  for (int i = 0; i < K; ++i) v[i] = V[i];
  for (int i = 0; i < K; ++i)
    for (int j = 0; j < K; ++j) m(i, j) = data[j * K + i];

  TimeRecorder tr;
  v = MatrixPowerEigen(m, 4, v);
  int64 s = 0;
  for (E i : v) s += i.value();
  // std::cout << s << std::endl;
  if (show) {
    std::cout << tr.Elapsed().Format() << std::endl;
  }
  assert(s == 247446585411LL);
  std::sort(std::begin(v), std::end(v));
}

template <typename E>
void TestEigen(const std::vector<int>& data, const std::vector<int>& V, E mod) {
  std::vector<E> v(K, 0);
  EigenMatrix<E> m = EigenMatrix<E>::Zero(K, K);

  for (int i = 0; i < K; ++i) v[i] = V[i] % mod;
  for (int i = 0; i < K; ++i)
    for (int j = 0; j < K; ++j) m(i, j) = data[j * K + i] % mod;

  TimeRecorder tr;
  v = MatrixPowerEigen(m, 4, v, mod);
  int64 s = 0;
  for (E i : v) s += i;
  // std::cout << s << std::endl;
  if (show) {
    std::cout << tr.Elapsed().Format() << std::endl;
  }
  assert(s == 247446585411LL);
  std::sort(std::begin(v), std::end(v));
}

template <typename E>
void TestPe(const std::vector<int>& data, const std::vector<int>& V) {
  std::vector<E> v(K, 0);
  PeMatrix<E> m(K, K);

  for (int i = 0; i < K; ++i) v[i] = V[i];
  for (int i = 0; i < K; ++i)
    for (int j = 0; j < K; ++j) m(i, j) = data[j * K + i];

  TimeRecorder tr;
  v = MatrixPowerPe(m, 4, v);
  int64 s = 0;
  for (E i : v) s += i.value();
  // std::cout << s << std::endl;
  if (show) {
    std::cout << tr.Elapsed().Format() << std::endl;
  }
  assert(s == 247446585411LL);
  std::sort(std::begin(v), std::end(v));
}

template <typename E>
void TestPe(const std::vector<int>& data, const std::vector<int>& V, E mod) {
  std::vector<E> v(K, 0);
  PeMatrix<E> m(K, K);

  for (int i = 0; i < K; ++i) v[i] = V[i] % mod;
  for (int i = 0; i < K; ++i)
    for (int j = 0; j < K; ++j) m(i, j) = data[j * K + i] % mod;

  TimeRecorder tr;
  v = MatrixPowerPe(m, 4, v, mod);
  int64 s = 0;
  for (E i : v) s += i;
  // std::cout << s << std::endl;
  if (show) {
    std::cout << tr.Elapsed().Format() << std::endl;
  }
  assert(s == 247446585411LL);
  std::sort(std::begin(v), std::end(v));
}

SL void TestEigenHelperMethod() {
  TimeRecorder tr;
  {
    auto res0 = MatrixPowerEigen<1000000007>(
        2,
        [=](auto& m) {
          m(0, 0) = 1;
          m(0, 1) = 1;
          m(1, 0) = 1;
        },
        100000);
    auto res1 = MatrixPowerEigen<1000000007>(
        2,
        [=](auto& m, auto& v) {
          m(0, 0) = 1;
          m(0, 1) = 1;
          m(1, 0) = 1;
          v[0] = 1;
          v[1] = 1;
        },
        100000);
  }

  {
    auto res0 = MatrixPowerEigen<NModCC64<1000000007>>(
        2,
        [=](auto& m) {
          m(0, 0) = 1;
          m(0, 1) = 1;
          m(1, 0) = 1;
        },
        100000);
    auto res1 = MatrixPowerEigen<NModCC64<1000000007>>(
        2,
        [=](auto& m, auto& v) {
          m(0, 0) = 1;
          m(0, 1) = 1;
          m(1, 0) = 1;
          v[0] = 1;
          v[1] = 1;
        },
        100000);
  }

  {
    auto res0 = MatrixPowerEigen(
        2,
        [=](auto& m) {
          m(0, 0) = 1;
          m(0, 1) = 1;
          m(1, 0) = 1;
        },
        100000, 1000000007);
    auto res1 = MatrixPowerEigen(
        2,
        [=](auto& m, auto& v) {
          m(0, 0) = 1;
          m(0, 1) = 1;
          m(1, 0) = 1;
          v[0] = 1;
          v[1] = 1;
        },
        100000, 1000000007);
  }

  std::cout << tr.Elapsed().Format() << std::endl;
}

SL void TestPeHelperMethod() {
  TimeRecorder tr;
  {
    auto res0 = MatrixPowerPe<1000000007>(
        2,
        [=](auto& m) {
          m(0, 0) = 1;
          m(0, 1) = 1;
          m(1, 0) = 1;
        },
        100000);
    auto res1 = MatrixPowerPe<1000000007>(
        2,
        [=](auto& m, auto& v) {
          m(0, 0) = 1;
          m(0, 1) = 1;
          m(1, 0) = 1;
          v[0] = 1;
          v[1] = 1;
        },
        100000);
  }

  {
    auto res0 = MatrixPowerPe<NModCC64<1000000007>>(
        2,
        [=](auto& m) {
          m(0, 0) = 1;
          m(0, 1) = 1;
          m(1, 0) = 1;
        },
        100000);
    auto res1 = MatrixPowerPe<NModCC64<1000000007>>(
        2,
        [=](auto& m, auto& v) {
          m(0, 0) = 1;
          m(0, 1) = 1;
          m(1, 0) = 1;
          v[0] = 1;
          v[1] = 1;
        },
        100000);
  }

  {
    auto res0 = MatrixPowerPe(
        2,
        [=](auto& m) {
          m(0, 0) = 1;
          m(0, 1) = 1;
          m(1, 0) = 1;
        },
        100000, 1000000007);
    auto res1 = MatrixPowerPe(
        2,
        [=](auto& m, auto& v) {
          m(0, 0) = 1;
          m(0, 1) = 1;
          m(1, 0) = 1;
          v[0] = 1;
          v[1] = 1;
        },
        100000, 1000000007);
  }
  std::cout << tr.Elapsed().Format() << std::endl;
}

SL void MatMulTest() {
  GlobalMod64::Set(mod);
  std::vector<int> data;
  for (int i = 0; i < K; ++i) {
    for (int j = 0; j < K; ++j) data.push_back(j * K + i);
  }
  std::vector<int> V;
  V.reserve(K);
  for (int i = 0; i < K; ++i) V.push_back(i);
  {
    using E = int64;
    std::vector<E> v(K, 0);
    EigenMatrix<E> m = EigenMatrix<E>::Zero(K, K);

    for (int i = 0; i < K; ++i) v[i] = V[i];
    for (int i = 0; i < K; ++i)
      for (int j = 0; j < K; ++j) m(i, j) = data[j * K + i];

    TimeRecorder tr;
    v = MatrixPower(m, 4, v, mod);
    int64 s = 0;
    for (E i : v) s += i;
    // std::cout << s << std::endl;
    if (show) {
      std::cout << tr.Elapsed().Format() << std::endl;
    }
    assert(s == 256670487618LL);
    std::sort(std::begin(v), std::end(v));
  }
  {
    using E = int128;
    std::vector<E> v(K, 0);
    EigenMatrix<E> m = EigenMatrix<E>::Zero(K, K);

    for (int i = 0; i < K; ++i) v[i] = V[i];
    for (int i = 0; i < K; ++i)
      for (int j = 0; j < K; ++j) m(i, j) = data[j * K + i];

    TimeRecorder tr;
    v = MatrixPower(m, 4, v, mod);
    int64 s = 0;
    for (E i : v) s += i;
    // std::cout << s << std::endl;
    if (show) {
      std::cout << tr.Elapsed().Format() << std::endl;
    }
    assert(s == 247446585411LL);
    std::sort(std::begin(v), std::end(v));
  }
  if (show) {
    std::cout << std::endl;
  }

  TimeRecorder tr;
  TestEigen<int128>(data, V, mod);

  if (show) {
    std::cout << std::endl;
  }

  TestEigen<NModNumber<CCMod64<mod>, APSB<int64, int64>>>(data, V);
  TestEigen<NModNumber<CCMod64<mod>, APSB<int64, int128>>>(data, V);
  TestEigen<NModNumber<CCMod64<mod>, APSB<int64, fake_int128>>>(data, V);
  TestEigen<NModNumber<CCMod64<mod>, APSB<int128, int128>>>(data, V);
  TestEigen<NModNumber<CCMod64<mod>, APSBL<int128>>>(data, V);

  TestEigen<NModNumber<GlobalMod64, APSB<int64, int64>>>(data, V);
  TestEigen<NModNumber<GlobalMod64, APSB<int64, int128>>>(data, V);
  TestEigen<NModNumber<GlobalMod64, APSB<int64, fake_int128>>>(data, V);
  TestEigen<NModNumber<GlobalMod64, APSB<int128, int128>>>(data, V);
  TestEigen<NModNumber<GlobalMod64, APSBL<int128>>>(data, V);

  if (show) {
    std::cout << std::endl;
  }

  TestEigen<NModNumberM<CCMod64<mod>, APSB<int64, int64>>>(data, V);
  TestEigen<NModNumberM<CCMod64<mod>, APSB<int64, int128>>>(data, V);
  TestEigen<NModNumberM<CCMod64<mod>, APSB<int64, fake_int128>>>(data, V);
  TestEigen<NModNumberM<CCMod64<mod>, APSB<int128, int128>>>(data, V);
  TestEigen<NModNumberM<CCMod64<mod>, APSBL<int128>>>(data, V);

  TestEigen<NModNumberM<GlobalMod64, APSB<int64, int64>>>(data, V);
  TestEigen<NModNumberM<GlobalMod64, APSB<int64, int128>>>(data, V);
  TestEigen<NModNumberM<GlobalMod64, APSB<int64, fake_int128>>>(data, V);
  TestEigen<NModNumberM<GlobalMod64, APSB<int128, int128>>>(data, V);
  TestEigen<NModNumberM<GlobalMod64, APSBL<int128>>>(data, V);

  if (show) {
    std::cout << std::endl;
  }
  TestEigenHelperMethod();
  if (show) {
    std::cout << std::endl;
  }
  std::cout << "Eigen " << tr.Elapsed().Format() << std::endl;
  if (show) {
    std::cout << std::endl;
  }

  tr.Record();
  TestPe<int128>(data, V, mod);

  if (show) {
    std::cout << std::endl;
  }

  TestPe<NModNumber<CCMod64<mod>, APSB<int64, int64>>>(data, V);
  TestPe<NModNumber<CCMod64<mod>, APSB<int64, int128>>>(data, V);
  TestPe<NModNumber<CCMod64<mod>, APSB<int64, fake_int128>>>(data, V);
  TestPe<NModNumber<CCMod64<mod>, APSB<int128, int128>>>(data, V);
  TestPe<NModNumber<CCMod64<mod>, APSBL<int128>>>(data, V);

  TestPe<NModNumber<GlobalMod64, APSB<int64, int64>>>(data, V);
  TestPe<NModNumber<GlobalMod64, APSB<int64, int128>>>(data, V);
  TestPe<NModNumber<GlobalMod64, APSB<int64, fake_int128>>>(data, V);
  TestPe<NModNumber<GlobalMod64, APSB<int128, int128>>>(data, V);
  TestPe<NModNumber<GlobalMod64, APSBL<int128>>>(data, V);

  if (show) {
    std::cout << std::endl;
  }
  TestPe<NModNumberM<CCMod64<mod>, APSB<int64, int64>>>(data, V);
  TestPe<NModNumberM<CCMod64<mod>, APSB<int64, int128>>>(data, V);
  TestPe<NModNumberM<CCMod64<mod>, APSB<int64, fake_int128>>>(data, V);
  TestPe<NModNumberM<CCMod64<mod>, APSB<int128, int128>>>(data, V);
  TestPe<NModNumberM<CCMod64<mod>, APSBL<int128>>>(data, V);

  TestPe<NModNumberM<GlobalMod64, APSB<int64, int64>>>(data, V);
  TestPe<NModNumberM<GlobalMod64, APSB<int64, int128>>>(data, V);
  TestPe<NModNumberM<GlobalMod64, APSB<int64, fake_int128>>>(data, V);
  TestPe<NModNumberM<GlobalMod64, APSB<int128, int128>>>(data, V);
  TestPe<NModNumberM<GlobalMod64, APSBL<int128>>>(data, V);

  if (show) {
    std::cout << std::endl;
  }
  TestPeHelperMethod();
  std::cout << "Pe " << tr.Elapsed().Format() << std::endl;
}

PE_REGISTER_TEST(&MatMulTest, "MatMulTest", SUPER);
#endif
}  // namespace mat_mul_test
