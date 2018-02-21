#pragma once

#include "curtinfrc/strategy/strategy.h"
#include "curtinfrc/drivetrain.h"

using namespace curtinfrc;

class DriveStarategy : public Strategy {
public:
  DriveStarategy(Drivetrain *_drive, double _throttle = 1.0f);
  void start() override;
  void tick(double time) override;
  void stop() override;

private:
  Drivetrain *drive;
  double throttle;
  bool left_bumper_toggle, right_bumper_toggle;
};
