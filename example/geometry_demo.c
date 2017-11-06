#include <pe.hpp>

template<typename T>
void demo(T a, T b)
{
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
  a.print();
  a.println();
  cout << dot(a, b) << endl;
  //cout << cross(a, b) << endl;
  cout << norm(a) << endl;
  cout << norm2(a) << endl;
  cout << a.norm2() << endl;
}

int main()
{
  demo(Point2D<int>{1, 2}, Point2D<int>{3, 4});
  demo(Point3D<int>{1, 2, 3}, Point3D<int>{4, 5, 6});
  return 0;
}
