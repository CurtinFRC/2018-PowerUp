#pragma once

#include "curtinfrc/motors/CurtinTalonSRX.h"
#include "curtinfrc/logger.h"

#include <RobotController.h>

namespace curtinfrc {
  template <unsigned int N_MOTORS>
  class Drivetrain {
  public:
    // Initialise
    Drivetrain(CurtinTalonSRX *l[N_MOTORS], CurtinTalonSRX *r[N_MOTORS])
    : log("log_drive", "left_speed,right_speed") {
      for(int i = 0; i < N_MOTORS; i++) { // Set array to be zero based
        left[i] = l[i];
        right[i] = r[i];
      }
      if(N_MOTORS > 1) {
        for(int i = 1; i < N_MOTORS; i++) { // Set other motors to follow
          left[i]->SetControlMode(CurtinTalonSRX::ControlMode::Follower);
          left[i]->Set(left[0]->GetDeviceID());
          right[i]->SetControlMode(CurtinTalonSRX::ControlMode::Follower);
          right[i]->Set(right[0]->GetDeviceID());
        }
      }
    }

    void set_left(double value) {
      left[0]->Set(value);
    }

    void set_right(double value) {
      right[0]->Set(value);
    }

    void log_write() {
      log.write(::frc::RobotController::GetFPGATime(), 2, left[0]->GetSelectedSensorPosition(0), right[0]->GetSelectedSensorPosition(0));
    }

  private:
    CurtinTalonSRX *left[N_MOTORS], *right[N_MOTORS];
    curtinfrc::Logger log;
  };
}
