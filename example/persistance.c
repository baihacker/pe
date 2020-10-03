#include <pe.hpp>
KVPersistance kv("fff.txt");
int main() {
  cerr << kv.Size() << endl;
  cerr << kv.Storage()[1] << endl;
  kv.Set(8, 9);
  kv.Set(9, 10);

  int64 sum = 0;
  kv.Visit([&](int64 k, int64 v) {
    sum += v;
    cerr << k << " " << v << endl;
    return true;
  });
  dbg(sum);
  return 0;
}