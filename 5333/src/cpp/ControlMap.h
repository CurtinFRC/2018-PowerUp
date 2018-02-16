#pragma once

namespace ControlMap {
  int init();

  double left_drive_power();
  double right_drive_power();

  bool drive_reverse();

  bool throttle_decrement();
  bool throttle_increment();

  double belevator_motor_power();

  double intake_motor_power();
  bool intake_claw_state();

  double winch_power();
};
