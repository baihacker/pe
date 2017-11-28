#include "pe_test.h"

namespace mat_mul_test {
#if ENABLE_EIGEN && PE_HAS_INT128
const int K = 500;
const int64 mod = 1000000007;

SL void mat_mul_test() {
  vector<int> data;
  for (int i = 0; i < K; ++i)
  for (int j = 0; j < K; ++j)
  data.push_back(j * K + i);
  vector<int> V;
  for (int i = 0; i < K; ++i)
  V.push_back(i);
  {
    std::vector<int64> v(K, 0);
    Mat64 m = Mat64::Zero(K, K);

    for (int i = 0; i < K; ++i) v[i] = V[i];
    rep(i, 0, K) rep(j, 0, K) m(i, j) = data[j*K+i];

    TimeRecorder tr;
    v = power_mod(m, 4, v, mod);
    int64 s = 0;
    for (auto i: v) s += i;
    // cout << s << endl;
    // cout << tr.elapsed().format() << endl;
    assert(s == 256670487618LL);
    sort(all(v));
  }
  {
    std::vector<int128> v(K, 0);
    Mat128 m = Mat128::Zero(K, K);

    for (int i = 0; i < K; ++i) v[i] = V[i];
    rep(i, 0, K) rep(j, 0, K) m(i, j) = data[j*K+i];

    TimeRecorder tr;
    v = power_mod(m, 4, v, mod);
    int128 s = 0;
    for (auto i: v) s += i;
    // cout << s << endl;
    // cout << tr.elapsed().format() << endl;
    assert(s == 247446585411LL);
    sort(all(v));
  }
  {
    std::vector<mint6464<mod>> v(K, 0);
    MatM<mod> m = MatM<mod>::Zero(K, K);

    for (int i = 0; i < K; ++i) v[i] = V[i];
    rep(i, 0, K) rep(j, 0, K) m(i, j) = data[j*K+i];

    TimeRecorder tr;
    v = power_mod(m, 4, v, mod);
    int64 s = 0;
    for (auto i: v) s += i.value();
    // cout << s << endl;
    // cout << tr.elapsed().format() << endl;
    assert(s == 247446585411LL);
    sort(all(v));
  }
  {
    std::vector<mintl<int128, mod>> v(K, 0);
    MatML<mod> m = MatML<mod>::Zero(K, K);

    for (int i = 0; i < K; ++i) v[i] = V[i];
    rep(i, 0, K) rep(j, 0, K) m(i, j) = data[j*K+i];

    TimeRecorder tr;
    v = power_mod(m, 4, v, mod);
    int64 s = 0;
    for (auto i: v) s += i.value();
    // cout << s << endl;
    // cout << tr.elapsed().format() << endl;
    assert(s == 247446585411LL);
    sort(all(v));
  }
}

PE_REGISTER_TEST(&mat_mul_test, "mat_mul_test", SMALL);
#endif
}
