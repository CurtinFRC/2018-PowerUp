#pragma once

#include "WPILib.h"
#include "curtinfrc/motors/CurtinTalonSRX.h"

#include "AHRS.h"
#include "Map.h"

using namespace frc;
using namespace curtinfrc;

class IO {
public:
  int init();

  CurtinTalonSRX *left_motors[Map::Motors::n_drive_motors];
  CurtinTalonSRX *right_motors[Map::Motors::n_drive_motors];

  CurtinTalonSRX *belev_motors[Map::Motors::n_belev_motors];
  CurtinTalonSRX *intake_motors_left[Map::Motors::n_intake_motors];
  CurtinTalonSRX *intake_motors_right[Map::Motors::n_intake_motors];

  DoubleSolenoid *shifter_solenoid;
  DoubleSolenoid *brake_solenoid;
  DoubleSolenoid *intake_solenoids[Map::Pneumatics::n_intake_solenoids];


  AHRS *navx;

  DigitalInput *belev_limit_max, *belev_limit_min;

  Joystick *left_joy, *right_joy;

  double get_left_Y();
  double get_left_X();
  double get_left_twist();

  bool get_left_trigger();
  bool get_left_button(int nButton);


  double get_right_Y();
  double get_right_X();
  double get_right_twist();

  bool get_right_trigger();
  bool get_right_button(int nButton);

  bool get_belev_limit_max();
  bool get_belev_limit_min();


  static IO *get_instance();
};
