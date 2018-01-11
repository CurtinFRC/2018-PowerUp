#include "components/Drive.h"

using namespace components;

void gyroPID::PIDWrite(double output) {
  thisisoutput = output;
}

double gyroPID::GetOutput() {
  return thisisoutput;
}

Drive::Drive(int l1, int l2, int l3, int r1, int r2, int r3) {
  left1 = new TalonSRX(l1);
  left2 = new TalonSRX(l2);
  left3 = new TalonSRX(l3);
  right1 = new TalonSRX(r1);
  right2 = new TalonSRX(r2);
  right3 = new TalonSRX(r3);

  imu = new AHRS(I2C::Port::kMXP);

  out = new gyroPID();

  turn = new PIDController(kP, kI, kD, 0.0, imu, out);
  turn->SetInputRange(-180.0, 180.0);
  turn->SetOutputRange(-1.0,1.0);
  turn->SetAbsoluteTolerance(tolerance);
  turn->SetContinuous(true);

  leftGear = new DoubleSolenoid(0,0,1);
  rightGear = new DoubleSolenoid(0,2,3);
}

void Drive::Stop() {
  left1->NeutralOutput();
  left2->NeutralOutput();
  left3->NeutralOutput();
  right1->NeutralOutput();
  right2->NeutralOutput();
  right3->NeutralOutput();
}

void Drive::TankDrive(double left, double right, bool square) {
  if(-deadzone < left && left < deadzone) left = 0;
  if(-deadzone < right && right < deadzone) right = 0;
  if(square) {
    left *= abs(left);
    right *= abs(right); // square inputs
  }
  left1->Set(ControlMode::PercentOutput, left);
  left2->Set(ControlMode::PercentOutput, left);
  left3->Set(ControlMode::PercentOutput, left);
  right1->Set(ControlMode::PercentOutput, right);
  right2->Set(ControlMode::PercentOutput, right);
  right3->Set(ControlMode::PercentOutput, right);
}

bool Drive::TurnAngle(double speed, double angle) {
  if(turning) {
    imu->GetAngle();
    TankDrive(out->GetOutput(),out->GetOutput()); //one of these may need to be negative later >:(
    return false;
  } else {
    imu->ZeroYaw();
    turn->Enable();
    turn->SetSetpoint(angle);
    return false;
  }
  if((abs(angle) + tolerance) > abs(imu->GetAngle())) {
    return true;
  }
}

void Drive::SetSlowGear() {
  rightGear->Set(rightGear->kForward);
  leftGear->Set(rightGear->kForward);

  currentGear = 0;
}

void Drive::SetFastGear() {
  rightGear->Set(rightGear->kReverse);
  leftGear->Set(rightGear->kReverse);

  currentGear = 1;
}

void Drive::ToggleGear() {
  if(currentGear) {
    SetSlowGear();
  } else {
    SetFastGear();
  }
}

void Drive::RunPeriodic() {
  if(currentGear) {
    SetFastGear();
  } else {
    SetSlowGear();
  }
}
