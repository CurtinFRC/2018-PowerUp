#include "Starategies\AutoBelevStarategy.h"

#include <iostream>
#include "WPILib.h"

#include "ControlMap.h"
#include "Map.h"

using namespace frc;
using namespace std;

void AutoBelevStarategy::start() {
  belev_motor->Set(0);
}

void AutoBelevStarategy::tick(double time) {
  double pid_input = belev_motor->GetEncoder(), pid_output;

  pid_output = pid_loop.calculate(pid_input, time);
  if (pid_loop.done(pid_input, time)) this->done = true;

  belev_motor->Set(pid_output);
}

void AutoBelevStarategy::stop() {
  belev_motor->Set(0);
}
