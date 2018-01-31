#include "curtinfrc/strategy/mp_strategy.h"
#include <iostream>

using namespace curtinfrc;

void MotionProfileStrategy::start() {
  double start_time = (static_cast<double>(RobotController::GetFPGATime()) / 1.0e6);
  _notifier = new frc::Notifier([=]() {
    double time = (static_cast<double>(RobotController::GetFPGATime()) / 1.0e6) - start_time;
    if (_cfg.mode == MotionProfileMode::TALON_MP)
      tick_talonmp(time);
    else
      tick_pathfinder(time);
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

    _escl->SetControlFramePeriod(ctre::phoenix::motorcontrol::ControlFrame::Control_4_Advanced, 5);
    _escr->SetControlFramePeriod(ctre::phoenix::motorcontrol::ControlFrame::Control_4_Advanced, 5);

    _escl->SetControlMode(CurtinTalonSRX::ControlMode::PercentOutput);
    _escr->SetControlMode(CurtinTalonSRX::ControlMode::PercentOutput);
  }

  double rate = (_cfg.mode == MotionProfileMode::TALON_MP ? 0.5 : 1) * _segments_left[0].dt;
  _notifier->StartPeriodic(rate);
}

void MotionProfileStrategy::tick(double time) {
  // Not important
}

void MotionProfileStrategy::tick_talonmp(double time) {
   // soon
  done = true;
}

void MotionProfileStrategy::tick_pathfinder(double time) {
  // Note: Voltage Units
  double l = pathfinder_follow_encoder(_ecfg_l, &_followl, _segments_left, _segments_length, _escl->GetSelectedSensorPosition(0));
  double r = pathfinder_follow_encoder(_ecfg_r, &_followr, _segments_right, _segments_length, _escr->GetSelectedSensorPosition(0));
  MPLogPoint *lp_left = &_lp_left[_followl.segment], *lp_right = &_lp_right[_followr.segment];
  Segment *s_left = &_segments_left[_followl.segment], *s_right = &_segments_right[_followr.segment];

  double gyro = fmod(-_ahrs->GetAngle(), 360);
  double heading = fmod(r2d(_followl.heading), 360);

  double angle_error = fmod(heading - gyro, 360);
  angle_error = angle_error > 180 ? -angle_error + 180 : angle_error;
  double turn = _cfg.kt * angle_error;

  double l_throttle = (l / _escl->GetBusVoltage());
  double r_throttle = (r / _escr->GetBusVoltage());
  _escl->Set(l_throttle - turn);
  _escr->Set(r_throttle + turn);

  // Logging Stuff
  lp_left->time = time;
  lp_left->output = l_throttle; lp_right->output = r_throttle;
  lp_left->output_real = l_throttle + turn; lp_right->output_real = r_throttle - turn;
  lp_left->voltage = l; lp_right->voltage = r;
  
  lp_left->pos_real = s_left->position - _followl.last_error;
  lp_left->pos_target = s_left->position;
  lp_left->vel_real = (_escl->GetSelectedSensorVelocity(0) * 10) / _cfg.enc_ticks_per_rev * PI * _cfg.wheel_diameter * 0.0254;
  lp_left->vel_target = s_left->velocity;

  lp_right->pos_real = s_right->position - _followr.last_error;
  lp_right->pos_target = s_right->position;
  lp_right->vel_real = (_escr->GetSelectedSensorVelocity(0) * 10) / _cfg.enc_ticks_per_rev * PI * _cfg.wheel_diameter * 0.0254;
  lp_right->vel_target = s_right->velocity;

  lp_left->angle_real = lp_right->angle_real = gyro;
  lp_left->angle_target = lp_right->angle_target = heading;

  if (_followl.finished && _followr.finished) done = true;
}

static void _write_single_logpoint(std::ofstream &outfile, MPLogPoint &point) {
  outfile << point.time << "," << point.output << "," << point.output_real << "," << point.voltage << "," << point.pos_real << "," << point.pos_target << ",";
  outfile << point.vel_real << "," << point.vel_target << "," << point.angle_real << "," << point.angle_target << "\n";
}

void MotionProfileStrategy::stop() {
  _notifier->Stop();
  _escl->SetControlMode(CurtinTalonSRX::ControlMode::PercentOutput);
  _escr->SetControlMode(CurtinTalonSRX::ControlMode::PercentOutput);
  _escl->Set(0);
  _escr->Set(0);

  _outfile_left << "time,output,output_real,voltage,pos_real,pos_target,vel_real,vel_target,angle_real,angle_target\n";
  _outfile_right << "time,output,output_real,voltage,pos_real,pos_target,vel_real,vel_target,angle_real,angle_target\n";
  for (int i = 0; i < _segments_length; i++) {
    _write_single_logpoint(_outfile_left, _lp_left[i]);
    _write_single_logpoint(_outfile_right, _lp_right[i]);
  }
  _outfile_left.close();
  _outfile_right.close();
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