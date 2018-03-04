#include "Starategies/IntakeStarategy.h"

#include <iostream>

#include "curtinfrc/toggle.h"

#include "ControlMap.h"
#include "Belev.h"

using namespace std;

IntakeStarategy::IntakeStarategy(BelevatorControl *_belev) {
  belev = _belev;
  intake_toggle = new Toggle();
}

void IntakeStarategy::start() {
  belev->intake(0);
  belev->claw(false);
}

void IntakeStarategy::tick(double time) {
  bool claw_state = ControlMap::intake_claw_state();

  if (intake_toggle->apply(claw_state)) belev->claw(intake_toggle->apply(claw_state));
  belev->intake(ControlMap::intake_motor_power());
}

void IntakeStarategy::stop() {
  belev->intake(0);
}
