#include <pe.hpp>
using namespace pe;

void MakeDb() {
  PeDb db("D:/");
  const int64 n = 100000000000000;
  db.MergePrimePi(PrimeS0Parallel<int64>(n));
#if PE_HAS_INT128
  db.MergePrimeSum(PrimeS1Parallel<int128>(n));
#endif
  db.Save();
}

int main() {
  PE_INIT(maxp = 70000000);
  // MakeDb();
  const int64 n = 10000000000;
  PeDb db("D:/");
  db.Load();

  {
    DVA<int64> dva = PrimeS0Ex<int64>(n);
    DVA<int64> dva1(n);
    db.FillPrimePi(dva1);
    for (int i = 0; i < dva.key_size; ++i) {
      if (dva.values[i] != dva1.values[i]) {
        std::cout << dva.values[i] << std::endl;
        std::cout << dva1.values[i] << std::endl;
      }
      assert(dva.values[i] == dva1.values[i]);
    }
    std::cout << dva[n] << std::endl;
    std::cout << dva1[n] << std::endl;
    // https://oeis.org/A006880
    std::cout << db.PrimePi(Power(10LL, 14)) << std::endl;
  }
#if PE_HAS_INT128
  {
    DVA<int128> dva = PrimeS1Ex<int128>(n);
    DVA<int128> dva1(n);
    db.FillPrimeSum(dva1);
    for (int i = 0; i < dva.key_size; ++i) {
      if (dva.values[i] != dva1.values[i]) {
        std::cout << dva.values[i] << std::endl;
        std::cout << dva1.values[i] << std::endl;
      }
      assert(dva.values[i] == dva1.values[i]);
    }
    std::cout << dva[n] << std::endl;
    std::cout << dva1[n] << std::endl;
    // https://oeis.org/A046731
    std::cout << db.PrimeSum(Power(10LL, 14)) << std::endl;
  }
#endif
  return 0;
}