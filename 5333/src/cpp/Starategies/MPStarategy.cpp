#include "Starategies/MPStarategy.h"

#include <string>
#include <iostream>

#include "curtinfrc/drivetrain.h"
#include "curtinfrc/strategy/mp_strategy.h"

#include "Map.h"
#include "IO.h"

using namespace curtinfrc;

static MotionProfileConfig mcfg_left = {
 Map::Robot::encoder_ticks_per_rot[0], Map::Robot::wheel_diam * 3.28,   // enc ticks, wheel diameter inches
 1.0 / 0.2 * 3.28, 0, 0,                        // P, I, D
 3.34 / 12.0 * 3.28, 0.76 / 12.0 * 3.28        // kV, kA
};
static MotionProfileConfig mcfg_right = mcfg_left;

static constexpr double kt = 3 * (1.0 / 80.0);

std::shared_ptr<Strategy> MPStarategy::make_strat(Drivetrain *drive, std::string mp_file_base) {
  try {
    mcfg_right.enc_ticks_per_rev = Map::Robot::encoder_ticks_per_rot[1];

    std::string left_mp_file = "/home/lvuser/paths/" + mp_file_base + "_left_detailed.csv";
    std::string right_mp_file = "/home/lvuser/paths/" + mp_file_base + "_right_detailed.csv";

    auto mode_left = std::make_shared<PathfinderMPMode>(
      IO::get_instance()->left_motors[0], mcfg_left, left_mp_file.c_str()
    );
    auto mode_right = std::make_shared<PathfinderMPMode>(
      IO::get_instance()->right_motors[0], mcfg_right, right_mp_file.c_str()
    );
    return std::make_shared<DrivetrainMotionProfileStrategy>(
      mode_left, mode_right, drive,
      IO::get_instance()->navx, kt
    );
  } catch (...) {
    return nullptr;
  }
}
