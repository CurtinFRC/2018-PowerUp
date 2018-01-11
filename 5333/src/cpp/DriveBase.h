#pragma once

// Indexes functions in DriveBase.cpp
class DriveControl {
public:
  double get_tank_left(double input_left_y);
  double get_tank_right(double input_right_y);
  void send_to_robot(double input_left_y, double input_right_y);
};
