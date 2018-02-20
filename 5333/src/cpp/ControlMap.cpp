#include "ControlMap.h"
#include "Map.h"
#include "IO.h"

#include "curtinfrc/toggle.h"

#include <iostream>
#include <string>

using namespace std;

static curtinfrc::Toggle *shifter_toggle;
static bool shifter_state;

int ControlMap::init() {
  shifter_toggle = new curtinfrc::Toggle(curtinfrc::Toggle::Trigger::True);
  shifter_state = true;

  return 0;
}

bool ControlMap::throttle_decrement() { return IO::get_instance()->get_right_button(11); }
bool ControlMap::throttle_increment() { return IO::get_instance()->get_right_button(12); }

double ControlMap::left_drive_power() {
  double left_power = 0;

  if (IO::get_instance()->get_right_button(4)) left_power = 1;
  else {
    double f_speed = IO::get_instance()->get_right_Y();
    double r_speed = -(IO::get_instance()->get_right_X() * 0.5) - (IO::get_instance()->get_right_X() * IO::get_instance()->get_right_twist() * IO::get_instance()->get_right_twist() * (1 - 0.5));

    if (f_speed >= 0.0) {
      left_power = r_speed >= 0.0 ? f_speed - r_speed : max(f_speed, -r_speed);
    } else {
      left_power = r_speed >= 0.0 ? -max(-f_speed, r_speed) : f_speed - r_speed;
    }
  }

  SmartDashboard::PutNumber("Left Power:", left_power);
  return left_power;
}

double ControlMap::right_drive_power() {
  double right_power = 0;

  if (IO::get_instance()->get_right_button(4)) right_power = 1;
  else {
    double f_speed = IO::get_instance()->get_right_Y();
    double r_speed = -(IO::get_instance()->get_right_X() * 0.5) - (IO::get_instance()->get_right_X() * IO::get_instance()->get_right_twist() * IO::get_instance()->get_right_twist() * (1 - 0.5));

    if (f_speed > 0.0) {
      right_power = r_speed > 0.0 ? max(f_speed, r_speed) : f_speed + r_speed;
    } else {
      right_power = r_speed > 0.0 ? f_speed + r_speed : -max(-f_speed, -r_speed);
    }
  }

  SmartDashboard::PutNumber("Right Power:", right_power);
  return right_power;
}

bool ControlMap::drive_reverse() { return IO::get_instance()->get_right_button(2); }

double ControlMap::belevator_motor_power() {
  if (IO::get_instance()->get_right_button(10) || IO::get_instance()->get_right_button(9))
    return (int)IO::get_instance()->get_right_button(10) - (int)IO::get_instance()->get_right_button(9);
  return IO::get_instance()->get_left_Y();
}

double ControlMap::intake_motor_power() { return (int)IO::get_instance()->get_right_button(5) - (int)IO::get_instance()->get_right_button(3); }
bool ControlMap::intake_claw_state() { return IO::get_instance()->get_right_button(7); }

bool ControlMap::winch_shifter_state() {
  if (shifter_toggle->apply(IO::get_instance()->get_right_button(6))) return (shifter_state = !shifter_state);
  if (shifter_toggle->apply(IO::get_instance()->get_left_button(2))) return (shifter_state = !shifter_state);
  return shifter_state;
}
