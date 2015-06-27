const int maxp = 100000;
#include <pe>

int main()
{
  init_primes();

  BEGIN_PARALLEL
    FROM 1 TO 100000000 EACH_BLOCK_IS 10000000 CACHE ""
    THREADS 10
    MAP
      {
        return is_prime_ex(key);
      }
    REDUCE
      {
        result += value;
        return result;
      }
  END_PARALLEL

  return 0;
}