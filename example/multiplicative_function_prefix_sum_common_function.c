#include <pe.hpp>
using namespace pe;

const int64 mod = 1000000007;

int main() {
  PE_INIT(maxp = 2000000, cal_phi = 1, cal_mu = 1);

  SFCounter sf_counter;
  MuSummer<int128> mu_summer;
  MuPhiSummer<int128> mu_phi_summer;
  Sigma0Summer<int128> sigma0_summer;
  MuPhiSumModer mu_phi_sum_moder(mod);
  Sigma0SumModer sigma0_sum_moder(mod);

  for (int64 n = 1; n <= 10000000000; n *= 10) {
    std::cout << n << " = " << n << std::endl;
    std::cout << "# square free number " << sf_counter.Cal(n) << std::endl;
    std::cout << "sum mu " << mu_summer.Cal(n) << std::endl;
    std::cout << "sum mu " << mu_phi_summer.CalSumMu(n) << std::endl;
    std::cout << "sum phi " << mu_phi_summer.CalSumPhi(n) << std::endl;
    std::cout << "sum sigma0 " << sigma0_summer.Cal(n) << std::endl;

    std::cout << "sum mu mod " << mod << " " << mu_phi_sum_moder.CalSumMu(n)
              << std::endl;
    std::cout << "sum phi mod " << mod << " " << mu_phi_sum_moder.CalSumPhi(n)
              << std::endl;
    std::cout << "sum sigma0 mod " << mod << " " << sigma0_sum_moder.Cal(n)
              << std::endl;
    std::cout << std::endl;
  }
  return 0;
}
