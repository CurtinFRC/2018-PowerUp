#pragma once

#include <ctre/Phoenix.h>

namespace components {
  class Lift {
    TalonSRX *motor1, *motor2;

    public:
      Lift(int m1, int m2);
      void SetHighPosition();
      void SetLowPosition();
      void SetSpeed(double speed);
      void RunPeriodic();
  };
}
