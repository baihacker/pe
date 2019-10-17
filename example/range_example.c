#include <pe.hpp>

void test(const map<int, int>& mem) {
  for (auto iter: range(mem)) {
    cout << iter.i << " " << iter.v << endl;
    // ++iter.v.second;
  }
  for (auto iter: range(mem)) {
    cout << iter.i << " " << iter.v << endl;
  }
}

int main() {
  vector<int> x{3,4,5,6};
  map<int, int> mem;
  mem[1] = 2;
  mem[2] = 4;
  test(mem);
  
  for (auto i : range(1, 10)) {
    dbg(i);
  }
  for (auto i : range(range(10, 20))) {
    cout << i.i << " " << i.v << endl;
  }

  for (auto i: range(range(static_cast<vector<int>&>(x)))) {
    cout << i.i << " " << i.v.i << " " << i.v.v << endl;
    ++i.v.v;
  }
  for (auto i: range(range(x))) {
    cout << i.i << " " << i.v.i << " " << i.v.v << endl;
  }
  return 0;
}