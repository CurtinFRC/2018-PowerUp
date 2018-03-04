#pragma once

#include <networktables/NetworkTableInstance.h>
#include <ctre/Phoenix.h>
#include <AHRS.h>
#include <PIDOutput.h>
#include <Timer.h>
/*
* Drive Base:
* Slow Gear = kReverse, direction?, 26.04:1
* Max speed: ~380 = ~0.873m/s ??
* Acceleration: ~0.418m/s/s ??
* Fast Gear = kForward, direction?, 7.08:1
* Max speed: ~380 = ~3.21m/s ??
* Forward = positive value, postive encoder value
* Reverse = negative value, negative encoder value
*/
// Write function for automated timeouts?
class gyroPID : public PIDOutput {
  private:
    double thisisoutput = 0;

  public:
    void PIDWrite(double output);
    double GetOutput();
};

namespace components {
  class Drive {
    AHRS *imu;
    PIDController *turn;
    gyroPID *out;
    DoubleSolenoid *gearMode; //Solenoids for gears
    NetworkTable *table;
    public:
      Drive(int l1, int l2, int l3, int r1, int r2, int r3, int fwd, int rev); // Constructor
      void TankDrive(double left, double right, bool square=false, double maxspeed=1);
      void Stop();
      void SetRampRate(double rate);
      bool EncoderTurn(double speed, double angle, double timeout=0);
      bool TurnAngle(double speed, double angle, double timeout=0);
      bool DriveDistance(double speed, double distance, double timeout=0);
      bool SetSlowGear();
      bool SetFastGear();
      void ToggleGear();
      void ResetEncoder();
      void RunPeriodic();

      bool turning = false;
    private:
      Timer *timeoutCheck, *positionCheck;
      TalonSRX *left1, *left2, *left3, *right1, *right2, *right3;
      const double PIE = 3.141592653589793238463;
      bool driving = false, checkingAngle = false; // State variables
      double turnTolerance = 2.0, driveTolerance = 40; // Tolerance variables
      double kSG = (80.0*(50.0/12.0)*(60.0/24.0)*(60.0/24.0))/(6.0*PIE*0.0254);  //(80*26.041666667)/0.4787787204 ~= 4351.35
      double kFG = (80.0*(50.0/12.0)*(34.0/50.0)*(60.0/24.0))/(6.0*PIE*0.0254);
      int slowGear = gearMode->kReverse;
      int fastGear = gearMode->kForward;
      double deadzone = 0.015;
      bool currentGear = false;
      int finalDistance = 0;
      double starting_time;
  };
}


//var = nt::NetworkTableInstance::GetDefault().GetTable("table")->GetEntry("turn").GetDouble(0);
// #include <networktables/NetworkTableInstance.h>
