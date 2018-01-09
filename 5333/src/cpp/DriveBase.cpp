#include "curtinfrc/math.h"
#include "DriveBase.h"
#include "IO.h"

using namespace curtinfrc; // Curtin's math functions

// Tank control object
double DriveControl::get_tank_left(double input_left_y) {
  return math::square_keep_sign(input_left_y); // Not inverted due to inverted motors
}
double DriveControl::get_tank_right(double input_right_y) {
  return -math::square_keep_sign(input_right_y);
}

void DriveControl::send_to_robot(double output_left, double output_right) { // Sends the outputs to motors
  IO::get_instance()->left_motors[0]->Set(output_left);
  IO::get_instance()->left_motors[1]->Set(output_left);

  IO::get_instance()->right_motors[0]->Set(output_right);
  IO::get_instance()->right_motors[1]->Set(output_right);
}
