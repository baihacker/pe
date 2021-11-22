#include <pe.hpp>

const int n = 1000000;
const int64 mod = 1000000007;

#define output(x) cout << (#x) << ": " << (x) << endl

#define ExecuteAndPrint1(method, arg) \
  cout << (#method) << "(" << arg << "): " << method(arg) << endl

#define ExecuteAndPrint2(method, arg1, arg2)       \
  cout << (#method) << "(" << arg1 << ", " << arg2 \
       << "): " << method(arg1, arg2) << endl

void NtBaseExample() {
  output(pcnt);
  for (int i = 0; i < 10; ++i) {
    output(plist[i]);
  }
  cout << endl;
  for (int64 i = -2; i <= 10; ++i) {
    cout << i << endl;
    ExecuteAndPrint1(IsPrime, i);
    ExecuteAndPrint1(IsPrimeEx, i);
    ExecuteAndPrint1(Factorize, i);
    ExecuteAndPrint1(GetFactors, i);
    ExecuteAndPrint1(GetPrimeFactors, i);
    ExecuteAndPrint1(GetRadFactors, i);
    cout << "GetRadFactorsWithMu(i)): ";
    int idx = 0;
    for (auto& iter : GetRadFactorsWithMu(i)) {
      if (idx++) {
        cout << ", ";
      }
      cout << "(" << iter.first << ", " << iter.second << ")";
    }
    cout << endl;
    ExecuteAndPrint1(CalPhi, i);
    ExecuteAndPrint1(CalMu, i);
    ExecuteAndPrint1(CalRad, i);
    cout << endl;
  }
}

void PrimeSieveExample() {
  int idx = 0;
  // Iterate the primes in the range.
  for (auto i : PrimeEnumerator<int64>(10, 40)) {
    if (idx++) {
      cout << ", ";
    }
    cout << i;
  }
  cout << endl;

  // Return a vector whose elements are the primes in the range.
  cout << GetPrimesInRange(10, 40) << endl;
}

void NtExample() {
  for (int64 i = 1; i <= 10; ++i) {
    for (int64 offset = -1; offset <= 1; ++offset) {
      const int64 n = i * i + offset;
      // Integer part of the square root.
      ExecuteAndPrint1(SqrtI, n);
    }
  }
  for (int64 i = 1; i <= 10; ++i) {
    for (int64 d = 1; d <= 5; ++d) {
      const int64 n = i * i;
      // Integer part of the dth root.
      ExecuteAndPrint2(RootI, n, d);
    }
  }

  // Gcd of one number is the number itself.
  cout << Gcd(24) << endl;
  // Gcd of two numbers.
  cout << Gcd(24, 36) << endl;
  // Gcd of more than 2 numbers.
  cout << Gcd(24, 36, 52) << endl;

  {
    auto t = ExGcd<int>(24, 36);
    cout << get<1>(t) * 24 + get<2>(t) * 36 << " = " << get<0>(t) << endl;
  }

  for (int i = 1; i < 7; ++i) {
    // Calculate the inverse and the programme exits if it doesn't exist.
    int ans = ModInv(i, 7);
    cout << ans * i % 7 << endl;  // Expected to be 1
  }

  {
    // Solve 3 * x = 4 (mod 7)
    // The solutions are x + k m where k is an integer.
    int x, m;
    // Whether the solution exist
    int have = SolveLinearEquation(3, 4, 7, x, m);
    // Output 1 4 7 since the solutions are 6 + k * 7.
    cout << have << " " << x << " " << m << endl;
  }
  {
    // Solve x = 1 (mod 3), x = 2 (mod 5)
    auto t = Crt2<int64>(1, 3, 2, 5);
    auto have = get<0>(t);
    // The solution are x + k m where k is an integer.
    auto x = get<1>(t);
    auto m = get<2>(t);
    // Output 1 7 15 since the solutions are 7 + k * 15.
    cout << have << " " << x << " " << m << endl;

    // CrtN(const T* val, const T* mod, int n) is also available.
  }

  for (int i = 1; i <= 10; ++i) {
    ExecuteAndPrint1(IsSquareFree, i);
  }
}

void PrimeSumExample() {
  int64 sum0 = 0;
  for (int i = 0; i < pcnt; ++i) {
    int p = plist[i];
    if (p > n) break;
    sum0 += p;
  }

  int64 sum1 = PrimeS1<int64>(n)[n];
  int64 sum2 = PrimeS1Ex<int64>(n)[n];
  int64 sum3 = PrimeS1Parallel<int64>(n)[n];

  cout << sum0 << " " << sum1 << " " << sum2 << " " << sum3 << endl;
}

void MuSumExample() {
  int64 sum0 = 0;
  for (int i = 1; i <= n; ++i) {
    sum0 += CalMu(i);
  }

  int64 sum1 = MuPhiSumModer(316227766016779, 10000).CalSumMu(n);

  cout << sum0 << " " << sum1 << endl;
}

void PhiSumExample() {
  int64 sum0 = 0;
  for (int i = 1; i <= n; ++i) {
    sum0 += CalPhi(i);
  }

  int64 sum1 = MuPhiSumModer(316227766016779, 10000).CalSumPhi(n);

  cout << sum0 << " " << sum1 << endl;
}

void SquareFreeNumberExample() {
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

  cout << count0 << " " << count1 << endl;
}

void LinearRecurrenceExample() {
  cout << NthElement({1, 2, 4, 8, 16, 32, 64}, mod, 7) << endl;
  cout << NthElement({1, 1, 2, 3, 5, 8, 13}, mod, 10000000000) << endl;
}

void PowerSumExample() {
  PowerSumModer a(mod);
  PowerSumModerB b(mod);
  PowerSumModerB1 c(mod);

  // (1^127+2^127+...+(1e18)^127) % mod
  cout << a.Cal(1e18, 127) << " " << b.Cal(1e18, 127) << " " << c.Cal(1e18, 127)
       << endl;
}

void MatrixPowerExample() {
  // m^n, m is a k*k matrix
  cout << MatrixPowerMod([](auto& m) { m(0, 0) = m(0, 1) = m(1, 0) = 1; }, 2,
                         10000000000, mod)
       << endl;

  // m^n*v, m is a k*k matrix
  cout << MatrixPowerMod(
              [](auto& m, auto& v) {
                m(0, 0) = m(0, 1) = m(1, 0) = 1;
                v[0] = v[1] = 1;
              },
              2, 10000000000, mod)
       << endl;
}

void BigNumberExample() {
  cout << Power(2_bi, 128) << endl;  // BigInteger
#if ENABLE_GMP
  cout << Power(2_mpi, 128) << endl;   // MpInteger, a wrapper of mpz_class
  cout << Power(MPZ(2), 128) << endl;  // Helper function of mpz_class
#endif
}

void FractionExample() {
  Fraction<bi> x;
  for (int i = 1; i < 100; ++i) {
    x = x + Fraction<bi>(1, i);
  }
  // 1/1 + 1/2 + 1/3 + ... + 1/99
  cout << x << endl;
}

void ModularArithmeticExample() {
  NMod64<mod> x = 1;
  for (int i = 1; i <= 10000; ++i) x *= i;

  cout << x << " " << FactModer(mod).Cal(10000) << endl;
}

void MultiprecisionFloatNumberExample() {
#if HAS_MPF
  Mpf::SetDefaultPrec(2000);
  Mpf x;
  Mpf one(1);
  for (int i = 1; i <= 100; ++i) {
    x += one / i;
  }
  // The same as SetPrecision[Sum[1/i, {i, 1, 100}], 100]
  cout << x.ToString(100) << endl;
#endif
}

void PolynomialMultiplicationExample() {
  vector<int64> a, b;
  for (int i = 0; i < 100000; ++i) a.pb(i), b.pb(i);
  auto c = PolyMul(a, b, mod);
  cout << c[12345] << endl;
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
          cal_mu = 1       // Initialize mu[i] fro i <= maxp
  );

  NtBaseExample();
  PrimeSieveExample();
  NtExample();
  PrimeSumExample();
  MuSumExample();
  PhiSumExample();
  SquareFreeNumberExample();
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