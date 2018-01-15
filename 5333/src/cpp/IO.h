#pragma once

#include "WPILib.h"
#include "curtinfrc/motors/CurtinTalonSRX.h"

using namespace frc;
using namespace curtinfrc;

class IO {
public:
  CurtinTalonSRX *left_motors[2];
  CurtinTalonSRX *right_motors[2];

  CurtinTalonSRX *lift_motor;
  CurtinTalonSRX *intake_motor;

  XboxController *xbox;

  void setup();

  double get_left_trigger();
  double get_right_trigger();
  bool get_left_bumper();
  bool get_right_bumper();
  double get_left_y();
  double get_right_y();

  static IO *get_instance();
};
