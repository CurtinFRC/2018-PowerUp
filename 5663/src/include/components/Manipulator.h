#pragma once

#include "WPILib.h"
#include <ctre/Phoenix.h>

namespace components {
  class Manipulator {
    TalonSRX *intake;
    DoubleSolenoid *restrainer;

    public:
      Manipulator(int intakePort, int restrainerFwd, int restrainerRev);
      Manipulator& operator=(const Manipulator&); //Copy Constructor
      void SetIntakeSpeed(double speed);
      void Restrain();
      void Release();
      void RunPeriodic();

    private:
      double intakeSpeed = 0.0;
      double deadzone = 0.1;
  };
}
