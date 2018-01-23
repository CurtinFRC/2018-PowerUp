#pragma once

#include <ctre/Phoenix.h>
#include <AHRS.h>
#include <PIDOutput.h>

class gyroPID : public PIDOutput {
  private:
    double thisisoutput = 0;

  public:
    void PIDWrite(double output);
    double GetOutput();
};

namespace components {
  class Drive {
    TalonSRX *left1, *left2, *left3, *right1, *right2, *right3;
    AHRS *imu;
    PIDController *turn;
    gyroPID *out;
    DoubleSolenoid *rightGear, *leftGear; //Solenoids for gears

    public:
      Drive(int l1, int l2, int l3, int r1, int r2, int r3); // Constructor
      Drive& operator=(const Drive&); //Copy Constructor
      void TankDrive(double left, double right, bool square=false);
      void Stop();
      bool TurnAngle(double speed, double angle);
      bool DriveDistance(double speed, double distance, bool holdAngle=true);
      void SetSlowGear();
      void SetFastGear();
      void ToggleGear();
      void RunPeriodic();

    private:
      bool turning = false, driving = false; // State variables
      double turnTolerance = 2.0, driveTolerance = 10.0; // Tolerance variables
      double kP = 0.025, kI = 0.0004, kD = 0.04, kM = 1440/0.4787787204;
      int slowGear = rightGear->kForward;
      int fastGear = rightGear->kReverse;
      double deadzone = 0.015;
      int currentGear = 0;
      int leftFinalDistance, rightFinalDistance;
  };
}

//nt::NetworkTableInstance::GetDefault().GetTable("table")->GetEntry("turn").SetDouble(0);
//var = nt::NetworkTableInstance::GetDefault().GetTable("table")->GetEntry("turn").GetDouble(0);
// #include <networktables/NetworkTableInstance.h>
