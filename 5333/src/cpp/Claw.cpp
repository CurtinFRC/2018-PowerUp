#include "WPIlib.h"
#include "Claw.h"
#include "IO.h"

#include <RobotController.h>

void ClawControl::send_to_robot(DoubleSolenoid::Value output) { // Sends the output to motor
  for(auto solenoid : IO::get_instance()->claw_solenoids) solenoid->Set(output);
}

void ClawControl::log_write() {
  auto io = IO::get_instance();
  log.write(::frc::RobotController::GetFPGATime(), 1, io->claw_solenoids[0]->Get());
}
