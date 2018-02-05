#pragma once

#include "curtinfrc/logger.h"

class IntakeControl {
public:
  IntakeControl() : log("log_intake", "left_wheel,right_wheel,left_solenoid,right_solenoid") {}
  void send_to_robot(bool open);
  void send_to_robot(double left, double right);
  void send_to_robot(double power);
  void log_write();
private:
  curtinfrc::Logger log;
};
