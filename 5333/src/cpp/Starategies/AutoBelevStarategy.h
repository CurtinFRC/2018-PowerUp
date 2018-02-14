#pragma once

#include "WPILib.h"
#include "curtinfrc/motors/CurtinTalonSRX.h"
#include "curtinfrc/strategy/strategy.h"

using namespace frc;
using namespace curtinfrc;

class AutoBelevStarategy : public Strategy {
public:
  AutoBelevStarategy(CurtinTalonSRX *_belev_motors[n_belev_motors], int *_belev_ticks, float _position);
  void start() override;
  void tick(double time) override;
  void stop() override;

private:
  CurtinTalonSRX *belev_motors[n_belev_motors];
  int *belev_ticks;
  float target;
};
