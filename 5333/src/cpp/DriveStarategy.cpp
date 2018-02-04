#include <iostream>

#include "DriveStarategy.h"
#include "curtinfrc/drivetrain.h"
#include "ControlMap.h"

#include "Map.h"

using namespace std;

DriveStarategy::DriveStarategy(Drivetrain *_drive, double _throttle) {
  drive = _drive;
  throttle = _throttle;

  left_bumper_toggle = right_bumper_toggle = false;
}

void DriveStarategy::start() {
  drive->set_left(0);
  drive->set_right(0);
  SmartDashboard::PutNumber("Throttle:", throttle);
}

void DriveStarategy::tick(double time) {
  // Only move if joystick is not in deadzone
  if(fabs(ControlMap::left_drive_power()) > Map::Controllers::deadzone) {
    double output_left = ControlMap::drive_reverse() ? ControlMap::right_drive_power() : ControlMap::left_drive_power();
    drive->set_left(output_left * throttle * (ControlMap::drive_reverse() ? -1 : 1));
  } else {
    drive->set_left(0);
  }

  if(fabs(ControlMap::right_drive_power()) > Map::Controllers::deadzone) {
    double output_right = ControlMap::drive_reverse() ? ControlMap::left_drive_power() : ControlMap::right_drive_power();
    drive->set_right(output_right * throttle * (ControlMap::drive_reverse() ? -1 : 1));
  } else {
    drive->set_right(0);
  }

  // Throttle Control
  if (left_bumper_toggle != ControlMap::throttle_decrement()) { // Prevent registering as multiple presses
    left_bumper_toggle = ControlMap::throttle_decrement();
    if (left_bumper_toggle) { // Left bumper decreases throttle, while right increases throttle
      throttle -= 0.1;
      throttle = round(max(throttle, 0.1) * 10) / 10;
      cout << "Throttle changed to " << throttle << endl;
      SmartDashboard::PutNumber("Throttle:", throttle);
    }
  } else if (right_bumper_toggle != ControlMap::throttle_increment()) {
    right_bumper_toggle = ControlMap::throttle_increment();
    if (right_bumper_toggle) {
      throttle += 0.1;
      throttle = round(min(throttle, 1.0) * 10) / 10;
      cout << "Throttle changed to " << throttle << endl;
      SmartDashboard::PutNumber("Throttle:", throttle);
    }
  }
}

void DriveStarategy::stop() {
  drive->set_left(0);
  drive->set_right(0);
}
