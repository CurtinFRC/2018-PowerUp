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

  double angle_error = fmod(heading - gyro, 360);
  angle_error = angle_error > 180 ? -angle_error + 180 : angle_error;
  double turn = 1.2 * (-1.0 / 80.0) * angle_error;

  std::cout << _ahrs->GetAngle() << ", " << r2d(_followl.heading) << ", " << angle_error << std::endl;

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
  std::cout << "Tuner Strategy Started" << std::endl;
  _escl->SetControlMode(CurtinTalonSRX::ControlMode::PercentOutput);
  _escr->SetControlMode(CurtinTalonSRX::ControlMode::PercentOutput);
  _ahrs_initial = _ahrs->GetAngle();
  _state = 0;
  _throttle = 0;
  _outfile << "time,voltage,distance,linear_vel\n";
  _outfile_accel << "time,voltage_applied,linear_vel,accel\n";
}

void MotionProfileTunerStrategy::tick(double time) {
  double rots = (fabs(_ahrs->GetAngle()) - fabs(_ahrs_initial)) / 360.0;
  if (_state == 0) {
    _escl->SetSelectedSensorPosition(0, 0, 0);
    _escr->SetSelectedSensorPosition(0, 0, 0);
    _state = 1;
  } else if (_state == 1) {
    std::cout << rots << std::endl;
    // Taking Data. Rotate ~10 times, get encoder distance, solve for circle diameter.
    _escl->Set(1);
    _escr->Set(-1);
    if (rots > 10)
      _state = 2;
  } else if (_state == 2) {
    // Calculate Data.
    _escl->Set(0);
    _escr->Set(0);

    double dist_native = (fabs(_escl->GetSelectedSensorPosition(0)) + fabs(_escr->GetSelectedSensorPosition(0))) / 2.0;
    double dist = (dist_native / _tpr) * PI * _wd;
    _set_time = time;

    _trackwidth = dist / (rots * PI);

    std::cout << "EMPIRICAL TRACKWIDTH: " << _trackwidth << " METRES" << std::endl;
    _state = 3;
  } else if (_state == 3) {
    // Voltage Ramp
    _escl->Set(_throttle);
    _escr->Set(-_throttle);

    if (time - _set_time > 2000) _state = 4;
  }
  else if (_state == 4) {
    // Voltage Measurement
    double voltage = _escl->GetMotorOutputVoltage();
    double dist_native = (fabs(_escl->GetSelectedSensorPosition(0)) + fabs(_escr->GetSelectedSensorPosition(0))) / 2.0;
    double dist = (dist_native / _tpr) * PI * _wd;
    double vel = ((fabs(_escl->GetSelectedSensorVelocity(0)) + fabs(_escr->GetSelectedSensorVelocity(0))) / 2.0 * 10) / _tpr * PI * _wd;  // m/s

    _outfile << time << "," << voltage << "," << dist << "," << vel << "\n";
    _throttle += 0.025;
    _state = 3;
    _set_time = time;
    if (_throttle > 1.0) {
      _state = 5;
    }
  } else if (_state == 5) {
    // Acceleration Measurement
    if (time - _set_time < 1000) {
      _escl->Set(0);
      _escr->Set(0);
    } else if (time - _set_time < 5000) {
      _escl->Set(0.6);
      _escr->Set(-0.6);

      double voltage = _escl->GetMotorOutputVoltage();
      double dt = (time - _last_time) / 1000.0;
      double vel = ((fabs(_escl->GetSelectedSensorVelocity(0)) + fabs(_escr->GetSelectedSensorVelocity(0))) / 2.0 * 10) / _tpr * PI * _wd;  // m/s
      double accel = (vel - _last_vel) / (dt);
      _last_vel = vel;

      _outfile_accel << time << "," << voltage << "," << vel << "," << accel << "\n";
    } else {
      done = true;
    }
  } else {
    done = true;
  }
  _last_time = time;
}

void MotionProfileTunerStrategy::stop() {
  std::cout << "Tuner Strategy Stopped" << std::endl;
  _escl->Set(0);
  _escr->Set(0);
  _outfile.close();
  _outfile_accel.close();
}