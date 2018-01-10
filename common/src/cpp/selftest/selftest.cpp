#include "curtinfrc/selftest/selftest.h"

#include <iostream>

using namespace curtinfrc;

TestState ISelfTestable::selftest() {
  if (!selftest_running()) {
    // Start
    selftest_setstate(TestState::RUNNING);
  }
  auto now = std::chrono::system_clock::now();
  std::chrono::duration<double, std::milli> elapsed = now - _selftest_start_time;
  selftest_setstate(selftest_run(elapsed.count()));
  return _selftest_state;
}

void ISelfTestable::selftest_setstate(TestState state) {
  switch (state) {
  case TestState::RUNNING:
    // Starting
    if (!selftest_running()) {
      std::cout << "========== TEST ==========" << std::endl;
      std::cout << "Starting " << selftest_name() << " self-test!" << std::endl;
      _selftest_start_time = std::chrono::system_clock::now();
      selftest_onstart();
    }
    break;
  case TestState::UNKNOWN:
  case TestState::PASSED:
  case TestState::FAILED:
    // Ending
    std::cout << "========== TEST ==========" << std::endl;
    std::cout << "Stopping " << selftest_name() << " self-test!" << std::endl;
    std::cout << "Result: " << selftest_name() << " " << (state == TestState::UNKNOWN ? "UNKNOWN" : state == TestState::FAILED ? "FAILED" : "PASSED") << std::endl;
  default:
    selftest_onstop();
  }
  _selftest_state = state;
}

bool ISelfTestable::selftest_running() const {
  return _selftest_state == TestState::RUNNING;
}

bool ISelfTestable::selftest_done() const {
  return _selftest_state == TestState::UNKNOWN || _selftest_state == TestState::PASSED || _selftest_state == TestState::FAILED;
}

TestState ISelfTestable::selftest_result() const {
  return _selftest_state;
}