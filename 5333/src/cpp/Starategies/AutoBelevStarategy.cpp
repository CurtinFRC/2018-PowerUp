#include "Starategies/AutoBelevStarategy.h"

#include <iostream>
#include "WPILib.h"
#include "curtinfrc/PID.h"

#include "IO.h"
#include "ControlMap.h"
#include "Map.h"
#include "Belev.h"

using namespace frc;
using namespace std;

void AutoBelevStarategy::start() {
  belev->lift(0);
}

void AutoBelevStarategy::tick(double time) {
  double pid_input = IO::get_instance()->belev_motors[0]->GetEncoder(), pid_output;

  pid_output = pid_loop.calculate(pid_input, time);
  if (pid_loop.done(pid_input, time)) this->done = true;

  belev->lift(pid_output);
}

void AutoBelevStarategy::stop() {
  belev->lift(0);
}
