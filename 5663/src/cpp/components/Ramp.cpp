#include "components/Ramp.h"

using namespace components;

// Constructor for Ramp class
Ramp::Ramp(int fwd, int rev) {
  solenoid = new DoubleSolenoid(fwd, rev);
  solenoid->Set(solenoid->kReverse);
}

// Deploy ramp
void Ramp::ConfirmIntentionalDeployment() {
  solenoid->Set(solenoid->kForward);
}
