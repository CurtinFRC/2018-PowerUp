#include "ControlMap.h"
#include "Map.h"
#include "IO.h"
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

#if JOY_CONTROL == 1 // Tank drive joysticks

bool ControlMap::throttle_decrement() { return IO::get_instance()->get_left_trigger(); }
bool ControlMap::throttle_increment() { return IO::get_instance()->get_right_trigger(); }

double ControlMap::left_drive_power() { return IO::get_instance()->get_left_Y(); }
double ControlMap::right_drive_power() { return IO::get_instance()->get_right_Y(); }

#elif JOY_CONTROL == 2 // Right joystick drive

bool ControlMap::throttle_decrement() { return IO::get_instance()->get_left_trigger(); }
bool ControlMap::throttle_increment() { return IO::get_instance()->get_right_trigger(); }

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

#elif JOY_CONTROL == 3 // Single joystick drive

bool ControlMap::throttle_decrement() {
  if (IO::get_instance()->get_left_trigger() && IO::get_instance()->get_right_trigger() || (!IO::get_instance()->get_left_trigger() && !IO::get_instance()->get_right_trigger())) {
    return IO::get_instance()->get_left_button(3); // For tank or during neither
  } else if (IO::get_instance()->get_left_trigger()) { // The following "if"s for the single control
    return IO::get_instance()->get_left_button(3);
  } else if (IO::get_instance()->get_right_trigger()) {
    return IO::get_instance()->get_right_button(3);
  } else {
    return false; // Just in case
  }
}
bool ControlMap::throttle_increment() {
  if (IO::get_instance()->get_left_trigger() && IO::get_instance()->get_right_trigger() || (!IO::get_instance()->get_left_trigger() && !IO::get_instance()->get_right_trigger())) {
    return IO::get_instance()->get_right_button(4);
  } else if (IO::get_instance()->get_left_trigger()) {
    return IO::get_instance()->get_left_button(4);
  } else if (IO::get_instance()->get_right_trigger()) {
    return IO::get_instance()->get_right_button(4);
  } else {
    return false;
  }
}

double ControlMap::left_drive_power() {
  if (IO::get_instance()->get_left_trigger() && IO::get_instance()->get_right_trigger()) { // Tank drive
    return IO::get_instance()->get_left_Y();
  } else if (IO::get_instance()->get_left_trigger()) { // Else single drive
    double f_speed = IO::get_instance()->get_left_Y();
    double r_speed = -IO::get_instance()->get_left_X();

    if (f_speed >= 0.0) {
      return r_speed >= 0.0 ? copysign(max(abs(f_speed), abs(r_speed)), f_speed) : f_speed + r_speed;
    } else {
      return r_speed >= 0.0 ? f_speed + r_speed : copysign(max(abs(f_speed), abs(r_speed)), f_speed);
    }
  } else if (IO::get_instance()->get_right_trigger()) {
    double f_speed = IO::get_instance()->get_right_Y();
    double r_speed = -IO::get_instance()->get_right_X();

    if (f_speed >= 0.0) {
      return r_speed >= 0.0 ? copysign(max(abs(f_speed), abs(r_speed)), f_speed) : f_speed + r_speed;
    } else {
      return r_speed >= 0.0 ? f_speed + r_speed : copysign(max(abs(f_speed), abs(r_speed)), f_speed);
    }
  } else {
    return 0;
  }
}

double ControlMap::right_drive_power() {
  if (IO::get_instance()->get_left_trigger() && IO::get_instance()->get_right_trigger()) {
    return IO::get_instance()->get_right_Y();
  } else if (IO::get_instance()->get_left_trigger()) {
    double f_speed = IO::get_instance()->get_left_Y();
    double r_speed = -IO::get_instance()->get_left_X();

    if (f_speed >= 0.0) {
      return r_speed >= 0.0 ? f_speed - r_speed : copysign(max(abs(f_speed), abs(r_speed)), f_speed);
    } else {
      return r_speed >= 0.0 ? copysign(max(abs(f_speed), abs(r_speed)), f_speed) : f_speed - r_speed;
    }
  } else if (IO::get_instance()->get_right_trigger()) {
    double f_speed = IO::get_instance()->get_right_Y();
    double r_speed = -IO::get_instance()->get_right_X();

    if (f_speed >= 0.0) {
      return r_speed >= 0.0 ? f_speed - r_speed : copysign(max(abs(f_speed), abs(r_speed)), f_speed);
    } else {
      return r_speed >= 0.0 ? copysign(max(abs(f_speed), abs(r_speed)), f_speed) : f_speed - r_speed;
    }
  } else {
    return 0;
  }
}

#endif

double ControlMap::belevator_motor_power() { return IO::get_instance()->get_right_twist(); }

bool ControlMap::claw_state() {
  return IO::get_instance()->get_left_twist() > 0.2; // Deadzone
}

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
  } else if (IO::get_instance()->get_left_xbox_stick() || IO::get_instance()->get_right_xbox_stick()) {
    mode = "Driving Overridden";
    left_power = IO::get_instance()->get_left_xbox_Y();
  } else {
    if (IO::get_instance()->get_left_trigger() && IO::get_instance()->get_right_trigger()) {
      left_power = IO::get_instance()->get_left_Y();
    } else if (IO::get_instance()->get_left_trigger()) {
      double f_speed = IO::get_instance()->get_left_Y();
      double r_speed = -IO::get_instance()->get_left_twist();
      if (abs(IO::get_instance()->get_left_X()) > 0.3) r_speed -= IO::get_instance()->get_left_X();
      r_speed = min(r_speed, 1.0);

      if (f_speed >= 0.0) {
        left_power = r_speed >= 0.0 ? copysign(max(abs(f_speed), abs(r_speed)), f_speed) : f_speed + r_speed;
      } else {
        left_power = r_speed >= 0.0 ? f_speed + r_speed : copysign(max(abs(f_speed), abs(r_speed)), f_speed);
      }
    } else if (IO::get_instance()->get_right_trigger()) {
      double f_speed = IO::get_instance()->get_right_Y();
      double r_speed = -IO::get_instance()->get_right_twist();
      if (abs(IO::get_instance()->get_right_X()) > 0.3) r_speed -= IO::get_instance()->get_right_X();
      r_speed = min(r_speed, 1.0);

      if (f_speed >= 0.0) {
        left_power = r_speed >= 0.0 ? copysign(max(abs(f_speed), abs(r_speed)), f_speed) : f_speed + r_speed;
      } else {
        left_power = r_speed >= 0.0 ? f_speed + r_speed : copysign(max(abs(f_speed), abs(r_speed)), f_speed);
      }
    }
  }

  SmartDashboard::PutString("Mode:", mode);
  return left_power;
}

double ControlMap::right_drive_power() {
  double right_power = 0;
  string mode = "Driving";

  if (IO::get_instance()->get_xbox_A()) {
    mode = "BRAKE";
    right_power = 0;
  } else if (IO::get_instance()->get_left_xbox_stick() || IO::get_instance()->get_right_xbox_stick()) {
    mode = "Driving Overridden";
    right_power = IO::get_instance()->get_right_xbox_Y();
  } else {
    if (IO::get_instance()->get_left_trigger() && IO::get_instance()->get_right_trigger()) {
      right_power = IO::get_instance()->get_right_Y();
    } else if (IO::get_instance()->get_left_trigger()) {
      double f_speed = IO::get_instance()->get_left_Y();
      double r_speed = -IO::get_instance()->get_left_twist();
      if (abs(IO::get_instance()->get_left_X()) > 0.3) r_speed -= IO::get_instance()->get_left_X();
      r_speed = min(r_speed, 1.0);

      if (f_speed >= 0.0) {
        right_power = r_speed >= 0.0 ? f_speed - r_speed : copysign(max(abs(f_speed), abs(r_speed)), f_speed);
      } else {
        right_power = r_speed >= 0.0 ? copysign(max(abs(f_speed), abs(r_speed)), f_speed) : f_speed - r_speed;
      }
    } else if (IO::get_instance()->get_right_trigger()) {
      double f_speed = IO::get_instance()->get_right_Y();
      double r_speed = -IO::get_instance()->get_right_twist();
      if (abs(IO::get_instance()->get_right_X()) > 0.3) r_speed -= IO::get_instance()->get_right_X();
      r_speed = min(r_speed, 1.0);

      if (f_speed >= 0.0) {
        right_power = r_speed >= 0.0 ? f_speed - r_speed : copysign(max(abs(f_speed), abs(r_speed)), f_speed);
      } else {
        right_power = r_speed >= 0.0 ? copysign(max(abs(f_speed), abs(r_speed)), f_speed) : f_speed - r_speed;
      }
    }
  }

  SmartDashboard::PutString("Mode:", mode);
  return right_power;
}

double ControlMap::belevator_motor_power() { return IO::get_instance()->get_right_twist(); }

bool ControlMap::claw_state() {
  return IO::get_instance()->get_left_twist() > 0.2; // Deadzone
}

#endif
