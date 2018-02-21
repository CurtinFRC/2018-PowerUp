#pragma once

#include "WPILib.h"
#include "curtinfrc/motors/CurtinTalonSRX.h"
#include "curtinfrc/strategy/strategy.h"
#include "curtinfrc/toggle.h"

#include "Belev.h"

using namespace curtinfrc;

class IntakeStarategy : public Strategy {
public:
  IntakeStarategy(BelevatorControl *_belev);
  void start() override;
  void tick(double time) override;
  void stop() override;

private:
  BelevatorControl *belev;
  curtinfrc::Toggle *intake_toggle;
};
