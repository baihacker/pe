#include "pe_test.h"

namespace init_inv_test {
constexpr int64 mod = 1000000007;

SL void InitInvTest() {
  constexpr int n = 1000000;
  std::vector<int> sresult(n + 1);
  std::vector<int64> lresult(n + 1);
  InitInverse(std::data(sresult), n, mod);
  InitInverse(std::data(lresult), n, mod);
  for (int i = 1; i <= n; ++i) {
    assert((int64)i * sresult[i] % mod == 1);
    assert(sresult[i] == lresult[i]);
  }
}

PE_REGISTER_TEST(&InitInvTest, "InitInvTest", SMALL);
SL void InitInvOverloadsTest() {
  // maxn >= mod repeats the inverses with period mod (0 has no inverse).
  for (int64 m : {int64(1), int64(2), int64(13), int64(97)}) {
    const int64 n = 3 * m + 5;
    std::vector<int64> a(n + 1);
    InitInverse(std::data(a), n, m);
    std::vector<int64> b(n + 1);
    InitInverse(Span<int64>(b), m);
    std::vector<int> c(n + 1);
    InitInverse(std::begin(c), n, m);
    for (int64 i = 0; i <= n; ++i) {
      assert(a[i] == b[i] && a[i] == c[i]);
      if (m == 1 || i % m == 0) {
        assert(a[i] == 0);
      } else {
        assert(i % m * a[i] % m == 1);
      }
    }
  }

  // NModNumber element types
  using MT = NModCC64<1000003>;
  std::vector<MT> v(2001);
  InitInverse(Span<MT>(v));
  std::vector<MT> w(2001);
  InitInverse(std::begin(w), 2000);
  for (int64 i = 1; i <= 2000; ++i) {
    assert((v[i] * MT(i)).value() == 1);
    assert(w[i] == v[i]);
  }
  using M13 = NModCC64<13>;
  std::vector<M13> p(50);
  InitInverse(Span<M13>(p));
  for (int64 i = 0; i < 50; ++i) {
    assert(p[i].value() == (i % 13 == 0 ? 0 : ModInv(i % 13, int64(13))));
  }
}

PE_REGISTER_TEST(&InitInvOverloadsTest, "InitInvOverloadsTest", SMALL);
}  // namespace init_inv_test
