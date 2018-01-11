#include "Arm.h"
#include "IO.h"

void ArmControl::send_to_robot(double output) { // Sends the outputs to motors
  IO::get_instance()->arm_motor->Set(output);
}
