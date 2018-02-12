#include "Intake.h"
#include "IO.h"

#include <RobotController.h>

void IntakeControl::send_to_robot(bool open) { // Set the pneumatics
  IO::get_instance()->intake_solenoids[0]->Set(open ? DoubleSolenoid::Value::kForward : DoubleSolenoid::Value::kOff);
}

void IntakeControl::send_to_robot(double left, double right) { // Set the intake motors
  for(auto motor : IO::get_instance()->intake_motors[0]) motor->Set(left);
  for(auto motor : IO::get_instance()->intake_motors[1]) motor->Set(right);
}

void IntakeControl::log_write() {
  auto io = IO::get_instance();
  log.write(::frc::RobotController::GetFPGATime(), 2, io->intake_motors[0][0]->GetSelectedSensorPosition(0), io->intake_motors[1][0]->GetSelectedSensorPosition(0), io->intake_solenoids[0]->Get(), io->intake_solenoids[1]->Get());
}
