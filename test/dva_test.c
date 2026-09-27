#include "pe_test.h"

namespace dva_test {
SL void TestS0() {
  auto orz = PrimePi<int64>(10000);
  assert(orz[10000] == 1229LL);

  orz = PrimeS0Ex<int64>(10000);
  assert(orz[10000] == 1229LL);

  orz = PrimePi<int64>(100000000);
  assert(orz[100000000] == 5761455LL);

  orz = PrimeS0Ex<int64>(100000000);
  assert(orz[100000000] == 5761455LL);
}

SL void TestS1() {
  int64 s = 0;
  for (int i = 2; i <= 10000; ++i) {
    if (IsPrime(i)) s += i;
  }
  assert(s == 5736396LL);

  auto orz = PrimeS1<int64>(10000);
  assert(orz[10000] == 5736396LL);

  orz = PrimeS1Ex<int64>(10000);
  assert(orz[10000] == 5736396LL);

  s = 0;
  for (int i = 2; i <= 1000000; ++i) {
    if (IsPrime(i)) s += i;
  }
  assert(s == 37550402023LL);

  orz = PrimeS1<int64>(1000000);
  assert(orz[1000000] == 37550402023LL);

  orz = PrimeS1Ex<int64>(1000000);
  assert(orz[1000000] == 37550402023LL);
}

SL void DvaTest() {
  TestS0();
  TestS1();
}

PE_REGISTER_TEST(&DvaTest, "DvaTest", SMALL);
// Brute force prefix sums up to kN. The values of the functions with large
// sums are kept modulo kP.
constexpr int64 kN = 200000;
constexpr int64 kP = 1000000007;
using MT = NModCC64<kP>;

struct PrefixTables {
  // Exact
  std::vector<int64> one, id, id2, mu, phi, sigma0, sigma1, prime_nu, sqfree,
      prime_cnt, prime_sum;
  // Modulo kP: idk[k] = sum x^k, sigmak[k] = sum sigma_k(x), k = 0..7
  std::vector<std::vector<int64>> idk, sigmak;
};

SL const PrefixTables& Tables() {
  static PrefixTables t = [] {
    PrefixTables t;
    std::vector<std::vector<int64>> sig(8, std::vector<int64>(kN + 1));
    std::vector<int64> nu(kN + 1);
    for (int64 d = 1; d <= kN; ++d) {
      int64 pw = 1;
      for (int k = 0; k < 8; ++k) {
        for (int64 x = d; x <= kN; x += d) sig[k][x] = (sig[k][x] + pw) % kP;
        pw = pw * d % kP;
      }
      if (d > 1 && IsPrime(d)) {
        for (int64 x = d; x <= kN; x += d) ++nu[x];
      }
    }
    auto prefix = [](auto f) {
      std::vector<int64> ret(kN + 1);
      for (int64 x = 1; x <= kN; ++x) ret[x] = ret[x - 1] + f(x);
      return ret;
    };
    t.one = prefix([](int64) { return 1; });
    t.id = prefix([](int64 x) { return x; });
    t.id2 = prefix([](int64 x) { return x * x; });
    t.mu = prefix([](int64 x) { return int64(mu[x]); });
    t.phi = prefix([](int64 x) { return int64(phi[x]); });
    t.sigma0 = prefix([](int64 x) { return CalSigma0(x); });
    t.sigma1 = prefix([](int64 x) { return CalSigma1(x); });
    t.prime_nu = prefix([&](int64 x) { return nu[x]; });
    t.sqfree = prefix([](int64 x) { return int64(mu[x] != 0); });
    t.prime_cnt = prefix([](int64 x) { return int64(IsPrime(x)); });
    t.prime_sum = prefix([](int64 x) { return IsPrime(x) ? x : 0; });
    t.idk.assign(8, std::vector<int64>(kN + 1));
    t.sigmak.assign(8, std::vector<int64>(kN + 1));
    for (int k = 0; k < 8; ++k) {
      for (int64 x = 1; x <= kN; ++x) {
        t.idk[k][x] = (t.idk[k][x - 1] + PowerMod(x, k, kP)) % kP;
        t.sigmak[k][x] = (t.sigmak[k][x - 1] + sig[k][x]) % kP;
      }
    }
    return t;
  }();
  return t;
}

template <typename T>
SL int64 V(const T& v) {
  if constexpr (IsNModNumberV<T>) {
    return v.value();
  } else {
    return v;
  }
}

// Checks every key of d against the prefix sums (compared modulo mod if
// mod > 0).
template <typename T>
SL void Check(const DVA<T>& d, int64 n, const std::vector<int64>& prefix,
              int64 mod = 0) {
  assert(d.n == n);
  for (int64 k : d.FKeys()) {
    const int64 e = mod ? (prefix[k] % mod + mod) % mod : prefix[k];
    assert(V(d[k]) == e);
  }
}

const int64 kSizes[] = {1, 2, 3, 10, 99, 100, 1000, 12345, kN};

SL void DvaStructureTest() {
  for (int64 n : kSizes) {
    DVA<int64> d(n, 7);
    const DVAShape shape(n);
    assert(d.m == SqrtI(n) && d.is_perfect_square == (d.m * d.m == n));
    assert(static_cast<int64>(std::size(d.keys)) == d.key_size);
    assert(shape.key_size == d.key_size && shape.m == d.m);
    // The keys are 0 and the distinct values of n / i, in increasing order.
    std::set<int64> expected = {0};
    for (int64 i = 1; i * i <= n; ++i) expected.insert(i), expected.insert(n / i);
    assert(std::vector<int64>(std::begin(expected), std::end(expected)) ==
           d.keys);
    for (int i = 0; i < d.key_size; ++i) {
      assert(d.IdxOfValue(d.keys[i]) == i && shape.IdxOfValue(d.keys[i]) == i);
    }
    assert(d.values[0] == 0);
    for (int i = 1; i < d.key_size; ++i) assert(d.values[i] == 7);
    d[n] = 3;
    assert(d.values.back() == 3);
    const DVA<int64>& cd = d;
    assert(cd[n] == 3);

    // Forward and backward views skip key 0.
    std::vector<int64> fk, bk;
    for (int64 k : d.FKeys()) fk.push_back(k);
    for (int64 k : d.BKeys()) bk.push_back(k);
    assert(fk == std::vector<int64>(std::begin(d.keys) + 1, std::end(d.keys)));
    std::reverse(std::begin(bk), std::end(bk));
    assert(bk == fk);
    int idx = 1;
    for (auto item : d.FItems()) {
      assert(item.idx == idx && item.key == d.keys[idx] &&
             item.value == d.values[idx]);
      ++idx;
    }
    assert(idx == d.key_size);
    for (auto item : d.BItems()) {
      --idx;
      assert(item.idx == idx && item.key == d.keys[idx]);
    }
    assert(idx == 1);
    auto it = d.begin();
    auto it2 = it++;
    assert(it2 == d.begin() && it != d.begin() && it.idx == 2);
    --it;
    assert(it == d.begin());
    auto rit = d.rbegin();
    rit++;
    ++rit;
    rit--;
    --rit;
    assert(rit == d.rbegin());

    // Copy, move, Fill, Resize
    DVA<int64> c(d);
    DVA<int64> m(std::move(c));
    assert(m.values == d.values);
    m.Fill(5);
    assert(m.values[0] == 0 && m[n] == 5);
    m.Resize(n + 1, 9);
    assert(m.n == n + 1 && m[n + 1] == 9);
  }
}

PE_REGISTER_TEST(&DvaStructureTest, "DvaStructureTest", SMALL);

SL void DvaPrefixSumTest() {
  const PrefixTables& t = Tables();
  for (int64 n : kSizes) {
    std::vector<int64> eps(kN + 1, 1);
    eps[0] = 0;
    Check(MakePrefixSumEpsilon<int64>(n), n, eps);
    Check(MakePrefixSumOne<int64>(n), n, t.one);
    Check(MakePrefixSumId<int64>(n), n, t.id);
    Check(MakePrefixSumId2<int64>(n), n, t.id2);
    Check(MakePrefixSumId3<MT>(n), n, t.idk[3], kP);
    Check(MakePrefixSumId4<MT>(n), n, t.idk[4], kP);
    Check(MakePrefixSumId5<MT>(n), n, t.idk[5], kP);
    Check(MakePrefixSumId6<MT>(n), n, t.idk[6], kP);
    Check(MakePrefixSumId7<MT>(n), n, t.idk[7], kP);
    const DVA<int64> ps_mu = MakePrefixSumMu<int64>(n);
    Check(ps_mu, n, t.mu);
    Check(MakePrefixSumPhi<int64>(n), n, t.phi);
    Check(MakePrefixSumPhi(ps_mu), n, t.phi);
    Check(MakePrefixSumMu<MT>(n), n, t.mu, kP);
  }
}

PE_REGISTER_TEST(&DvaPrefixSumTest, "DvaPrefixSumTest", SMALL);

SL void DvaConvTest() {
  const PrefixTables& t = Tables();
  for (int64 n : kSizes) {
    const DVA<int64> one = MakePrefixSumOne<int64>(n);
    const DVA<int64> id = MakePrefixSumId<int64>(n);
    const DVA<int64> ps_mu = MakePrefixSumMu<int64>(n);

    // 1 * 1 = sigma0, id * 1 = sigma1, mu * 1 = epsilon
    const DVA<int64> s0 = DVAConv(one, one);
    Check(s0, n, t.sigma0);
    Check(one * one, n, t.sigma0);
    Check(DVAConv(id, one), n, t.sigma1);
    std::vector<int64> eps(kN + 1, 1);
    eps[0] = 0;
    Check(ps_mu * one, n, eps);
    for (int64 k : s0.FKeys()) assert(DVAConvAt(one, one, k) == s0[k]);

    // The generic convolution with the standard transform:
    // sum over y in [miny, maxy] of g(y) H(maxx / y).
    const std::function<int64(int64, int64, int64, int64, const DVA<int64>&)>
        trans = [](int64 maxx, int64, int64 maxy, int64 delta_g,
                   const DVA<int64>& ps_h) { return delta_g * ps_h[maxx / maxy]; };
    Check(DVAConv(id, one, trans), n, t.sigma1);
    for (int64 k : s0.FKeys()) {
      assert(DVAConvAt(one, one, trans, k) == s0[k]);
    }

    // sum_{d^2 | x} mu(d) = [x is square free]
    Check(DVAConvDivSquare(ps_mu, one), n, t.sqfree);

    // f = g * h with h = 1 and f = sigma0 gives g = 1.
    const DVA<int64> g = DVAConvInverse(one, s0);
    Check(g, n, t.one);
    for (int64 k : g.FKeys()) assert(DVAConvInverseAt(one, g, s0, k) == g[k]);

    Check(DVAAdd(one, id), n, PolyAdd(t.one, t.id));
    Check(one + id, n, PolyAdd(t.one, t.id));
    Check(DVASub(id, one), n, PolySub(t.id, t.one));
    Check(id - one, n, PolySub(t.id, t.one));

    // Modular element type
    const DVA<MT> one_m = MakePrefixSumOne<MT>(n);
    const DVA<MT> id3_m = MakePrefixSumId3<MT>(n);
    Check(DVAConv(id3_m, one_m), n, t.sigmak[3], kP);
    Check(DVAConvInverse(one_m, DVAConv(one_m, one_m)), n, t.one, kP);
  }
}

PE_REGISTER_TEST(&DvaConvTest, "DvaConvTest", SMALL);

SL void DvaPrefixSumMakerTest() {
  const PrefixTables& t = Tables();
  for (int64 n : {int64(1), int64(10), int64(1000), kN}) {
    DVAPrefixSumMaker<MT> maker(n);
    std::vector<int64> eps(kN + 1, 1);
    eps[0] = 0;
    Check(maker.Epsilon(), n, eps, kP);
    Check(maker.Mu(), n, t.mu, kP);
    Check(maker.Phi(), n, t.phi, kP);
    Check(maker.One(), n, t.one, kP);
    Check(maker.Id0(), n, t.one, kP);
    Check(maker.Id(), n, t.id, kP);
    Check(maker.Id1(), n, t.id, kP);
    Check(maker.Id2(), n, t.idk[2], kP);
    Check(maker.Id3(), n, t.idk[3], kP);
    Check(maker.Id4(), n, t.idk[4], kP);
    Check(maker.Id5(), n, t.idk[5], kP);
    Check(maker.Id6(), n, t.idk[6], kP);
    Check(maker.Id7(), n, t.idk[7], kP);
    Check(maker.PrimeP0(), n, t.prime_cnt, kP);
    Check(maker.PrimeQ(), n, t.prime_cnt, kP);
    Check(maker.PrimeP1(), n, t.prime_sum, kP);
    Check(maker.D0(), n, t.sigmak[0], kP);
    Check(maker.Sigma0(), n, t.sigmak[0], kP);
    Check(maker.D1(), n, t.sigmak[1], kP);
    Check(maker.Sigma1(), n, t.sigmak[1], kP);
    Check(maker.D2(), n, t.sigmak[2], kP);
    Check(maker.Sigma2(), n, t.sigmak[2], kP);
    Check(maker.D3(), n, t.sigmak[3], kP);
    Check(maker.Sigma3(), n, t.sigmak[3], kP);
    Check(maker.D4(), n, t.sigmak[4], kP);
    Check(maker.Sigma4(), n, t.sigmak[4], kP);
    Check(maker.D5(), n, t.sigmak[5], kP);
    Check(maker.Sigma5(), n, t.sigmak[5], kP);
    Check(maker.D6(), n, t.sigmak[6], kP);
    Check(maker.Sigma6(), n, t.sigmak[6], kP);
    Check(maker.D7(), n, t.sigmak[7], kP);
    Check(maker.Sigma7(), n, t.sigmak[7], kP);
    Check(maker.PrimeNu(), n, t.prime_nu, kP);
    Check(maker.MuSquare(), n, t.sqfree, kP);
    Check(maker.SquareFree(), n, t.sqfree, kP);
    // Cached: the same object is returned.
    assert(&maker.Mu() == &maker.Make(Ntf::Mu));
  }
}

PE_REGISTER_TEST(&DvaPrefixSumMakerTest, "DvaPrefixSumMakerTest", SMALL);
}  // namespace dva_test
