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
// 111.8cm long
// 99.1cm wide
using namespace components;
using namespace autonomous;

namespace autonomous {
  class Autonomous {

    public:
      Autonomous(Drive *drive, Lift *lift, Manipulator *man);
      void SetFarMode(int choice);
      void SetStageOne(int mode, int startingPosition, int wait);
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
      bool Sw1L();
      bool Sw2L();
      bool Sw3L();
      bool Sw1R();
      bool Sw2R();
      bool Sw3R();
      bool Sc1L();
      bool Sc2L();
      bool Sc3L();
      bool Sc1R();
      bool Sc2R();
      bool Sc3R();

      std::string gameData;
      std::function<bool()> autoFunction,
      stage1 = [=](){return this->Baseline();},
      stage2 = [=](){return this->Stop();},
      stage3 = [=](){return this->Stop();};
      bool FarModeEnabled = true;
      int autoState = 0, currentStage = 0, startingPosition = -1;
      bool waiting = false; int waitStart = 0;
  };
}
