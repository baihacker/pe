#include "pe_test.h"

namespace prime_pi_sum_test {
int64 ps[1000001];
int64 pc[1000001];

SL void VerifyCnt(const int64 n, const DVA<int64>& result) {
  auto v = (int64)sqrt(n);
  for (int j = 1; j <= v; ++j) {
    assert(result[j] == pc[j]);
    assert(result[n / j] == pc[n / j]);
  }
}

SL void VerifySum(const int64 n, const DVA<int64>& result) {
  auto v = (int64)sqrt(n);
  for (int j = 1; j <= v; ++j) {
    assert(result[j] == ps[j]);
    assert(result[n / j] == ps[n / j]);
  }
}

SL void SmallTest() {
  for (int i = 1; i <= 100000; ++i) {
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

  assert((PrimePi<int64>(10000000))[10000000] == pmpi[7]);
  assert((PrimePi<int64>(100000000))[100000000] == pmpi[8]);
  assert((PrimePi<int64>(1000000000))[1000000000] == pmpi[9]);
  assert((PrimePi<int64>(10000000000))[10000000000] == pmpi[10]);
  // assert((PrimePi<int64>(100000000000))[100000000000] == pmpi[11]);
  // assert((PrimePi<int64>(1000000000000))[1000000000000] == pmpi[12]);
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
}  // namespace prime_pi_sum_test
