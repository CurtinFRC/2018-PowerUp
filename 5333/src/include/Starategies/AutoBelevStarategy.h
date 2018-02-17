#pragma once

#include "../Map.h"

#include "WPILib.h"
#include "curtinfrc/motors/CurtinTalonSRX.h"
#include "curtinfrc/strategy/strategy.h"
#include "PID.h"

using namespace frc;
using namespace curtinfrc;

class AutoBelevStarategy : public Strategy {
public:
  AutoBelevStarategy(CurtinTalonSRX *_belev_motor, float _position)
    : pid_loop(Map::PID::Belev::kp, Map::PID::Belev::ki, Map::PID::Belev::kd, Map::PID::Belev::kde) {
      belev_motor = _belev_motor;
      pid_loop.set_target(_position * Map::Robot::belev_ticks_per_metre);
    };
  void start() override;
  void tick(double time) override;
  void stop() override;

private:
  CurtinTalonSRX *belev_motor;
  PID pid_loop;
};
