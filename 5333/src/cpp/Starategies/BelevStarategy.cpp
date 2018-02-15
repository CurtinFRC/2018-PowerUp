#include <iostream>

#include "BelevStarategy.h"

#include "..\ControlMap.h"
#include "..\IO.h"
#include "..\Map.h"

using namespace std;

BelevStarategy::BelevStarategy(CurtinTalonSRX *_belev_motor) {
  belev_motor = _belev_motor;
}

void BelevStarategy::start() {
  belev_motor->SetControlMode(CurtinTalonSRX::ControlMode::PercentOutput);
  belev_motor->configure_pidf(0.0, 0.0, 0.0, 0.0);
  belev_motor->Set(0);
}

void BelevStarategy::tick(double time) {
  int output = ControlMap::belevator_motor_power();

  if (IO::get_instance()->get_belev_limit_max() && output > 0) output = 0;
  else if (IO::get_instance()->get_belev_limit_min() && output < 0) output = 0;

  belev_motor->Set(output);
}

void BelevStarategy::stop() {
  belev_motor->Set(0);
}
