#include <pe>

int main()
{
  bn N = 1;
  for (int i = 1; i <= 20; ++i) N *= i;

  bn v = 0;
  for (bn n = N; !n.IsZero(); v += n, n = n / 2);
  
  bn mod = bn(2) ^ 48;
  bn ans = 1;
  FacModer moder(2, 48);
  int i = 0;
  while (N > 1) {
    ans = ans * moder.cal(N) % mod;
    N = N / 2;
  }
  v = v % 4;
  
  ans = ans * (bn(2) ^ v[0]) % mod;

  cout << ans << endl;
  cout << ans.ToInt() << endl;
  printf("%I64x\n", (int64)ans.ToInt());
  return 0;
}