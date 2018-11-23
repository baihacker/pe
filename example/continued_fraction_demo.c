#include "pe.hpp"

template<typename GBI>
void demo() {
  vi data{1,2,2,2,2,2,2,2,2,2};
  for (int i = 0; i < 10; ++i) {
    cout << from_continued_fraction<GBI>(data, i) << endl;
  }
  for (int i = 50; i <= 50; ++i) {
  cout << i << " " << to_continued_fraction<GBI>(1, 6, i) << endl;
  }

  cout << to_continued_fraction<GBI>(1, 2, 10) << endl;

  cout << from_continued_fraction<GBI>(to_continued_fraction<GBI>(1,2,50)) << endl;
}

int main() {
  demo<bi>();
  demo<Mpz>();
  return 0;
}


