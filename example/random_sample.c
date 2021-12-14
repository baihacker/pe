#include <pe.hpp>
using namespace pe;

const int sample_count = 1000000;

const double coe15 = 1. / (1ULL << 15);
double CalPi_CRand15() {
  int ok = 0;
  for (int i = 0; i < sample_count; ++i) {
    double p[]{coe15 * CRand15(), coe15 * CRand15()};
    if (p[0] * p[0] + p[1] * p[1] < 1) {
      ++ok;
    }
  }
  return 4. * ok / sample_count;
}

const double coe31 = 1. / (1ULL << 31);
double CalPi_CRand31() {
  int ok = 0;
  for (int i = 0; i < sample_count; ++i) {
    double p[]{coe31 * CRand31(), coe31 * CRand31()};
    if (p[0] * p[0] + p[1] * p[1] < 1) {
      ++ok;
    }
  }
  return 4. * ok / sample_count;
}

const double coe63 = 1. / (1ULL << 63);
double CalPi_CRand63() {
  int ok = 0;
  for (int i = 0; i < sample_count; ++i) {
    double p[]{coe63 * CRand63(), coe63 * CRand63()};
    if (p[0] * p[0] + p[1] * p[1] < 1) {
      ++ok;
    }
  }
  return 4. * ok / sample_count;
}

const int maxn = 2000000000;
auto rand_generator = MakeUniformGenerator(0, maxn - 1);
const double randcoe = 1. / maxn;
double CalPi_Rand() {
  int ok = 0;
  for (int i = 0; i < sample_count; ++i) {
    double p[]{randcoe * rand_generator(), randcoe * rand_generator()};
    if (p[0] * p[0] + p[1] * p[1] < 1) {
      ++ok;
    }
  }
  return 4. * ok / sample_count;
}

double CalPi_Halton() {
  int ok = 0;
  for (int i = 0; i < sample_count; ++i) {
    auto p = Halton(i, 2);
    if (p[0] * p[0] + p[1] * p[1] < 1) {
      ++ok;
    }
  }
  return 4. * ok / sample_count;
}

#if HAS_MPF
double CalPi_Mpf() {
  gmp_randstate_t state;
  gmp_randinit_mt(state);
  Mpf::SetDefaultPrec(200);
  int ok = 0;
  for (int i = 0; i < sample_count; ++i) {
    Mpf a;
    Mpf b;
    mpf_urandomb(a.mpf(), state, 128);
    mpf_urandomb(b.mpf(), state, 128);
    if (a * a + b * b < 1) {
      ++ok;
    }
  }
  return 4. * ok / sample_count;
}
#endif

int main() {
  PE_INIT(maxp = 1000000);
  printf("CRand15\t%.16f\n", CalPi_CRand15());
  printf("CRand31\t%.16f\n", CalPi_CRand31());
  printf("CRand63\t%.16f\n", CalPi_CRand63());
  printf("Rand\t%.16f\n", CalPi_Rand());
  printf("Halton\t%.16f\n", CalPi_Halton());
#if HAS_MPF
  printf("Mpf\t%.16f\n", CalPi_Mpf());
#endif
  return 0;
}