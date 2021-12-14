#include <pe.hpp>
using namespace pe;

const int64 N = 1000000000;
LargeMemory lm;

int main() {
  float* data = reinterpret_cast<float*>(lm.Allocate(N * sizeof(float)));
  dbg("memory ready");

  for (int i = 0; i < N; ++i) data[i] = 1. * rand() / RAND_MAX;
  dbg("data ready");

  TimeRecorder tr;
  ParallelSort<30>(data, data + N);
  // std::sort(data, data+N);
  dbg("sorted");

  std::cerr << tr.Elapsed().Format() << std::endl;
  return 0;
}