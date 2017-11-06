#include <pe.hpp>

void performance_test()
{
  TimeRecorder tr;
  int s = 0;
  for (int j = 0; j < 5; ++j)
  {
    bn x = 1;
    for (int i = 1; i <= 50000; ++i) x *= i;
    s += x[0];
  }
  cerr << s << " " << tr.elapsed().format() << endl;
}

int main()
{
  performance_test();
  return 0;
}