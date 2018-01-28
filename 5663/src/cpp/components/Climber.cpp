#include "components/Climber.h"

using namespace components;

// Constructor for Climber class
Climber::Climber(int m) {
  motor = new TalonSRX(m);
}

// Set climber motor speed
void Climber::SetSpeed(double speed) {
  if(speed > 0.1) {
    motor->Set(ControlMode::PercentOutput, fabs(speed));
  } else {
    motor->NeutralOutput();
  }
}
