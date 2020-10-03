#include <pe.hpp>

const int64 N = 1000000000;
LargeMemory lm;

int main() {
  float* data = (float*)lm.Allocate(N * sizeof(float));
  dbg("memory ready");

  for (int i = 0; i < N; ++i) data[i] = 1. * rand() / RAND_MAX;
  dbg("data ready");

  TimeRecorder tr;
  ParallelSort<30>(data, data + N);
  // sort(data, data+N);
  dbg("sorted");

  cerr << tr.Elapsed().Format() << endl;
  return 0;
}