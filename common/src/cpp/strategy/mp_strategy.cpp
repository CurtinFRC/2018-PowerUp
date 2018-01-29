#include "curtinfrc/strategy/mp_strategy.h"
#include <iostream>

using namespace curtinfrc;

void MotionProfileStrategy::start() {
  _notifier = new frc::Notifier([=]() {
    if (_cfg.mode == MotionProfileMode::TALON_MP)
      tick_talonmp();
    else
      tick_pathfinder();
  });

  if (_cfg.mode == MotionProfileMode::TALON_MP) {
    _escl->SetControlMode(CurtinTalonSRX::ControlMode::MotionProfile);
    _escr->SetControlMode(CurtinTalonSRX::ControlMode::MotionProfile);
  } else {
    _ecfg_l = {
      _escl->GetSelectedSensorPosition(0),
      _cfg.enc_ticks_per_rev, _cfg.wheel_diameter * 0.0254 * PI, _cfg.kp, 0, _cfg.kd, _cfg.kv, _cfg.ka
    };

    _ecfg_r = {
      _escr->GetSelectedSensorPosition(0),
      _cfg.enc_ticks_per_rev, _cfg.wheel_diameter * 0.0254 * PI, _cfg.kp, 0, _cfg.kd, _cfg.kv, _cfg.ka
    };

    _escl->SetControlMode(CurtinTalonSRX::ControlMode::PercentOutput);
    _escr->SetControlMode(CurtinTalonSRX::ControlMode::PercentOutput);
  }

  double rate = (_cfg.mode == MotionProfileMode::TALON_MP ? 0.5 : 1) * _segments_left[0].dt;
  _notifier->StartPeriodic(rate);
}

void MotionProfileStrategy::tick(double time) {
  // Not important
}

void MotionProfileStrategy::tick_talonmp() {
   // soon
  done = true;
}

void MotionProfileStrategy::tick_pathfinder() {
  double l = pathfinder_follow_encoder(_ecfg_l, &_followl, _segments_left, _segments_length, _escl->GetSelectedSensorPosition(0));
  double r = pathfinder_follow_encoder(_ecfg_r, &_followr, _segments_right, _segments_length, _escr->GetSelectedSensorPosition(0));

  double gyro = fmod(_ahrs->GetAngle(), 360);
  double heading = fmod(r2d(_followl.heading), 360);

  double angle_error = fmod(heading - gyro, 360) - 180;
  double turn = 0.8 * (-1.0 / 80.0) * angle_error;

  _escl->Set(l + turn);
  _escr->Set(r - turn);

  if (_followl.finished || _followr.finished) done = true;
}

void MotionProfileStrategy::stop() {
  _notifier->Stop();
  _escl->SetControlMode(CurtinTalonSRX::ControlMode::PercentOutput);
  _escr->SetControlMode(CurtinTalonSRX::ControlMode::PercentOutput);
  _escl->Set(0);
  _escr->Set(0);
}

// TUNING //

void MotionProfileTunerStrategy::start() {
  _escl->SetControlMode(CurtinTalonSRX::ControlMode::PercentOutput);
  _escr->SetControlMode(CurtinTalonSRX::ControlMode::PercentOutput);
  _ahrs_initial = _ahrs->GetAngle();
  _state = 0;
  _throttle = 0;
  _outfile << "time,voltage,rate,linear_vel\n";
}

void MotionProfileTunerStrategy::tick(double time) {
  double rots = (fabs(_ahrs->GetAngle()) - fabs(_ahrs_initial)) / 360.0;
  if (_state == 0) {
    _escl->SetSelectedSensorPosition(0, 0, 0);
    _escr->SetSelectedSensorPosition(0, 0, 0);
    _state = 1;
  } else if (_state == 1) {
    // Taking Data. Rotate ~10 times, get encoder distance, solve for circle diameter.
    _escl->Set(1);
    _escr->Set(-1);
    _ahrs_rate = _ahrs->GetRate();
    if (rots > 10)
      _state = 2;
  } else if (_state == 2) {
    // Calculate Data.
    _escl->Set(0);
    _escr->Set(0);

    double dist_native = (fabs(_escl->GetSelectedSensorPosition(0)) + fabs(_escr->GetSelectedSensorPosition(0))) / 2.0;
    double dist = (dist_native / _tpr) * PI * _wd;

    _trackwidth = dist / (rots * PI);
    double angular_vel = d2r(_ahrs_rate);
    double linear_vel = angular_vel * _trackwidth * 0.5;

    std::cout << "EMPIRICAL TRACKWIDTH: " << _trackwidth << " METRES" << std::endl;
    std::cout << "EMPIRICAL FINAL VELOCITY: " << linear_vel << " METRES/SEC" << std::endl;
    std::cout << "AHRS RATE: " << _ahrs_rate << " DEG/SEC" << std::endl;
    _state = 3;
  } else if (_state == 3) {
    // Voltage Ramp (takes ~ 5 seconds)
    _throttle += 0.01;
    _escl->Set(_throttle);
    _escr->Set(-_throttle);

    double voltage = _escl->GetMotorOutputVoltage();
    _ahrs_rate = _ahrs->GetRate();
    double linear_vel = d2r(_ahrs_rate) * _trackwidth * 0.5;

    _outfile << time << "," << voltage << "," << _ahrs_rate << "," << linear_vel << "\n";
    if (_throttle >= 1.0) done = true;
  } else {
    done = true;
  }
}

void MotionProfileTunerStrategy::stop() {
  _escl->Set(0);
  _escr->Set(0);
  _outfile.close();
}