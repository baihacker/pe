#ifndef PE_POLY_
#define PE_POLY_

#include "pe_base"
#include "pe_type_traits"
#include "pe_mod"
#include "pe_nt"
#include "pe_poly_base"

namespace pe {
struct NModPoly {
  int64 mod;
  std::vector<int64> data;

  NModPoly(int64 mod = 1) : mod(mod) {}

  NModPoly(const std::vector<int64>& data, int64 mod,
           int adjust_leading_zero = 1)
      : data(data), mod(mod) {
    AdjustMod();
    if (adjust_leading_zero) {
      AdjustLeadingZeros();
    }
  }

  NModPoly(std::vector<int64>&& data, int64 mod, int adjust_leading_zero = 1)
      : data(std::move(data)), mod(mod) {
    AdjustMod();
    if (adjust_leading_zero) {
      AdjustLeadingZeros();
    }
  }

  NModPoly& operator=(const std::vector<int64>& v) {
    data = v;
    return *this;
  }

  NModPoly& operator=(const std::vector<int64>&& v) {
    data = v;
    return *this;
  }

  NModPoly(const NModPoly& p) = default;
  NModPoly(NModPoly&& p) = default;
  NModPoly& operator=(const NModPoly& other) = default;
  NModPoly& operator=(NModPoly&& other) = default;

  NModPoly& AdjustLeadingZeros() {
    AdjustPolyLeadingZero(data);
    return *this;
  }

  NModPoly& AdjustMod() {
    for (auto& iter : data) {
      iter = Mod(iter, mod);
    }
    return *this;
  }

  int64 deg() const { return static_cast<int64>(std::size(data) - 1); }
  int64 size() const { return static_cast<int64>(std::size(data)); }

  NModPoly& Resize(int64 n) {
    const int64 m = static_cast<int64>(std::size(data));
    data.resize(n);
    for (int64 i = m; i < n; ++i) {
      data[i] = 0;
    }
    return *this;
  }

  NModPoly& Redeg(int64 n) { return Resize(n + 1); }

  int64& operator[](int64 idx) {
    PE_ASSERT(idx >= 0 && idx < static_cast<int64>(std::size(data)));
    return data[idx];
  }

  int64 operator[](int64 idx) const {
    PE_ASSERT(idx >= 0 && idx < static_cast<int64>(std::size(data)));
    return data[idx];
  }

  int64 At(int64 idx) const {
    return idx >= 0 && idx < static_cast<int64>(std::size(data)) ? data[idx]
                                                                 : 0;
  }

  bool IsZero() const { return std::size(data) == 1 && data[0] == 0; }

  int64 ValueAt(int64 v) const {
    int64 ret = 0;
    const int64 t = Mod(v, mod);
    for (int64 i = deg(); i >= 0; --i) {
      ret = AddMod(MulMod(ret, t, mod), data[i], mod);
    }
    return ret;
  }

  NModPoly LowerTerms(int64 n, int adjust_leading_zero = 1) const {
    const int64 m = std::min(n, static_cast<int64>(std::size(data)));
    return NModPoly(std::vector<int64>(data.begin(), data.begin() + m), mod,
                    adjust_leading_zero);
  }

  NModPoly Inv(int64 n) const;
};

template <int64 M>
struct NModPolyT : public NModPoly {
  NModPolyT() : NModPoly(M) {}

  NModPolyT(const std::vector<int64>& data) : NModPoly(data, M) {}

  NModPolyT(std::vector<int64>&& data) : NModPoly(std::move(data), M) {}

  NModPolyT(std::initializer_list<int64> l)
      : NModPolyT(std::vector<int64>(l)) {}
};

SL NModPoly PolyMul(const NModPoly& X, const NModPoly& Y) {
  return NModPoly{PolyMul(X.data, Y.data, X.mod), X.mod};
}

SL NModPoly PolyInv(const NModPoly& x, int64 n) {
  return NModPoly(PolyInv(x.data, n, x.mod), x.mod);
}

#define PPOLY_DIV_AND_MOD_IMPL(PolyDivAndMod, PolyDivAndModImpl)       \
  SL std::tuple<NModPoly, NModPoly> PolyDivAndMod(const NModPoly& X,   \
                                                  const NModPoly& Y) { \
    auto [q, r] = PolyDivAndModImpl(X.data, Y.data, X.mod);            \
    return std::make_tuple(NModPoly(std::move(q), X.mod),              \
                           NModPoly(std::move(r), X.mod));             \
  }

#define PPOLY_DIV_IMPL(PolyDiv, PolyDivImpl)                    \
  SL NModPoly PolyDiv(const NModPoly& X, const NModPoly& Y) {   \
    return NModPoly(PolyDivImpl(X.data, Y.data, X.mod), X.mod); \
  }

#define PPOLY_MOD_IMPL(PolyMod, PolyModImpl)                    \
  SL NModPoly PolyMod(const NModPoly& X, const NModPoly& Y) {   \
    return NModPoly(PolyModImpl(X.data, Y.data, X.mod), X.mod); \
  }

PPOLY_DIV_AND_MOD_IMPL(PolyDivAndModDc, pe::PolyDivAndModDc)
PPOLY_DIV_IMPL(PolyDivDc, pe::PolyDivDc)
PPOLY_MOD_IMPL(PolyModDc, pe::PolyModDc)

PPOLY_DIV_AND_MOD_IMPL(PolyDivAndModNormal, pe::PolyDivAndModNormal)
PPOLY_DIV_IMPL(PolyDivNormal, pe::PolyDivNormal)
PPOLY_MOD_IMPL(PolyModNormal, pe::PolyModNormal)

PPOLY_DIV_AND_MOD_IMPL(PolyDivAndMod, pe::PolyDivAndMod)
PPOLY_DIV_IMPL(PolyDiv, pe::PolyDiv)
PPOLY_MOD_IMPL(PolyMod, pe::PolyMod)

inline NModPoly NModPoly::Inv(int64 n) const {
  auto t = PolyInv(data, n, mod);
  return NModPoly(std::vector<int64>(static_cast<int64*>(&t[0]),
                                     static_cast<int64*>(&t[0]) + std::size(t)),
                  mod);
}

NModPoly operator<<(const NModPoly& p, int64 m) {
  return NModPoly(PolyShiftLeft<int64>(p.data, m), p.mod);
}

NModPoly operator>>(const NModPoly& p, int64 m) {
  return NModPoly(PolyShiftRight<int64>(p.data, m), p.mod);
}

SL NModPoly operator-(const NModPoly& x, const NModPoly& y) {
  return NModPoly(PolySub(x.data, y.data, x.mod), x.mod).AdjustLeadingZeros();
}

SL NModPoly operator+(const NModPoly& x, const NModPoly& y) {
  return NModPoly(PolyAdd(x.data, y.data, x.mod), x.mod).AdjustLeadingZeros();
}

SL NModPoly operator*(const NModPoly& x, int64 v) {
  std::vector<int64> data(x.data);
  for (auto& iter : data) {
    iter = iter * v % x.mod;
  }
  return NModPoly(std::move(data), x.mod);
}

SL NModPoly operator*(int64 v, const NModPoly& x) { return x * v; }

SL NModPoly operator*(const NModPoly& x, const NModPoly& y) {
  return PolyMul(x, y);
}

SL NModPoly operator/(const NModPoly& x, const NModPoly& y) {
  return PolyDiv(x, y);
}

SL NModPoly operator%(const NModPoly& x, const NModPoly& y) {
  return PolyMod(x, y);
}

SL int operator==(const NModPoly& x, const NModPoly& y) {
  return x.mod == y.mod && x.data == y.data;
}

// x^n % mod
SL NModPoly operator%(int64 n, const NModPoly& mod) {
  NModPoly x{{0, 1}, mod.mod};
  NModPoly ret{{1}, mod.mod};
  for (; n > 0; n >>= 1) {
    if (n & 1) {
      ret = PolyMod(x * ret, mod);
    }
    if (n > 1) {
      x = PolyMod(x * x, mod);
    }
  }
  return ret;
}

SL std::ostream& operator<<(std::ostream& o, const NModPoly& p) {
  const int64 n = static_cast<int64>(std::size(p.data));
  for (int64 i = 0; i < n - 1; ++i) {
    o << p.data[i] << ", ";
  }
  return o << p.data[n - 1];
}
}  // namespace pe
#endif
