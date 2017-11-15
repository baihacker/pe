/*
  Project Euler 483	Repeated permutation
  reference answer:
  N = 100:           53817203945.52453
  output:            53817203945
  N = 150:           55335570173801.14
  output:            55335570173801
*/
#define USE_BIG_NUMBER
#include <pe.hpp>
const int N = 100;
bn choose[505][505];
bn fac[500];
void init()
{
  for (int i = 0; i <= 500; ++i)
  for (int j = 0; j <= i; ++j)
  choose[i][j] = (j == i || j == 0) ? 1 : choose[i-1][j] + choose[i-1][j-1];
  fac[0] = 1;
  for (int i = 1; i <= 500; ++i)
  fac[i] = fac[i-1] * i;
}
bn dp[N+1];
map<int128, bn> orz[N+1];
int main()
{
  init();
  dp[0] = 1;
  orz[0][1] = 1;
  for (int i = 1; i <= N; ++i)
  {
    cerr << i << " ";
    for (int j = N; j >= i; --j)
    {
      bn total = 0;
      map<int128, bn> inc;
      const int curr_step = i;
      const int n = j;
      for (int x = 1; x * curr_step <= n; ++x)
      {
        bn t = 1, u = 1;
        for (int i = 0, j = n; i < x; ++i)
        {
          t = t * choose[j][curr_step];
          j -= curr_step;
          u = u * fac[curr_step-1];
        }
        bn magic = t / fac[x] * u;
        total += magic * dp[n-curr_step*x];
        foreach(it, orz[n-x*curr_step])
        {
          int128 d = __gcd((int128)curr_step, it.first);
          int128 now = curr_step / d * it.first;
          inc[now] += it.second * magic;
        }
      }
      dp[n] += total;
      foreach(it, inc)
      orz[n][it.first] += it.second;
    }
    cerr << dp[N] << endl;
  }
  
  cerr << dp[N] << endl;
  bn s = 0;
  foreach(it, orz[N])
  s += bn(it.first) * bn(it.first) * it.second;
  cerr << s / dp[N] << endl;
  return 0;
}
