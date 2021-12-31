#include "pe_test.h"

#if 0
#include "extended_signed_int_test.c"
#else
#include "algo_test.c"
#include "array_test.c"
#include "bi_test.c"
#include "bi_test1.c"
#include "bit_test.c"
#include "comb_moder_test.c"
#include "dva_test.c"
#include "extended_signed_int_test.c"
#include "extended_unsigned_int_test.c"
#include "fact_ppower_mod_test.c"
#include "init_inv_test.c"
#include "int128_test.c"
#include "mat_mul_test.c"
#include "misc_test.c"
#include "mod_test.c"
#include "mpf_test.c"
#include "mpi_test.c"
#include "nt_test.c"
#include "parallel_sort_test.c"
#include "poly_test.c"
#include "poly_algo_test.c"
#include "poly_div_test.c"
#include "poly_mul_test.c"
#include "fft_test.c"
#include "prime_pi_sum_test.c"
#include "square_root_test.c"
#include "tree_test.c"
#endif

static TimeUsage __time_usage;

PeTest& GetTester() {
  static PeTest tester;
  return tester;
}

TestSize enabled_test_size[] {
#if defined(TEST_ALL)
  SMALL, MEDIUM, BIG, SUPER, SPECIFIED,
#else
  SPECIFIED,
#endif
};

SL bool IsEnabledTestSize(TestSize size) {
  for (auto& iter : enabled_test_size) {
    if (iter == size) {
      return true;
    }
  }
  return false;
}

std::vector<std::vector<std::string>> result;

static inline void AddResult(const std::string& key, const std::string& value) {
  result.push_back({key, value});
}

template <typename T>
SL std::string AsString(const T& v) {
  return std::to_string(v);
}

SL std::string AsString(const std::string& str) { return str; }
SL std::string AsString(const char* str) { return str; }

#define AddDefined(x) AddResult((#x), AsString(x))
#define AddNotDefined(x) AddResult((#x), "n/a")

void DisplayCompilerInfo() {
#if defined(__GNUC__)
  AddDefined(__GNUC__);
  AddDefined(__GNUC_MINOR__);
  AddDefined(__GNUC_PATCHLEVEL__);
#endif

#if defined(__clang_version__)
  AddDefined(__clang_version__);
#endif

#if defined(_MSC_VER)
  AddDefined(_MSC_VER);
#endif

  AddDefined(__cplusplus);

#if defined(__STRICT_ANSI__)
  AddDefined(__STRICT_ANSI__);
#else
  AddNotDefined(__STRICT_ANSI__);
#endif

#if defined(_MSVC_LANG)
  AddDefined(_MSVC_LANG);
#endif

#if defined(_WIN32)
  AddDefined(_WIN32);
#else
  AddNotDefined(_WIN32);
#endif

#if defined(_WIN64)
  AddDefined(_WIN64);
#else
  AddNotDefined(_WIN64);
#endif

#if defined(__APPLE__)
  AddDefined(__APPLE__);
#else
  AddNotDefined(__APPLE__);
#endif

#if defined(__x86_64)
  AddDefined(__x86_64);
#else
  AddNotDefined(__x86_64);
#endif

#if defined(_M_X64)
  AddDefined(_M_X64);
#else
  AddNotDefined(_M_X64);
#endif

#if defined(_X86_)
  AddDefined(_X86_);
#else
  AddNotDefined(_X86_);
#endif

#if defined(__BYTE_ORDER__)
  AddDefined(__BYTE_ORDER__);
#else
  AddNotDefined(__BYTE_ORDER__);
#endif

#if defined(__ORDER_LITTLE_ENDIAN__)
  AddDefined(__ORDER_LITTLE_ENDIAN__);
#else
  AddNotDefined(__ORDER_LITTLE_ENDIAN__);
#endif

#if defined(__ORDER_BIG_ENDIAN__)
  AddDefined(__ORDER_BIG_ENDIAN__);
#else
  AddNotDefined(__ORDER_BIG_ENDIAN__);
#endif

#if defined(__GLIBCXX_TYPE_INT_N_0)
  AddResult("__GLIBCXX_TYPE_INT_N_0", "is defined");
#else
  AddNotDefined(__GLIBCXX_TYPE_INT_N_0);
#endif
#if defined(__SIZEOF_INT128__)
  AddDefined(__SIZEOF_INT128__);
  AddDefined(std::numeric_limits<__int128>::is_integer);
  AddDefined(std::is_integral_v<__int128>);
  AddDefined(std::is_signed_v<__int128>);
  AddDefined(std::is_unsigned_v<__int128>);
  AddDefined(std::numeric_limits<unsigned __int128>::is_integer);
  AddDefined(std::is_integral_v<unsigned __int128>);
  AddDefined(std::is_signed_v<unsigned __int128>);
  AddDefined(std::is_unsigned_v<unsigned __int128>);
#else
  AddNotDefined(__SIZEOF_INT128__);
#endif

#if defined(__SIZEOF_FLOAT128__)
  AddDefined(__SIZEOF_FLOAT128__);
#else
  AddNotDefined(__SIZEOF_FLOAT128__);
#endif

#if defined(__FLOAT128__)
  show(__FLOAT128__);
#else
  AddNotDefined(__FLOAT128__);
#endif

#if defined(__GLIBCXX__)
  AddDefined(__GLIBCXX__);
#else
  AddNotDefined(__GLIBCXX__);
#endif

#if defined(_LIBCPP_VERSION)
  AddDefined(_LIBCPP_VERSION);
#else
  AddNotDefined(_LIBCPP_VERSION);
#endif

#if defined(__AVX2__)
  AddDefined(__AVX2__);
#else
  AddNotDefined(__AVX2__);
#endif

#if defined(_OPENMP)
  AddDefined(_OPENMP);
#else
  AddNotDefined(_OPENMP);
#endif

  int max_size[2] = {0};
  for (auto& iter : result) {
    for (int i = 0; i < 2; ++i) {
      int t = iter[i].size();
      if (t > max_size[i]) max_size[i] = t;
    }
  }

  for (auto& iter : result) {
    printf("%-*s\t\t%-*s\n", max_size[0], iter[0].c_str(), max_size[1],
           iter[1].c_str());
  }
}
int main() {
  DisplayCompilerInfo();

  PeInitializer()
      .set_cal_phi()
      .set_cal_mu()
      .set_fft_k()
      .set_ntt32_k()
      .set_ntt64_k()
      .Init();

  dbg(maxp);
  dbg(maxp2);

  auto& tester = GetTester();
  const int size = (int)tester.tests.size();

  bool isFirstTest = true;

  for (int i = 0; i < size; ++i) {
    auto& item = tester.tests[i];
    if (!IsEnabledTestSize(item.testSize)) {
      continue;
    }
#ifdef NO_SMALL_TEST
    if (item.testSize == SMALL) {
      continue;
    }
#endif
#ifdef NO_MEDIUM_TEST
    if (item.testSize == MEDIUM) {
      continue;
    }
#endif
#ifdef NO_BIG_TEST
    if (item.testSize == BIG) {
      continue;
    }
#endif
#ifdef NO_SUPER_TEST
    if (item.testSize == SUPER) {
      continue;
    }
#endif
    if (!isFirstTest) {
      std::cout << std::endl;
    }
    std::cout << "Begin " << item.description << std::endl;
    TimeRecorder tr;
    item.test();
    std::cout << "End " << item.description << std::endl;
    std::cout << "Time usage " << tr.Elapsed().Format() << std::endl;
    isFirstTest = false;
  }
  return 0;
}