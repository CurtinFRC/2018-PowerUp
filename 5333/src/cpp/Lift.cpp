#include "Lift.h"
#include "IO.h"

void LiftControl::send_to_robot(double output) { // Sends the output to motor
  IO::get_instance()->lift_motor->Set(output);
}
