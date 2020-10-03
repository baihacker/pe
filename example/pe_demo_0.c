#include <pe.hpp>

int main() {
  int orz = 1;
  int64 value = 10;
  for (; orz <= 14; ++orz, value *= 10) {
    cerr << 1. * pmpi[orz] / EstimatePmpi(value) << endl;
  }

  InitPrimes();

  // Factorize
  auto f = Factorize(303741899);
  foreach (it, f)
    cerr << it.first << " " << it.second << endl;
  auto fs = Factors(65700);
  sort(all(fs));
  foreach (it, fs)
    cerr << it << endl;

  // prime test
  volatile int now = clock();
  int cnt = 0;
  for (int i = 10000; i >= 2; --i) {
    int u = IsPrime(i);
    int v = IsPrimeEx(i);
    if (u != v) {
      cerr << "error:\t";
      dbg(i);
    }
    // cerr << i << " " << IsPrime(i) << " " << IsPrimeEx(i) << endl;
    cnt += IsPrime(i);
  }
  dbg(cnt);
  cerr << "a\t" << clock() - now << endl;
  cnt = 0;
  now = clock();
  for (int i = 100001; i <= 10000000; ++i) {
    cnt += IsPrimeEx(i);
  }
  dbg(cnt);
  cerr << "b\t" << clock() - now << endl;

  // random number generator
  srand(time(NULL));
  auto gen = MakeUniformGenerator(rand(), -7, 10);
  for (int i = 0; i < 20; ++i) cerr << gen() << " ";
  cerr << endl;
  return 0;
}