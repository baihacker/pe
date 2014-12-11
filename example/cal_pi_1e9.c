const int maxp = 10000000;
#include <pe_base>
#include <pe_nt>
#include <pe_threads>
#include <pe_util>

Threads th;

int ans = 0;

void work(int id)
{
  const int start = (id - 1) * 1000000 + 1;
  const int end = id * 1000000;
  int local_ans = 0;
  
  th.lock();
  cerr << "start:\t" << id << endl;
  th.unlock();
  
  for (int i = start; i <= end; ++i)
  if (is_prime_ex(i)) ++local_ans;
  
  th.lock();
  cerr << "finish:\t" << id << endl;
  ans += local_ans;
  cerr << "global ans:\t" << ans << endl;
  th.unlock();
}

int main()
{
  init_primes();
  th.init(work, 8);
  for (int id = 1; id <= 1000; ++id)
  {
    th.wait_for_queue(3);
    th.add_request(id);
  }
  th.wait_for_end();
  
  cerr << "expected : " << pmpi[9] << endl;
  cerr << "received : " << ans << endl;
  return 0;
}