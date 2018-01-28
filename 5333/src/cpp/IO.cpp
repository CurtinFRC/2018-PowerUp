#include "IO.h"
#include "Map.h"

static IO *io;

int IO::init() { // Sets up IO
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


  try {
    navx = new AHRS(SPI::Port::kMXP);
  } catch (std::exception& ex) {
    std::string err_string = "Error instantiating navX MXP:  ";
    err_string += ex.what();
    DriverStation::ReportError(err_string.c_str());
  }

  navx->ZeroYaw();

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

  #ifdef XBOX_CONTROL
  xbox = new XboxController(Map::Controllers::xbox);

  #elif JOY_CONTROL
  left_joy = new Joystick(Map::Controllers::joy[0]);
  right_joy = new Joystick(Map::Controllers::joy[1]);

  #elif DRIVER_TRAINING
  left_joy = new Joystick(Map::Controllers::joy[0]);
  right_joy = new Joystick(Map::Controllers::joy[1]);

  xbox = new XboxController(Map::Controllers::xbox);

  #endif

  return 0;
}

// Aliases
#ifdef XBOX_CONTROL

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

#elif JOY_CONTROL

double IO::get_left_Y() { return left_joy->GetY(); }
double IO::get_left_X() { return left_joy->GetX(); }
double IO::get_left_twist() { return left_joy->GetZ(); }

bool IO::get_left_trigger() { return left_joy->GetTrigger(); }
bool IO::get_left_button(int nButton) { return left_joy->GetRawButton(nButton); }


double IO::get_right_Y() { return right_joy->GetY(); }
double IO::get_right_X() { return right_joy->GetX(); }
double IO::get_right_twist() { return right_joy->GetZ(); }

bool IO::get_right_trigger() { return right_joy->GetTrigger(); }
bool IO::get_right_button(int nButton) { return right_joy->GetRawButton(nButton); }

#elif DRIVER_TRAINING

bool IO::get_left_xbox_bumper() { return xbox->GetBumper(XboxController::JoystickHand::kLeftHand); }
double IO::get_left_xbox_Y() { return xbox->GetY(XboxController::JoystickHand::kLeftHand); }
bool IO::get_left_xbox_stick() { return xbox->GetStickButton(XboxController::JoystickHand::kLeftHand); }

bool IO::get_right_xbox_bumper() { return xbox->GetBumper(XboxController::JoystickHand::kRightHand); }
double IO::get_right_xbox_Y() { return xbox->GetY(XboxController::JoystickHand::kRightHand); }
bool IO::get_right_xbox_stick() { return xbox->GetStickButton(XboxController::JoystickHand::kRightHand); }

bool IO::get_xbox_A() { return xbox->GetAButton(); }



double IO::get_left_Y() { return left_joy->GetY(); }
double IO::get_left_X() { return left_joy->GetX(); }
double IO::get_left_twist() { return left_joy->GetZ(); }

bool IO::get_left_trigger() { return left_joy->GetTrigger(); }
bool IO::get_left_button(int nButton) { return left_joy->GetRawButton(nButton); }


double IO::get_right_Y() { return right_joy->GetY(); }
double IO::get_right_X() { return right_joy->GetX(); }
double IO::get_right_twist() { return right_joy->GetZ(); }

bool IO::get_right_trigger() { return right_joy->GetTrigger(); }
bool IO::get_right_button(int nButton) { return right_joy->GetRawButton(nButton); }

#endif

IO *IO::get_instance() { // Only make one instance of IO
  if (io == NULL) {
    io = new IO();
    io->init();
  }
  return io;
}
