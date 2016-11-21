const int maxp = 10000000;
#include <pe>

// example of MultiThreadsTask
struct CalPI : public ParallelRangeT<CalPI>
{
  int64 update_result(int64 result, int64 value)
  {
    return result + value;
  }
  int64 work_on_block(int64 first, int64 last, int64 worker)
  {
    int64 t = 0;
    for (int64 i = first; i <= last; ++i) t += is_prime_ex(i);
    return t;
  }
};

int main()
{
  init_primes();
  int64 result = CalPI().from(1).to(100000000).divided_by(10000000).threads(4).start().result();
  cerr << "expected : " << pmpi[8] << endl;
  cerr << "received : " << result << endl;
  return 0;
}