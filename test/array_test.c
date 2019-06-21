#include "pe_test.h"

/**
struct XYZ {
  XYZ() {dbg("xyz");}
};
struct B {
  B(XYZ& xyz) : xyz(xyz) {
    dbg("B constructed");
  }
  XYZ& xyz;
};
struct A : public B {
  A() : B(xyz) {
    dbg("A constructed");
  }
  XYZ xyz;
};
*/


namespace array_test {
SL void array_test() {
  PeArray<int, 2> vec({5,6});
  for (int i = 0; i < 5; ++i)
    for (int j = 0; j < 6; ++j) {
      vec[i][j] = i*j;
    }
}
PE_REGISTER_TEST(&array_test, "array_test", SPECIFIED);
}