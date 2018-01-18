#include "ControlMap.h"
#include "Map.h"
#include "IO.h"

using namespace std;

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

#if JOY_CONTROL == 1

double ControlMap::left_drive_power() { return IO::get_instance()->get_left_Y(); }
double ControlMap::right_drive_power() { return IO::get_instance()->get_right_Y(); }

#elif JOY_CONTROL == 2

double ControlMap::left_drive_power() {
  double f_speed = IO::get_instance()->get_right_Y();
  double r_speed = -IO::get_instance()->get_right_X();

  if (f_speed >= 0.0) {
    return r_speed >= 0.0 ? copysign(max(abs(f_speed), abs(r_speed)), f_speed) : f_speed + r_speed;
  } else {
    return r_speed >= 0.0 ? f_speed + r_speed : copysign(max(abs(f_speed), abs(r_speed)), f_speed);
  }
}

double ControlMap::right_drive_power() {
  double f_speed = IO::get_instance()->get_right_Y();
  double r_speed = -IO::get_instance()->get_right_X();

  if (f_speed >= 0.0) {
    return r_speed >= 0.0 ? f_speed - r_speed : copysign(max(abs(f_speed), abs(r_speed)), f_speed);
  } else {
    return r_speed >= 0.0 ? copysign(max(abs(f_speed), abs(r_speed)), f_speed) : f_speed - r_speed;
  }
}

#endif

bool ControlMap::throttle_decrement() { return IO::get_instance()->get_left_trigger(); }
bool ControlMap::throttle_increment() { return IO::get_instance()->get_right_trigger(); }

double ControlMap::belevator_motor_power() { return IO::get_instance()->get_right_twist(); }

bool ControlMap::claw_state() {
  return IO::get_instance()->get_left_twist() > 0.2; // Deadzone
}

#endif
