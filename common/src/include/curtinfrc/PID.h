#pragma once

class PID {
public:
  PID(double _kp, double _ki, double _kd, double _kde);
  void set_target(double _target);
  double calculate(double input, double t);
  bool done(double input, double t);

private:
  double kp, ki, kd, kde;
  double target, last_time;
  double err, last_err, err_int;
};
