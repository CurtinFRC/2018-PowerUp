#pragma once

#include "WPILib.h"
#include "curtinfrc/logger.h"

class ClawControl { // Probably pneumatics
public:
  ClawControl() : log("log_claw", "claw_piston") {}
  void send_to_robot(DoubleSolenoid::Value input);
  void log_write();
private:
  curtinfrc::Logger log;
};
