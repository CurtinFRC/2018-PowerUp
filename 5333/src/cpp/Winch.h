#pragma once

#include "curtinfrc/logger.h"

class WinchControl {
public:
  WinchControl() : log("log_winch", "winch_motor") {}
  void send_to_robot(double power);
  void log_write();
private:
  curtinfrc::Logger log;
};
