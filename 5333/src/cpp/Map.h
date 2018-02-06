#pragma once

#include "WPILib.h"

#define nSolenoidPorts 2


#define DRIVER_TRAINING 1

namespace Map { // Map ports
  namespace Controllers {
    #ifdef XBOX_CONTROL
    const int xbox = 0;
    constexpr double deadzone = 0.04;

    #elif JOY_CONTROL
    constexpr int joy[2] = {0, 1};
    constexpr double deadzone = 0.08;

    #elif DRIVER_TRAINING
    constexpr int joy[2] = {0, 1};
    const int xbox = 2;
    constexpr double deadzone = 0.08;
    constexpr double xbox_trigger_deadzone = 0.2;

    #endif
  };

  namespace Sensors { };

  namespace Motors {
    const int n_drive_motors                             = 2;
    constexpr int left_motors[n_drive_motors]            = {36, 35};
    constexpr int right_motors[n_drive_motors]           = {32, 34};

    const int n_belev_motors                             = 1;
    constexpr int belev_motors[n_belev_motors]           = {37};

    const int n_intake_motors                            = 1;
    constexpr int intake_motors_left[n_intake_motors]    = {38};
    constexpr int intake_motors_right[n_intake_motors]   = {39};

    const int n_winch_motors                             = 1;
    constexpr int winch_motors[n_winch_motors]           = {30};
  };

  namespace Pneumatics {
    const int n_intake_solenoids                                           = 1;
    constexpr int intake_solenoids[n_intake_solenoids][nSolenoidPorts]     = {{2, 5}};

    const int n_brake_solenoids                                            = 1;
    constexpr int brake_solenoids[n_brake_solenoids][nSolenoidPorts]       = {{1, 6}};

    const int n_shifter_solenoids                                          = 1;
    constexpr int shifter_solenoids[n_shifter_solenoids][nSolenoidPorts]   = {{0, 7}};
  };

  namespace Robot { // Robot stats
    constexpr float max_v             = 9.8;
    constexpr float max_a             = 3;
    constexpr float max_j             = 60;
    constexpr float wheel_circ        = 0.4788; // Metres
    constexpr float track_width       = 0.61; // Metres
    const int encoder_ticks_per_rot   = 1000;
  };

  namespace PID {
    namespace Pathfinder {
      constexpr double kp = 0.08;
      constexpr double ki = 0; // 0.00005
      constexpr double kd = 0;
      constexpr double kf = 0;
    };

    namespace Navx {
      constexpr double kp = 1;
      constexpr double ki = 0;
      constexpr double kd = 0;
    };
  };
}
