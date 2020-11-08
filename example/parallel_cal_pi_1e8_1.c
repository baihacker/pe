#include <pe.hpp>

struct CalPI : public ParallelRangeT<CalPI> {
  int64 UpdateResult(int64 result, int64 value) { return result + value; }
  int64 WorkOnBlock(int64 first, int64 last, int64 worker) {
    int64 t = 0;
    for (int64 i = first; i <= last; ++i) t += IsPrimeEx(i);
    return t;
  }
};

int main() {
  InitPrimes();
  int64 result = CalPI()
                     .From(1)
                     .To(100000000)
                     .DividedBy(10000000)
                     .SetThreadsCount(4)
                     .Start()
                     .Result();
  cerr << "expected : " << pmpi[8] << endl;
  cerr << "received : " << result << endl;
  return 0;
}