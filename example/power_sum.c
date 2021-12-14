#include <pe.hpp>
using namespace pe;

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
    std::cout << "k = " << k << std::endl;
    for (int64 n = 1; n <= 1000000000; n *= 10) {
      std::cout << "n = " << n << std::endl;
      int64 ans[32];
      InitPowerSumMod<int64>(n, k, ans, comb, mod);
      std::cout << "InitPowerSumMod   " << ans[k] << std::endl;
      std::cout << "PowerSumModer     " << moder0.Cal(n, k) << std::endl;
      std::cout << "PowerSumModerB    " << moder1.Cal(n, k) << std::endl;
      std::cout << "PowerSumModerB1   " << moder2.Cal(n, k) << std::endl;
      if (k >= 1 && k <= 7) {
        std::cout << "P" << k << "SumMod          " << sum_function[k](n, mod)
                  << std::endl;
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }
  return 0;
}