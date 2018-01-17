#include "components/Manipulator.h"

using namespace components;

// Constructor for Manipulator class
Manipulator::Manipulator(int intakePort, int restrainerFwd, int restrainerRev) {
  intake = new TalonSRX(intakePort);
  restrainer = new DoubleSolenoid(0, restrainerFwd, restrainerRev);
}

// Set speed of intake motor
void Manipulator::SetIntakeSpeed(double speed) {
  intake->Set(ControlMode::PercentOutput, speed);
}

// Extend actuator to close restrainer and restrain power cube
void Manipulator::Restrain() {
  intakeSpeed = 0;
  intake->Set(ControlMode::PercentOutput, 0);
  restrainer->Set(restrainer->kForward);
  SmartDashboard::PutNumber("arm", restrainer->Get());
  SmartDashboard::PutNumber("intakeSpeed", intakeSpeed);
}

// Retract actuator to open restrainer and release power cube
void Manipulator::Release() {
  intakeSpeed += 0.02;
  intake->Set(ControlMode::PercentOutput, intakeSpeed);
  restrainer->Set(restrainer->kReverse);
  SmartDashboard::PutNumber("arm", restrainer->Get());
  SmartDashboard::PutNumber("intakeSpeed", intakeSpeed);
}
