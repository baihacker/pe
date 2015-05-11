const int maxp = 10000000;
#include <pe_base>
#include <pe_nt>
#include <pe_threads>
#include <pe_util>

// example of MultiThreadsTask
struct CalPI : public MultiThreadsTask<CalPI>
{
using MultiThreadsTask<CalPI>::MultiThreadsTask;

void update_result(int64 local_result)
{
  result_ += local_result;
}

void work(int64 input, int64& local_result, int worker_idx)
{
  local_result += is_prime_ex(input);
}
};

int main()
{
  init_primes();
  auto runner = CalPI(TaskDivision(1, 100000000, 1000000));
  int64 result = runner.run(4);
  cerr << "expected : " << pmpi[8] << endl;
  cerr << "received : " << runner.value() << endl;
  return 0;
}