#include "Winch.h"
#include "IO.h"

#include <RobotController.h>

void WinchControl::send_to_robot(double power) { // Positive is climbs up (retract)
  for (auto motor : IO::get_instance()->winch_motors) motor->Set(power);
}

void WinchControl::log_write() {
  auto io = IO::get_instance();
  log.write(::frc::RobotController::GetFPGATime(), 1, io->winch_motors[0]->GetSelectedSensorPosition(0));
}
