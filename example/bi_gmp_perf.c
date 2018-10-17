#include <pe.hpp>

void run() {
  mpz_class f = 1;
  TimeRecorder tr;
  for (unsigned i = 1; i <= 100000; ++i) f *= i;
  cout << tr.elapsed().format() << endl;

  tr.record();
  for (unsigned i = 1; i <= 100000; ++i) f /= i;
  cout << tr.elapsed().format() << endl;
  cout << f << endl;

  tr.record();
  for (unsigned i = 1; i <= 100000; ++i) f = f * 3 % 1000000007;
  cout << tr.elapsed().format() << endl;
  cout << f << endl;

  mpz_class modf = 1000000007;
  modf = mpz_class(modf * modf) * modf;
  tr.record();
  for (unsigned i = 1; i <= 100000; ++i) f = f * 3 % modf;
  cout << tr.elapsed().format() << endl;
  cout << f << endl;

  tr.record();
  for (unsigned i = 1; i <= 100000; ++i) f = f * f % modf;
  cout << tr.elapsed().format() << endl;
  cout << f << endl;

  cout << endl;

  bi g = 1;
  tr.record();
  for (unsigned i = 1; i <= 100000; ++i) g *= i;
  cout << tr.elapsed().format() << endl;

  tr.record();
  for (unsigned i = 1; i <= 100000; ++i) g /= i;
  cout << tr.elapsed().format() << endl;
  cout << g << endl;

  tr.record();
  for (unsigned i = 1; i <= 100000; ++i) g = g * 3 % 1000000007;
  cout << tr.elapsed().format() << endl;
  cout << g << endl;

  bi modg = 1000000007;
  modg = modg * modg * modg;
  tr.record();
  for (unsigned i = 1; i <= 100000; ++i) g = g * 3 % modg;
  cout << tr.elapsed().format() << endl;
  cout << g << endl;

  tr.record();
  for (unsigned i = 1; i <= 100000; ++i) g = g * g % modg;
  cout << tr.elapsed().format() << endl;
  cout << g << endl;
}

int main() {
#if ENABLE_GMP
  run();
#endif
  return 0;
}