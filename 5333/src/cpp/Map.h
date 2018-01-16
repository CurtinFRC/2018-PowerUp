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

    static constexpr int belev_motors[1] = {37};

    static constexpr int intake_motors[2][1] = {{38}, {39}};
  };

  struct Pneumatics {
    static constexpr int intake_solenoids[2][2] = {{1, 2}, {2, 4}};
    static constexpr int claw_solenoids[1][2] = {{0, 0}};
  };
}
