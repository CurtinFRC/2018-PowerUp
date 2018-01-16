#pragma once

#include "WPILib.h"
#include <ctre/Phoenix.h>

namespace components {
  class Manipulator {
    TalonSRX *intake;
    DoubleSolenoid *restrainer;

    public:
      Manipulator(int intakePort, int restrainerFwd, int restrainerRev);
      void SetIntakeSpeed(double speed);
      void Restrain();
      void Release();
  };
}
