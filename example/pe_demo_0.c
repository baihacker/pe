#include <pe_base>
const int maxp = 100000;
#include <pe_nt>
#include <pe_threads>
#include <cmath>
#include <iostream>
using namespace std;
int main()
{
  int orz = 1;
  int64 value = 10;
  for (; orz <= 14; ++orz, value *= 10)
  {
    cerr << 1.*pmpi[orz] / estimate_pmpi(value) << endl;
  }
  
  init_primes();
  
  // factorize
  auto f = factorize(303741899);
  foreach(it, f) cerr << it.first << " " << it.second << endl;
  auto fs = get_factors(65700);
  sort(all(fs));
  foreach(it, fs) cerr << it << endl;
  
  // prime test
  volatile int now = clock();
  int cnt = 0;
  for (int i = 10000; i >= 2; --i)
  {
    int u = is_prime(i);
    int v = is_prime_ex(i);
    if (u != v)
    {
      cerr << "error:\t";
      dbg(i);
    }
    //cerr << i << " " << is_prime(i) << " " << is_prime_ex(i) << endl;
    cnt += is_prime(i);
  }
  dbg(cnt);
  cerr << "a\t" << clock() - now << endl;
  cnt = 0;
  now = clock();
  for (int i = 100001; i <= 10000000; ++i)
  {
    cnt += is_prime_ex(i);
  }
  dbg(cnt);
  cerr << "b\t" << clock() - now << endl;

  // random number generator
  srand(time(NULL));
  auto gen = make_uniform_generator(rand(), -7, 10);
  for (int i = 0; i < 20; ++i)
  cerr << gen() << " ";
  cerr << endl;
  return 0;
}