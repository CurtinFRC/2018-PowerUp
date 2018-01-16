#pragma once

#include "WPILib.h"

namespace Map { // Map ports
  struct Controllers {
    static const int xbox = 0;
  };

  struct Sensors { };

  struct Motors {
    static constexpr int left_motors[2] = {32, 34};
    static constexpr int right_motors[2] = {35, 36};

    static constexpr int lift_motor[1] = {37};

    static constexpr int intake_motor[1] = {39};
    // Add claw_motor later
  };

  struct Pneumatics { };
}
