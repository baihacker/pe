#include <pe.hpp>
using namespace pe;

// Compute the 3rd power of the number which contains 3.
const int64 mod = 1000000007;
using MT = NMod64<mod>;

int64 cal(int64 n) {
  const int64 m = n;
  for (; n; n /= 10)
    if (n % 10 == 3) return m * m % mod * m % mod;
  return 0;
}

int64 bf(int64 n) { return XRange(1LL, n).Map<int64>(cal).SumMod(mod); }

int main() {
  PE_INIT(maxp = 1000000);

  // state 0: initial state
  // state 1: 3 is not seen
  // state 2: 3 is seen
  DfaSummer<MT> summer1;
  summer1.Init(3, 3, 10, 16);
  for (int i = 1; i <= 9; ++i)
    if (i != 3) summer1.AddTrans(0, i, 1);
  summer1.AddTrans(0, 3, 2);
  summer1.AddTrans(0, 0, 0);
  for (int i = 0; i <= 9; ++i)
    if (i != 3) summer1.AddTrans(1, i, 1);
  summer1.AddTrans(1, 3, 2);
  for (int i = 0; i <= 9; ++i) summer1.AddTrans(2, i, 2);
  summer1.MarkTargetState(2);

  // In summer2, the dfa doesn't accept leading zeros, in other words, if it's
  // initial state and 0 comes, the targe state is invalid. So we need to call
  // set_count_each_len(1).
  //
  // state 0: initial state
  // state 1: 3 is not seen
  // state 2: 3 is seen
  // state 3: invalid
  DfaSummer<MT> summer2;
  summer2.Init(4, 3, 10, 16);
  for (int i = 1; i <= 9; ++i)
    if (i != 3) summer2.AddTrans(0, i, 1);
  summer2.AddTrans(0, 3, 2);
  for (int i = 0; i <= 9; ++i)
    if (i != 3) summer2.AddTrans(1, i, 1);
  summer2.AddTrans(1, 3, 2);
  for (int i = 0; i <= 9; ++i) summer2.AddTrans(2, i, 2);
  summer2.MarkTargetState(2);
  summer2.AddTrans(0, 0, 3);
  for (int i = 0; i <= 9; ++i) summer2.AddTrans(3, i, 3);
  summer2.set_count_each_len(1);

  for (int64 n = 10; n <= 1000000; n *= 10) {
    auto a = bf(n);
    auto b = summer1.Cal(n);
    auto c = summer2.Cal(n);
    std::cout << n << "\t" << a << "\t" << b << "\t" << c << std::endl;
  }
  return 0;
}