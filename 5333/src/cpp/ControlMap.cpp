#include "ControlMap.h"
#include "Map.h"
#include "IO.h"

#ifdef XBOX_CONTROL

double ControlMap::left_drive_power() { return IO::get_instance()->get_left_Y(); }
double ControlMap::right_drive_power() { return IO::get_instance()->get_right_Y(); }

bool ControlMap::throttle_decrement() { return IO::get_instance()->get_left_bumper(); }
bool ControlMap::throttle_increment() { return IO::get_instance()->get_right_bumper(); }

double ControlMap::belevator_motor_power() {
  return IO::get_instance()->get_right_trigger() - IO::get_instance()->get_left_trigger();
}

bool ControlMap::claw_state() {
  return IO::get_instance()->get_left_stick() || IO::get_instance()->get_right_stick();
}

#elif JOY_CONTROL

double ControlMap::left_drive_power() { return IO::get_instance()->get_left_Y(); }
double ControlMap::right_drive_power() { return IO::get_instance()->get_right_Y(); }

bool ControlMap::throttle_decrement() { return IO::get_instance()->get_left_trigger(); }
bool ControlMap::throttle_increment() { return IO::get_instance()->get_right_trigger(); }

double ControlMap::belevator_motor_power() { return IO::get_instance()->get_right_twist(); }

bool ControlMap::claw_state() {
  return IO::get_instance()->get_left_Z() > 0.2; // Deadzone
}

#endif
