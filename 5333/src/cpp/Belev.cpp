#include "Belev.h"
#include "IO.h"

#include <RobotController.h>

void BelevatorControl::lift_speed(double output) {
  if (IO::get_instance()->get_belev_limit_max() && output > 0) output = 0;
  else if (IO::get_instance()->get_belev_limit_min() && output < 0) output = 0;

  for (auto motor : IO::get_instance()->belev_motors) motor->Set(output); // Set for all motors
}

void BelevatorControl::claw(bool open) {
  for (auto solonoid : IO::get_instance()->intake_solenoids) solonoid->Set(open ? DoubleSolenoid::Value::kForward : DoubleSolenoid::Value::kOff);
}

void BelevatorControl::intake(double left, double right) {
  for (auto motor : IO::get_instance()->intake_motors_left) motor->Set(left);
  for (auto motor : IO::get_instance()->intake_motors_right) motor->Set(right);
}

void BelevatorControl::intake(double power) {
  intake(power, power);
}

void BelevatorControl::log_write() {
  auto io = IO::get_instance();
  log.write(::frc::RobotController::GetFPGATime(), 5, io->belev_motors[0]->GetSelectedSensorPosition(0), io->intake_solenoids[0]->Get(), io->intake_solenoids[1]->Get(), io->intake_motors_left[0]->GetSelectedSensorPosition(0), io->intake_motors_right[0]->GetSelectedSensorPosition(0));
}
