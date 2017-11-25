#include <pe>

const int64 mod = 1000000007;
int main() {
  {
    PolyM<97> x{1LL, 96LL};
    cout << x.inv(20) << endl;
    cout << x.inv(20) * x << endl;
  }
  
  {
    PolyM<97> x;
    for (int i = 1; i < 100; ++i)
      x.data.push_back(i%97);
    cout << x.inv(100) * x << endl;
  }
  
  {
    // Fibonacci sequence
    PolyM<mod> x{1, mod-1, mod-1};

    TimeRecorder tr;
    cout << x.inv(100000)[99999] << endl;
    cout << tr.elapsed().format() << endl;
  }
  
  {
    // Fibonacci sequence
    vector<int64> A{1, -1, -1};
    vector<int64> B{1};

    TimeRecorder tr;
    cerr << gf_at<mod>(A, B, 99999) << endl;
    cout << tr.elapsed().format() << endl;
  }
  return 0;
}