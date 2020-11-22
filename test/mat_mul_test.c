#include "pe_test.h"

namespace mat_mul_test {
#if ENABLE_EIGEN && PE_HAS_INT128
const int K = 500;
const int64 mod = 1000000007;
const int show = 1;

template <typename E>
void Test(const vector<int>& data, const vector<int>& V) {
  std::vector<E> v(K, 0);
  EigenMatrix<E> m = EigenMatrix<E>::Zero(K, K);

  for (int i = 0; i < K; ++i) v[i] = V[i];
  rep(i, 0, K) rep(j, 0, K) m(i, j) = data[j * K + i];

  TimeRecorder tr;
  v = MatrixPower(m, 4, v);
  int64 s = 0;
  for (auto i : v) s += i.value();
  // cout << s << endl;
  if (show) {
    cout << tr.Elapsed().Format() << endl;
  }
  assert(s == 247446585411LL);
  sort(all(v));
}

SL void TestHelperMethod() {
  {
    auto res0 = MatrixPowerMod<1000000007>(
        [=](auto& m) {
          m(0, 0) = 1;
          m(0, 1) = 1;
          m(1, 0) = 1;
        },
        2, 100000);
    auto res1 = MatrixPowerMod<1000000007>(
        [=](auto& m, auto& v) {
          m(0, 0) = 1;
          m(0, 1) = 1;
          m(1, 0) = 1;
          v[0] = 1;
          v[1] = 1;
        },
        2, 100000);
  }

  {
    auto res0 = MatrixPowerMod<NMod64<1000000007>>(
        [=](auto& m) {
          m(0, 0) = 1;
          m(0, 1) = 1;
          m(1, 0) = 1;
        },
        2, 100000);
    auto res1 = MatrixPowerMod<NMod64<1000000007>>(
        [=](auto& m, auto& v) {
          m(0, 0) = 1;
          m(0, 1) = 1;
          m(1, 0) = 1;
          v[0] = 1;
          v[1] = 1;
        },
        2, 100000);
  }

  {
    auto res0 = MatrixPowerMod(
        [=](auto& m) {
          m(0, 0) = 1;
          m(0, 1) = 1;
          m(1, 0) = 1;
        },
        2, 100000, 1000000007);
    auto res1 = MatrixPowerMod(
        [=](auto& m, auto& v) {
          m(0, 0) = 1;
          m(0, 1) = 1;
          m(1, 0) = 1;
          v[0] = 1;
          v[1] = 1;
        },
        2, 100000, 1000000007);
  }
}

SL void MatMulTest() {
  DEFAULT_MOD = mod;
  vector<int> data;
  for (int i = 0; i < K; ++i)
    for (int j = 0; j < K; ++j) data.push_back(j * K + i);
  vector<int> V;
  for (int i = 0; i < K; ++i) V.push_back(i);
  {
    typedef int64 E;
    std::vector<E> v(K, 0);
    EigenMatrix<E> m = EigenMatrix<E>::Zero(K, K);

    for (int i = 0; i < K; ++i) v[i] = V[i];
    rep(i, 0, K) rep(j, 0, K) m(i, j) = data[j * K + i];

    TimeRecorder tr;
    v = MatrixPower(m, 4, v, mod);
    int64 s = 0;
    for (auto i : v) s += i;
    // cout << s << endl;
    if (show) {
      cout << tr.Elapsed().Format() << endl;
    }
    assert(s == 256670487618LL);
    sort(all(v));
  }
  {
    typedef int128 E;
    std::vector<E> v(K, 0);
    EigenMatrix<E> m = EigenMatrix<E>::Zero(K, K);

    for (int i = 0; i < K; ++i) v[i] = V[i];
    rep(i, 0, K) rep(j, 0, K) m(i, j) = data[j * K + i];

    TimeRecorder tr;
    v = MatrixPower(m, 4, v, mod);
    int64 s = 0;
    for (auto i : v) s += i;
    // cout << s << endl;
    if (show) {
      cout << tr.Elapsed().Format() << endl;
    }
    assert(s == 247446585411LL);
    sort(all(v));
  }
  if (show) {
    cout << endl;
  }
  Test<NModNumber<CCMod64<mod>, APSB<int64, int64>>>(data, V);
  Test<NModNumber<DefaultMod, APSB<int64, int64>>>(data, V);

  Test<NModNumber<CCMod64<mod>, APSB<int64, int128>>>(data, V);
  Test<NModNumber<DefaultMod, APSB<int64, int128>>>(data, V);

  Test<NModNumber<CCMod64<mod>, APSB<int64, fake_int128>>>(data, V);
  Test<NModNumber<DefaultMod, APSB<int64, fake_int128>>>(data, V);

  Test<NModNumber<CCMod64<mod>, APSB<int128, int128>>>(data, V);
  Test<NModNumber<DefaultMod, APSB<int128, int128>>>(data, V);

  Test<NModNumber<CCMod64<mod>, APSBL<int128>>>(data, V);
  Test<NModNumber<DefaultMod, APSBL<int128>>>(data, V);
  if (show) {
    cout << endl;
  }
  Test<NModNumberM<CCMod64<mod>, APSB<int64, int64>>>(data, V);
  Test<NModNumberM<DefaultMod, APSB<int64, int64>>>(data, V);

  Test<NModNumberM<CCMod64<mod>, APSB<int64, int128>>>(data, V);
  Test<NModNumberM<DefaultMod, APSB<int64, int128>>>(data, V);

  Test<NModNumberM<CCMod64<mod>, APSB<int64, fake_int128>>>(data, V);
  Test<NModNumberM<DefaultMod, APSB<int64, fake_int128>>>(data, V);

  Test<NModNumberM<CCMod64<mod>, APSB<int128, int128>>>(data, V);
  Test<NModNumberM<DefaultMod, APSB<int128, int128>>>(data, V);

  Test<NModNumberM<CCMod64<mod>, APSBL<int128>>>(data, V);
  Test<NModNumberM<DefaultMod, APSBL<int128>>>(data, V);

  TestHelperMethod();
}

PE_REGISTER_TEST(&MatMulTest, "MatMulTest", SUPER);
#endif
}  // namespace mat_mul_test
