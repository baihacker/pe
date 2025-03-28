#ifndef __PE_TEST_H__
#define __PE_TEST_H__

#define PE_TEST_MODE
#include <pe>
using namespace pe;

using TestMethodT = std::function<void()>;

enum TestSize {
  SMALL = 0,
  MEDIUM = 1,
  BIG = 2,
  SUPER = 3,
  SPECIFIED = 4,
};

struct TestItem {
  TestMethodT test;
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
