#include "Intake.h"
#include "IO.h"

#include <RobotController.h>

void IntakeControl::send_to_robot(bool open) { // Set the pneumatics
  IO::get_instance()->intake_solenoids[0]->Set(open ? DoubleSolenoid::Value::kForward : DoubleSolenoid::Value::kOff);
}

void IntakeControl::send_to_robot(double left, double right) { // Set the intake motors
  for (auto motor : IO::get_instance()->intake_motors_left) motor->Set(left);
  for (auto motor : IO::get_instance()->intake_motors_right) motor->Set(right);
}

void IntakeControl::send_to_robot(double power) {
  send_to_robot(power, power);
}

void IntakeControl::log_write() {
  auto io = IO::get_instance();
  log.write(::frc::RobotController::GetFPGATime(), 2, io->intake_motors_left[0]->GetSelectedSensorPosition(0), io->intake_motors_right[0]->GetSelectedSensorPosition(0), io->intake_solenoids[0]->Get(), io->intake_solenoids[1]->Get());
}
