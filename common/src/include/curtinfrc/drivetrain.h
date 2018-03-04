#pragma once

#include "curtinfrc/motors/EncoderProvider.h"
#include "curtinfrc/strategy/strategy.h"

#include <RobotController.h>
#include <SpeedController.h>

namespace curtinfrc {

  class Drivetrain {
  public:
    Drivetrain(EncoderProvider *enc_left, EncoderProvider *enc_right, SpeedController *left, SpeedController *right)
      : _left_enc(enc_left), _right_enc(enc_right),
        _left(left), _right(right) { }

    void set_left(double value) {
      _left->Set(value);
    }

    void set_right(double value) {
      _right->Set(value);
    }

    void set_both(double value) {
      set_left(value);
      set_right(value);
    }

    void log_write() { }

    StrategyController &strategy_controller() {
      return strat_controller;
    }

  private:
    SpeedController *_left, *_right;
    EncoderProvider *_left_enc, *_right_enc;
    StrategyController strat_controller;
  };
}
