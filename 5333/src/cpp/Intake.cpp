#include "Intake.h"
#include "IO.h"

void IntakeControl::send_to_robot(bool open) { // Set the pneumatics
  IO::get_instance()->intake_solenoids[0]->Set(open ? DoubleSolenoid::Value::kForward : DoubleSolenoid::Value::kOff);
}

void IntakeControl::send_to_robot(double left, double right) { // Set the intake motors
  for(auto motor : IO::get_instance()->intake_motors[0]) motor->Set(left);
  for(auto motor : IO::get_instance()->intake_motors[1]) motor->Set(right);
}
