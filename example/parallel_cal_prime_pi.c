#include <pe.hpp>

const int TN = 8;
int64 CalPi0(int64 n) {
  int64 result = PARALLEL_RESULT(
  BEGIN_PARALLEL
    FROM 1 TO n EACH_BLOCK_IS 10000000 CACHE ""
    THREADS TN
    MAP {
        return IsPrimeEx(key);
      }
    REDUCE {
        result += value;
        return result;
      }
  END_PARALLEL);
  return result;
}

struct CalPI : public ParallelRangeT<CalPI> {
  int64 UpdateResult(int64 result, int64 value) { return result + value; }
  int64 WorkOnBlock(int64 first, int64 last, int64 worker) {
    int64 t = 0;
    for (int64 i = first; i <= last; ++i) t += IsPrimeEx(i);
    return t;
  }
};

int64 CalPi1(int64 n) {
  return CalPI()
      .From(1)
      .To(n)
      .DividedBy(10000000)
      .SetThreadsCount(TN)
      .Start()
      .Result();
}

int main() {
  PE_INIT(maxp = 2000000);

  const int m = 8;
  const int n = Power(10, m);

  int64 ans0 = CalPi0(n);
  int64 ans1 = CalPi1(n);
  cout << "n = " << n << endl;
  cout << "Expected: " << pmpi[m] << endl;
  cout << "CalPi0: " << ans0 << endl;
  cout << "CalPi1: " << ans1 << endl;

  return 0;
}