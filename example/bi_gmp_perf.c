#include <pe.hpp>

void run() {
  mpz_class f = 1;
  TimeRecorder tr;
  for (unsigned i = 1; i <= 100000; ++i) f *= i;
  cout << tr.Elapsed().Format() << endl;

  tr.Record();
  for (unsigned i = 1; i <= 100000; ++i) f /= i;
  cout << tr.Elapsed().Format() << endl;
  cout << f << endl;

  tr.Record();
  for (unsigned i = 1; i <= 100000; ++i) f = f * 3 % 1000000007;
  cout << tr.Elapsed().Format() << endl;
  cout << f << endl;

  mpz_class modf = 1000000007;
  modf = mpz_class(modf * modf) * modf;
  tr.Record();
  for (unsigned i = 1; i <= 100000; ++i) f = f * 3 % modf;
  cout << tr.Elapsed().Format() << endl;
  cout << f << endl;

  tr.Record();
  for (unsigned i = 1; i <= 100000; ++i) f = f * f % modf;
  cout << tr.Elapsed().Format() << endl;
  cout << f << endl;

  cout << endl;

  bi g = 1;
  tr.Record();
  for (unsigned i = 1; i <= 100000; ++i) g *= i;
  cout << tr.Elapsed().Format() << endl;

  tr.Record();
  for (unsigned i = 1; i <= 100000; ++i) g /= i;
  cout << tr.Elapsed().Format() << endl;
  cout << g << endl;

  tr.Record();
  for (unsigned i = 1; i <= 100000; ++i) g = g * 3 % 1000000007;
  cout << tr.Elapsed().Format() << endl;
  cout << g << endl;

  bi modg = 1000000007;
  modg = modg * modg * modg;
  tr.Record();
  for (unsigned i = 1; i <= 100000; ++i) g = g * 3 % modg;
  cout << tr.Elapsed().Format() << endl;
  cout << g << endl;

  tr.Record();
  for (unsigned i = 1; i <= 100000; ++i) g = g * g % modg;
  cout << tr.Elapsed().Format() << endl;
  cout << g << endl;
}

int main() {
#if ENABLE_GMP
  run();
#endif
  return 0;
}