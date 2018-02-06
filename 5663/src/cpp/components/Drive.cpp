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

  left1->SetSensorPhase(false);   //Invert encoders
  right1->SetSensorPhase(true);

  left1->SetInverted(true);	//Invert motors
  left2->SetInverted(true);
  left3->SetInverted(true);

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

  gearMode = new DoubleSolenoid(0,0,1);
  SmartDashboard::PutNumber("drive count", 0);
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
  SmartDashboard::PutNumber("Left speed", left);
  SmartDashboard::PutNumber("Right speed", right);
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
bool Drive::DriveDistance(double speed, double distance, double timeout) {
  if(!driving) { // Run setup
    SmartDashboard::PutNumber("drive count", 1);
	  starting_time = 0; //current time
    int encoderCount = kM * distance;
    leftFinalDistance = left1->GetSelectedSensorPosition(0) + encoderCount;
    rightFinalDistance = right1->GetSelectedSensorPosition(0) + encoderCount;
    left1->SetSelectedSensorPosition(0,0,0);
    right1->SetSelectedSensorPosition(0,0,0);
    //setup PID and start driving...
    left1->ConfigNominalOutputForward(0,0); //configuring the left encoder PID
    left1->ConfigNominalOutputReverse(0,0);
    left1->ConfigPeakOutputForward(1,10);
    left1->ConfigPeakOutputReverse(-1,10);
    left1->ConfigMotionCruiseVelocity(600, 0);
    left1->ConfigMotionAcceleration(600, 0);

    left1->Config_kF(0,1.705,0); //set left PID-F values
    left1->Config_kP(0,0.1,0);
    left1->Config_kI(0,0.0,0);
    left1->Config_kD(0,0.0,0);

    right1->ConfigNominalOutputForward(0,0); //configuring the right encoder PID
    right1->ConfigNominalOutputReverse(0,0);
    right1->ConfigPeakOutputForward(1,10);
    right1->ConfigPeakOutputReverse(-1,10);
    right1->ConfigMotionCruiseVelocity(600, 0); //max = 710
    right1->ConfigMotionAcceleration(600, 0);

    right1->Config_kF(0,1.705,0); //set right PID-F values    //VALUE FOR TOUGHBOX MINI 0.05536513205
    right1->Config_kP(0,0.1,0);
    right1->Config_kI(0,0.0,0);
    right1->Config_kD(0,0.0,0);

    driving = true;
  } else {
    //run driving code
    left1->Set(ControlMode::MotionMagic, leftFinalDistance); //drive code in this format
    right1->Set(ControlMode::MotionMagic, rightFinalDistance);
    SmartDashboard::PutNumber("drive count", 2);
    SmartDashboard::PutNumber("Left speed", left1->GetMotorOutputPercent());
    SmartDashboard::PutNumber("Right speed", right1->GetMotorOutputPercent());
    SmartDashboard::PutNumber("Left Drive encoder pos", left1->GetSelectedSensorPosition(0));
    SmartDashboard::PutNumber("Right Drive encoder pos", right1->GetSelectedSensorPosition(0));
    if(abs(leftFinalDistance) + driveTolerance > abs(left1->GetSelectedSensorPosition(0)) &&  abs(leftFinalDistance) - driveTolerance < abs(left1->GetSelectedSensorPosition(0)) && left1->GetSelectedSensorPosition(0) * leftFinalDistance >= 0) {
      if(abs(rightFinalDistance) + driveTolerance > abs(right1->GetSelectedSensorPosition(0)) &&  abs(rightFinalDistance) - driveTolerance < abs(right1->GetSelectedSensorPosition(0)) && right1->GetSelectedSensorPosition(0) * rightFinalDistance >= 0) {
        Stop();
        driving = false;
        return true;
      }
    }
	// if (timeout != -1 && starting_time + timeout < 0 /*current time*/) {
	// 	return true;
	// }
}
  return false;
}

// Set gear to slow gear
void Drive::SetSlowGear() {
  if(slowGear == gearMode->kForward) {
    gearMode->Set(gearMode->kForward);
  } else {
    gearMode->Set(gearMode->kReverse);
  }
  currentGear = false;
}

// Set gear to fast gear
void Drive::SetFastGear() {
  if(fastGear == gearMode->kForward) {
    gearMode->Set(gearMode->kForward);
  } else {
    gearMode->Set(gearMode->kReverse);
  }
  currentGear = true;
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
    SmartDashboard::PutString("Gear Mode", "Fast");
    SetFastGear();
  } else {
    SmartDashboard::PutString("Gear Mode", "Slow");
    SetSlowGear();
  }

  SmartDashboard::PutNumber("Left Drive encoder pos", left1->GetSelectedSensorPosition(0));
  SmartDashboard::PutNumber("Right Drive encoder pos", right1->GetSelectedSensorPosition(0));
  SmartDashboard::PutNumber("Left Drive velocity", left1->GetSelectedSensorVelocity(0));
  SmartDashboard::PutNumber("Right Drive velocity", right1->GetSelectedSensorVelocity(0));
}
