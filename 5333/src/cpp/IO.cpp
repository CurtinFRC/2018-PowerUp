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

  belev_motors[0] = new CurtinTalonSRX(Map::Motors::belev_motors[0]);
  intake_motors[0][0] = new CurtinTalonSRX(Map::Motors::intake_motors[0][0]);
  intake_motors[1][0] = new CurtinTalonSRX(Map::Motors::intake_motors[1][0]);

  intake_solenoids[0] = new DoubleSolenoid(
    Map::Pneumatics::intake_solenoids[0][0],
    Map::Pneumatics::intake_solenoids[0][1],
    Map::Pneumatics::intake_solenoids[0][1] + 1
  );
  intake_solenoids[1] = new DoubleSolenoid(
    Map::Pneumatics::intake_solenoids[1][0],
    Map::Pneumatics::intake_solenoids[1][1],
    Map::Pneumatics::intake_solenoids[1][1] + 1
  );
  claw_solenoids[0] = new DoubleSolenoid(
    Map::Pneumatics::claw_solenoids[0][0],
    Map::Pneumatics::claw_solenoids[0][1],
    Map::Pneumatics::claw_solenoids[0][1] + 1
  );

  xbox = new XboxController(Map::Controllers::xbox);
}

// Aliases
double IO::get_left_trigger() { return xbox->GetTriggerAxis(XboxController::JoystickHand::kLeftHand); }
bool IO::get_left_bumper() { return xbox->GetBumper(XboxController::JoystickHand::kLeftHand); }
double IO::get_left_X() { return xbox->GetX(XboxController::JoystickHand::kLeftHand); }
double IO::get_left_Y() { return xbox->GetY(XboxController::JoystickHand::kLeftHand); }
bool IO::get_left_stick() { return xbox->GetStickButton(XboxController::JoystickHand::kLeftHand); }

double IO::get_right_trigger() { return xbox->GetTriggerAxis(XboxController::JoystickHand::kRightHand); }
bool IO::get_right_bumper() { return xbox->GetBumper(XboxController::JoystickHand::kRightHand); }
double IO::get_right_X() { return xbox->GetX(XboxController::JoystickHand::kRightHand); }
double IO::get_right_Y() { return xbox->GetY(XboxController::JoystickHand::kRightHand); }
bool IO::get_right_stick() { return xbox->GetStickButton(XboxController::JoystickHand::kRightHand); }

bool IO::get_A() { return xbox->GetAButton(); }
bool IO::get_B() { return xbox->GetBButton(); }
bool IO::get_X() { return xbox->GetXButton(); }
bool IO::get_Y() { return xbox->GetYButton(); }
bool IO::get_back() { return xbox->GetBackButton(); }
bool IO::get_start() { return xbox->GetStartButton(); }

IO *IO::get_instance() { // Only make one instance of IO
  if (io == NULL) {
    io = new IO();
    io->setup();
  }
  return io;
}
