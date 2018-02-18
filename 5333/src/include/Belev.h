#pragma once

#include "curtinfrc/logger.h"

class BelevatorControl {
public:
  BelevatorControl() : log("log_belev", "belev_motors,intake_left_solenoid,intake_right_solenoid,intake_left,intake_right") {}
  enum class Gear {
    High,
    Low
  };

  void tick();
  void lift(double power);
  void winch_mode(Gear mode = Gear::High);
  void winch_brake(bool enabled);
  void claw(bool open);
  void intake(double left, double right);
  void intake(double power);
  void log_write();
private:
  curtinfrc::Logger log;
};
