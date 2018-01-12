#pragma once

#include "curtinfrc/selftest/selftest.h"

#include <SpeedController.h>
#include <ctre/phoenix/MotorControl/CAN/BaseMotorController.h>
#include <ctre/phoenix/MotorControl/CAN/TalonSRX.h>

namespace curtinfrc {

  class CurtinTalonSRX : public virtual ctre::phoenix::motorcontrol::can::TalonSRX,
    public virtual frc::SpeedController,
    public ISelfTestable {
  public:
    using ctre::phoenix::motorcontrol::can::TalonSRX::TalonSRX;
    using ctre::phoenix::motorcontrol::can::BaseMotorController::BaseMotorController;
    
    CurtinTalonSRX() = delete;
    CurtinTalonSRX(int c) : BaseMotorController(c | 0x02040000), TalonSRX(c) {}

    typedef ctre::phoenix::motorcontrol::ControlMode ControlMode;

    // SPEEDCONTROLLER //

    void SetControlMode(ControlMode mode);
    void Set(double speed) override;
    double Get() const override;
    void SetInverted(bool inv) override;
    bool GetInverted() const override;
    void Disable() override;
    void StopMotor() override;
    void PIDWrite(double val) override;

    // SELF TEST //

    std::string selftest_name() {
      return ("CAN Talon SRX (" + std::to_string(TalonSRX::GetDeviceID()) + ")");
    }

    double  selftest_current_min = 1.0;
    double  selftest_current_max = 30.0;
    double  selftest_speed = 1.0;
    double  selftest_duration = 0.5;

    void selftest_onstart() override;
    TestState selftest_run(double elapsed_time_ms);
    void selftest_onstop() override;
  private:
    ControlMode _ctrl_mode = ControlMode::PercentOutput;
    double _val;
    int _st_stage;
  };

} // ns curtinfrc