#include "Belev.h"
#include "IO.h"

#include <RobotController.h>

void BelevatorControl::tick() {
  if (IO::get_instance()->get_belev_limit_min()) IO::get_instance()->belev_motors[0]->SetSelectedSensorPosition(0, 0, 0);
}

void BelevatorControl::lift(double power) {
  IO::get_instance()->belev_motors[0]->Set(power);
}

void BelevatorControl::winch_mode(BelevatorControl::Gear mode) {
  IO::get_instance()->shifter_solenoid->Set(mode == BelevatorControl::Gear::High ? DoubleSolenoid::Value::kForward : DoubleSolenoid::Value::kReverse);
}

void BelevatorControl::winch_brake(bool enabled) {
  IO::get_instance()->brake_solenoid->Set(enabled ? DoubleSolenoid::Value::kForward : DoubleSolenoid::Value::kReverse);
}


void BelevatorControl::claw(bool open) {
  for (auto solenoid : IO::get_instance()->intake_solenoids) solenoid->Set(open ? DoubleSolenoid::Value::kForward : DoubleSolenoid::Value::kReverse);
}

void BelevatorControl::intake(double left, double right) {
  for (auto motor : IO::get_instance()->intake_motors_left) motor->Set(left);
  for (auto motor : IO::get_instance()->intake_motors_right) motor->Set(right);
}

void BelevatorControl::intake(double power) {
  intake(power, power);
}