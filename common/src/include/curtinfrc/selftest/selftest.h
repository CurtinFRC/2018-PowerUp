#pragma once

#include <string>
#include <chrono>

namespace curtinfrc {
  enum class TestState {
    INCOMPLETE, // Not yet run
    RUNNING,    // Currently running
    UNKNOWN,    // We don't know if its failed or passed, but it is complete
    PASSED,     // System works
    FAILED      // System encountered a fault
  };

  /**
   * ISelfTestable is an interface class for a device or subsystem that can be tested automatically.
   * In most cases, this is a single motor controller or sensor, or a subsystem of multiple of those.
   * This involves sending values to these sensors/actuators and reading the output. This means the
   * system is active, and is useful for testing whether a system is broken or not. This would usually
   * be run, in sequence, before the start of a match.
   */
  class ISelfTestable {
  protected:
    /**
     * Called when the Self Test is started
     */
    virtual void selftest_onstart() { }

    /**
     * Called when the Self Test is run periodically
     * \arg \c elapsed_time_ms The time in milliseconds since the selftest started
     * \return the state. By default, this should be RUNNING, FAILED or PASSED.
     */
    virtual TestState selftest_run(double elapsed_time_ms) = 0;

    /**
     * Called when the Self Test is stopped
     */
    virtual void selftest_onstop() { }
  public:
    /**
     * \return the name of the self test target
     */
    virtual std::string selftest_name() = 0;

    /**
     * Call the selftest periodically
     */
    TestState selftest();

    void selftest_setstate(TestState);

    /**
     * \return is the self test running?
     */
    bool selftest_running() const;
    /**
     * \return has the self test completed?
     */
    bool selftest_done() const;
    TestState selftest_result() const;

  private:
    TestState _selftest_state = TestState::INCOMPLETE;
    std::chrono::time_point<std::chrono::system_clock> _selftest_start_time;

  };
} // ns curtinfrc