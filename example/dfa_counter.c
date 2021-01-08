#include <pe.hpp>

// Count the number which contains 3.

int64 check(int64 n) {
  for (; n; n /= 10)
    if (n % 10 == 3) return 1;
  return 0;
}

int64 bf(int64 n) { return XRange(1LL, n).Map<int64>(check).Sum(); }

int main() {
  PE_INIT(maxp = 1000000);

  // state 0: initial state
  // state 1: 3 is not seen
  // state 2: 3 is seen
  DfaCounter<int64> counter1;
  counter1.Init(3, 10, 16);
  for (int i = 1; i <= 9; ++i)
    if (i != 3) counter1.AddTrans(0, i, 1);
  counter1.AddTrans(0, 3, 2);
  counter1.AddTrans(0, 0, 0);
  for (int i = 0; i <= 9; ++i)
    if (i != 3) counter1.AddTrans(1, i, 1);
  counter1.AddTrans(1, 3, 2);
  for (int i = 0; i <= 9; ++i) counter1.AddTrans(2, i, 2);
  counter1.MarkAsCount(2);

  // In counter2, the dfa doesn't accept leading zeros, in other words, if it's
  // initial state and 0 comes, the targe state is invalid. So we need to call
  // set_count_each_len(1).
  //
  // state 0: initial state
  // state 1: 3 is not seen
  // state 2: 3 is seen
  // state 3: invalid
  DfaCounter<int64> counter2;
  counter2.Init(4, 10, 16);
  for (int i = 1; i <= 9; ++i)
    if (i != 3) counter2.AddTrans(0, i, 1);
  counter2.AddTrans(0, 3, 2);
  for (int i = 0; i <= 9; ++i)
    if (i != 3) counter2.AddTrans(1, i, 1);
  counter2.AddTrans(1, 3, 2);
  for (int i = 0; i <= 9; ++i) counter2.AddTrans(2, i, 2);
  counter2.MarkAsCount(2);
  counter2.AddTrans(0, 0, 3);
  for (int i = 0; i <= 9; ++i) counter2.AddTrans(3, i, 3);
  counter2.set_count_each_len(1);

  for (int64 n = 10; n <= 1000000; n *= 10) {
    int64 a = bf(n);
    int64 b = counter1.Cal(n);
    int64 c = counter2.Cal(n);
    cout << n << "\t" << a << "\t" << b << "\t" << c << endl;
  }
  return 0;
}