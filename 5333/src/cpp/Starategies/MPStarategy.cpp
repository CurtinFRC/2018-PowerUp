#include "Starategies\MPStarategy.h"

#include <string>

#include "curtinfrc/drivetrain.h"
#include "curtinfrc/strategy/mp_strategy.h"

#include "Map.h"
#include "IO.h"

using namespace curtinfrc;

static constexpr MotionProfileConfig mcfg = {
 1440, Map::Robot::wheel_diam / f_t_m / f_t_m,   // enc ticks, wheel diameter inches
 1.0 / 0.2 * f_t_m, 0, 0,                        // P, I, D
 3.34 / 12.0 * f_t_m, 0.76 / 12.0 * f_t_m        // kV, kA
};

static constexpr double kt = 3 * (1.0 / 80.0);

std::shared_ptr<Strategy> MPStarategy::make_strat(Drivetrain *drive, std::string mp_file_base) {
  try {
    std::string left_mp_file = "/home/lvuser/paths/" + mp_file_base + ".left.csv";
    std::string right_mp_file = "/home/lvuser/paths/" + mp_file_base + ".right.csv";

    auto mode_left = std::make_shared<PathfinderMPMode>(
      IO::get_instance()->left_motors[0], mcfg, left_mp_file.c_str()
    );
    auto mode_right = std::make_shared<PathfinderMPMode>(
      IO::get_instance()->right_motors[0], mcfg, right_mp_file.c_str()
    );
    return std::make_shared<DrivetrainMotionProfileStrategy>(
      mode_left, mode_right, drive,
      IO::get_instance()->navx, kt
    );
  } catch (...) {
    return nullptr;
  }
}
