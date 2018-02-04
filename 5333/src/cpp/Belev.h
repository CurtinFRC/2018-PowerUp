#pragma once

#include "curtinfrc/logger.h"

class BelevatorControl {
public:
  BelevatorControl() : log("log_belev", "belev_motor") {}
  void send_to_robot(double input);
  void log_write();
private:
  curtinfrc::Logger log;
};
