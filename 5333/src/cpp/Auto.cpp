#include "Auto.h"
#include "IO.h"
#include "MPStarategy.h"

#include <openrio/powerup/MatchData.h>
#include "curtinfrc/strategy/mp_strategy.h"

using namespace curtinfrc;
using namespace OpenRIO::PowerUp;

AutoControl::AutoControl(Drivetrain *drive_) {
  drive = drive_;
}

void AutoControl::init() {
  scale = MatchData::OwnedSide::UNKNOWN;
}

void AutoControl::tick() {
  if (scale == MatchData::OwnedSide::UNKNOWN) {
    if ((scale = MatchData::get_owned_side(MatchData::GameFeature::SWITCH_NEAR)) != MatchData::OwnedSide::UNKNOWN) {
      near_switch = MatchData::get_owned_side(MatchData::GameFeature::SWITCH_NEAR);
      far_switch = MatchData::get_owned_side(MatchData::GameFeature::SWITCH_NEAR);

      std::shared_ptr<Strategy> strat = nullptr;
      if (near_switch == MatchData::OwnedSide::LEFT && scale == MatchData::OwnedSide::LEFT)
        strat = MPStarategy::make_strat(drive, "/home/lvuser/paths/test_left.csv", "/home/lvuser/paths/test_right.csv");
      drive->strategy_controller().set_active(strat);
    }
  } else {
    // Do Something?
  }
}
