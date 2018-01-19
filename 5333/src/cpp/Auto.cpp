#include "Auto.h"
#include <pathfinder.h>

int AutoControl::init() {
  points[0] = { -4, -1, d2r(45) };
  points[1] = { -1, 2, 0 };
  points[2] = {  2, 4, 0 };

  return 0;
}
