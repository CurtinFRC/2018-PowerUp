#include "curtinfrc/motors/CurtinTalonSRX.h"

#include <iostream>
#include <cmath>

using namespace curtinfrc;

void CurtinTalonSRX::SetControlMode(ControlMode mode) {
  _ctrl_mode = mode;
}

void CurtinTalonSRX::Set(double speed) {
  TalonSRX::Set(_ctrl_mode, speed);
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