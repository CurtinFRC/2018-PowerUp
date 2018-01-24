#pragma once

#include <pathfinder.h>

#define nPOINTS 3

class AutoControl {
public:
  int init();
  void tick();

  int segment_length;
  Segment segments_left[1024], segments_right[1024];

private:
  const double _kp = 0.08;
  const double _ki = 0; // 0.00005
  const double _kd = 0;
  const double _kf = 0;
};
