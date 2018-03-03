#include "curtinfrc/strategy/mp_strategy.h"
#include <iostream>

using namespace curtinfrc;

// BASE STRATEGY //

void MotionProfileStrategy::start() {
  _notifier = new frc::Notifier([=]() {
    if (_running) {
      _esc->Set(_mode->calculate());
      done = _mode->done;
    }
  });
  _mode->init();
  _running = true;
  _notifier->StartPeriodic(_mode->ctrl_period());
}

void MotionProfileStrategy::tick(double time) {}

void MotionProfileStrategy::stop() {
  _running = false;
  _notifier->Stop();
  delete _notifier;
  _esc->StopMotor();
}

// DRIVETRAIN //

void DrivetrainMotionProfileStrategy::start() {
  _notifier = new frc::Notifier([=]() {
    if (_running) {
      double l = _mode_left->calculate();
      double r = _mode_right->calculate();

      if (_ahrs != nullptr && _mode_left->gyro_capable() && _mode_right->gyro_capable()) {
        double gyro = fmod(-_ahrs->GetYaw(), 360);
        double heading = _mode_left->gyro_desired();

        double angle_error = fmod(heading - gyro, 360);
        angle_error = angle_error > 180 ? angle_error - 360 : angle_error;
        double turn = _ahrs_kP * angle_error;

        l -= turn;
        r += turn;
      }

      _drivetrain->set_left(l);
      _drivetrain->set_right(r);
    }
  });
  _mode_left->init();
  _mode_right->init();
  _running = true;
  _notifier->StartPeriodic(_mode_left->ctrl_period());
}

void DrivetrainMotionProfileStrategy::tick(double time) { }

void DrivetrainMotionProfileStrategy::stop() {
  _running = false;
  _notifier->Stop();
  delete _notifier;
  _drivetrain->set_both(0);
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
