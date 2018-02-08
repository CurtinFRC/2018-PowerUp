#pragma once
#include <DigitalInput.h>
#include <ctre/Phoenix.h>
#include <SmartDashboard/SmartDashboard.h>
/*
* Lift:
* Encoder: 0 low -> 25000 top
* Lift Max Height: ?m
* Max speed: ~2000
* Lift Up = negative speed value, postive encoder value
* Lift Down = postive speed value, negative encoder value
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
      void SetSpeed(double speed);
      void ResetEncoder();
      void RunPeriodic();

      int GetLiftPosition(){return motor1->GetSelectedSensorPosition(0);}
    private:
      bool manualMode = false;
      int pos = 3, midPosition = 7000, highPosition = 15000;
      double deadzone = 0.05, topspeed = 1800;
  };
}
