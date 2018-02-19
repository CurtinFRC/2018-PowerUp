#pragma once
#include <DigitalInput.h>
#include <ctre/Phoenix.h>
#include <SmartDashboard/SmartDashboard.h>
/*
* Lift:
* Encoder: 0 low -> 25000 top
* Lift Max Height: 2.27m
* Max velocity: ~2000
* Lift Up = positive speed value, postive encoder value
* Lift Down = negative speed value, negative encoder value
*/

namespace components {
  class Lift {
    TalonSRX *motor1, *motor2;
    DigitalInput *topSwitch, *lowSwitch;

    public:
      Lift(int m1, int m2);
      Lift& operator=(const Lift&); //Copy Constructor
      //Add height to encoder postion function??
      void SetHighPosition();
      void SetMidPosition();
      void SetLowPosition();
      void Stop();
      void SetSpeed(double speed);
      void ResetEncoder();
      void RunPeriodic();
      void OverrideLift(bool overridden);

      int GetLiftPosition(){return motor1->GetSelectedSensorPosition(0);}
    private:
      bool manualMode = false;
      bool encoderOverride = false;
      int pos = 3, lastpos = 3, midPosition = 11000, highPosition = 25000;
      double deadzone = 0.25, maxVelocity = 1800;
  };
}
