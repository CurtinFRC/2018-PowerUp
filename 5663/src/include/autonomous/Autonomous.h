#pragma once

#include "WPILib.h"
#include <Timer.h>
#include <string>
#include "autonomous/BaselineStrategy.h"
#include "components/Drive.h"
#include "components/Lift.h"
#include "components/Manipulator.h"
#include "components/Ramp.h"
#include "curtinfrc/strategy/strategy.h"

using namespace components;
//using namespace curtinfrc;
using namespace autonomous;

namespace autonomous {
  class Autonomous {

    public:
      Autonomous(Drive *drive, Lift *lift, Manipulator *man);
      void SetStageOne(int mode, int startingPosition);
      void SetStageTwo(int mode);
      void SetStageThree(int mode);
      void ChooseStage();
      void RunPeriodic();

    private:
      Lift *autoLift;
      Drive *autoDrive;
      Manipulator *autoMan;
      Timer *timer;

      bool Stop();
      bool Wait(int delay);
      bool BackDrive();
      bool Baseline();
      bool S1L();
      bool S2L();
      bool S3L();
      bool S1R();
      bool S2R();
      bool S3R();

      std::string gameData = "LRL";
      
      std::function<bool()> autoFunction,
      stage1 = [=](){return this->Baseline();},
      stage2 = [=](){return this->Stop();},
      stage3 = [=](){return this->Stop();};

      int autoState = 0, currentStage = 0, startingPosition = -1;
      bool waiting = false;
  };
}
