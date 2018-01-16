#pragma once

#include "WPILib.h"
#include "curtinfrc/motors/CurtinTalonSRX.h"

using namespace frc;
using namespace curtinfrc;

class IO {
public:
  CurtinTalonSRX *left_motors[2];
  CurtinTalonSRX *right_motors[2];

  CurtinTalonSRX *belev_motors[1];
  CurtinTalonSRX *intake_motors[2][1];

  DoubleSolenoid *intake_solenoids[2];
  DoubleSolenoid *claw_solenoids[1];

  XboxController *xbox;

  void setup();

  double get_left_trigger();
  bool get_left_bumper();
  double get_left_X();
  double get_left_Y();
  bool get_left_stick();

  double get_right_trigger();
  bool get_right_bumper();
  double get_right_X();
  double get_right_Y();
  bool get_right_stick();

  bool get_A();
  bool get_B();
  bool get_X();
  bool get_Y();
  bool get_back();
  bool get_start();

  static IO *get_instance();
};
