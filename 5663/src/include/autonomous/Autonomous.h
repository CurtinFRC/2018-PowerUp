#pragma once

#include "WPILib.h"
#include "components/Drive.h"
#include "components/Lift.h"
#include "components/Manipulator.h"
#include "components/Ramp.h"
#include <string>

using namespace components;

namespace autonomous {
  class Autonomous {
    Lift *autoLift;
    Drive *autoDrive;
    Manipulator *autoMan;
    public:
      Autonomous(Drive drive, Lift lift, Manipulator man, Ramp ramp);
      void ChooseRoutine(int autoMode, int startingPosition);
      void RunPeriodic();
    private:
      bool Baseline();
      bool S1L();
      bool S2L();
      bool S3L();
      bool S1R();
      bool S2R();
      bool S3R();

      std::string gameData;
      std::function<bool()> autoFunction;
      int AutoStage = 0;
  };
}
