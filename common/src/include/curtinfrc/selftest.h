#pragma once

namespace curtinfrc {
	enum class TestState {
		PASSED, FAILED, UNKNOWN
	};

	class ISelfTestable {
	public:
		TestState selftest();
		virtual TestState selftest_run() = 0;
		void selftest_reset();
		bool selftest_conducted() const;

	private:
		TestState _last_test_state = TestState::UNKNOWN;
	};
} // ns curtinfrc