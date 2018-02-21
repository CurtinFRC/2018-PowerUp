#pragma once

#include "curtinfrc/drivetrain.h"
#include <openrio/powerup/MatchData.h>

using namespace curtinfrc;
using namespace OpenRIO::PowerUp;

class AutoControl {
public:
  AutoControl(Drivetrain *drive_);
  void init();
  void tick();
private:
  MatchData::OwnedSide near_switch, scale, far_switch;
  Drivetrain *drive;
};
