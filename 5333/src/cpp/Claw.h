#pragma once

#include "WPILib.h"

class ClawControl { // Probably pneumatics
public:
  void send_to_robot(DoubleSolenoid::Value input);
};
