#include <pe>

int main() {
  PolyM<97> x{1LL, 96LL};
  cout << x.inv(20) << endl;
  cout << x.inv(20) * x << endl;
  PolyM<97> y;
  for (int i = 1; i < 100; ++i)
    y.data.push_back(i%97);
  cout << y.inv(100) * y << endl;
  return 0;
}