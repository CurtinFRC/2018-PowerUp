#include "Intake.h"
#include "IO.h"

void IntakeControl::send_to_robot(double output) { // Sends the output to motor
  IO::get_instance()->intake_motor[0]->Set(output);
}
