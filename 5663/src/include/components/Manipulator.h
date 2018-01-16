#pragma once

#include "WPILib.h"
#include <ctre/Phoenix.h>

namespace components {
  class Manipulator {
    TalonSRX *intake;
    DoubleSolenoid *restrainer;

    public:
      Manipulator(int intakePort, int restrainerFwd, int restrainerRev);
<<<<<<< HEAD
      void SetIntakeSpeed(double speed);
=======
      void SetIntakeSpeed(int speed);
>>>>>>> f18d3e0019cdbea3d4159615c678b0a926e73ae1
      void Restrain();
      void Release();
  };
}
