#include "pe_test.h"

namespace square_root_test {
SL void SquareRootCorrectnessTest() {
  int f = 0;
  for (int64 i = 1; i <= 10000000; ++i) {
    const int64 num = i * i;
    for (int64 offset = -10; offset <= 10; ++offset) {
      int64 now = offset + num;
      int64 u = SquareRootSafe(now);
      int64 v = SquareRootUnsafe(now);
      int64 w = SquareRoot(now);
      if (u != v || u != w || v != w) {
        cerr << "failed " << now << ":" << endl;
        cerr << u << " " << v << " " << w << endl;
        ++f;
      }
    }
  }
  if (f == 0) {
    cerr << "Correctness test pass!" << endl;
  } else {
    cerr << "Correctness tess failed!" << endl;
  }
}

SL void SquareRootPerformanceTest() {
  const int N = 10000000;
  TimeRecorder tr;
  int64 s = 0;
  for (int64 i = 10; i <= N; ++i) {
    const int64 num = i * i;
    for (int64 offset = -10; offset <= 10; ++offset) {
      int64 now = offset + num;
      int64 u = SquareRootSafe(now);
      s += u;
    }
  }
  cerr << "SquareRootSafe : " << tr.Elapsed().Format() << " " << s << endl;
  s = 0;
  tr.Record();
  for (int64 i = 10; i <= N; ++i) {
    const int64 num = i * i;
    for (int64 offset = -10; offset <= 10; ++offset) {
      int64 now = offset + num;
      int64 u = SquareRootUnsafe(now);
      s += u;
    }
  }
  cerr << "SquareRootUnsafe : " << tr.Elapsed().Format() << " " << s << endl;
  s = 0;
  tr.Record();
  for (int64 i = 10; i <= N; ++i) {
    const int64 num = i * i;
    for (int64 offset = -10; offset <= 10; ++offset) {
      int64 now = offset + num;
      int64 u = SquareRoot(now);
      s += u;
    }
  }
  cerr << "SquareRoot : " << tr.Elapsed().Format() << " " << s << endl;
}

PE_REGISTER_TEST(&SquareRootCorrectnessTest, "SquareRootCorrectnessTest",
                 MEDIUM);
PE_REGISTER_TEST(&SquareRootPerformanceTest, "SquareRootPerformanceTest",
                 MEDIUM);
}  // namespace square_root_test
