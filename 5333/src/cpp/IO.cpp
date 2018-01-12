#include "IO.h"

static IO *io;

void IO::setup() { // Sets up IO
  // Assign ports to the pointers, as instance to be called from other classes
  left_motors[0] = new CANTalon(0);
  left_motors[1] = new CANTalon(1);
  right_motors[0] = new CANTalon(2);
  right_motors[1] = new CANTalon(3);

  lift_motor[0] = new CANTalon(4);

  // loader = new DoubleSolonoid(1,0,1);

  xbox = new XboxController(0);
}

double IO::get_left_trigger() {
  return xbox->GetTriggerAxis(XboxController::JoystickHand::kLeftHand);
}
double IO::get_right_trigger() {
  return xbox->GetTriggerAxis(XboxController::JoystickHand::kRightHand);
}

double IO::get_left_y() {
  return xbox->GetTriggerAxis(XboxController::JoystickHand::kRightHand);
}
double IO::get_right_y() {
  return xbox->GetTriggerAxis(XboxController::JoystickHand::kRightHand);
}
xbox->GetY(XboxController::JoystickHand::kLeftHand)

IO *IO::get_instance() { // Only make one instance of IO
  if (io == NULL) {
    io = new IO();
    io->setup();
  }
  return io;
}
