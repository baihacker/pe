const int maxp = 100000;
#include "pe"

int main()
{
  int128 x = 1;
  //for (int i = 0; i < 100; ++i) cout << x << endl, x <<= 1;
  for (int i = 0; i < 130; ++i)
  {
    cout << i << endl;
    cout << -x << endl;
    cout << x << endl;
    print_int128(-x); puts("");
    print_int128(x); puts("");
    cout << bn(-x) << endl;
    cout << bn(x) << endl;
    x <<= 1;
  }
  x = 0;
  cout << "zero" << endl;
  cout << -x << endl;
  cout << x << endl;
  print_int128(-x); puts("");
  print_int128(x); puts("");
  cout << bn(-x) << endl;
  cout << bn(x) << endl;
  return 0;
}