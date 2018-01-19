#include "Belev.h"
#include "IO.h"

void BelevatorControl::send_to_robot(double output) { // Sends the output to motor
  for(auto motor : IO::get_instance()->belev_motors) motor->Set(output); // Set for all motors
}
