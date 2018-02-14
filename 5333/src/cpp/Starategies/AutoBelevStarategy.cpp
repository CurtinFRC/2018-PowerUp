#include <iostream>

#include "WPILib.h"
#include "AutoBelevStarategy.h"
#include "..\ControlMap.h"

#include "..\Map.h"

using namespace frc;
using namespace std;

AutoBelevStarategy::AutoBelevStarategy(CurtinTalonSRX *_belev_motors[n_belev_motors], int *_belev_ticks, float _position) {
  belev_motors = _belev_motors;
  belev_ticks = _belev_ticks;
  target = _position * Map::Robot::belev_ticks_per_metre;
}

void AutoBelevStarategy::start() {
  belev_motors[0]->Set(0);
  belev_motors[0]->SetControlMode(ControlMode::Position);
  belev_motors[0]->SetFeedbackDevice(FeedbackDevice::QuadEncoder);
  belev_motors[0]->configure_pidf(0.01, 0.001, 0.0, 0.0);
  belev_motors[0]->EnableControl();
  }
}

void AutoBelevStarategy::tick(double time) {
  belev_motors[0]->Set(target);
  if (abs(belev_motors[0]->GetSelectedSensorPosition() - target) < 10) this->done = true; // 10 is temp const
}

void AutoBelevStarategy::stop() {
  belev_motors[0]->SetControlMode(ControlMode::PercentVbus);
  belev_motors[0]->configure_pidf(0.0, 0.0, 0.0, 0.0);
  belev_motors[0]->EnableControl();
  belev_motors[0]->Set(0);
}
