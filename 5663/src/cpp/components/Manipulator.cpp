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
  if(fabs(speed) > deadzone) {
    intakeSpeed = speed;
  } else intakeSpeed = 0;
}

// Extend actuator to close restrainer and restrain power cube
void Manipulator::Restrain() {
  restrainer->Set(restrainer->kForward);
  SmartDashboard::PutNumber("arm", restrainer->Get());
}

// Retract actuator to open restrainer and grab power cube
void Manipulator::Release() {
  if(!intakeOverride) intakeSpeed = -1;
  restrainer->Set(restrainer->kReverse);
  SmartDashboard::PutNumber("arm", restrainer->Get());
}

void Manipulator::OverrideIntake(bool overridden) {
  intakeOverride = overridden;
}

void Manipulator::RunPeriodic() {
  intake->Set(intakeSpeed);
  SmartDashboard::PutNumber("Intake Speed", intakeSpeed);
  if(restrainer->Get() == restrainer->kReverse) SmartDashboard::PutBoolean("Open/Closed", true); //Open
  else SmartDashboard::PutBoolean("Open/Closed", false); //Closed
}
