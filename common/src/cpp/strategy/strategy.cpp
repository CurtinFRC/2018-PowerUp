#include "curtinfrc/strategy/strategy.h"

#include <chrono>

using namespace curtinfrc;

void StrategyController::periodic() {
  if (active != nullptr) {
    auto now = std::chrono::system_clock::now();
    double now_ms = static_cast<double>(std::chrono::time_point_cast<std::chrono::milliseconds>(now).time_since_epoch().count());
    if (active->first_run) {
      active->first_run = false;
      active->start_time = now_ms;
      active->start();
    }
    active->tick(now_ms - active->start_time);
    if (active->done || (active->timeout > 0 && (now_ms - active->start_time) > active->timeout)) {
      active->stop();
      active = active->_next;
    }
  }
}

void StrategyController::set_active(std::shared_ptr<Strategy> strat) {
  if (active != nullptr) {
    active->stop();
  }
  active = strat;
}

void StrategyController::append(std::shared_ptr<Strategy> strat) {
  std::shared_ptr<Strategy> current = active;
  bool crawl = true;
  while (crawl) {
    if (current->_next != nullptr) {
      current = current->_next;
    } else {
      current->_next = strat;
      crawl = false;
    }
  }
}