#pragma once

#include "curtinfrc/logger.h"

class BelevatorControl {
public:
  BelevatorControl();
  void lift_speed(double input);
  void claw(bool open);
  void intake(double left, double right);
  void intake(double power);
};
