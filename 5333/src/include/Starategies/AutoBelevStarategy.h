#pragma once

#include "WPILib.h"
#include "curtinfrc/motors/CurtinTalonSRX.h"
#include "curtinfrc/strategy/strategy.h"
#include "curtinfrc/PID.h"

#include "../Map.h"
#include "../Belev.h"

using namespace frc;
using namespace curtinfrc;

class AutoBelevStarategy : public Strategy {
public:
  AutoBelevStarategy(BelevatorControl *_belev, float _position)
    : pid_loop(Map::PID::Belev::kp, Map::PID::Belev::ki, Map::PID::Belev::kd, Map::PID::Belev::kde) {
      belev = _belev;
      pid_loop.set_target(_position * Map::Robot::belev_ticks_per_metre);
    };
  void start() override;
  void tick(double time) override;
  void stop() override;

private:
  BelevatorControl *belev;
  PID pid_loop;
};
