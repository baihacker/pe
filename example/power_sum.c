#include <pe.hpp>

const int64 mod = 1000000007;
using MT = NMod64<mod>;

using SumfunctionType = int64 (*)(int64 n, int64 mod);
SumfunctionType sum_function[8] = {nullptr,   &P1SumMod, &P2SumMod, &P3SumMod,
                                   &P4SumMod, &P5SumMod, &P6SumMod, &P7SumMod};

int main() {
  PE_INIT(maxp = 2000000);
  PowerSumModer moder0(mod);    // maxk = 128
  PowerSumModerB moder1(mod);   // maxk = 128
  PowerSumModerB1 moder2(mod);  // maxk = 128
  int64 comb[32][32];
  InitComb(comb, 31, mod);
  for (int k = 0; k <= 10; ++k) {
    cout << "k = " << k << endl;
    for (int64 n = 1; n <= 1000000000; n *= 10) {
      cout << "n = " << n << endl;
      int64 ans[32];
      InitPowerSumMod<int64>(n, k, ans, comb, mod);
      cout << "InitPowerSumMod   " << ans[k] << endl;
      cout << "PowerSumModer     " << moder0.Cal(n, k) << endl;
      cout << "PowerSumModerB    " << moder1.Cal(n, k) << endl;
      cout << "PowerSumModerB1   " << moder2.Cal(n, k) << endl;
      if (k >= 1 && k <= 7) {
        cout << "P" << k << "SumMod          " << sum_function[k](n, mod)
             << endl;
      }
      cout << endl;
    }
    cout << endl;
  }
  return 0;
}