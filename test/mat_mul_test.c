#include "pe_test.h"

namespace mat_mul_test {
#if ENABLE_EIGEN && PE_HAS_INT128
const int K = 500;
const int64 mod = 1000000007;
const int show = 1;

template <typename E>
void test(const vector<int>& data, const vector<int>& V) {
  std::vector<E> v(K, 0);
  MatT<E> m = MatT<E>::Zero(K, K);

  for (int i = 0; i < K; ++i) v[i] = V[i];
  rep(i, 0, K) rep(j, 0, K) m(i, j) = data[j * K + i];

  TimeRecorder tr;
  v = power_mod(m, 4, v, mod);
  int64 s = 0;
  for (auto i : v) s += i.value();
  // cout << s << endl;
  if (show) {
    cout << tr.elapsed().format() << endl;
  }
  assert(s == 247446585411LL);
  sort(all(v));
}

SL void mat_mul_test() {
  DEFAULT_MOD = mod;
  vector<int> data;
  for (int i = 0; i < K; ++i)
    for (int j = 0; j < K; ++j) data.push_back(j * K + i);
  vector<int> V;
  for (int i = 0; i < K; ++i) V.push_back(i);
  {
    typedef int64 E;
    std::vector<E> v(K, 0);
    MatT<E> m = MatT<E>::Zero(K, K);

    for (int i = 0; i < K; ++i) v[i] = V[i];
    rep(i, 0, K) rep(j, 0, K) m(i, j) = data[j * K + i];

    TimeRecorder tr;
    v = power_mod(m, 4, v, mod);
    int64 s = 0;
    for (auto i : v) s += i;
    // cout << s << endl;
    if (show) {
      cout << tr.elapsed().format() << endl;
    }
    assert(s == 256670487618LL);
    sort(all(v));
  }
  {
    typedef int128 E;
    std::vector<E> v(K, 0);
    MatT<E> m = MatT<E>::Zero(K, K);

    for (int i = 0; i < K; ++i) v[i] = V[i];
    rep(i, 0, K) rep(j, 0, K) m(i, j) = data[j * K + i];

    TimeRecorder tr;
    v = power_mod(m, 4, v, mod);
    int64 s = 0;
    for (auto i : v) s += i;
    // cout << s << endl;
    if (show) {
      cout << tr.elapsed().format() << endl;
    }
    assert(s == 247446585411LL);
    sort(all(v));
  }
  if (show) {
    cout << endl;
  }
  test<NModNumber<CCMod<mod>, APSB<int64, int64>>>(data, V);
  test<NModNumber<DefaultMod, APSB<int64, int64>>>(data, V);

  test<NModNumber<CCMod<mod>, APSB<int64, int128>>>(data, V);
  test<NModNumber<DefaultMod, APSB<int64, int128>>>(data, V);

  test<NModNumber<CCMod<mod>, APSB<int64, fake_int128>>>(data, V);
  test<NModNumber<DefaultMod, APSB<int64, fake_int128>>>(data, V);

  test<NModNumber<CCMod<mod>, APSB<int128, int128>>>(data, V);
  test<NModNumber<DefaultMod, APSB<int128, int128>>>(data, V);

  test<NModNumber<CCMod<mod>, APSBL<int128>>>(data, V);
  test<NModNumber<DefaultMod, APSBL<int128>>>(data, V);
  if (show) {
    cout << endl;
  }
  test<NModNumberM<CCMod<mod>, APSB<int64, int64>>>(data, V);
  test<NModNumberM<DefaultMod, APSB<int64, int64>>>(data, V);

  test<NModNumberM<CCMod<mod>, APSB<int64, int128>>>(data, V);
  test<NModNumberM<DefaultMod, APSB<int64, int128>>>(data, V);

  test<NModNumberM<CCMod<mod>, APSB<int64, fake_int128>>>(data, V);
  test<NModNumberM<DefaultMod, APSB<int64, fake_int128>>>(data, V);

  test<NModNumberM<CCMod<mod>, APSB<int128, int128>>>(data, V);
  test<NModNumberM<DefaultMod, APSB<int128, int128>>>(data, V);

  test<NModNumberM<CCMod<mod>, APSBL<int128>>>(data, V);
  test<NModNumberM<DefaultMod, APSBL<int128>>>(data, V);
}

PE_REGISTER_TEST(&mat_mul_test, "mat_mul_test", MEDIUM);
#endif
}  // namespace mat_mul_test
