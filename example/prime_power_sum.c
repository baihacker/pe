#include <pe.hpp>

const int64 mod = 1000000007;
using MT = NMod64<mod>;

void prime_s0() {
  CachedPi cp;
  for (int64 n = 1; n <= 10000000000; n *= 10) {
    cout << "n = " << n << endl;
    cout << "PrimeS0         " << PrimeS0<int64>(n)[n] % mod << endl;
    cout << "PrimeS0Parallel " << PrimeS0Parallel<int64>(n)[n] % mod << endl;
    cout << "PrimeS0Ex       " << PrimeS0Ex<int64>(n)[n] % mod << endl;
    cout << "PrimeS0         " << PrimeS0<MT>(n)[n] << endl;
    cout << "PrimeS0Parallel " << PrimeS0Parallel<MT>(n)[n] << endl;
    cout << "PrimeS0Ex       " << PrimeS0Ex<MT>(n)[n] << endl;
    cout << "PrimeSkEx       " << PrimeSkEx<mod>(n, 0)[n] << endl;
    cout << "CachedPi        " << cp.Cal(n) % mod << endl;
    cout << endl;
  }
}

void prime_s1() {
  for (int64 n = 1; n <= 10000000000; n *= 10) {
    cout << "n = " << n << endl;
    cout << "PrimeS1         " << PrimeS1<int64>(n)[n] % mod << endl;
    cout << "PrimeS1Parallel " << PrimeS1Parallel<int64>(n)[n] % mod << endl;
    cout << "PrimeS1Ex       " << PrimeS1Ex<int64>(n)[n] % mod << endl;
    cout << "PrimeS1         " << PrimeS1<MT>(n)[n] << endl;
    cout << "PrimeS1Parallel " << PrimeS1Parallel<MT>(n)[n] << endl;
    cout << "PrimeS1Ex       " << PrimeS1Ex<MT>(n)[n] << endl;
    cout << "PrimeSkEx       " << PrimeSkEx<mod>(n, 1)[n] << endl;
    cout << endl;
  }
}

void prime_pmod_s0() {
  for (int64 n = 1; n <= 1000000000; n *= 10) {
    cout << "n = " << n << endl;
    for (int pmod = 2; pmod <= 7; ++pmod) {
      auto ans0 = PrimeS0PMod<MT>(n, pmod);
      auto ans1 = PrimeSkPMod<mod>(n, 0, pmod);
      cout << "pmod = " << pmod << endl;
      for (int j = 0; j < pmod; ++j) {
        // number of prime such that p % pmod = j
        cout << "p % " << pmod << " = " << j << " " << ans0[j][n] << " "
             << ans1[j][n] << endl;
      }
      cout << endl;
    }
    cout << endl;
  }
}

void prime_pmod_s1() {
  for (int64 n = 1; n <= 1000000000; n *= 10) {
    cout << "n = " << n << endl;
    for (int pmod = 2; pmod <= 7; ++pmod) {
      auto ans0 = PrimeS1PMod<MT>(n, pmod);
      auto ans1 = PrimeSkPMod<mod>(n, 1, pmod);
      cout << "pmod = " << pmod << endl;
      for (int j = 0; j < pmod; ++j) {
        // sum of prime such that p % pmod = j
        cout << "p % " << pmod << " = " << j << " " << ans0[j][n] << " "
             << ans1[j][n] << endl;
      }
      cout << endl;
    }
    cout << endl;
  }
}

int main() {
  PE_INIT(maxp = 2000000);

  prime_s0();
  prime_s1();
  prime_pmod_s0();
  prime_pmod_s1();
  return 0;
}