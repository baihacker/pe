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
        std::cerr << "failed " << now << ":" << std::endl;
        std::cerr << u << " " << v << " " << w << std::endl;
        ++f;
      }
    }
  }
  if (f == 0) {
    std::cerr << "Correctness test pass!" << std::endl;
  } else {
    std::cerr << "Correctness tess failed!" << std::endl;
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
  std::cerr << "SquareRootSafe : " << tr.Elapsed().Format() << " " << s
            << std::endl;
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
  std::cerr << "SquareRootUnsafe : " << tr.Elapsed().Format() << " " << s
            << std::endl;
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
  std::cerr << "SquareRoot : " << tr.Elapsed().Format() << " " << s
            << std::endl;
}

PE_REGISTER_TEST(&SquareRootCorrectnessTest, "SquareRootCorrectnessTest",
                 MEDIUM);
PE_REGISTER_TEST(&SquareRootPerformanceTest, "SquareRootPerformanceTest",
                 MEDIUM);
}  // namespace square_root_test
