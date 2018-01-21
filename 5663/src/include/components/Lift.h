#pragma once

#include <ctre/Phoenix.h>
#include <SmartDashboard/SmartDashboard.h>

namespace components {
  class Lift {
    TalonSRX *motor1, *motor2;

    public:
      Lift(int m1, int m2);
      Lift& operator=(const Lift&); //Copy Constructor
      void SetHighPosition();
      void SetMidPosition();
      void SetLowPosition();
      void SetSpeed(double speed);
      void ResetEncoder();
      void RunPeriodic();

    private:
      bool manualMode = false;
      int pos;
      double deadzone = 0.1;
  };
}
