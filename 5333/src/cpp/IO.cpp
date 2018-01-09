#include "IO.h"

static IO *io;

void IO::setup() { // Sets up IO
  // Assign ports to the pointers, as instance to be called from other classes
  left_motors[0] = new Victor(0);
  left_motors[1] = new Victor(1);
  right_motors[0] = new Victor(2);
  right_motors[1] = new Victor(3);

  // loader = new DoubleSolonoid(1,0,1);

  xbox = new XboxController(0);
}

IO *IO::get_instance() { // Only make one instance of IO
  if (io == NULL) {
    io = new IO();
    io->setup();
  }
  return io;
}
