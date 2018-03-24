#include <pe.hpp>

const int64 mod = 1000000007;
int main() {
  {
    Poly x{{1LL, 96LL}, 97};
    cout << x.inv(20) << endl;
    cout << x.inv(20) * x << endl;
  }

  {
    Poly x{{}, 97};
    for (int i = 1; i < 100; ++i)
      x.data.push_back(i%97);
    cout << x.inv(100) * x << endl;
  }

  {
    // Fibonacci sequence
    Poly x{{1, mod-1, mod-1}, mod};

    TimeRecorder tr;
    cout << x.inv(100000)[99999] << endl;
    cout << tr.elapsed().format() << endl;
  }

  {
    // Fibonacci sequence
    vector<int64> A{1, -1, -1};
    vector<int64> B{1};

    TimeRecorder tr;
    cerr << gf_at({A, mod}, {B, mod}, 99999) << endl;
    cout << tr.elapsed().format() << endl;
  }
  return 0;
}