#include <pe>

int main()
{
  bn N = 1;
  for (int i = 1; i <= 20; ++i) N *= i;

  bn v = 0;
  for (bn n = N; !n.isZero(); n = n / 2, v += n);

  bn mod = power(bn(2), 48);
  bn ans = 1;
  FacModer moder(2, 48);
  int i = 0;
  while (N > 1) {
    ans = ans * moder.cal(N) % mod;
    N = N / 2;
  }
  v = v % 4;
  
  ans = ans * (power(bn(2), v[0])) % mod;

  cout << ans << endl;
  cout << ans.toInt<int64>() << endl;
  printf("%I64x\n", (int64)ans.toInt<int64>());
  return 0;
}