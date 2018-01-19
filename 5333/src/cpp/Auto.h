#pragma once

#include <pathfinder.h>

#define nPOINTS 3

class AutoControl {
public:
  int init();

  Waypoint points[nPOINTS];
};
