#include <pe.hpp>

void test0() {
  NModNumber<CCMod<5>> orz(1);
  cout << sizeof(orz) << endl;
  orz = orz + 4LL;
  orz = 4 + orz;
  orz = orz + 4;
  orz = orz + orz;
  orz = orz - 4;
  orz = 4 - orz;
  orz = orz - orz;
  orz = orz * 4;
  orz = 4 * orz;
  orz = orz * orz;
  orz += 4;
  orz += orz;
  orz -= orz;
  orz *= orz;
  orz.fix_value();
  cout << orz << endl;
  dbg(orz == orz);
  dbg(orz != orz);
  dbg(orz > orz);
  dbg(orz >= orz);
  dbg(orz < orz);
  dbg(orz <= orz);
}

void test1() {
  NModNumberM<DefaultMod> orz(1);
  cout << sizeof(orz) << endl;
  orz = orz + 4LL;
  orz = 4 + orz;
  orz = orz + 4;
  orz = orz + orz;
  orz = orz - 4;
  orz = 4 - orz;
  orz = orz - orz;
  orz = orz * 4;
  orz = 4 * orz;
  orz = orz * orz;
  orz += 4;
  orz += orz;
  orz -= orz;
  orz *= orz;
  orz.fix_value();
  cout << orz << endl;
  dbg(orz == orz);
  dbg(orz != orz);
  dbg(orz > orz);
  dbg(orz >= orz);
  dbg(orz < orz);
  dbg(orz <= orz);
}

const int N = 10000000;

NMod<1000000007, APSB<int, int64>> u[N + 5];
NMod<1000000007, APSB<int, int128>> v[N + 5];
NMod<1000000007, APSB<int, fake_int128>> w[N + 5];

void perf() {
  TimeRecorder tr;

  u[0] = 1;
  for (int i = 1; i <= N; ++i) u[i] = u[i - 1] * i;
  cerr << tr.elapsed().format() << endl;

  tr.record();
  v[0] = 1;
  for (int i = 1; i <= N; ++i) v[i] = v[i - 1] * i;
  cerr << tr.elapsed().format() << endl;

  tr.record();
  w[0] = 1;
  for (int i = 1; i <= N; ++i) w[i] = w[i - 1] * i;
  cerr << tr.elapsed().format() << endl;

  for (int i = 0; i <= N; ++i) {
    assert(u[i].value() == v[i].value());
    assert(u[i].value() == w[i].value());
  }
}

int main() {
  test0();
  test1();
  perf();
  return 0;
}