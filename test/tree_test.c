#include "pe_test.h"

namespace tree_test {
SL void RuBitTest() {
  const int n = 100;
  int data[n + 1] = {0};
  RUBit<int> tree(n);
  for (int i = 0; i < 1000; ++i) {
    if (rand() % 2 == 0) {
      for (int i = 0; i < 100; ++i) {
        int u = rand() % n + 1, v = rand() % n + 1;
        int w = rand() % n - 50;
        if (u > v) std::swap(u, v);
        tree.Update(u, v, w);
        for (int j = u; j <= v; ++j) data[j] += w;
      }
    } else {
      for (int i = 1; i <= 100; ++i) {
        assert(tree.Query(i) == data[i]);
      }
    }
  }
}

SL void RsqBitTest() {
  const int n = 100;
  int data[n + 1] = {0};
  RSQBit<int> tree(n);
  for (int i = 0; i < 100; ++i) {
    if (rand() % 2 == 0) {
      for (int i = 0; i < 100; ++i) {
        int u = rand() % n + 1, v = rand() % n + 1;
        if (u > v) std::swap(u, v);
        int s = 0;
        for (int j = u; j <= v; ++j) s += data[j];
        assert(tree.Query(u, v) == s);
      }
    } else {
      for (int i = 1; i <= 100; ++i) {
        int w = rand() % n - 50;
        data[i] += w;
        tree.Update(i, w);
      }
    }
  }
}

SL void TreeTest() {
  RuBitTest();
  RsqBitTest();
}

PE_REGISTER_TEST(&TreeTest, "TreeTest", SMALL);
}  // namespace tree_test
