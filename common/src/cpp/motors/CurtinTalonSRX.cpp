#include "curtinfrc/motors/CurtinTalonSRX.h"

#include <iostream>
#include <cstdio>
#include <cmath>

using namespace curtinfrc;

void CurtinTalonSRX::SetControlMode(ControlMode mode) {
  _ctrl_mode = mode;
}

void CurtinTalonSRX::Set(double speed) {
  TalonSRX::Set(_ctrl_mode, speed);
  _val = speed;
}

void CurtinTalonSRX::SetDual(ControlMode mode, double speed) {
  TalonSRX::Set(mode, speed);
  _val = speed;
}

double CurtinTalonSRX::Get() const {
  return _val;
}

void CurtinTalonSRX::SetInverted(bool inv) {
  TalonSRX::SetInverted(inv);
}

bool CurtinTalonSRX::GetInverted() const {
  return TalonSRX::GetInverted();
}

void CurtinTalonSRX::Disable() {
  TalonSRX::NeutralOutput();
}

void CurtinTalonSRX::StopMotor() {
  TalonSRX::NeutralOutput();
}

void CurtinTalonSRX::PIDWrite(double val) {
  Set(val);
}

uint64_t CurtinTalonSRX::GetEncoder() {
  return TalonSRX::GetSelectedSensorPosition(0);
}

void CurtinTalonSRX::selftest_onstart() {
  StopMotor();
  _st_stage = 0;
}

TestState CurtinTalonSRX::selftest_run(double elapsed_ms) {
  switch (_st_stage) {
  case 0: // Start Testing
    std::cout << selftest_name() << ": Running Motor..." << std::endl;
    _st_stage++;
  case 1: // Hold Motor Speed 
    Set(selftest_speed);
    if (elapsed_ms > selftest_duration)
      _st_stage++;
    break;
  case 2: // Take Measurements and Stop Test
    double current = abs(GetOutputCurrent());
    if (current > selftest_current_max || current < selftest_current_min) {
      std::cout << selftest_name() << ": Current FAULT @ " << current << std::endl;
      std::cout << "\tMin: " << selftest_current_min << " Max: " << selftest_current_max << std::endl;
      return TestState::FAILED;
    } else {
      std::cout << selftest_name() << ": Current Nominal @ " << current << std::endl;
    }

    return TestState::PASSED;
    break;
  }
  return TestState::RUNNING;
}

void CurtinTalonSRX::selftest_onstop() {
  _st_stage = -1;
  StopMotor();
}

void CurtinTalonSRX::configure_pidf(double kp, double ki, double kd, double kf) {
  TalonSRX::Config_kP(0, kp, 0);
  TalonSRX::Config_kI(0, ki, 0);
  TalonSRX::Config_kD(0, kd, 0);
  TalonSRX::Config_kF(0, kf, 0);
}

void CurtinTalonSRX::configure_encoder_edges_per_rev(unsigned int tpr) {
  ticks_per_rev = tpr;
}

void CurtinTalonSRX::configure_wheel_diameter(double diameter) {
  wheel_diameter = diameter * 0.0254; // convert inches to metres
}

void CurtinTalonSRX::configure_mp_update_rate(unsigned int milliseconds) {
  TalonSRX::ConfigMotionProfileTrajectoryPeriod(milliseconds, 0);
  TalonSRX::ChangeMotionControlFramePeriod(milliseconds / 2);
}

bool CurtinTalonSRX::load_pathfinder(Segment *segments, int length) {
  if (_mp_load_offset == 0) {
    TalonSRX::ClearMotionProfileTrajectories();
    SetControlMode(CurtinTalonSRX::ControlMode::MotionProfile);
  }

  double rev_per_m = 1 / (PI * wheel_diameter);
  for (int i = _mp_load_offset; i < length; i++) {
    if (TalonSRX::IsMotionProfileTopLevelBufferFull()) {
      _mp_load_offset = i;
      return false;
    }

    Segment *s = &segments[i];
    double rpm = (s->velocity / (wheel_diameter / 2 * 0.10472));
    double pos = s->position * rev_per_m * ticks_per_rev;
    double vel = (rpm / 60.0) * ticks_per_rev * 10;
    ctre::phoenix::motion::TrajectoryPoint tp = {
      pos,
      vel,
      0,    // heading (degrees). Only needed if using Pidgeon IMU
      0, 0, // slot select
      i == (length - 1),  // last point?
      i == 0,             // zero sensor?
      ctre::phoenix::motion::TrajectoryDuration_0ms
    };

    // Fails if the buffer becomes full
    if (TalonSRX::PushMotionProfileTrajectory(tp) != ctre::phoenix::ErrorCode::OKAY) {
      _mp_load_offset = i + 1;
      return false;
    }
  }
  _mp_load_offset = length - 1;
  return true;
}

void CurtinTalonSRX::reset_mp() {
  _mp_load_offset = 0;
  TalonSRX::ClearMotionProfileTrajectories();
}

void CurtinTalonSRX::enable_mp() {
  Set(ctre::phoenix::motion::SetValueMotionProfile::Enable);
}

void CurtinTalonSRX::hold_mp() {
  Set(ctre::phoenix::motion::SetValueMotionProfile::Hold);
}

void CurtinTalonSRX::disable_mp() {
  Set(ctre::phoenix::motion::SetValueMotionProfile::Disable);
}

CurtinTalonSRX::MotionProfileStatus CurtinTalonSRX::process_mp() {
  TalonSRX::ProcessMotionProfileBuffer();
  CurtinTalonSRX::MotionProfileStatus status;
  GetMotionProfileStatus(status);
  return status;
}