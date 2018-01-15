#include "Claw.h"
#include "IO.h"

void ClawControl::send_to_robot(double output) { // Sends the output to motor
  IO::get_instance()->intake_motor->Set(output);
}
