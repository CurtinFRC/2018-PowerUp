#pragma once

namespace ControlMap {
  double left_drive_power();
  double right_drive_power();

  bool throttle_decrement();
  bool throttle_increment();

  double belevator_motor_power();

  bool claw_state();
};
