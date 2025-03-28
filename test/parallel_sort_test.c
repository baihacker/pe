#include "pe_test.h"

namespace parallel_sort_test {
constexpr int n = 1000000;

SL void ParallelSortTest() {
  std::vector<int> arr(n);
  for (int i = 0; i < n; ++i) arr[i] = rand();
  TimeRecorder tr;
  ParallelSort<8>(&arr[0], &arr[0] + n);
  std::cerr << tr.Elapsed().Format() << std::endl;
  for (int i = 1; i < n; ++i) {
    assert(arr[i - 1] <= arr[i]);
  }
}

PE_REGISTER_TEST(&ParallelSortTest, "ParallelSortTest", SMALL);
}  // namespace parallel_sort_test
