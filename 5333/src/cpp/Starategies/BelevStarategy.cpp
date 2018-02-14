#include <iostream>

#include "BelevStarategy.h"
#include "..\ControlMap.h"

#include "..\Map.h"

using namespace std;

BelevStarategy::BelevStarategy(CurtinTalonSRX *_belev_motors[n_belev_motors], int *_belev_ticks, float _position) {
  belev_motors = _belev_motors;
  belev_ticks = _belev_ticks;
  position = _position;
}

void BelevStarategy::lift_speed(double output) { // Limit max and min height
  if (IO::get_instance()->get_belev_limit_max() && output > 0) output = 0;
  else if (IO::get_instance()->get_belev_limit_min() && output < 0) output = 0;

  for (auto motor : IO::get_instance()->belev_motors) motor->Set(output); // Set for all motors
}

void BelevStarategy::start() {
  for (auto motor : belev_motors) motor->Set(0);
}

void BelevStarategy::tick(double time) {

  this->done = true;
}

void BelevStarategy::stop() {
  for (auto motor : belev_motors) motor->Set(0);
}
