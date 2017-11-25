#ifndef __PE_TEST_H__
#define __PE_TEST_H__

#define PE_TEST_MODE
#include <pe>

typedef std::function<void (void)> TestMethodT;

struct TestItem {
  TestMethodT test;
  std::string description;
};

struct PeTest {
  std::vector<TestItem> tests;
};

PeTest& getTester();

struct InitializeHelper {
  InitializeHelper(const std::function<void (void)>& action) {
    action();
  }
};

#define MAKE_INITIALIZER_NAME_IMPL(LINE_NUMBER) __initializeHelper##LINE_NUMBER
#define MAKE_INITIALIZER_NAME(LINE_NUMBER) MAKE_INITIALIZER_NAME_IMPL(LINE_NUMBER)

#define PE_REGISTER_TEST(t, d) \
static InitializeHelper MAKE_INITIALIZER_NAME(__LINE__)([](){getTester().tests.push_back((TestItem){t, d});})

#endif
