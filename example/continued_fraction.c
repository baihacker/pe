#include "pe.hpp"

template <typename GBI>
void demo() {
  vi data{1, 2, 2, 2, 2, 2, 2, 2, 2, 2};
  for (int i = 0; i < 10; ++i) {
    cout << FromCf<GBI>(data, i) << endl;
  }
  cout << FromCfN<GBI>(data) << endl;
  for (int i = 50; i <= 50; ++i) {
    cout << i << " " << ToCf<GBI>(1, 6, i) << endl;
  }

  cout << ToCf<GBI>(1, 2, 10) << endl;

  cout << FromCf<GBI>(ToCf<GBI>(1, 2, 50)) << endl;
}

int main() {
  demo<bi>();
  demo<Mpz>();
  return 0;
}
