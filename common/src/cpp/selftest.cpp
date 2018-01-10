#include "curtinfrc/selftest.h"

using namespace curtinfrc;

TestState ISelfTestable::selftest() {
	if (!selftest_conducted) {
		selftest_run();
	}
	return _last_test_state;
}

void ISelfTestable::selftest_reset() {
	_last_test_state = TestState::UNKNOWN;
}

bool ISelfTestable::selftest_conducted() const {
	return _last_test_state != TestState::UNKNOWN;
}