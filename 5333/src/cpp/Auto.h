#pragma once

#include <pathfinder.h>

#define nPOINTS 3

class AutoControl {
public:
  int init();
  void tick();

  int segment_length;
  Segment segments_left[1024], segments_right[1024];
  EncoderFollower *leftfollower, *rightfollower;
};
