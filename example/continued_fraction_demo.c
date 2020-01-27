#include "pe.hpp"

template <typename GBI>
void demo() {
  vi data{1, 2, 2, 2, 2, 2, 2, 2, 2, 2};
  for (int i = 0; i < 10; ++i) {
    cout << from_cf<GBI>(data, i) << endl;
  }
  cout << from_cf_n<GBI>(data) << endl;
  for (int i = 50; i <= 50; ++i) {
    cout << i << " " << to_cf<GBI>(1, 6, i) << endl;
  }

  cout << to_cf<GBI>(1, 2, 10) << endl;

  cout << from_cf<GBI>(to_cf<GBI>(1, 2, 50)) << endl;
}

int main() {
  demo<bi>();
  demo<Mpz>();
  return 0;
}
