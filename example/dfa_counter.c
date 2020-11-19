#include <pe.hpp>

// Count the number which contains 3.

int64 check(int64 n) {
  for (; n; n /= 10)
    if (n % 10 == 3) return 1;
  return 0;
}

int64 bf(int64 n) { return XRange(1LL, n).Map<int64>(check).Sum(); }

int main() {
  PE_INIT(maxp=1000000);

  // state 0: initial state
  // state 1: 3 is not seen
  // state 2: 3 is seen
  DfaCounter<int64> counter;
  counter.Init(3, 10, 16);
  for (int i = 1; i <= 9; ++i)
    if (i != 3) counter.AddTrans(0, i, 1);
  counter.AddTrans(0, 3, 2);
  counter.AddTrans(0, 0, 0);
  for (int i = 0; i <= 9; ++i)
    if (i != 3) counter.AddTrans(1, i, 1);
  counter.AddTrans(1, 3, 2);
  for (int i = 0; i <= 9; ++i) counter.AddTrans(2, i, 2);
  counter.MarkAsCount(2);

  for (int64 n = 10; n <= 1000000; n *= 10) {
    int64 a = bf(n);
    int64 b = counter.Cal(n);
    cout << n << "\t" << a << "\t" << b << endl;
  }
  return 0;
}