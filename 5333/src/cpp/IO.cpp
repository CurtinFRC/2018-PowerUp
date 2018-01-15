#include "IO.h"
#include "Map.h"

static IO *io;

void IO::setup() { // Sets up IO
  // Assign ports to the pointers, as instance to be called from other classes
  left_motors[0] = new CurtinTalonSRX(Map::Motors::left_motors[0]);
  left_motors[0]->SetInverted(true); // Inverts left
  left_motors[1] = new CurtinTalonSRX(Map::Motors::left_motors[1]);
  left_motors[1]->SetInverted(true);

  right_motors[0] = new CurtinTalonSRX(Map::Motors::right_motors[0]);
  right_motors[1] = new CurtinTalonSRX(Map::Motors::right_motors[1]);

  lift_motor[0] = new CurtinTalonSRX(Map::Motors::lift_motor[0]);
  intake_motor[0] = new CurtinTalonSRX(Map::Motors::intake_motor[0]);
  
  // loader = new DoubleSolonoid(1,0,1);

  xbox = new XboxController(Map::Controllers::xbox);
}

// Aliases
double IO::get_left_trigger() { return xbox->GetTriggerAxis(XboxController::JoystickHand::kLeftHand); }
double IO::get_right_trigger() { return xbox->GetTriggerAxis(XboxController::JoystickHand::kRightHand); }
double IO::get_left_y() { return xbox->GetY(XboxController::JoystickHand::kLeftHand); }
double IO::get_right_y() { return xbox->GetY(XboxController::JoystickHand::kRightHand); }
bool IO::get_left_bumper() { return xbox->GetBumper(XboxController::JoystickHand::kLeftHand); }
bool IO::get_right_bumper() { return xbox->GetBumper(XboxController::JoystickHand::kRightHand); }

IO *IO::get_instance() { // Only make one instance of IO
  if (io == NULL) {
    io = new IO();
    io->setup();
  }
  return io;
}
