#include "ControlMap.h"
#include "Map.h"
#include "IO.h"

#include <iostream>
#include <string>

using namespace std;

#ifdef XBOX_CONTROL

int ControlMap::init() { return 0; }

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

int ControlMap::init() { return 0; }

#elif DRIVER_TRAINING

int ControlMap::init() { return 0; }

bool ControlMap::throttle_decrement() {
  if (IO::get_instance()->get_left_xbox_bumper()) { return true; }

  if (IO::get_instance()->get_left_trigger() && IO::get_instance()->get_right_trigger() || (!IO::get_instance()->get_left_trigger() && !IO::get_instance()->get_right_trigger())) {
    return IO::get_instance()->get_left_button(3);
  } else if (IO::get_instance()->get_left_trigger()) {
    return IO::get_instance()->get_left_button(3);
  } else if (IO::get_instance()->get_right_trigger()) {
    return IO::get_instance()->get_right_button(3);
  }

  return false;
}
bool ControlMap::throttle_increment() {
  if (IO::get_instance()->get_right_xbox_bumper()) { return true; }

  if (IO::get_instance()->get_left_trigger() && IO::get_instance()->get_right_trigger() || (!IO::get_instance()->get_left_trigger() && !IO::get_instance()->get_right_trigger())) {
    return IO::get_instance()->get_right_button(4);
  } else if (IO::get_instance()->get_left_trigger()) {
    return IO::get_instance()->get_left_button(4);
  } else if (IO::get_instance()->get_right_trigger()) {
    return IO::get_instance()->get_right_button(4);
  }

  return false;
}

double ControlMap::left_drive_power() {
  double left_power = 0;
  string mode = "Driving";

  if (IO::get_instance()->get_xbox_A()) {
    mode = "BRAKE";
    left_power = 0;
  } else if (IO::get_instance()->get_left_xbox_stick() || IO::get_instance()->get_right_xbox_stick() || IO::get_instance()->get_left_xbox_trigger() || IO::get_instance()->get_right_xbox_trigger()) {
    mode = "Driving Overridden";
    left_power = IO::get_instance()->get_left_xbox_Y();
  } else {
    if (IO::get_instance()->get_left_trigger() && IO::get_instance()->get_right_trigger()) {
      left_power = IO::get_instance()->get_left_Y();
    } else if (IO::get_instance()->get_left_trigger()) {
      double f_speed = IO::get_instance()->get_left_Y();
      double r_speed = -(IO::get_instance()->get_left_X() * 0.5) - (IO::get_instance()->get_left_X() * IO::get_instance()->get_left_twist() * IO::get_instance()->get_left_twist() * (1 - 0.5));

      if (f_speed >= 0.0) {
        left_power = r_speed >= 0.0 ? f_speed - r_speed : max(f_speed, -r_speed);
      } else {
        left_power = r_speed >= 0.0 ? -max(-f_speed, r_speed) : f_speed - r_speed;
      }
    } else if (IO::get_instance()->get_right_trigger()) {
      double f_speed = IO::get_instance()->get_right_Y();
      double r_speed = -(IO::get_instance()->get_right_X() * 0.5) - (IO::get_instance()->get_right_X() * IO::get_instance()->get_right_twist() * IO::get_instance()->get_right_twist() * (1 - 0.5));

      if (f_speed >= 0.0) {
        left_power = r_speed >= 0.0 ? f_speed - r_speed : max(f_speed, -r_speed);
      } else {
        left_power = r_speed >= 0.0 ? -max(-f_speed, r_speed) : f_speed - r_speed;
      }
    }
  }

  SmartDashboard::PutString("Mode:", mode);
  SmartDashboard::PutNumber("Left Power:", left_power);
  return left_power;
}

double ControlMap::right_drive_power() {
  double right_power = 0;
  string mode = "Driving";

  if (IO::get_instance()->get_xbox_A()) {
    mode = "BRAKE";
    right_power = 0;
  } else if (IO::get_instance()->get_left_xbox_stick() || IO::get_instance()->get_right_xbox_stick() || IO::get_instance()->get_left_xbox_trigger() || IO::get_instance()->get_right_xbox_trigger()) {
    mode = "Driving Overridden";
    right_power = IO::get_instance()->get_right_xbox_Y();
  } else {
    if (IO::get_instance()->get_left_trigger() && IO::get_instance()->get_right_trigger()) {
      right_power = IO::get_instance()->get_right_Y();
    } else if (IO::get_instance()->get_left_trigger()) {
      double f_speed = IO::get_instance()->get_left_Y();
      double r_speed = -(IO::get_instance()->get_left_X() * 0.5) - (IO::get_instance()->get_left_X() * IO::get_instance()->get_left_twist() * IO::get_instance()->get_left_twist() * (1 - 0.5));

      if (f_speed > 0.0) {
        right_power = r_speed > 0.0 ? max(f_speed, r_speed) : f_speed + r_speed;
      } else {
        right_power = r_speed > 0.0 ? f_speed + r_speed : -max(-f_speed, -r_speed);
      }
    } else if (IO::get_instance()->get_right_trigger()) {
      double f_speed = IO::get_instance()->get_right_Y();
      double r_speed = -(IO::get_instance()->get_right_X() * 0.5) - (IO::get_instance()->get_right_X() * IO::get_instance()->get_right_twist() * IO::get_instance()->get_right_twist() * (1 - 0.5));

      if (f_speed > 0.0) {
        right_power = r_speed > 0.0 ? max(f_speed, r_speed) : f_speed + r_speed;
      } else {
        right_power = r_speed > 0.0 ? f_speed + r_speed : -max(-f_speed, -r_speed);
      }
    }
  }

  SmartDashboard::PutString("Mode:", mode);
  SmartDashboard::PutNumber("Right Power:", right_power);
  return right_power;
}

bool ControlMap::drive_reverse() {
  if (IO::get_instance()->get_right_trigger() && IO::get_instance()->get_left_trigger())
    return IO::get_instance()->get_left_button(2) || IO::get_instance()->get_right_button(2);
  else if (IO::get_instance()->get_right_trigger())
    return IO::get_instance()->get_right_button(2);
  else if (IO::get_instance()->get_left_trigger())
    return IO::get_instance()->get_left_button(2);

  return false;
}

double ControlMap::belevator_motor_power() { return IO::get_instance()->get_right_twist(); }

double ControlMap::intake_motor_power() {}
bool ControlMap::intake_claw_state() {}

double ControlMap::winch_power() { return (IO::get_instance()->get_left_button(11) ? -1 : 0) + (IO::get_instance()->get_left_button(12) ? 1 : 0); }

#endif
