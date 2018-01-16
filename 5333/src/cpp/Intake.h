#pragma once

class IntakeControl {
public:
  void send_to_robot(bool open);
  void send_to_robot(double left, double right);
};
