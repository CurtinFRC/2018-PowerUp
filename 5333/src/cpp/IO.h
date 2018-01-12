#pragma once
#include "WPILib.h"
#include "curtinfrc/motors/CurtinTalonSRX.h"

using namespace frc;
using namespace curtinfrc;

class IO {
public:
  CurtinTalonSRX *left_motors[2];
  CurtinTalonSRX *right_motors[2];

  CurtinTalonSRX *lift_motor[1];
  CurtinTalonSRX *intake_motor[1];

  XboxController *xbox;

  void setup();

  double get_left_trigger();
  double get_right_trigger();
  double get_left_y();
  double get_right_y();
  bool get_right_bumper();

  static IO *get_instance();
};
