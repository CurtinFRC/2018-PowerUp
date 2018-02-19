#pragma once

#include "WPILib.h"
#include <cmath>

#define nSolenoidPorts 2


#define DRIVER_TRAINING 1


const double f_t_m = 0.3048; // feet to metres

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

  namespace Sensors {
    const int belev_limit_max   = 1;
    const int belev_limit_min   = 2;
  };

  namespace Motors {
    const int n_drive_motors                             = 2;
    constexpr int left_motors[n_drive_motors]            = {36, 35};
    constexpr int right_motors[n_drive_motors]           = {32, 34};

    const int n_belev_motors                             = 1;
    constexpr int belev_motors[n_belev_motors]           = {37};

    const int n_intake_motors                            = 1;
    constexpr int intake_motors_left[n_intake_motors]    = {38};
    constexpr int intake_motors_right[n_intake_motors]   = {39};
  };

  namespace Pneumatics {
    const int n_intake_solenoids                                           = 1;
    constexpr int intake_solenoids[n_intake_solenoids][nSolenoidPorts]     = {{2, 5}};

    constexpr int brake_solenoid[nSolenoidPorts]       = {1, 6};

    constexpr int shifter_solenoid[nSolenoidPorts]   = {0, 7};
  };

  namespace Robot { // Robot stats
    constexpr float max_v             = 9.8;
    constexpr float max_a             = 3;
    constexpr float max_j             = 60;
    constexpr float wheel_diam        = 0.1524;  // Metres
    constexpr float wheel_circ        = 0.4788;  // Metres
    constexpr float track_width       = 0.61;    // Metres
    constexpr int encoder_ticks_per_rot[2]   = { 1440, 1000 };

    // Belev winch
    const int belev_encoder_ticks_per_rot   = 1;
    constexpr float belev_winch_diameter    = 0.37;  // Metres

    constexpr double belev_ticks_per_metre = belev_encoder_ticks_per_rot / (M_PI * belev_winch_diameter);
  };

  namespace PID {
    namespace Pathfinder {
      constexpr double kp = 0.08;
      constexpr double ki = 0; // 0.00005
      constexpr double kd = 0;
      constexpr double kf = 0;
    };

    namespace Turn {
      constexpr double kp = 0.01;
      constexpr double ki = 0.0;
      constexpr double kd = 0.0;

      constexpr double kde = 1;
    };

    namespace Belev {
      constexpr double kp = 0.01;
      constexpr double ki = 0.0;
      constexpr double kd = 0.0;

      constexpr double kde = 1;
    };
  };
}
