#include "Belev.h"
#include "IO.h"

#include <RobotController.h>

void BelevatorControl::send_to_robot(double output) { // Sends the output to motor
  for(auto motor : IO::get_instance()->belev_motors) motor->Set(output); // Set for all motors
}

void BelevatorControl::log_write() {
  auto io = IO::get_instance();
  log.write(::frc::RobotController::GetFPGATime(), 1, io->belev_motors[0]->GetSelectedSensorPosition(0));
}
