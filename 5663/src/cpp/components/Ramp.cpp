#include "components/Ramp.h"

using namespace components;

// Constructor for Ramp class
Ramp::Ramp(int fwd, int rev, int fsFwd, int fsRev) {
  solenoid = new DoubleSolenoid(fwd, rev);
  solenoid->Set(solenoid->kForward);

  fsSolenoid = new DoubleSolenoid(fsFwd, fsRev);
  fsSolenoid->Set(solenoid->kReverse);
  SmartDashboard::PutBoolean("Foul Stoppers Released", false);
}

// Deploy ramp
void Ramp::ConfirmIntentionalDeployment() {
  solenoid->Set(solenoid->kReverse);
  rampsDown = true;
}

void Ramp::ReleaseFoulStopper() {
  if(rampsDown) fsSolenoid->Set(fsSolenoid->kForward);
  SmartDashboard::PutBoolean("Foul Stoppers Released", true);
}

void Ramp::ResetFoulStopper() {
  fsSolenoid->Set(fsSolenoid->kReverse);
  SmartDashboard::PutBoolean("Foul Stoppers Released", false);
}

// Reset ramp
void Ramp::Reset() {
  rampsDown = false;
  solenoid->Set(solenoid->kForward);
  fsSolenoid->Set(solenoid->kReverse);
}
