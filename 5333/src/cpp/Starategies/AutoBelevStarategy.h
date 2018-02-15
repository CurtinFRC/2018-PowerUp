#pragma once

#include "../Map.h"

#include "WPILib.h"
#include "curtinfrc/motors/CurtinTalonSRX.h"
#include "curtinfrc/strategy/strategy.h"

using namespace frc;
using namespace curtinfrc;

class AutoBelevStarategy : public Strategy {
public:
  AutoBelevStarategy(CurtinTalonSRX *_belev_motor, float _position);
  void start() override;
  void tick(double time) override;
  void stop() override;

private:
  CurtinTalonSRX *belev_motor;
  float target;
};
