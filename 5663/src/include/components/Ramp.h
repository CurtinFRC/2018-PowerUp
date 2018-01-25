#pragma once

#include "WPILib.h"

namespace components {
  class Ramp {
    DoubleSolenoid *solenoid;

    public:
      Ramp(int fwd, int rev);
      void ConfirmIntentionalDeployment();
  };
}
