#pragma once

#include "curtinfrc/motors/CurtinTalonSRX.h"
#include "curtinfrc/strategy/strategy.h"

using namespace curtinfrc;

class BelevStarategy : public Strategy {
public:
  BelevStarategy(CurtinTalonSRX *_belev_motors[n_belev_motors], int *_belev_ticks, float _position);
  void lift_speed() override;
  void start() override;
  void tick(double time) override;
  void stop() override;

private:
  CurtinTalonSRX *belev_motors[n_belev_motors];
  int *belev_ticks;
  float position;
};
