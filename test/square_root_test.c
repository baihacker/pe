#include <pe.hpp>

void correctness_test()
{
  int f = 0;
  for (int64 i = 1; i <= 100000000; ++i)
  {
    const int64 num = i*i;
    for (int64 offset = -10; offset <= 10; ++offset)
    {
      int64 now = offset + num;
      int64 u = square_root_safe(now);
      int64 v = square_root_unsafe(now);
      int64 w = square_root(now);
      if (u != v || u != w || v != w)
      {
        cerr << "failed " << now << ":" << endl;
        cerr << u << " " << v << " " << w << endl;
        ++f;
      }
    }
  }
  if (f == 0)
  cerr << "Correctness test pass!" << endl;
  else
  cerr << "Correctness tess failed!" << endl;
}

void performance_test()
{
  const int N = 100000000;
  TimeRecorder tr;
  int64 s = 0;
  for (int64 i = 10; i <= N; ++i)
  {
    const int64 num = i*i;
    for (int64 offset = -10; offset <= 10; ++offset)
    {
      int64 now = offset + num;
      int64 u = square_root_safe(now);
      s += u;
    }
  }
  cerr << "square_root_safe : " << tr.elapsed().format() << " " << s << endl;
  s = 0;
  tr.record();
  for (int64 i = 10; i <= N; ++i)
  {
    const int64 num = i*i;
    for (int64 offset = -10; offset <= 10; ++offset)
    {
      int64 now = offset + num;
      int64 u = square_root_unsafe(now);
      s += u;
    }
  }
  cerr << "square_root_unsafe : " << tr.elapsed().format() << " " << s << endl;
  s = 0;
  tr.record();
  for (int64 i = 10; i <= N; ++i)
  {
    const int64 num = i*i;
    for (int64 offset = -10; offset <= 10; ++offset)
    {
      int64 now = offset + num;
      int64 u = square_root(now);
      s += u;
    }
  }
  cerr << "square_root : " << tr.elapsed().format() << " " << s << endl;
}

int main()
{
  performance_test();

  return 0;
}