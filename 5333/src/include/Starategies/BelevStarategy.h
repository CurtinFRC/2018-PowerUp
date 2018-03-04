#pragma once

#include "curtinfrc/motors/CurtinTalonSRX.h"
#include "curtinfrc/strategy/strategy.h"

#include "../Map.h"
#include "../Belev.h"

using namespace curtinfrc;

class BelevStarategy : public Strategy {
public:
  BelevStarategy(BelevatorControl *_belev);
  void start() override;
  void tick(double time) override;
  void stop() override;

private:
  BelevatorControl *belev;
};
