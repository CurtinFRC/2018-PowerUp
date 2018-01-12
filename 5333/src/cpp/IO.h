#pragma once
#include "WPILib.h"
#include "CANTalon.h"

using namespace frc;

class IO {
public:
  CANTalon *left_motors[2];
  CANTalon *right_motors[2];

  CANTalon *lift_motor[1];

  // DoubleSolonoid *loader;

  XboxController *xbox;

  double get_left_trigger();
  double get_right_trigger();
  double get_left_y();
  double get_right_y();

  void setup();
  static IO *get_instance();
};
