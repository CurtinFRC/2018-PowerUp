#include "Auto.h"
#include "IO.h"

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

      // Auto Init
      // Note: wheelbase width: 0.72
      MotionProfileConfig mcfg = {
        1440, 6,                  // enc ticks, wheel diameter inches
        1.0 / 0.2, 0, 0,          // P, I, D
        3.34 / 12.0, 0.76 / 12.0  // kV, kA
      };
      double kt = 3 * (1.0 / 80.0);

      // TODO: We can put all this into a generalized class (i.e. put MPConfig into a class, take filename only as argument)
      // We'll also need to start working on chaining strategies for auto (e.g. path, cube drop switch, path, cube pickup, path, cube drop scale)
      std::shared_ptr<Strategy> strat = nullptr;
      if (near_switch == MatchData::OwnedSide::LEFT && scale == MatchData::OwnedSide::LEFT) {
        auto mode_left = std::make_shared<PathfinderMPMode>(
          IO::get_instance()->left_motors[0], mcfg, "/home/lvuser/paths/test_left.csv"
        );
        auto mode_right = std::make_shared<PathfinderMPMode>(
          IO::get_instance()->right_motors[0], mcfg, "/home/lvuser/paths/test_right.csv"
        );
        strat = std::make_shared<DrivetrainMotionProfileStrategy>(
          mode_left, mode_right, drive,
          IO::get_instance()->navx, kt
        );
      }
      drive->strategy_controller().set_active(strat);
    }
  } else {
    // Do Something?
  }
}
