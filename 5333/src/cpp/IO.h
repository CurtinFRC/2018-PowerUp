#pragma once
#include "WPILib.h"

using namespace frc;

class IO {
public:
  Victor *left_motors[2];
  Victor *right_motors[2];

  Victor *lift_motor[1];

  // DoubleSolonoid *loader;

  XboxController *xbox;

  void setup();
  static IO *get_instance();
};
