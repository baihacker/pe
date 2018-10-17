#include "pe_test.h"

namespace prime_phi_sum_test {
int64 ps[1000001];
int64 pc[1000001];

SL void verify_cnt(const int64 n, const DVA<int64>& result) {
  int64 v = (int64)sqrt(n);
  for (int j = 1; j <= v; ++j) {
    assert(result[j] == pc[j]);
    assert(result[n / j] == pc[n / j]);
  }
}

SL void verify_sum(const int64 n, const DVA<int64>& result) {
  int64 v = (int64)sqrt(n);
  for (int j = 1; j <= v; ++j) {
    assert(result[j] == ps[j]);
    assert(result[n / j] == ps[n / j]);
  }
}

SL void small_test() {
  for (int i = 1; i <= 100000; ++i) {
    const int n = i;
    verify_cnt(n, prime_pi<int64>(i));
    verify_sum(n, prime_sum<int64>(i));
  }
}

SL void prime_phi_sum_test() {
  for (int i = 1; i <= 1000000; ++i) {
    pc[i] = pc[i - 1] + (is_prime(i) ? 1 : 0);
    ps[i] = ps[i - 1] + (is_prime(i) ? i : 0);
  }

  small_test();

  assert((prime_pi<int64>(10000000))[10000000] == pmpi[7]);
  assert((prime_pi<int64>(100000000))[100000000] == pmpi[8]);
  assert((prime_pi<int64>(1000000000))[1000000000] == pmpi[9]);
  assert((prime_pi<int64>(10000000000))[10000000000] == pmpi[10]);
  assert((prime_pi<int64>(100000000000))[100000000000] == pmpi[11]);
  assert((prime_pi<int64>(1000000000000))[1000000000000] == pmpi[12]);

  const int64 N = 100000;
  for (int mod = 1; mod <= 30; ++mod) {
    int64 result[32] = {0};
    for (int i = 0; i < pcnt && plist[i] <= N; ++i) ++result[plist[i] % mod];
    auto v = prime_s0_pmod<int64>(N, mod);
    for (int j = 0; j < mod; ++j) {
      assert(result[j] == v[j][N]);
    }
  }
  for (int mod = 1; mod <= 30; ++mod) {
    int64 result[32] = {0};
    for (int i = 0; i < pcnt && plist[i] <= N; ++i)
      result[plist[i] % mod] += plist[i];
    auto v = prime_s1_pmod<int64>(N, mod);
    for (int j = 0; j < mod; ++j) {
      assert(result[j] == v[j][N]);
    }
  }
  const int64 M = 10007;
  for (int mod = 1; mod <= 30; ++mod) {
    int64 result[32] = {0};
    for (int i = 0; i < pcnt && plist[i] <= N; ++i) ++result[plist[i] % mod];
    auto v = prime_s0_pmod<NModNumber6464<M>>(N, mod);
    for (int j = 0; j < mod; ++j) {
      assert(result[j] % M == v[j][N].value());
    }
  }
  for (int mod = 1; mod <= 30; ++mod) {
    int64 result[32] = {0};
    for (int i = 0; i < pcnt && plist[i] <= N; ++i)
      result[plist[i] % mod] += plist[i];
    auto v = prime_s1_pmod<NModNumber6464<M>>(N, mod);
    for (int j = 0; j < mod; ++j) {
      assert(result[j] % M == v[j][N].value());
    }
  }
}

PE_REGISTER_TEST(&prime_phi_sum_test, "prime_phi_sum_test", BIG);
}  // namespace prime_phi_sum_test
