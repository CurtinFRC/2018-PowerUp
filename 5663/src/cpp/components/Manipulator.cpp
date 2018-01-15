#include "components/Manipulator.h"

using namespace components;

// Constructor for Manipulator class
Manipulator::Manipulator(int intakePort, int restrainerFwd, int restrainerRev) {
  intake = new TalonSRX(intakePort);
  restrainer = new DoubleSolenoid(restrainerFwd, restrainerRev);
}

// Set speed of intake motor
void Manipulator::SetIntakeSpeed(int speed) {
  intake->Set(ControlMode::PercentOutput, speed);
}

// Extend actuator to close restrainer and restrain power cube
void Manipulator::Restrain() {
  restrainer->Set(restrainer->kForward);
}

// Retract actuator to open restrainer and release power cube
void Manipulator::Release() {
  restrainer->Set(restrainer->kReverse);
}
