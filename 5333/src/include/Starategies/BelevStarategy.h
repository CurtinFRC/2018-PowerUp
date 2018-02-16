#pragma once

#include "curtinfrc/motors/CurtinTalonSRX.h"
#include "curtinfrc/strategy/strategy.h"

#include "../Map.h"

using namespace curtinfrc;

class BelevStarategy : public Strategy {
public:
  BelevStarategy(CurtinTalonSRX *_belev_motor);
  void start() override;
  void tick(double time) override;
  void stop() override;

private:
  CurtinTalonSRX *belev_motor;
};
