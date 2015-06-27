const int maxp = 100000;
#include <pe>

int main()
{
  init_primes();
  
  BEGIN_PARALLEL
    FROM 1 TO 100000000 EACH_BLOCK_IS 10000000 CACHE ""
    MAP
      {
        return is_prime_ex(key);
      }
    REDUCE
      {
        result += value;
        return result;
      }
    GO 10
  END_PARALLEL
  
  return 0;
}