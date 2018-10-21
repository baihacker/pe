#include <pe.hpp>

int main() {
  pe().maxPrime(1000000).init();

  int64 result = PARALLEL_RESULT(
  BEGIN_PARALLEL
    FROM 1 TO 100000000 EACH_BLOCK_IS 10000000 CACHE ""
    THREADS 10
    MAP {
        return is_prime_ex(key);
      }
    REDUCE {
        result += value;
        return result;
      }
  END_PARALLEL);
  dbg(result);

  return 0;
}