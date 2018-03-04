#pragma once

#include "curtinfrc/strategy/strategy.h"
#include "curtinfrc/drivetrain.h"
#include "curtinfrc/PID.h"

#include "../Map.h"

using namespace curtinfrc;

class TurnStarategy : public Strategy {
public:
  TurnStarategy(Drivetrain *_drive, double abs_ang, double _throttle = 0.6f)
    : pid_loop(Map::PID::Turn::kp, Map::PID::Turn::ki, Map::PID::Turn::kd, Map::PID::Turn::kde) {
      drive = _drive;
      throttle = _throttle;

      pid_loop.set_target(abs_ang);
    };
  void start() override;
  void tick(double time) override;
  void stop() override;

private:
  Drivetrain *drive;
  double throttle;
  PID pid_loop;
};
