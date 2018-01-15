#include "components/Lift.h"
#include <SmartDashboard/SmartDashboard.h>
using namespace components;

// Constructor for Lift class
Lift::Lift(int m1, int m2) {
  motor1 = new TalonSRX(m1);
  motor2 = new TalonSRX(m2);

  motor2->Set(ControlMode::Follower, m1);

  motor1->ConfigNominalOutputForward(0, 0);
	motor1->ConfigNominalOutputReverse(0, 0);
	motor1->ConfigPeakOutputForward(1, 0);
	motor1->ConfigPeakOutputReverse(-1, 0);
  motor1->Config_kF(0, 0.0147, 10);
  motor1->ConfigMotionAcceleration(1500, 10);
  motor1->ConfigMotionCruiseVelocity(3600, 10);
  motor1->SetSensorPhase(true);
}

// Move lift to high position (for scale)
void Lift::SetHighPosition() {
  motor1->Set(ControlMode::MotionMagic, 994);
  pos = 2;
  manualMode = false;
}

// Move lift to mid position (for switch)
void Lift::SetMidPosition() {
  motor1->Set(ControlMode::MotionMagic, 1988);
  pos = 1;
  manualMode = false;
}

// Move lift to low position
void Lift::SetLowPosition() {
  motor1->Set(ControlMode::MotionMagic, 497);
  pos = 0;
  manualMode = false;
}

// Set speed of Lift class motors
void Lift::SetSpeed(double speed) {
  if(abs(speed) < deadzone) {
    speed = 0;
    if(manualMode) {
      motor1->Set(ControlMode::Velocity, 0);
    }
  } else {
    manualMode = true;
    speed *= abs(speed);
    motor1->Set(ControlMode::Velocity, speed); //Need to test later
    pos = 3;
  }
}

// Reset Lift class motor encoder
void Lift::ResetEncoder() {
  motor1->SetSelectedSensorPosition(0, 0, 10);
}

// Run periodic tasks
void Lift::RunPeriodic() {
  SmartDashboard::PutNumber("Lift Encoder", motor1->GetSelectedSensorPosition(0));

  switch(pos) {
    case 0:
      SmartDashboard::PutString("Lift Position", "Low");
      break;
    case 1:
      SmartDashboard::PutString("Lift Position", "Mid");
      break;
    case 2:
      SmartDashboard::PutString("Lift Position", "High");
      break;
    default:
      SmartDashboard::PutString("Lift Position", "Manual");
      break;
  }
}
