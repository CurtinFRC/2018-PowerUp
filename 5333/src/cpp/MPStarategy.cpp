#include "MPStarategy.h"
#include "IO.h"

#include "curtinfrc/drivetrain.h"
#include "curtinfrc/strategy/mp_strategy.h"

#include <string>

using namespace curtinfrc;

static constexpr MotionProfileConfig mcfg = {
 1440, 6,                  // enc ticks, wheel diameter inches
 1.0 / 0.2, 0, 0,          // P, I, D
 3.34 / 12.0, 0.76 / 12.0  // kV, kA
};

static constexpr double kt = 3 * (1.0 / 80.0);

std::shared_ptr<Strategy> MPStarategy::make_strat(Drivetrain *drive, std::string left_mp_file, std::string right_mp_file) {
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
}
