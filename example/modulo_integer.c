const int maxp = 100000;
#include <pe>
template<typename S>
struct mtype_policy<S, bn>
{
  typedef S   ints;
  typedef bn   intb;
  static ints add(ints a, ints b, ints mod)
  {
    intb c = (intb)a + b;
    if (c >= mod) c -= mod;
    return c.ToInt();
  }
  static ints sub(ints a, ints b, ints mod)
  {
    intb c = (intb)a - b;
    if (c < 0) c += mod;
    return c.ToInt();
  }
  static ints mul(ints a, ints b, ints mod)
  {
    intb c = (intb)a * b % mod;
    return c.ToInt();
  }
};

template<>
struct mtype_policy<bn, bn>
{
  typedef bn   ints;
  typedef bn   intb;
  static ints add(ints a, ints b, ints mod)
  {
    intb c = (intb)a + b;
    if (c >= mod) c -= mod;
    return c.ToInt();
  }
  static ints sub(ints a, ints b, ints mod)
  {
    intb c = (intb)a - b;
    if (c < 0) c += mod;
    return c.ToInt();
  }
  static ints mul(ints a, ints b, ints mod)
  {
    intb c = (intb)a * b % mod;
    return c.ToInt();
  }
};

void basic()
{
  mint3232<10005> orz;
  cerr << orz << endl;
  orz = orz * 12312321;
  cerr << mint3232<10005>(12312321) << " " << 12312321 % 10005 << endl;
  cerr << orz << endl;
  cerr << 12312321%10005 << endl;
}

const int N = 10000000;
mint3264<1000000007> u[N+5];
//mintd3264 v[N+5];
//mint<int32_t, bn, 1000000007> v[N+5];
mint<int32_t, fake_int128, 1000000007> v[N+5];
void perf()
{
  TimeRecorder tr;
  u[0] = 1;
  for (int i = 1; i <= N; ++i)
  u[i] = u[i-1] * i;
  cerr << tr.elapsed().format() << endl;
  tr.record();
  //v[0].MOD = 1000000007;
  v[0] = 1;
  for (int i = 1; i <= N; ++i)
  v[i] = v[i-1] * i;
  cerr << tr.elapsed().format() << endl;
  for (int i = 0; i <= N; ++i)
  assert(u[i].value() == v[i].value());
}
int main()
{
  perf();
  return 0;
}