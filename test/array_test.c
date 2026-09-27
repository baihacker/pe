#include "pe_test.h"

namespace array_test {
// Counts the live objects to check construction and destruction.
struct Counted {
  static inline int alive = 0;
  int value;
  Counted(int v = 7) : value(v) { ++alive; }
  ~Counted() { --alive; }
};

SL void ArrayTest() {
  {
    DArray<int, 2> vec({5, 6});
    for (int i = 0; i < 5; ++i)
      for (int j = 0; j < 6; ++j) vec[i][j] = i * j;
    // Row-major, contiguous storage.
    for (int i = 0; i < 5; ++i)
      for (int j = 0; j < 6; ++j) assert(vec.data()[i * 6 + j] == i * j);
    auto ref = vec.Ref();
    assert(ref[4][5] == 20);
    const auto& cvec = vec;
    assert(cvec[3][2] == 6);

    // Reset with an initial value for every element.
    vec.Reset({3, 2}, 9);
    for (int i = 0; i < 3; ++i)
      for (int j = 0; j < 2; ++j) assert(vec[i][j] == 9);
    vec.Clear();
    vec.Clear();  // idempotent
    assert(vec.data() == nullptr);
  }
  {
    DArray<int64, 3> cube({2, 3, 4}, int64(-1));
    for (int i = 0; i < 2; ++i)
      for (int j = 0; j < 3; ++j)
        for (int k = 0; k < 4; ++k) {
          assert(cube[i][j][k] == -1);
          cube[i][j][k] = i * 100 + j * 10 + k;
        }
    assert(cube.data()[1 * 12 + 2 * 4 + 3] == 123);
    DArray<int, 1> one;  // default: a single element per dimension
    one[0] = 5;
    assert(one[0] == 5);
  }
  {
    // Every constructed element is destroyed exactly once.
    {
      DArray<Counted, 2> d({4, 5}, 3);
      assert(Counted::alive == 20 && d[3][4].value == 3);
      d.Reset({2, 2});
      assert(Counted::alive == 4 && d[1][1].value == 7);
      Array<Counted, 3, 3> a;
      assert(Counted::alive == 13);
      a.Clear();
      assert(Counted::alive == 4);
    }
    assert(Counted::alive == 0);
  }
  {
    Array<int, 4, 5> arr(5);
    for (int i = 0; i < 4; ++i)
      for (int j = 0; j < 5; ++j) {
        assert(arr[i][j] == 5);
        arr[i][j] = i * j;
      }
    assert(arr.data()[3 * 5 + 4] == 12 && arr.Ref()[2][3] == 6);
    static_assert(ArrayShape<4, 5>::D == 2 && ArrayShape<4, 5>::EC == 20);
    static_assert(ArrayShape<2, 3, 7>::D == 3 && ArrayShape<2, 3, 7>::EC == 42);

    AArray<int, StdAllocator, 4, 5> arr1(5);
    for (int i = 0; i < 4; ++i)
      for (int j = 0; j < 5; ++j) arr1[i][j] = i * j;
    assert(arr1[3][4] == 12);

    Array<int64, 2, 3, 4> arr3(0);
    arr3[1][2][3] = 42;
    assert(arr3.data()[23] == 42);
  }
}
PE_REGISTER_TEST(&ArrayTest, "ArrayTest", SMALL);
}  // namespace array_test
