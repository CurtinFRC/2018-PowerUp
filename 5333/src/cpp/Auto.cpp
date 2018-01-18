#include "Auto.h"
#include <pathfinder.h>

int AutoControl::init() {
  points[0] = { -4, -1, d2r(45) };      // Waypoint @ x=-4, y=-1, exit angle=45 degrees
  points[1] = { -1, 2, 0 };             // Waypoint @ x=-1, y= 2, exit angle= 0 radians
  points[2] = {  2, 4, 0 };             // Waypoint @ x= 2, y= 4, exit angle= 0 radians

  return 0;
}
