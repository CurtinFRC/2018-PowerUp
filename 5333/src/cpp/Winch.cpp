#include "Winch.h"
#include "IO.h"

#include <RobotController.h>

WinchControl::WinchControl() {}

void WinchControl::send_to_robot(double power) { // Positive is climbs up (retract)
  for (auto motor : IO::get_instance()->winch_motors) motor->Set(power);
}
