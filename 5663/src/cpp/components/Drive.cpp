#include "components/Drive.h"

using namespace components;

void gyroPID::PIDWrite(double output) {
  thisisoutput = output;
}

double gyroPID::GetOutput() {
  return thisisoutput;
}

// Constructor for Drive class
Drive::Drive(int l1, int l2, int l3, int r1, int r2, int r3, int fwd, int rev) {
  nt::NetworkTableInstance::GetDefault().GetTable("table")->GetEntry("turn").SetDouble(0);
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

  imu = new AHRS(I2C::Port::kOnboard);

  out = new gyroPID();

  turn = new PIDController(0.01, 0, 0, 0.0, imu, out);
  turn->SetInputRange(-180.0, 180.0);
  turn->SetOutputRange(-1.0,1.0);
  turn->SetAbsoluteTolerance(turnTolerance);
  turn->SetContinuous(true);

  gearMode = new DoubleSolenoid(0,fwd,rev);
  timeoutCheck = new Timer(); positionCheck = new Timer();
  timeoutCheck->Start(); positionCheck->Start();
}

// Stop all Drive class motors
void Drive::Stop() {
  left1->NeutralOutput();
  right1->NeutralOutput();
}

//Set time to get to full speed
void Drive::SetRampRate(double rate) {
  SmartDashboard::PutNumber("Ramp Rate", rate);
  left1->ConfigOpenloopRamp(rate,0);
  right1->ConfigOpenloopRamp(rate,0);
}

// Set speed of Drive class motors
void Drive::TankDrive(double left, double right, bool square, double maxspeed) {
  if(-deadzone < left && left < deadzone) left = 0;
  if(-deadzone < right && right < deadzone) right = 0;
  if(square) {
    left *= fabs(left);
    right *= fabs(right); // square inputs
  }
  if(left > maxspeed) left = maxspeed;
  else if(left < -maxspeed) left = -maxspeed;
  if(right > maxspeed) right = maxspeed;
  else if(right < -maxspeed) right = -maxspeed;

  left1->Set(ControlMode::PercentOutput, left);
  right1->Set(ControlMode::PercentOutput, right);
  SmartDashboard::PutNumber("maxspeed", maxspeed);
}

bool Drive::EncoderTurn(double speed, double angle, double timeout) {
  if(turning) {
    left1->Set(ControlMode::MotionMagic, finalDistance); //drive code in this format
    right1->Set(ControlMode::MotionMagic, -finalDistance);

    if(abs(finalDistance) + driveTolerance > abs(left1->GetSelectedSensorPosition(0)) &&  abs(finalDistance) - driveTolerance < abs(left1->GetSelectedSensorPosition(0)) && left1->GetSelectedSensorPosition(0) * finalDistance >= 0) {
      if(abs(finalDistance) + driveTolerance > abs(right1->GetSelectedSensorPosition(0)) &&  abs(finalDistance) - driveTolerance < abs(right1->GetSelectedSensorPosition(0)) && right1->GetSelectedSensorPosition(0) * finalDistance >= 0) {
        Stop();
        driving = false;
        return true;
      }
    }

	  if(timeoutCheck->HasPeriodPassed(timeout) && timeout != 0) {
      Stop();
      driving = false;
	 	  return true;
    }
  } else {
    left1->SetSelectedSensorPosition(0,0,10);
    right1->SetSelectedSensorPosition(0,0,10);
    timeoutCheck->Reset();
    int encoderCount = (2.199114857712855 * (angle/360.0)) * kFG;
    double F = 3.5, P = 4.0, I = 0, D = 0; // P = 2.0
    int acceleration = 200;
    left1->SetSelectedSensorPosition(0,0,10);
    right1->SetSelectedSensorPosition(0,0,10);
    finalDistance = encoderCount;
    //setup PID and start driving...
    left1->ConfigNominalOutputForward(0,0); //configuring the left encoder PID
    left1->ConfigNominalOutputReverse(0,0);
    left1->ConfigPeakOutputForward(1,10);
    left1->ConfigPeakOutputReverse(-1,10);
    left1->ConfigMotionCruiseVelocity(620*speed, 0);
    left1->ConfigMotionAcceleration(acceleration, 0);

    left1->Config_kF(0,F,0); //set left PID-F values
    left1->Config_kP(0,P,0);  //4.2
    left1->Config_kI(0,I,0);
    left1->Config_kD(0,D,0);

    right1->ConfigNominalOutputForward(0,0); //configuring the right encoder PID
    right1->ConfigNominalOutputReverse(0,0);
    right1->ConfigPeakOutputForward(1,10);
    right1->ConfigPeakOutputReverse(-1,10);
    right1->ConfigMotionCruiseVelocity(630*speed, 0);
    right1->ConfigMotionAcceleration(acceleration, 0);

    right1->Config_kF(0,F,0); //set right PID-F values
    right1->Config_kP(0,P,0);
    right1->Config_kI(0,I,0);
    right1->Config_kD(0,D,0);
  }
}

// Start or continue a turn
bool Drive::TurnAngle(double speed, double angle, double timeout) {
  if(turning) {

    if(currentGear) {
      if(fabs(turn->GetError()) > 30) turn->SetPID(0.02, 0.0, 0.068); //fast  //0.05 = D
      else turn->SetPID(0.02, 0.0005, 0.06);
      SetRampRate(0.8);
    }
    else {
      turn->SetPID(0.02, 0.0, 0.005); //slow
      SetRampRate(0.3);
    }

    TankDrive(out->GetOutput(), -out->GetOutput());

    if((abs(angle) - turnTolerance) < abs(imu->GetAngle()) &&
    (abs(angle) + turnTolerance) > abs(imu->GetAngle()) && angle*imu->GetAngle() >= 0) {
        positionCheck->Reset();
        checkingAngle = true;
    } else checkingAngle = false;

    if(positionCheck->HasPeriodPassed(0.3) && checkingAngle) {
      turning = false;
      SetRampRate(0);
      turn->Disable();
      return true;
    }

    if(timeout != 0 && timeoutCheck->HasPeriodPassed(timeout)) {  //Whole function timeout
      turning = false;
      SetRampRate(0);
      turn->Disable();
      return true;
    }

  } else {
    imu->ZeroYaw();
    turn->SetOutputRange(-speed,speed);
    turn->Enable();
    turn->SetSetpoint(angle);
    turning = true;
    checkingAngle = false;
    timeoutCheck->Reset();
  }
  return false;
}

// Start or continue a forward drive and stop at the given distance
bool Drive::DriveDistance(double speed, double distance, double timeout) {
  if(!driving) { // Run setup
    int encoderCount = 0;
    encoderCount = kFG * distance;
    // if(currentGear) encoderCount = kFG * distance;
    // else encoderCount = kSG * distance;
    double F = 3.5, P = 4.0, I = 0, D = 0; // P = 2.0
    int acceleration = 380;
    if(currentGear) acceleration = 200;
    left1->SetSelectedSensorPosition(0,0,10);
    right1->SetSelectedSensorPosition(0,0,10);
    finalDistance = encoderCount;
    //setup PID and start driving...
    left1->ConfigNominalOutputForward(0,0); //configuring the left encoder PID
    left1->ConfigNominalOutputReverse(0,0);
    left1->ConfigPeakOutputForward(1,10);
    left1->ConfigPeakOutputReverse(-1,10);
    left1->ConfigMotionCruiseVelocity(620*speed, 0);
    left1->ConfigMotionAcceleration(acceleration, 0);

    left1->Config_kF(0,F,0); //set left PID-F values
    left1->Config_kP(0,P,0);  //4.2
    left1->Config_kI(0,I,0);
    left1->Config_kD(0,D,0);

    right1->ConfigNominalOutputForward(0,0); //configuring the right encoder PID
    right1->ConfigNominalOutputReverse(0,0);
    right1->ConfigPeakOutputForward(1,10);
    right1->ConfigPeakOutputReverse(-1,10);
    right1->ConfigMotionCruiseVelocity(630*speed, 0);
    right1->ConfigMotionAcceleration(acceleration, 0);

    right1->Config_kF(0,F,0); //set right PID-F values
    right1->Config_kP(0,P,0);
    right1->Config_kI(0,I,0);
    right1->Config_kD(0,D,0);

    driving = true;
    timeoutCheck->Reset();
  } else {
    //run driving code
    left1->Set(ControlMode::MotionMagic, finalDistance); //drive code in this format
    right1->Set(ControlMode::MotionMagic, finalDistance);

    if(abs(finalDistance) + driveTolerance > abs(left1->GetSelectedSensorPosition(0)) &&  abs(finalDistance) - driveTolerance < abs(left1->GetSelectedSensorPosition(0)) && left1->GetSelectedSensorPosition(0) * finalDistance >= 0) {
      if(abs(finalDistance) + driveTolerance > abs(right1->GetSelectedSensorPosition(0)) &&  abs(finalDistance) - driveTolerance < abs(right1->GetSelectedSensorPosition(0)) && right1->GetSelectedSensorPosition(0) * finalDistance >= 0) {
        Stop();
        driving = false;
        return true;
      }
    }

	  if(timeoutCheck->HasPeriodPassed(timeout) && timeout != 0) {
      Stop();
      driving = false;
	 	  return true;
    }
}
  return false;
}

// Set gear to slow gear
bool Drive::SetSlowGear() {
  if(slowGear == gearMode->kForward) {
    gearMode->Set(gearMode->kForward);
  } else {
    gearMode->Set(gearMode->kReverse);
  }
  bool returnValue = currentGear;
  currentGear = false;
  return returnValue;
}

// Set gear to fast gear
bool Drive::SetFastGear() {
  if(fastGear == gearMode->kForward) {
    gearMode->Set(gearMode->kForward);
  } else {
    gearMode->Set(gearMode->kReverse);
  }
  bool returnValue = currentGear;
  currentGear = true;
  return !returnValue;
}

// Toggle between slow and fast gear
void Drive::ToggleGear() {
  if(currentGear) {
    SetSlowGear();
  } else {
    SetFastGear();
  }
}

// Zero drivebase encoders
void Drive::ResetEncoder() {
  left1->SetSelectedSensorPosition(0, 0, 10);
  right1->SetSelectedSensorPosition(0, 0, 10);
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
  if(!turning) {
    turn->Disable();
  }
  SmartDashboard::PutBoolean("Gear Mode Bool", currentGear);
  SmartDashboard::PutNumber("Left speed", left1->GetMotorOutputPercent());
  SmartDashboard::PutNumber("Right speed", right1->GetMotorOutputPercent());
  SmartDashboard::PutNumber("output", out->GetOutput());
  SmartDashboard::PutBoolean("imu connected", imu->IsConnected());
  SmartDashboard::PutNumber("Yaw", imu->GetYaw());
  SmartDashboard::PutNumber("Roll", imu->GetRoll());
  SmartDashboard::PutNumber("Pitch", imu->GetPitch());
  SmartDashboard::PutNumber("Left encoder", left1->GetSelectedSensorPosition(0));
  SmartDashboard::PutNumber("Right encoder", right1->GetSelectedSensorPosition(0));
  SmartDashboard::PutNumber("Left velocity", left1->GetSelectedSensorVelocity(0));
  SmartDashboard::PutNumber("Right velocity", right1->GetSelectedSensorVelocity(0));
}
