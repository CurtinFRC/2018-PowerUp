#pragma once

#include "WPILib.h"
#include <ctre/Phoenix.h>
#include <Spark.h>
/*
* Intake:
* Wheels In = negative value
* Wheels Out = postive value
* Grabber Close = kForward
* Grabber Open = kReverse
*/

namespace components {
  class Manipulator {
    Spark *intake;
    DoubleSolenoid *restrainer;

    public:
      Manipulator(int intakePort, int restrainerFwd, int restrainerRev);
      Manipulator& operator=(const Manipulator&); //Copy Constructor
      void SetIntakeSpeed(double speed);
      void Restrain();
      void Release();
      void RunPeriodic();
      void OverrideIntake(bool overridden);

    private:
      bool intakeOverride = false;
      double intakeSpeed = 0.0;
      double deadzone = 0.05;
  };
}
