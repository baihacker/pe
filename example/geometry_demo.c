#include <pe.hpp>

template <typename T>
void demo(T a, T b) {
  cout << a << endl;
  cout << b << endl;
  cout << a + b << endl;
  cout << a - b << endl;
  cout << (a < b) << endl;
  cout << (a <= b) << endl;
  cout << (a > b) << endl;
  cout << (a >= b) << endl;
  cout << (a == b) << endl;
  cout << (a != b) << endl;
  a.Print();
  a.PrintLn();
  cout << Dot(a, b) << endl;
  // cout << Cross(a, b) << endl;
  cout << Norm(a) << endl;
  cout << Norm2(a) << endl;
  cout << a.Norm2() << endl;
}

int main() {
  demo(Point2D<int>{1, 2}, Point2D<int>{3, 4});
  demo(Point3D<int>{1, 2, 3}, Point3D<int>{4, 5, 6});
  return 0;
}
