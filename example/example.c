#include <pe.hpp>

const int n = 1000000;
const int64 mod = 1000000007;

void prime_sum() {
  int64 sum0 = 0;
  for (int i = 0; i < pcnt; ++i) {
    int p = plist[i];
    if (p > n) break;
    sum0 += p;
  }

  int64 sum1 = prime_s1<int64>(n)[n];
  int64 sum2 = prime_s1_ex<int64>(n)[n];
  int64 sum3 = prime_s1_parallel<int64>(n)[n];

  cout << sum0 << " " << sum1 << " " << sum2 << " " << sum3 << endl;
}

void mu_sum() {
  int64 sum0 = 0;
  for (int i = 1; i <= n; ++i) {
    sum0 += cal_mu(i);
  }

  int64 sum1 = MuPhiSumModer(316227766016779, 10000).get_sum_mu(n);

  cout << sum0 << " " << sum1 << endl;
}

void phi_sum() {
  int64 sum0 = 0;
  for (int i = 1; i <= n; ++i) {
    sum0 += cal_phi(i);
  }

  int64 sum1 = MuPhiSumModer(316227766016779, 10000).get_sum_phi(n);

  cout << sum0 << " " << sum1 << endl;
}

void squere_free_number_count() {
  int64 count0 = 0;
  for (int i = 1; i <= n; ++i) {
    int ok = 1;
    for (auto& iter : factorize(i)) {
      if (iter.second > 1) {
        ok = 0;
        break;
      }
    }
    count0 += ok;
  }

  int64 count1 = SFCounter(10000).get(n);

  cout << count0 << " " << count1 << endl;
}

void linear_recurrence() {
  cout << nth_element({1, 2, 4, 8, 16, 32, 64}, mod, 7) << endl;
  cout << nth_element({1, 1, 2, 3, 5, 8, 13}, mod, 10000000000) << endl;
}

void power_sum() {
  PowerSumModer a(mod);
  PowerSumModerB b(mod);
  PowerSumModerB1 c(mod);

  // (1^127+2^127+...+(1e18)^127) % mod
  cout << a.cal(1e18, 127) << " " << b.cal(1e18, 127) << " " << c.cal(1e18, 127)
       << endl;
}

void matrix_power() {
  // m^n, m is a k*k matrix
  cout << power_mod([](auto& m) { m(0, 0) = m(0, 1) = m(1, 0) = 1; }, 2,
                    10000000000, mod)
       << endl;

  // m^n*v, m is a k*k matrix
  cout << power_mod(
              [](auto& m, auto& v) {
                m(0, 0) = m(0, 1) = m(1, 0) = 1;
                v[0] = v[1] = 1;
              },
              2, 10000000000, mod)
       << endl;
}

void big_number() {
  cout << power(2_bi, 128) << endl; // BigInteger
  cout << power(2_mpi, 128) << endl; // MpInteger, a wrapper of mpz_class
  cout << power(MPZ(2), 128) << endl; // Helper function of mpz_class
}

void fraction() {
  Fraction<bi> x;
  for (int i = 1; i < 100; ++i) {
    x = x + Fraction<bi>(1, i);
  }
  // 1/1 + 1/2 + 1/3 + ... + 1/99
  cout << x << endl;
}

void modular() {
  NMod64<mod> x = 1;
  for (int i = 1; i <= 10000; ++i) x *= i;

  cout << x << " " << FactModer(mod).cal(10000) << endl;
}


void multi_precision_float() {
  Mpf::setDefaultPrec(2000);
  Mpf x;
  Mpf one(1);
  for (int i = 1; i <= 100; ++i) {
    x += one / i;
  }
  // The same as SetPrecision[Sum[1/i, {i, 1, 100}], 100]
  cout << x.toString(100) << endl;
}

void poly_mul_example() {
  vector<int64> a, b;
  for (int i = 0; i < 100000; ++i)
    a.pb(i), b.pb(i);
  auto c = poly_mul(a, b, mod);
  cout << c[12345] << endl;
}

int main() {
  pe().maxPrime(2000000).calMu().calPhi().init();

  prime_sum();
  mu_sum();
  phi_sum();
  squere_free_number_count();
  linear_recurrence();
  power_sum();
  matrix_power();
  big_number();
  fraction();
  modular();
  multi_precision_float();
  poly_mul_example();

  return 0;
}