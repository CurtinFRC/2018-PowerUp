#pragma once

#include "WPILib.h"

namespace components {
  class Ramp {
    DoubleSolenoid *solenoid;
    DoubleSolenoid *fsSolenoid;

    public:
      Ramp(int fwd, int rev, int fsFwd, int fsRev);
      void ConfirmIntentionalDeployment();
      void ReleaseFoulStopper();
      void ResetFoulStopper();
      void Reset();

      bool rampsDown = false;
  };
}
