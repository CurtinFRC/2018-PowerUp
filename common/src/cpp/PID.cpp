#include "PID.h"

PID::PID(double _kp, double _ki, double _kd, double _kde) {
  kp = _kp;
  ki = _ki;
  kd = _kd;

  kde = _kde;

  err = last_err = err_int = last_time = 0;
}

void PID::set_target(double _target) {
  target = _target;

  err = last_err = err_int = 0;
}

double PID::calculate(double input, double t) {
  double dt = t - last_time, output;
  last_time = t;

  if (dt == 0) dt = 1;
  if (last_err == 0) last_err = err;

  output = kp * err + ki * (err_int += err * dt) + kd * (err - last_err) / dt;
  last_err = err;

  return output;
}

bool PID::done(double input, double t) { return (err - last_err) / (t - last_time) < kde; }
