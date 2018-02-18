#include "Starategies\IntakeStarategy.h"

#include <iostream>

#include "Toggle.h"

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
  if (intake_toggle->apply(ControlMap::intake_claw_state())) belev->claw(intake_toggle->mode);
  belev->intake(ControlMap::intake_motor_power());
}

void IntakeStarategy::stop() {
  belev->intake(0);
}
