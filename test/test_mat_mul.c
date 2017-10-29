const int maxp = 100000;
#include <pe>

const int K = 2817;
const int64 mod = 1000000007;

int main()
{
  if (1)
  {
    std::vector<int64> v(K, 0);
    Mat64 m = Mat64::Zero(K, K);
    rep(i, 0, K) rep(j, 0, K) m(i, j) = rand();
    TimeRecorder tr;
    v = power_mod(m, 2, v, mod);
    int64 s = 0;
    for (auto i: v) s += i;
    cout << s << endl;
    cout << tr.elapsed().format() << endl;
    sort(all(v));
  }
  #if 0
  {
    Mat1 m = Mat1::Zero(K, K);
    rep(i, 0, K) rep(j, 0, K) m(i, j) = rand();
    TimeRecorder tr;
    m = m * m;
    int64 s = 0;
    rep(i, 0, K) rep(j, 0, K) s += m(i, j).value();
    cout << s << endl;
    cout << tr.elapsed().format() << endl;
  }
  #endif
}