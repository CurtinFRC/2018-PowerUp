#include "components/Lift.h"

using namespace components;

// Constructor for Lift class
Lift::Lift(int m1, int m2) {
  motor1 = new TalonSRX(m1);
  motor2 = new TalonSRX(m2);
  motor2->Set(ControlMode::Follower, m1);
}

void Lift::SetHighPosition() {
  motor1->Set(ControlMode::Position, 40);
}

void Lift::SetLowPosition() {
  motor1->Set(ControlMode::Position, 0);
}

void Lift::SetSpeed(double speed) {

}

void Lift::RunPeriodic() {

}
