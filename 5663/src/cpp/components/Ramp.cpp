#include "components/Ramp.h"

using namespace components;

// Constructor for Ramp class
Ramp::Ramp(int fwd, int rev, int fsFwd, int fsRev) {
  solenoid = new DoubleSolenoid(fwd, rev);
  solenoid->Set(solenoid->kForward);

  fsSolenoid = new DoubleSolenoid(fsFwd, fsRev);
  fsSolenoid->Set(solenoid->kReverse);
}

// Deploy ramp
void Ramp::ConfirmIntentionalDeployment() {
  solenoid->Set(solenoid->kReverse);
}

void Ramp::ReleaseFoulStopper() {
  fsSolenoid->Set(fsSolenoid->kForward);
}

// Reset ramp
void Ramp::Reset() {
  solenoid->Set(solenoid->kForward);
  fsSolenoid->Set(solenoid->kReverse);
}
