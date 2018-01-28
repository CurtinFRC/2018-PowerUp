#include "components/Drive.h"

using namespace components;

void gyroPID::PIDWrite(double output) {
  thisisoutput = output;
}

double gyroPID::GetOutput() {
  return thisisoutput;
}

// Constructor for Drive class
Drive::Drive(int l1, int l2, int l3, int r1, int r2, int r3) {
  left1 = new TalonSRX(l1);
  left2 = new TalonSRX(l2);
  left3 = new TalonSRX(l3);
  right1 = new TalonSRX(r1);
  right2 = new TalonSRX(r2);
  right3 = new TalonSRX(r3);

  left1->SetSensorPhase(true);   //Invert encoders
  //right1->SetSensorPhase(true);

  left2->Set(ControlMode::Follower, l1);
  left3->Set(ControlMode::Follower, l1);
  right2->Set(ControlMode::Follower, r1);
  right3->Set(ControlMode::Follower, r1);

  imu = new AHRS(I2C::Port::kMXP);

  out = new gyroPID();

  turn = new PIDController(kP, kI, kD, 0.0, imu, out);
  turn->SetInputRange(-180.0, 180.0);
  turn->SetOutputRange(-1.0,1.0);
  turn->SetAbsoluteTolerance(turnTolerance);
  turn->SetContinuous(true);

  leftGear = new DoubleSolenoid(0,4,5);
  rightGear = new DoubleSolenoid(0,2,3);
}

// Stop all Drive class motors
void Drive::Stop() {
  left1->NeutralOutput();
  right1->NeutralOutput();
}

// Set speed of Drive class motors
void Drive::TankDrive(double left, double right, bool square) {
  if(-deadzone < left && left < deadzone) left = 0;
  if(-deadzone < right && right < deadzone) right = 0;
  if(square) {
    left *= fabs(left);
    right *= fabs(right); // square inputs
  }
  left1->Set(ControlMode::PercentOutput, left);
  right1->Set(ControlMode::PercentOutput, right);
}

// Start or continue a turn
bool Drive::TurnAngle(double speed, double angle) {
  if(turning) {
    imu->GetAngle();
    TankDrive(out->GetOutput(),out->GetOutput()); //one of these may need to be negative later >:(

    if((abs(angle) - turnTolerance) < abs(imu->GetAngle()) && (abs(angle) + turnTolerance) > abs(imu->GetAngle()) && angle*imu->GetAngle() >= 0) {
      turning = false;
      turn->Disable();
      return true;
    }
  } else {
    imu->ZeroYaw();
    turn->SetOutputRange(-speed,speed);
    turn->Enable();
    turn->SetSetpoint(angle);
    turning = true;
  }

  return false;
}

// Start or continue a forward drive and stop at the given distance
bool Drive::DriveDistance(double speed, double distance, bool holdAngle) {
  if(driving) {
    //run driving code
    left1->Set(ControlMode::Position, leftFinalDistance); //drive code in this format
    right1->Set(ControlMode::Position, rightFinalDistance);

    if(abs(leftFinalDistance) + driveTolerance > abs(left1->GetSelectedSensorPosition(0)) &&  abs(leftFinalDistance) - driveTolerance < abs(left1->GetSelectedSensorPosition(0)) && left1->GetSelectedSensorPosition(0) * leftFinalDistance >= 0) {
      if(abs(rightFinalDistance) + driveTolerance > abs(right1->GetSelectedSensorPosition(0)) &&  abs(rightFinalDistance) - driveTolerance < abs(right1->GetSelectedSensorPosition(0)) && right1->GetSelectedSensorPosition(0) * rightFinalDistance >= 0) {
        Stop();
        driving = false;
        return true;
      }
    }
  } else { // Run setup
    int encoderCount = kM * distance;
    leftFinalDistance = left1->GetSelectedSensorPosition(0) + encoderCount;
    rightFinalDistance = right1->GetSelectedSensorPosition(0) + encoderCount;

    //setup PID and start driving...
    left1->ConfigNominalOutputForward(0,10); //configuring the left encoder PID
    left1->ConfigNominalOutputReverse(0,10);
    left1->ConfigPeakOutputForward(speed,10);
    left1->ConfigPeakOutputReverse(-speed,10);

    left1->Config_kF(0,0,0); //set left PID-F values    //VALUE FOR TOUGHBOX MINI
    left1->Config_kP(0,5.0,0);
    left1->Config_kI(0,0.0,0);
    left1->Config_kD(0,0.0,0);

    right1->ConfigNominalOutputForward(0,10); //configuring the right encoder PID
    right1->ConfigNominalOutputReverse(0,10);
    right1->ConfigPeakOutputForward(speed,10);
    right1->ConfigPeakOutputReverse(-speed,10);

    right1->Config_kF(0,0,0); //set right PID-F values    //VALUE FOR TOUGHBOX MINI
    right1->Config_kP(0,5.0,0);
    right1->Config_kI(0,0.0,0);
    right1->Config_kD(0,0.0,0);

    driving = true;
  }

  return false;
}

// Set gear to slow gear
void Drive::SetSlowGear() {
  if(slowGear == rightGear->kForward) {
    rightGear->Set(rightGear->kForward);
    leftGear->Set(rightGear->kForward);
  } else {
    rightGear->Set(rightGear->kReverse);
    leftGear->Set(rightGear->kReverse);
  }
  currentGear = 0;
}

// Set gear to fast gear
void Drive::SetFastGear() {
  if(fastGear == rightGear->kForward) {
    rightGear->Set(rightGear->kForward);
    leftGear->Set(rightGear->kForward);
  } else {
    rightGear->Set(rightGear->kReverse);
    leftGear->Set(rightGear->kReverse);
  }
  currentGear = 1;
}

// Toggle between slow and fast gear
void Drive::ToggleGear() {
  if(currentGear) {
    SetSlowGear();
  } else {
    SetFastGear();
  }
}

// Run periodic tasks
void Drive::RunPeriodic() {
  if(currentGear) {
    SetFastGear();
  } else {
    SetSlowGear();
  }

  SmartDashboard::PutNumber("Left Drive encoder pos", left1->GetSelectedSensorPosition(0));
  SmartDashboard::PutNumber("Right Drive encoder pos", right1->GetSelectedSensorPosition(0));
}
