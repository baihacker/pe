#include "pe_test.h"

namespace parallel_sort_test {
const int n = 10000000;
int arr[n];

SL void parallel_sort_test() {
  for (int i = 0; i < n; ++i) arr[i] = rand();
  TimeRecorder tr;
  parallel_sort<8>(arr, arr + n);
  cerr << tr.elapsed().format() << endl;
  for (int i = 1; i < n; ++i) {
    assert(arr[i - 1] <= arr[i]);
  }
}

PE_REGISTER_TEST(&parallel_sort_test, "parallel_sort_test", SMALL);
}  // namespace parallel_sort_test
