#include "pe_test.h"

namespace tree_test {
SL void rubit_test() {
  const int n = 100;
  int data[n+1] = {0};
  RUBit<int> tree(n);
  for (int i = 0; i < 1000; ++i) {
    if (rand() % 2 == 0) {
      for (int i = 0; i < 100; ++i) {
        int u = rand() % n + 1, v = rand() % n + 1;
        int w = rand() % n - 50;
        if (u > v) swap(u, v);
        tree.update(u, v, w);
        for (int j = u; j <= v; ++j)
          data[j] += w;
      }
    } else {
      for (int i = 1; i <= 100; ++i) {
        assert(tree.query(i) == data[i]);
      }
    }
  }
}

SL void rsqbit_test() {
  const int n = 100;
  int data[n+1] = {0};
  RSQBit<int> tree(n);
  for (int i = 0; i < 100; ++i) {
    if (rand() % 2 == 0) {
      for (int i = 0; i < 100; ++i) {
        int u = rand() % n + 1, v = rand() % n + 1;
        if (u > v) swap(u, v);
        int s = 0;
        for (int j = u; j <= v; ++j)
          s += data[j];
        assert(tree.query(u, v) == s);
      }
    } else {
      for (int i = 1; i <= 100; ++i) {
        int w = rand() % n - 50;
        data[i] += w;
        tree.update(i, w);
      }
    }
  }
}

SL void tree_test() {
  rubit_test();
  rsqbit_test();
}

PE_REGISTER_TEST(&tree_test, "tree_test", SMALL);
}
