#include "IO.h"
#include "Map.h"
#include "SmartDashboard/SmartDashboard.h"

static IO *io;

int IO::init() { // Sets up IO

  // Assign ports to the pointers, as instance to be called from other classes
  left_motors[0] = new CurtinTalonSRX(Map::Motors::left_motors[0]);
  left_motors[0]->SetInverted(false);
  left_motors[1] = new CurtinTalonSRX(Map::Motors::left_motors[1]);
  left_motors[1]->SetInverted(false);
  left_motors[1]->SetDual(CurtinTalonSRX::ControlMode::Follower, left_motors[0]->GetDeviceID());

  right_motors[0] = new CurtinTalonSRX(Map::Motors::right_motors[0]);
  right_motors[0]->SetInverted(true);
  right_motors[1] = new CurtinTalonSRX(Map::Motors::right_motors[1]);
  right_motors[1]->SetInverted(true);
  right_motors[1]->SetDual(CurtinTalonSRX::ControlMode::Follower, right_motors[0]->GetDeviceID());

  for (int n = 0; n < Map::Motors::n_belev_motors; n++) {
    if (n == 0) {
      belev_motors[n] = new CurtinTalonSRX(Map::Motors::belev_motors[0]);
      belev_motors[n]->SetInverted(false);
    } else {
      belev_motors[n] = new CurtinTalonSRX(Map::Motors::belev_motors[n]);
      belev_motors[n]->SetInverted(false);
      belev_motors[n]->SetDual(CurtinTalonSRX::ControlMode::Follower, belev_motors[0]->GetDeviceID());
    }
  }

  for (int n = 0; n < Map::Motors::n_intake_motors; n++) {
    if (n == 0) {
      intake_motors_left[0] = new CurtinTalonSRX(Map::Motors::intake_motors_left[n]);
      intake_motors_left[0]->SetInverted(false);

      intake_motors_right[0] = new CurtinTalonSRX(Map::Motors::intake_motors_right[n]);
      intake_motors_right[0]->SetInverted(true);
    } else {
      intake_motors_left[n] = new CurtinTalonSRX(Map::Motors::intake_motors_left[n]);
      intake_motors_left[n]->SetInverted(false);
      intake_motors_left[n]->SetDual(CurtinTalonSRX::ControlMode::Follower, intake_motors_left[0]->GetDeviceID());

      intake_motors_right[n] = new CurtinTalonSRX(Map::Motors::intake_motors_right[n]);
      intake_motors_right[n]->SetInverted(true);
      intake_motors_right[n]->SetDual(CurtinTalonSRX::ControlMode::Follower, intake_motors_right[0]->GetDeviceID());
    }
  }


  try {
    navx = new AHRS(I2C::Port::kOnboard);
  } catch (std::exception& ex) {
    std::string err_string = "Error instantiating navX MXP:  ";
    err_string += ex.what();
    DriverStation::ReportError(err_string.c_str());
  }

  navx->ZeroYaw();

  belev_limit_max = new DigitalInput(Map::Sensors::belev_limit_max);
  belev_limit_min = new DigitalInput(Map::Sensors::belev_limit_min);


  for (int n = 0; n < Map::Pneumatics::n_intake_solenoids; n++) intake_solenoids[n] = new DoubleSolenoid(0, Map::Pneumatics::intake_solenoids[n][0], Map::Pneumatics::intake_solenoids[n][1]);
  brake_solenoid = new DoubleSolenoid(0, Map::Pneumatics::brake_solenoid[0], Map::Pneumatics::brake_solenoid[1]);
  shifter_solenoid = new DoubleSolenoid(0, Map::Pneumatics::shifter_solenoid[0], Map::Pneumatics::shifter_solenoid[1]);

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
double IO::get_left_Y() { return -xbox->GetY(XboxController::JoystickHand::kLeftHand); }
bool IO::get_left_stick() { return xbox->GetStickButton(XboxController::JoystickHand::kLeftHand); }

double IO::get_right_trigger() { return xbox->GetTriggerAxis(XboxController::JoystickHand::kRightHand); }
bool IO::get_right_bumper() { return xbox->GetBumper(XboxController::JoystickHand::kRightHand); }
double IO::get_right_X() { return xbox->GetX(XboxController::JoystickHand::kRightHand); }
double IO::get_right_Y() { return -xbox->GetY(XboxController::JoystickHand::kRightHand); }
bool IO::get_right_stick() { return xbox->GetStickButton(XboxController::JoystickHand::kRightHand); }

bool IO::get_A() { return xbox->GetAButton(); }
bool IO::get_B() { return xbox->GetBButton(); }
bool IO::get_X() { return xbox->GetXButton(); }
bool IO::get_Y() { return xbox->GetYButton(); }
bool IO::get_back() { return xbox->GetBackButton(); }
bool IO::get_start() { return xbox->GetStartButton(); }

#elif JOY_CONTROL

double IO::get_left_Y() { return -left_joy->GetY(); }
double IO::get_left_X() { return left_joy->GetX(); }
double IO::get_left_twist() { return left_joy->GetZ(); }

bool IO::get_left_trigger() { return left_joy->GetTrigger(); }
bool IO::get_left_button(int nButton) { return left_joy->GetRawButton(nButton); }


double IO::get_right_Y() { return -right_joy->GetY(); }
double IO::get_right_X() { return right_joy->GetX(); }
double IO::get_right_twist() { return right_joy->GetZ(); }

bool IO::get_right_trigger() { return right_joy->GetTrigger(); }
bool IO::get_right_button(int nButton) { return right_joy->GetRawButton(nButton); }

#elif DRIVER_TRAINING

bool IO::get_left_xbox_trigger() { return xbox->GetTriggerAxis(XboxController::JoystickHand::kLeftHand) > Map::Controllers::xbox_trigger_deadzone; }
bool IO::get_left_xbox_bumper() { return xbox->GetBumper(XboxController::JoystickHand::kLeftHand); }
double IO::get_left_xbox_Y() { return -xbox->GetY(XboxController::JoystickHand::kLeftHand); }
bool IO::get_left_xbox_stick() { return xbox->GetStickButton(XboxController::JoystickHand::kLeftHand); }

bool IO::get_right_xbox_trigger() { return xbox->GetTriggerAxis(XboxController::JoystickHand::kRightHand) > Map::Controllers::xbox_trigger_deadzone; }
bool IO::get_right_xbox_bumper() { return xbox->GetBumper(XboxController::JoystickHand::kRightHand); }
double IO::get_right_xbox_Y() { return -xbox->GetY(XboxController::JoystickHand::kRightHand); }
bool IO::get_right_xbox_stick() { return xbox->GetStickButton(XboxController::JoystickHand::kRightHand); }

bool IO::get_xbox_A() { return xbox->GetAButton(); }



double IO::get_left_Y() { return -left_joy->GetY(); }
double IO::get_left_X() { return left_joy->GetX(); }
double IO::get_left_twist() {
  SmartDashboard::PutNumber("Left Z:", -left_joy->GetZ());
  return -left_joy->GetZ();
}

bool IO::get_left_trigger() { return left_joy->GetTrigger(); }
bool IO::get_left_button(int nButton) { return left_joy->GetRawButton(nButton); }


double IO::get_right_Y() { return -right_joy->GetY(); }
double IO::get_right_X() { return right_joy->GetX(); }
double IO::get_right_twist() { return -right_joy->GetZ(); }

bool IO::get_right_trigger() { return right_joy->GetTrigger(); }
bool IO::get_right_button(int nButton) { return right_joy->GetRawButton(nButton); }

#endif


bool IO::get_belev_limit_max() { return belev_limit_max->Get(); }
bool IO::get_belev_limit_min() { return belev_limit_min->Get(); }


IO *IO::get_instance() { // Only make one instance of IO
  if (io == NULL) {
    io = new IO();
    io->init();
  }
  return io;
}
