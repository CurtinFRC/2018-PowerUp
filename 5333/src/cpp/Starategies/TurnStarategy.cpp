#include "Starategies/TurnStarategy.h"

#include "curtinfrc/drivetrain.h"
#include "curtinfrc/PID.h"

#include "IO.h"
#include "Map.h"

using namespace std;

void TurnStarategy::start() {
  drive->set_left(0);
  drive->set_right(0);
}

void TurnStarategy::tick(double time) {
  double pid_input = IO::get_instance()->navx->GetYaw(), pid_output;

  pid_output = pid_loop.calculate(pid_input, time);
  if (pid_loop.done(pid_input, time)) this->done = true;

  drive->set_left(throttle * pid_output);
  drive->set_right(-throttle * pid_output);
}

void TurnStarategy::stop() {
  drive->set_left(0);
  drive->set_right(0);
}
