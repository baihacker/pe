#include "pe_test.h"

namespace prime_pi_sum_test {
std::vector<int64> ps(1000001);
std::vector<int64> pc(1000001);

SL void VerifyCnt(const int64 n, const DVA<int64>& result) {
  int64 v = static_cast<int64>(std::sqrt(n));
  for (int j = 1; j <= v; ++j) {
    assert(result[j] == pc[j]);
    assert(result[n / j] == pc[n / j]);
  }
}

SL void VerifySum(const int64 n, const DVA<int64>& result) {
  int64 v = static_cast<int64>(std::sqrt(n));
  for (int j = 1; j <= v; ++j) {
    assert(result[j] == ps[j]);
    assert(result[n / j] == ps[n / j]);
  }
}

// PrimeCountingVariantsTest checks every key for a few larger n.
SL void SmallTest() {
  for (int i = 1; i <= 3000; ++i) {
    const int n = i;
    VerifyCnt(n, PrimePi<int64>(i));
    VerifySum(n, PrimeSum<int64>(i));
  }
}

SL void PrimePiSumTest() {
  for (int i = 1; i <= 1000000; ++i) {
    pc[i] = pc[i - 1] + (IsPrime(i) ? 1 : 0);
    ps[i] = ps[i - 1] + (IsPrime(i) ? i : 0);
  }

  SmallTest();

  assert((PrimePi<int64>(10000000))[10000000] == kPrimePi[7]);
  assert((PrimePi<int64>(100000000))[100000000] == kPrimePi[8]);
  assert((PrimePi<int64>(1000000000))[1000000000] == kPrimePi[9]);
  assert((PrimePi<int64>(10000000000))[10000000000] == kPrimePi[10]);
  // assert((PrimePi<int64>(100000000000))[100000000000] == kPrimePi[11]);
  // assert((PrimePi<int64>(1000000000000))[1000000000000] == kPrimePi[12]);
}

PE_REGISTER_TEST(&PrimePiSumTest, "PrimePiSumTest", BIG);

SL void PrimePiSumPModTest() {
  const int64 N = 100000;
  for (int mod = 1; mod <= 30; ++mod) {
    int64 result[32] = {0};
    for (int i = 0; i < pcnt && plist[i] <= N; ++i) ++result[plist[i] % mod];
    auto v = PrimeS0PMod<int64>(N, mod);
    for (int j = 0; j < mod; ++j) {
      assert(result[j] == v[j][N]);
    }
  }
  for (int mod = 1; mod <= 30; ++mod) {
    int64 result[32] = {0};
    for (int i = 0; i < pcnt && plist[i] <= N; ++i) {
      result[plist[i] % mod] += plist[i];
    }
    auto v = PrimeS1PMod<int64>(N, mod);
    for (int j = 0; j < mod; ++j) {
      assert(result[j] == v[j][N]);
    }
  }
  const int64 M = 10007;
  for (int mod = 1; mod <= 30; ++mod) {
    int64 result[32] = {0};
    for (int i = 0; i < pcnt && plist[i] <= N; ++i) ++result[plist[i] % mod];
    auto v = PrimeS0PMod<NModNumber<CCMod64<M>>>(N, mod);
    for (int j = 0; j < mod; ++j) {
      assert(result[j] % M == v[j][N].value());
    }
  }
  for (int mod = 1; mod <= 30; ++mod) {
    int64 result[32] = {0};
    for (int i = 0; i < pcnt && plist[i] <= N; ++i) {
      result[plist[i] % mod] += plist[i];
    }
    auto v = PrimeS1PMod<NModNumber<CCMod64<M>>>(N, mod);
    for (int j = 0; j < mod; ++j) {
      assert(result[j] % M == v[j][N].value());
    }
  }
}
PE_REGISTER_TEST(&PrimePiSumPModTest, "PrimePiSumPModTest", SMALL);
// Brute force tables up to kN for the prime counting variants and the summers.
constexpr int64 kN = 200000;
constexpr int64 kP = 1000000007;

struct Tables {
  std::vector<int64> cnt, sum, sum2, mu, phi, sigma0;
  // sum of p^k mod kP for primes p <= x, k = 0..5
  std::vector<std::vector<int64>> sumk;
};

SL const Tables& GetTables() {
  static Tables t = [] {
    Tables t;
    t.cnt.assign(kN + 1, 0);
    t.sum = t.sum2 = t.mu = t.phi = t.sigma0 = t.cnt;
    t.sumk.assign(6, t.cnt);
    for (int64 x = 1; x <= kN; ++x) {
      const bool p = IsPrime(x);
      t.cnt[x] = t.cnt[x - 1] + p;
      t.sum[x] = t.sum[x - 1] + (p ? x : 0);
      t.sum2[x] = t.sum2[x - 1] + (p ? x * x : 0);
      t.mu[x] = t.mu[x - 1] + mu[x];
      t.phi[x] = t.phi[x - 1] + phi[x];
      t.sigma0[x] = t.sigma0[x - 1] + CalSigma0(x);
      for (int k = 0; k < 6; ++k) {
        t.sumk[k][x] = (t.sumk[k][x - 1] + (p ? PowerMod(x, k, kP) : 0)) % kP;
      }
    }
    return t;
  }();
  return t;
}

template <typename T>
SL void CheckKeys(const DVA<T>& d, int64 n, const std::vector<int64>& prefix) {
  assert(d.n == n);
  for (int64 k : d.FKeys()) assert(d[k] == prefix[k]);
}

const int64 kSizes[] = {1, 2, 3, 10, 99, 100, 1000, 12345, kN};

SL void PrimeCountingVariantsTest() {
  const Tables& t = GetTables();
  for (int64 n : kSizes) {
    CheckKeys(PrimeS0<int64>(n), n, t.cnt);
    CheckKeys(PrimePi<int64>(n), n, t.cnt);
    CheckKeys(PrimeS0Parallel<int64>(n), n, t.cnt);
    CheckKeys(PrimeS0Ex<int64>(n), n, t.cnt);
    CheckKeys(PrimeS1<int64>(n), n, t.sum);
    CheckKeys(PrimeSum<int64>(n), n, t.sum);
    CheckKeys(PrimeS1Parallel<int64>(n), n, t.sum);
    CheckKeys(PrimeS1Ex<int64>(n), n, t.sum);

    // Sums of p^k modulo kP, with and without precomputed powers.
    std::vector<int64> pk;
    for (int k = 0; k < 6; ++k) {
      pk.clear();
      for (int i = 0; i < pcnt && plist[i] <= SqrtI(n) + 1; ++i) {
        pk.push_back(PowerMod(int64(plist[i]), k, kP));
      }
      CheckKeys(PrimeSkEx(n, k, nullptr, kP), n, t.sumk[k]);
      CheckKeys(PrimeSkEx(n, k, pk.data(), kP), n, t.sumk[k]);
      CheckKeys(PrimeSkEx<kP>(n, k), n, t.sumk[k]);
    }

    // Split by the residue of p modulo pmod.
    for (int pmod : {1, 3, 4}) {
      for (int k : {0, 1, 3}) {
        const auto parts = PrimeSkPMod<kP>(n, k, pmod);
        assert(static_cast<int>(std::size(parts)) == pmod);
        for (int r = 0; r < pmod; ++r) {
          int64 expected = 0;
          for (int64 x = 2; x <= n; ++x) {
            if (x % pmod == r && IsPrime(x)) {
              expected = (expected + PowerMod(x, k, kP)) % kP;
            }
          }
          assert(parts[r][n].value() == expected);
        }
      }
    }
  }
}

PE_REGISTER_TEST(&PrimeCountingVariantsTest, "PrimeCountingVariantsTest",
                 SMALL);

SL void SummersTest() {
  const Tables& t = GetTables();
  // Small pivots make the summers recurse.
  MuSummer<int64> mus(1000);
  MuPhiSummer<int64> mups(1000);
  Sigma0Summer<int64> s0(1000);
  Sigma0SumModer s0m(kP, 1000);
  CachedPi cpi(1000);
  for (int64 n = 1; n <= kN; n += n / 50 + 1) {
    assert(mus(n) == t.mu[n] && mus.Cal(n) == t.mu[n]);
    assert(mups.CalSumMu(n) == t.mu[n]);
    assert(mups.CalSumPhi(n) == t.phi[n]);
    assert(s0(n) == t.sigma0[n] && s0.Cal(n) == t.sigma0[n]);
    assert(s0m(n) == t.sigma0[n] % kP);
    assert(cpi(n) == t.cnt[n] && cpi.Cal(n) == t.cnt[n]);
  }
  // Larger n: the summers agree with each other and with the known values.
  const int64 big = 1000000000;
  assert(CachedPi()(big) == kPrimePi[9]);
  assert(MuSummer<int64>()(big) == MuPhiSummer<int64>().CalSumMu(big));
  assert(MuPhiSumModer(kP).CalSumMu(big) ==
         (MuSummer<int64>()(big) % kP + kP) % kP);
  assert(Sigma0SumModer(kP)(big) == Sigma0Summer<int64>()(big) % kP);
}

PE_REGISTER_TEST(&SummersTest, "SummersTest", SMALL);
}  // namespace prime_pi_sum_test
