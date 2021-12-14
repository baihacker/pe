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
  std::string description;
  TestSize testSize;
};

struct PeTest {
  std::vector<TestItem> tests;
};

PeTest& GetTester();

struct InitializeHelper {
  InitializeHelper(const std::function<void(void)>& action) { action(); }
};

#define MAKE_INITIALIZER_NAME_IMPL(LINE_NUMBER) __initializeHelper##LINE_NUMBER
#define MAKE_INITIALIZER_NAME(LINE_NUMBER) \
  MAKE_INITIALIZER_NAME_IMPL(LINE_NUMBER)

#define PE_REGISTER_TEST(t, d, s)                                \
  static InitializeHelper MAKE_INITIALIZER_NAME(__LINE__)([]() { \
    GetTester().tests.push_back({t, d, s});                      \
  })

#endif
