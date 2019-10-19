#include <pe.hpp>

void test(const map<int, int>& mem) {
  for (auto iter : irange(mem)) {
    cout << iter.i << " " << iter.v << endl;
    // ++iter.v.second;
  }
  for (auto iter : irange(mem)) {
    cout << iter.i << " " << iter.v << endl;
  }
}

int main() {
  vector<int> x{3, 4, 5, 6};
  map<int, int> mem;
  mem[1] = 2;
  mem[2] = 4;
  test(mem);

  for (auto i : range(1, 10)) {
    dbg(i);
  }
  for (auto i : irange(range(10, 20))) {
    cout << i.i << " " << i.v << endl;
  }

  for (auto i : irange(irange(static_cast<vector<int>&>(x)))) {
    cout << i.i << " " << i.v.i << " " << i.v.v << endl;
    ++i.v.v;
  }
  for (auto i : irange(irange(x))) {
    cout << i.i << " " << i.v.i << " " << i.v.v << endl;
  }
  for (auto i : range(1, 10, 2)) {
    cout << i << endl;
  }
  for (auto i : range(0, 10, 2)) {
    cout << i << endl;
  }
  for (auto i : range(10, 0, -2)) {
    cout << i << endl;
  }
  for (auto i : range(10, 1, -2)) {
    cout << i << endl;
  }
  for (auto i : irange(range(10, 1, -2))) {
    cout << i.i << " " << i.v << endl;
  }
  for (auto i : range(x)) {
    cout << i << endl;
  }
  for (auto& i : range(mem)) {
    cout << i.first << " " << i.second << endl;
  }
  for (auto& i : range(x).filter([](auto v) { return v % 2 == 0; })) {
    cout << i << endl;
  }
  cout << range(x)
              .filter([](auto v) { return v % 2 == 0; })
              .reduce(0, [](auto a, auto b) { return a + b; })
       << endl;
  cout << range(1, 10)
              .filter([](auto v) { return v % 2 == 0; })
              .filter([](auto v) { return v % 3 == 0; })
              .map([](auto v) { return v * 3; })
              .map([](auto v) { return v * 3; })
              .reduce(0, [](auto a, auto b) { return a + b; })
       << endl;
  cout << range(1, 10).filter([](auto v) { return v % 2 == 0; }).toVector()
       << endl;
  cout << range(1, 10).filter([](auto v) { return v % 3 == 0; }).toSet()
       << endl;
  return 0;
}