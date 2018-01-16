#include "WPIlib.h"
#include "Claw.h"
#include "IO.h"

void ClawControl::send_to_robot(DoubleSolenoid::Value output) { // Sends the output to motor
  IO::get_instance()->claw_solenoid->Set(output);
}
