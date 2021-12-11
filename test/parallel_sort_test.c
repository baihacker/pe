#include "pe_test.h"

namespace parallel_sort_test {
const int n = 10000000;
int arr[n];

SL void ParallelSortTest() {
  for (int i = 0; i < n; ++i) arr[i] = rand();
  TimeRecorder tr;
  ParallelSort<8>(arr, arr + n);
  std::cerr << tr.Elapsed().Format() << std::endl;
  for (int i = 1; i < n; ++i) {
    assert(arr[i - 1] <= arr[i]);
  }
}

PE_REGISTER_TEST(&ParallelSortTest, "ParallelSortTest", SMALL);
}  // namespace parallel_sort_test
