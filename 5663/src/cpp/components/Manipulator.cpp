#include "components/Manipulator.h"
#include <Spark.h>
using namespace components;

// Constructor for Manipulator class
Manipulator::Manipulator(int intakePort, int restrainerFwd, int restrainerRev) {
  intake = new Spark(intakePort);
  restrainer = new DoubleSolenoid(0, restrainerFwd, restrainerRev);
}

// Set speed of intake motor
void Manipulator::SetIntakeSpeed(double speed) {
  if(fabs(speed) < deadzone) {
    intakeSpeed = speed;
  }
}

// Extend actuator to close restrainer and restrain power cube
void Manipulator::Restrain() {
  //intakeSpeed = 0;
  restrainer->Set(restrainer->kForward);
  SmartDashboard::PutNumber("arm", restrainer->Get());
}

// Retract actuator to open restrainer and release power cube
void Manipulator::Release() {
  //intakeSpeed += 0.02;
  restrainer->Set(restrainer->kReverse);
  SmartDashboard::PutNumber("arm", restrainer->Get());
}

void Manipulator::RunPeriodic() {
  if(intakeSpeed > 1) intakeSpeed = 1;
  if(intakeSpeed < -1) intakeSpeed = -1;
  intake->Set(intakeSpeed);
  SmartDashboard::PutNumber("intakeSpeed", intakeSpeed);
}
