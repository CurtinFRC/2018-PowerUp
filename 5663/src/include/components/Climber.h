#pragma once

#include "WPILib.h"
#include <ctre/Phoenix.h>

namespace components {
  class Climber {
    TalonSRX *motor;

    public:
      Climber(int m);
      void SetSpeed(double speed);
  };
}
