const int maxp = 1000000;
#include "pe"

int64 ps[1000001];
int64 pc[1000001];

SL void verify_cnt(const int64 n, vector<int64>& s, vector<int64>& l)
{
  int64 v = sqrt(n);
  assert(sz(s) == v + 1);
  assert(sz(l) == v + 1);
  for (int j = 1; j <= v; ++j)
  {
    assert(s[j] == pc[j]);
    assert(l[j] == pc[n/j]);
  }
}

SL void verify_sum(const int64 n, vector<int64>& s, vector<int64>& l)
{
  int64 v = sqrt(n);
  assert(sz(s) == v + 1);
  assert(sz(l) == v + 1);
  for (int j = 1; j <= v; ++j)
  {
    assert(s[j] == ps[j]);
    assert(l[j] == ps[n/j]);
  }
}

void small_test()
{
  for (int i = 1; i <= 100000; ++i)
  {
    const int n = i;
    auto c = prime_phi(i);
    verify_cnt(n, get<0>(c), get<1>(c));
    auto s = prime_sum(i);
    verify_sum(n, get<0>(s), get<1>(s));
    auto cs = prime_phi_sum(i);
    verify_cnt(n, get<0>(cs), get<1>(cs));
    verify_sum(n, get<2>(cs), get<3>(cs));
  }
}

int main()
{
  init_primes();
  for (int i = 1; i <= 1000000; ++i)
  {
    pc[i] = pc[i-1] + (is_prime(i) ? 1 : 0);
    ps[i] = ps[i-1] + (is_prime(i) ? i : 0);
  }

  small_test();

  assert(get<1>(prime_phi(10000000))[1] == pmpi[7]);
  assert(get<1>(prime_phi(100000000))[1] == pmpi[8]);
  assert(get<1>(prime_phi(1000000000))[1] == pmpi[9]);
  assert(get<1>(prime_phi(10000000000))[1] == pmpi[10]);
  assert(get<1>(prime_phi(100000000000))[1] == pmpi[11]);
  assert(get<1>(prime_phi(1000000000000))[1] == pmpi[12]);

  return 0;
}