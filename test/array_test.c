#include "pe_test.h"

// struct XYZ {
//   XYZ() {dbg("xyz");}
// };
// struct B {
//   B(XYZ& xyz) : xyz(xyz) {
//     dbg("B constructed");
//   }
//   XYZ& xyz;
// };
// struct A : public B {
//   A() : B(xyz) {
//     dbg("A constructed");
//   }
//   XYZ xyz;
// };

namespace array_test {
SL void ArrayTest() {
  DArray<int, 2> vec({5, 6});
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 6; ++j) {
      vec[i][j] = i * j;
    }
  }

  vec.Reset({3, 2});
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 2; ++j) vec[i][j] = i * j;
  }

  Array<int, 4, 5> arr(5);
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 5; ++j) arr[i][j] = i * j;
  }

  AArray<int, StdAllocator, 4, 5> arr1(5);
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 5; ++j) arr1[i][j] = i * j;
  }
}
PE_REGISTER_TEST(&ArrayTest, "ArrayTest", SMALL);
}  // namespace array_test