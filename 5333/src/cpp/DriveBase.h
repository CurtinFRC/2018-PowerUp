#pragma once

#include "IO.h"

// Indexes functions in DriveBase.cpp
class DriveControl {
public:
  DriveControl(CurtinSRX *left[2], CurtinSRX *right[2]);

  double get_tank_left(double input_left_y);
  double get_tank_right(double input_right_y);

  void set_left(double input_left_speed);
  void set_right(double input_right_speed);

private:
  CurtinSRX *left[2];
  CurtinSRX *right[2];
};
