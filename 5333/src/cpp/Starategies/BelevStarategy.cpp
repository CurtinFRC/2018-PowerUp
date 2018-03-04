#include "Starategies/BelevStarategy.h"

#include <iostream>

#include "ControlMap.h"
#include "IO.h"
#include "Map.h"
#include "Belev.h"

using namespace std;

BelevStarategy::BelevStarategy(BelevatorControl *_belev) {
  belev = _belev;
}

void BelevStarategy::start() {
  belev->lift(0);
}

void BelevStarategy::tick(double time) {
  double output = ControlMap::belevator_motor_power() * 0.8;

  if (IO::get_instance()->get_belev_limit_max() && output > 0) output = 0;
  else if (IO::get_instance()->get_belev_limit_min() && output < 0) output = 0;

  belev->lift(output);
}

void BelevStarategy::stop() {
  belev->lift(0);
}
