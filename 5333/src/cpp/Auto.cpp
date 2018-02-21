#include "Auto.h"
#include "IO.h"
#include "Starategies/MPStarategy.h"

#include <openrio/powerup/MatchData.h>
#include "curtinfrc/strategy/mp_strategy.h"

#include <iostream>

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
    if ((scale = MatchData::get_owned_side(MatchData::GameFeature::SCALE)) != MatchData::OwnedSide::UNKNOWN) {
      near_switch = MatchData::get_owned_side(MatchData::GameFeature::SWITCH_NEAR);
      far_switch = MatchData::get_owned_side(MatchData::GameFeature::SWITCH_FAR);

      std::shared_ptr<Strategy> strat = nullptr;
      strat = MPStarategy::make_strat(drive, "d3_Rswitch");
      drive->strategy_controller().set_active(strat);

      std::cout << "WE NOT DUN GOOFED" << std::endl;
    }
  } else {
    // Do Something?
  }
}
