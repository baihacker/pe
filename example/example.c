#include <pe.hpp>
using namespace pe;

const int n = 1000000;
const int64 mod = 1000000007;

#define output(x) std::cout << (#x) << ": " << (x) << std::endl

#define ExecuteAndPrint1(method, arg) \
  std::cout << (#method) << "(" << arg << "): " << method(arg) << std::endl

#define ExecuteAndPrint2(method, arg1, arg2)            \
  std::cout << (#method) << "(" << arg1 << ", " << arg2 \
            << "): " << method(arg1, arg2) << std::endl

void NtBaseExample() {
  std::cout << std::endl << __func__ << std::endl;

  {
    // pcnt is the number of primes no more than 2000000.
    output(pcnt);
    // Print the first 10 primes.
    for (int i = 0; i < 10; ++i) {
      output(plist[i]);
    }
    std::cout << std::endl;
  }

  // Print value of fundamental arithmetic function and other functions
  for (int64 i = -2; i <= 10; ++i) {
    std::cout << i << std::endl;
    ExecuteAndPrint1(IsPrime, i);
    ExecuteAndPrint1(IsPrimeEx, i);
    ExecuteAndPrint1(Factorize, i);
    ExecuteAndPrint1(GetFactors, i);
    ExecuteAndPrint1(GetPrimeFactors, i);
    ExecuteAndPrint1(GetRadFactors, i);
    std::cout << "GetRadFactorsWithMu(i)): ";
    int idx = 0;
    for (auto& iter : GetRadFactorsWithMu(i)) {
      if (idx++) {
        std::cout << ", ";
      }
      std::cout << "(" << iter.first << ", " << iter.second << ")";
    }
    std::cout << std::endl;
    ExecuteAndPrint1(CalPhi, i);
    ExecuteAndPrint1(CalMu, i);
    ExecuteAndPrint1(CalRad, i);
    std::cout << std::endl;
  }

  // Prime sieve
  {
    int idx = 0;
    // Iterate the primes in the range and print them.
    for (auto i : PrimeEnumerator<int64>(10, 40)) {
      if (idx++) {
        std::cout << ", ";
      }
      std::cout << i;
    }
    std::cout << std::endl;

    // Output a vector whose elements are the primes in the range.
    std::cout << GetPrimesInRange(10, 40) << std::endl;
  }
}

void NtExample() {
  std::cout << std::endl << __func__ << std::endl;

  for (int64 i = 1; i <= 10; ++i) {
    for (int64 offset = -1; offset <= 1; ++offset) {
      const int64 n = i * i + offset;
      // Integer part of the square root.
      ExecuteAndPrint1(SqrtI, n);
    }
  }

  for (int64 i = 1; i <= 10; ++i) {
    for (int d = 1; d <= 5; ++d) {
      const int64 n = i * i;
      // Integer part of the dth root.
      ExecuteAndPrint2(RootI, n, d);
    }
  }

  // Gcd of one number is the number itself.
  std::cout << Gcd(24) << std::endl;
  // Gcd of two numbers.
  std::cout << Gcd(24, 36) << std::endl;
  // Gcd of more than 2 numbers.
  std::cout << Gcd(24, 36, 52) << std::endl;

  {
    auto t = ExGcd<int>(24, 36);
    // x * 24 + y * 36 = d (greatest common divisor)
    auto d = std::get<0>(t);
    auto x = std::get<1>(t);
    auto y = std::get<2>(t);
    // -1 1 12
    std::cout << x << " " << y << " " << d << std::endl;
    std::cout << x * 24 + y * 36 << std::endl;
  }

  for (int i = 1; i < 7; ++i) {
    // Calculate the inverse and the programme exits if it doesn't exist.
    int ans = ModInv(i, 7);
    std::cout << ans * i % 7 << std::endl;  // Expected to be 1
  }

  {
    // Solve 3 * x = 4 (mod 7)
    auto t = SolveLinearEquation(3, 4, 7);
    // Whether the solution exists.
    auto have = std::get<0>(t);
    // The solutions are x + k m where k is an integer.
    auto x = std::get<1>(t);
    auto m = std::get<2>(t);
    // Output 1 4 7 since the solutions are 6 + k * 7.
    std::cout << have << " " << x << " " << m << std::endl;
  }

  {
    // Solve x = 1 (mod 3), x = 2 (mod 5)
    // Whether the solution exists.
    int have;
    // The solution are x + k m where k is an integer.
    int64 x, m;
    std::tie(have, x, m) = Crt2<int64>(1, 3, 2, 5);
    // Output 1 7 15 since the solutions are 7 + k * 15.
    std::cout << have << " " << x << " " << m << std::endl;

    // CrtN(const T* val, const T* mod, int n) is also available.
  }

  for (int i = 1; i <= 10; ++i) {
    ExecuteAndPrint1(IsSquareFree, i);
  }

  {
    int64 invs[10];
    InitInv(invs, 9, mod);
    for (int i = 1; i <= 9; ++i) {
      std::cout << invs[i] * i % mod << std::endl;  // Expected to be 1.
    }
  }

  {
    int64 comb[10][10];
    InitComb(comb, 9);
    std::cout << comb[5][2] << std::endl;  // Binomial(5, 2) = 10.
  }

  {
    int64 fac[10];
    int64 ifac[10];
    InitSeqProd2<int64>(fac, ifac, 1, 9, mod);
    std::cout << fac[5] << std::endl;  // 5! = 120
    for (int i = 1; i <= 9; ++i) {
      std::cout << fac[i] * ifac[i] % mod << std::endl;  // Expected to be 1.
    }
  }

  {
    // Output 4 and 9 since 4^2 = 9^2 = 3 (mod 13)
    std::cout << SquareRootMod(3, 13) << std::endl;
  }

  {
    // Output 7, 8, 11 since 7^3 = 8^3 = 11^3 = 5 (mod 13)
    std::cout << RootMod(5, 3, 13) << std::endl;
  }
}

void PrimeCountExample() {
  std::cout << std::endl << __func__ << std::endl;

  {
    int64 count0 = 0;
    for (int i = 1; i <= n; ++i)
      if (IsPrimeEx(i)) {
        ++count0;
      }

    // A DVA structure that hold the prime count for 1, 2, 3, ..., n/3, n/2,
    // n/1.
    int64 count1 = PrimeS0<int64>(n)[n];
    int64 count2 = PrimeS0Ex<int64>(n)[n];
    int64 count3 = PrimeS0Parallel<int64>(n)[n];
    // An object that can be used to calculate prime count for different n.
    int64 count4 = CachedPi().Cal(n);

    std::cout << count0 << " " << count1 << " " << count2 << " " << count3
              << " " << count4 << std::endl;
  }
  {
    int64 count01 = 0;  // p % 4 = 1
    int64 count03 = 0;  // p % 4 = 3
    for (int i = 1; i <= n; ++i)
      if (IsPrimeEx(i)) {
        if (i % 4 == 1) ++count01;
        if (i % 4 == 3) ++count03;
      }
    auto result = PrimeS0PMod<int64>(n, 4);
    std::cout << count01 << " " << result[1][n] << std::endl;
    std::cout << count03 << " " << result[3][n] << std::endl;
  }
}

void PrimeSumExample() {
  std::cout << std::endl << __func__ << std::endl;

  {
    int64 sum0 = 0;
    for (int i = 1; i <= n; ++i)
      if (IsPrimeEx(i)) {
        sum0 += i;
      }

    int64 sum1 = PrimeS1<int64>(n)[n];
    int64 sum2 = PrimeS1Ex<int64>(n)[n];
    int64 sum3 = PrimeS1Parallel<int64>(n)[n];

    std::cout << sum0 << " " << sum1 << " " << sum2 << " " << sum3 << std::endl;
  }
  {
    int64 sum01 = 0;  // p % 4 = 1
    int64 sum03 = 0;  // p % 4 = 3
    for (int i = 1; i <= n; ++i)
      if (IsPrimeEx(i)) {
        if (i % 4 == 1) sum01 += i;
        if (i % 4 == 3) sum03 += i;
      }
    auto result = PrimeS1PMod<int64>(n, 4);
    std::cout << sum01 << " " << result[1][n] << std::endl;
    std::cout << sum03 << " " << result[3][n] << std::endl;
  }
}

void PrimePowerSumExample() {
  std::cout << std::endl << __func__ << std::endl;

  {
    const int E = 7;
    int64 sum0 = 0;
    for (int i = 1; i <= n; ++i)
      if (IsPrimeEx(i)) {
        sum0 += PowerMod(i, E, mod);
      }
    sum0 %= mod;

    int64 sum1 = PrimeSkEx<mod>(n, E)[n];

    std::cout << sum0 << " " << sum1 << std::endl;
  }
  {
    const int E = 7;
    int64 sum01 = 0;  // p % 4 = 1
    int64 sum03 = 0;  // p % 4 = 3
    for (int i = 1; i <= n; ++i)
      if (IsPrimeEx(i)) {
        if (i % 4 == 1) sum01 += PowerMod(i, E, mod);
        if (i % 4 == 3) sum03 += PowerMod(i, E, mod);
      }
    sum01 %= mod;
    sum03 %= mod;
    auto result = PrimeSkPMod<mod>(n, E, 4);
    std::cout << sum01 << " " << result[1][n] << std::endl;
    std::cout << sum03 << " " << result[3][n] << std::endl;
  }
}

void MuSumExample() {
  std::cout << std::endl << __func__ << std::endl;

  int64 sum0 = 0;
  for (int i = 1; i <= n; ++i) {
    sum0 += CalMu(i);
  }

  int64 sum1 = MuPhiSumModer(316227766016779, 10000).CalSumMu(n);

  std::cout << sum0 << " " << sum1 << std::endl;
}

void PhiSumExample() {
  std::cout << std::endl << __func__ << std::endl;

  int64 sum0 = 0;
  for (int i = 1; i <= n; ++i) {
    sum0 += CalPhi(i);
  }

  int64 sum1 = MuPhiSumModer(316227766016779, 10000).CalSumPhi(n);

  std::cout << sum0 << " " << sum1 << std::endl;
}

void SquareFreeNumberCountExample() {
  std::cout << std::endl << __func__ << std::endl;

  int64 count0 = 0;
  for (int i = 1; i <= n; ++i) {
    int ok = 1;
    for (auto& iter : Factorize(i)) {
      if (iter.second > 1) {
        ok = 0;
        break;
      }
    }
    count0 += ok;
  }

  int64 count1 = SFCounter(10000).Cal(n);

  std::cout << count0 << " " << count1 << std::endl;
}

void LinearRecurrenceExample() {
  std::cout << std::endl << __func__ << std::endl;

  {
    // Find the linear recurrence automatically and output the nth element.
    std::cout << NthElement({1, 2, 4, 8, 16, 32, 64}, mod, 7) << std::endl;
    std::cout << NthElement({1, 1, 2, 3, 5, 8, 13}, mod, 10000000000)
              << std::endl;
  }
  {
    // a[i+1] = a[i] * rec[0] + a[i-1] * rec[1] ...
    // a[0] = init[0], a[1] = init[1], ...
    std::vector<int64> rec{1, 1};
    std::vector<int64> init{1, 1};
    for (int i = 2; i <= 5; ++i) {
      std::cout << LinearRecurrence(rec, init, mod, i) << std::endl;
    }
  }
  {
    std::vector<int64> rec{1, 1};
    std::vector<int64> init{1, 1};
    for (int i = 0; i <= 5; ++i) {
      std::cout << LinearRecurrenceSum(rec, init, mod, i) << std::endl;
    }
  }
}

void PowerSumExample() {
  std::cout << std::endl << __func__ << std::endl;

  PowerSumModer a(mod);
  PowerSumModerB b(mod);
  PowerSumModerB1 c(mod);

  const int64 N = Power(10LL, 18);
  // (1^127+2^127+...+(1e18)^127) % mod
  std::cout << a.Cal(N, 127) << " " << b.Cal(N, 127) << " " << c.Cal(N, 127)
            << std::endl;
}

void MatrixPowerExample() {
  std::cout << std::endl << __func__ << std::endl;

  // m^n, m is a k*k matrix
  std::cout << MatrixPowerMod([](auto& m) { m(0, 0) = m(0, 1) = m(1, 0) = 1; },
                              2, 10000000000, mod)
            << std::endl;

  // m^n*v, m is a k*k matrix
  std::cout << MatrixPowerMod(
                   [](auto& m, auto& v) {
                     m(0, 0) = m(0, 1) = m(1, 0) = 1;
                     v[0] = v[1] = 1;
                   },
                   2, 10000000000, mod)
            << std::endl;
}

void BigNumberExample() {
  std::cout << std::endl << __func__ << std::endl;

  std::cout << Power(2_bi, 128) << std::endl;  // BigInteger
#if ENABLE_GMP
  std::cout << Power(2_mpi, 128)
            << std::endl;  // MpInteger, a wrapper of mpz_class
  std::cout << Power(MPZ(2), 128) << std::endl;  // Helper function of mpz_class
#endif
}

void FractionExample() {
  std::cout << std::endl << __func__ << std::endl;

  Fraction<bi> x;
  for (int i = 1; i < 100; ++i) {
    x = x + Fraction<bi>(1, i);
  }
  // 1/1 + 1/2 + 1/3 + ... + 1/99
  std::cout << x << std::endl;
}

void ModularArithmeticExample() {
  std::cout << std::endl << __func__ << std::endl;

  NMod64<mod> x = 1;
  for (int i = 1; i <= 10000; ++i) {
    x *= i;
  }

  std::cout << x << " " << FactModer(mod).Cal(10000) << std::endl;
}

void MultiprecisionFloatNumberExample() {
  std::cout << std::endl << __func__ << std::endl;

#if HAS_MPF
  // 2000 bit precision.
  Mpf::SetDefaultPrec(2000);
  Mpf x;
  Mpf one(1);
  for (int i = 1; i <= 100; ++i) {
    x += one / i;
  }
  // The same as SetPrecision[Sum[1/i, {i, 1, 100}], 100]
  std::cout << x.ToString(100) << std::endl;
#endif
}

void PolynomialMultiplicationExample() {
  std::cout << std::endl << __func__ << std::endl;

  std::vector<int64> a, b;
  for (int i = 0; i < 100000; ++i) {
    a.push_back(i);
    b.push_back(i);
  }
  auto c = PolyMul(a, b, mod);
  std::cout << c[12345] << std::endl;
}

// Latest compiling command:
// g++.exe" "example.c" -o a.exe --std=c++17 -fno-diagnostics-color -O3
// -march=native -mtune=native -fopenmp -lquadmath -Wl,--stack,268435456 -static
// -lbf -lgmpxx -lflint -lmpfr -lntl -lgmp -lprimesieve -lprimecount
int main() {
  PE_INIT(maxp = 2000000,  // Initialize a prime table which stores the primes
                           // no more than maxp. This table is used by many
                           // internal functions.
                           // plist points to the first element of the prime
                           // table.
                           // pcnt is the size of the table.
          cal_phi = 1,     // Initialize the phi[i] for i <= maxp
          cal_mu = 1       // Initialize mu[i] for i <= maxp
  );

  NtBaseExample();
  NtExample();
  PrimeCountExample();
  PrimeSumExample();
  PrimePowerSumExample();
  MuSumExample();
  PhiSumExample();
  SquareFreeNumberCountExample();
  LinearRecurrenceExample();
  PowerSumExample();
  MatrixPowerExample();
  BigNumberExample();
  FractionExample();
  ModularArithmeticExample();
  MultiprecisionFloatNumberExample();
  PolynomialMultiplicationExample();

  return 0;
}