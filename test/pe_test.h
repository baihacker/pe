#ifndef __PE_TEST_H__
#define __PE_TEST_H__

#define PE_TEST_MODE

#include <pe>
using namespace pe;

enum TestSize {
  SMALL = 0,
  MEDIUM = 1,
  BIG = 2,
  SUPER = 3,
  SPECIFIED = 4,
};

SL bool SameStringIgnoreCase(std::string_view a, std::string_view b) {
  if (a.size() != b.size()) {
    return false;
  }
  for (int i = 0; i < a.size(); ++i) {
    if (std::tolower(a[i]) != std::tolower(b[i])) {
      return false;
    }
  }
  return true;
}

SL std::optional<TestSize> ParseTestSize(std::string_view test_size) {
  if (SameStringIgnoreCase(test_size, "SMALL")) {
    return SMALL;
  } else if (SameStringIgnoreCase(test_size, "MEDIUM")) {
    return MEDIUM;
  } else if (SameStringIgnoreCase(test_size, "BIG")) {
    return BIG;
  } else if (SameStringIgnoreCase(test_size, "SUPER")) {
    return SUPER;
  } else if (SameStringIgnoreCase(test_size, "SPECIFIED")) {
    return SPECIFIED;
  } else {
    return std::nullopt;
  }
}

SL std::vector<TestSize> ParseTestSizeList(std::string s) {
  std::vector<TestSize> ret;

  std::replace(std::begin(s), std::end(s), ',', ' ');
  std::replace(std::begin(s), std::end(s), '(', ' ');
  std::replace(std::begin(s), std::end(s), ')', ' ');
  std::stringstream ss(s);
  std::string item;
  while (ss >> item) {
    std::optional<TestSize> size = ParseTestSize(item);
    if (!size.has_value()) {
      std::cerr << "Cannot parse test size: " << item << std::endl;
    } else {
      ret.push_back(*size);
    }
  }
  return ret;
}

struct TestItem {
  std::function<void()> test;
  std::string file;
  std::string description;
  TestSize test_size;
};

struct TestRegistry {
  std::vector<TestItem> tests;
};

TestRegistry& GetTestRegistry();

#define MAKE_INITIALIZER_NAME_IMPL(LINE_NUMBER) __register_test_##LINE_NUMBER
#define MAKE_INITIALIZER_NAME(LINE_NUMBER) \
  MAKE_INITIALIZER_NAME_IMPL(LINE_NUMBER)

#if PE_HAS_CPP20
#define PE_REGISTER_TEST(test, description, test_size)                   \
  static int MAKE_INITIALIZER_NAME(__LINE__) = ([]() {                   \
    GetTestRegistry().tests.push_back(                                   \
        {test, std::source_location::current().file_name(), description, \
         test_size});                                                    \
    return 0;                                                            \
  })()
#else
#define MAKE_FILE_NAME_IMPL(FILE_NAME) std::string(FILE_NAME)
#define MAKE_FILE_NAME(FILE_NAME) MAKE_FILE_NAME_IMPL(FILE_NAME)
#define PE_REGISTER_TEST(test, description, test_size)             \
  static int MAKE_INITIALIZER_NAME(__LINE__) = ([]() {             \
    GetTestRegistry().tests.push_back(                             \
        {test, MAKE_FILE_NAME(__FILE__), description, test_size}); \
    return 0;                                                      \
  })()
#endif

#endif
