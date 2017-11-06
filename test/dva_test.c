#include <pe.hpp>

int main()
{
  init_primes();

  auto orz = prime_pi(10000);
  dbg(orz[10000]); // 455052511LL
  
  orz = prime_pi(100000000);
  dbg(orz[100000000]); // 5761455LL
  
  orz = prime_s1(10000);
  dbg(orz[10000]);
  
  int64 s = 0;
  for (int i = 2; i <= 10000; ++i) if (is_prime(i)) s += i;
  dbg(s);
  
  orz = prime_s1(1000000);
  dbg(orz[1000000]);
  
  s = 0;
  for (int i = 2; i <= 1000000; ++i) if (is_prime(i)) s += i;
  dbg(s);
  return 0;
}