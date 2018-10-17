#include <pe.hpp>
KVPersistance kv("fff.txt");
int main() {
  cerr << kv.size() << endl;
  cerr << kv.storage()[1] << endl;
  kv.set(8, 9);
  kv.set(9, 10);

  int64 sum = 0;
  kv.visit([&](int64 k, int64 v) {
    sum += v;
    cerr << k << " " << v << endl;
    return true;
  });
  dbg(sum);
  return 0;
}