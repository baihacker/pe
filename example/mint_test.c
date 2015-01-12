const int maxp = 100000;
#include <pe>

void basic()
{
  mint3232<10005> orz;
  cerr << orz << endl;
  orz = orz * 12312321;
  cerr << mint3232<10005>(12312321) << " " << 12312321 % 10005 << endl;
  cerr << orz << endl;
  cerr << 12312321%10005 << endl;
}
mint3264<1000000007> u[1000000+5];
mintd3264 v[1000000+5];
void perf()
{
  TimeRecorder tr;
  u[0] = 1;
  for (int i = 1; i <= 1000000; ++i)
  u[i] = u[i-1] * i;
  cerr << tr.elapsed().format() << endl;
  tr.record();
  v[0].MOD = 1000000007;
  v[0] = 1;
  for (int i = 1; i <= 1000000; ++i)
  v[i] = v[i-1] * i;
  cerr << tr.elapsed().format() << endl;
  for (int i = 0; i <= 1000000; ++i)
  assert(u[i].value() == v[i].value());
}
int main()
{
  perf();
  return 0;
}