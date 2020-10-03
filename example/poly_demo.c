#include <pe.hpp>

const int64 mod = 1000000007;
int main() {
  {
    NModPoly x{{1LL, 96LL}, 97};
    cout << x.Inv(20) << endl;
    cout << x.Inv(20) * x << endl;
  }

  {
    NModPoly x{{}, 97};
    for (int i = 1; i < 100; ++i) x.data.push_back(i % 97);
    cout << x.Inv(100) * x << endl;
  }

  {
    // Fibonacci sequence
    NModPoly x{{1, mod - 1, mod - 1}, mod};

    TimeRecorder tr;
    cout << x.Inv(100000)[99999] << endl;
    cout << tr.Elapsed().Format() << endl;
  }

  {
    // Fibonacci sequence
    vector<int64> A{1, -1, -1};
    vector<int64> B{1};

    TimeRecorder tr;
    cerr << GfAt({A, mod}, {B, mod}, 99999) << endl;
    cout << tr.Elapsed().Format() << endl;
  }
  return 0;
}