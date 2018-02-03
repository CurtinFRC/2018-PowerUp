#pragma once

#include "WPILib.h"
#include <string>
#include "autonomous/BaselineStrategy.h"
#include "components/Drive.h"
#include "components/Lift.h"
#include "components/Manipulator.h"
#include "components/Ramp.h"
#include "curtinfrc/strategy/strategy.h"

using namespace components;
using namespace curtinfrc;
using namespace autonomous;

namespace autonomous {
  class Autonomous {

    public:
      Autonomous(Drive *drive, Lift lift, Manipulator man, Ramp ramp);
      void ChooseRoutine(int autoMode, int startingPosition);
      void RunPeriodic();

    private:
      Lift *autoLift;
      Drive *autoDrive;
      Manipulator *autoMan;
      BaselineStrategy *baseline;
      StrategyController *stratCon;
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
      int autoState;
  };
}
