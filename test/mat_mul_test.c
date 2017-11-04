const int maxp = 100000;
#include <pe>

const int K = 2817;
const int64 mod = 1000000007;

int main()
{
  {
    srand(123);
    std::vector<int64> v(K, 0);
    Mat64 m = Mat64::Zero(K, K);

    for (auto& i: v) i = rand();
    rep(i, 0, K) rep(j, 0, K) m(i, j) = rand();

    TimeRecorder tr;
    v = power_mod(m, 4, v, mod);
    int64 s = 0;
    for (auto i: v) s += i;
    cout << s << endl;
    cout << tr.elapsed().format() << endl;
    sort(all(v));
  }
  {
    srand(123);
    std::vector<int128> v(K, 0);
    Mat128 m = Mat128::Zero(K, K);

    for (auto& i: v) i = rand();
    rep(i, 0, K) rep(j, 0, K) m(i, j) = rand();

    TimeRecorder tr;
    v = power_mod(m, 4, v, mod);
    int128 s = 0;
    for (auto i: v) s += i;
    cout << s << endl;
    cout << tr.elapsed().format() << endl;
    sort(all(v));
  }
  {
    srand(123);
    std::vector<mint6464<mod>> v(K, 0);
    MatM<mod> m = MatM<mod>::Zero(K, K);

    for (auto& i: v) i = rand();
    rep(i, 0, K) rep(j, 0, K) m(i, j) = rand();

    TimeRecorder tr;
    v = power_mod(m, 4, v, mod);
    int64 s = 0;
    for (auto i: v) s += i.value();
    cout << s << endl;
    cout << tr.elapsed().format() << endl;
    sort(all(v));
  }
  {
    srand(123);
    std::vector<mintl<int128, mod>> v(K, 0);
    MatML<mod> m = MatML<mod>::Zero(K, K);

    for (auto& i: v) i = rand();
    rep(i, 0, K) rep(j, 0, K) m(i, j) = rand();

    TimeRecorder tr;
    v = power_mod(m, 4, v, mod);
    int64 s = 0;
    for (auto i: v) s += i.value();
    cout << s << endl;
    cout << tr.elapsed().format() << endl;
    sort(all(v));
  }
  return 0;
}