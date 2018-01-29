#pragma once

#include <memory>
#include <vector>

namespace curtinfrc {

  /**
   * A 'Strategy' is a single component in a sequential chain of events or actions
   * it is a class wrapper for a periodic-based function run in a sequential manner
   */
  class Strategy {
  public:
    /**
     * Called upon the Strategy being run for the first time per instance
     */
    virtual void start() {}

    /**
     * Called at the same rate as the robot base class
     * \param \c time Time since start (milliseconds)
     */
    virtual void tick(double time) = 0;

    /**
     * Called upon the Strategy being 'done'
     */
    virtual void stop() {}

    /**
     * Set the strategy to execute after this one. Permitted to be nullptr
     */
    void then(std::shared_ptr<Strategy> next) { _next = next; }

    void set_timeout(double seconds) { timeout = seconds*1000; }

    bool done = false, first_run = true;
    double start_time = 0, timeout = -1;
    std::shared_ptr<Strategy> _next = nullptr;
  };

  /**
   * The StrategyController is the conductor for the Strategy system. It keeps
   * track of the currently running strategy and coordinates when to switch to
   * the next one.
   */
  class StrategyController {
  public:
    /**
     * Called periodically at the same rate (or higher) than the main robot class,
     * in any state.
     */
    void periodic();

    /**
     * Set the currently active Strategy. If there is already a Strategy in place,
     * this will kick out the previous Strategy and replace it.
     */
    void set_active(std::shared_ptr<Strategy> strat);

    /**
     * Add a Strategy onto the end of the Strategy chain. This will execute after all
     * currently queued strategies are completed.
     */
    void append(std::shared_ptr<Strategy> strat);

  private:
    std::shared_ptr<Strategy> active;
  };
} // namespace curtinfrc
