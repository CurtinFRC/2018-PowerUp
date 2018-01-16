#include "WPIlib.h"
#include "Claw.h"
#include "IO.h"

void ClawControl::send_to_robot(DoubleSolenoid::Value output) { // Sends the output to motor
  for(auto solenoid : IO::get_instance()->claw_solenoids) solenoid->Set(output);
}
