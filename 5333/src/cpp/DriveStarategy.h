#pragma once

#include "curtinfrc/strategy/strategy.h"
#include "curtinfrc/drivetrain.h"

using namespace curtinfrc;

class DriveStarategy : public Strategy {
public:
  DriveStarategy(Drivetrain *_drive, double _throttle = 0.6f);
  void start() override;
  void tick(double time) override;
  void stop() override;

private:
  double throttle;
  bool left_bumper_toggle, right_bumper_toggle;
  Drivetrain *drive;
};
