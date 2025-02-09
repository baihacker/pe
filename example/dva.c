#include <pe.hpp>
using namespace std;
using namespace pe;

const int64 mod = 1000000007;
using MT = NMod64<mod>;

int main() {
  PE_INIT(maxp = 10000000, cal_phi = 1, cal_mu = 1);
  const int64 n = 10000000;

  auto eps = MakePrefixSumEpsilon<MT>(n);
  auto one = MakePrefixSumOne<MT>(n);
  auto mu = MakePrefixSumMu<MT>(n);
  auto id = MakePrefixSumId<MT>(n);
  auto phi = MakePrefixSumPhi<MT>(n);
  {
    // eps = one * mu
    auto t = DVAConv<MT>(one, mu);
    for (int i = 1; i < t.key_size; ++i) {
      PE_ASSERT(t.values[i] == eps.values[i]);
      // cout << t.values[i] << " " << eps.values[i] << endl;
      // cout << (t.values[i] == eps.values[i]) << endl;
    }
  }
  {
    // phi = mu * id
    auto t = DVAConv<MT>(mu, id);
    for (int i = 1; i < t.key_size; ++i) {
      PE_ASSERT(t.values[i] == phi.values[i]);
    }
  }
  {
    // id = one * phi
    auto t = DVAConv<MT>(one, phi);
    for (int i = 1; i < t.key_size; ++i) {
      PE_ASSERT(t.values[i] == id.values[i]);
    }
  }

  // Divisor count
  DVA<MT> d0(n);
  {
    for (int i = 1; i <= n; ++i) {
      int64 me = 1;
      for (auto iter : Factorize(i)) me *= iter.second + 1;
      d0[i] += me;
    }
    for (int i = 1; i < d0.key_size; ++i) d0.values[i] += d0.values[i - 1];
  }
  {
    // d0 = one * one
    // d0 = id0 * one
    auto t = DVAConv<MT>(one, one);
    for (int i = 1; i < t.key_size; ++i) {
      PE_ASSERT(t.values[i] == d0.values[i]);
    }
  }

  // Divisor sum
  DVA<MT> d1(n);
  {
    for (int i = 1; i <= n; ++i) {
      int64 me = 0;
      for (auto iter : GetFactors(i)) me += iter;
      d1[i] += me;
    }
    for (int i = 1; i < d1.key_size; ++i) d1.values[i] += d1.values[i - 1];
  }
  {
    // d1 = id * one
    // d1 = id1 * one
    auto t = DVAConv<MT>(id, one);
    for (int i = 1; i < t.key_size; ++i) {
      PE_ASSERT(t.values[i] == d1.values[i]);
    }
  }

  // Divisor square sum
  DVA<MT> d2(n);
  {
    for (int i = 1; i <= n; ++i) {
      int64 me = 0;
      for (auto iter : GetFactors(i)) me += iter * iter;
      d2[i] += me;
    }
    for (int i = 1; i < d2.key_size; ++i) d2.values[i] += d2.values[i - 1];
  }
  DVA<MT> id2(n);
  {
    for (int i = 1; i < id2.key_size; ++i) {
      id2.values[i] = P2SumMod(id2.keys[i], mod);
    }
  }
  {
    // d2 = id2 * one
    auto t = DVAConv<MT>(id2, one);
    for (int i = 1; i < t.key_size; ++i) {
      PE_ASSERT(t.values[i] == d2.values[i]);
    }
  }

  // Prime omega
  // https://en.wikipedia.org/wiki/Prime_omega_function
  DVA<MT> omega(n);
  {
    for (int i = 1; i <= n; ++i) {
      omega[i] += Factorize(i).size();
    }
    for (int i = 1; i < omega.key_size; ++i)
      omega.values[i] += omega.values[i - 1];
  }
  {
    // omega = primeq * one
    auto t = DVAConv<MT>(PrimeS0<MT>(n), one);
    for (int i = 1; i < t.key_size; ++i) {
      PE_ASSERT(t.values[i] == omega.values[i]);
    }
  }
  return 0;
}

// https://en.wikipedia.org/wiki/Arithmetic_function#Relations_among_the_functions