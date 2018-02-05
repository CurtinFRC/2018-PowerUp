#pragma once

#include "curtinfrc/logger.h"

class WinchControl {
public:
  WinchControl();
  void send_to_robot(double power);
};
