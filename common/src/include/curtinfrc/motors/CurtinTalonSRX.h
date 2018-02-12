#pragma once

#include "curtinfrc/selftest/selftest.h"
#include "curtinfrc/motors/EncoderProvider.h"

extern "C" {
  #include <pathfinder.h>
}

#include <SpeedController.h>
#include <ctre/phoenix/MotorControl/CAN/BaseMotorController.h>
#include <ctre/phoenix/MotorControl/CAN/TalonSRX.h>

namespace curtinfrc {

  class CurtinTalonSRX : public virtual ctre::phoenix::motorcontrol::can::TalonSRX,
    public virtual frc::SpeedController,
    public ISelfTestable,
    public EncoderProvider {
  public:
    using ctre::phoenix::motorcontrol::can::TalonSRX::TalonSRX;
    using ctre::phoenix::motorcontrol::can::BaseMotorController::BaseMotorController;

    CurtinTalonSRX() = delete;
    CurtinTalonSRX(int c) : BaseMotorController(c | 0x02040000), TalonSRX(c) {}

    typedef ctre::phoenix::motorcontrol::ControlMode ControlMode;
    typedef ctre::phoenix::motion::MotionProfileStatus MotionProfileStatus;

    // SPEEDCONTROLLER //

    void SetControlMode(ControlMode mode);
    void Set(double speed) override;
    void SetDual(ControlMode mode, double speed);
    double Get() const override;
    void SetInverted(bool inv) override;
    bool GetInverted() const override;
    void Disable() override;
    void StopMotor() override;
    void PIDWrite(double val) override;
    uint64_t GetEncoder() override;

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

    // MOTION PROFILING //

    void configure_pidf(double kp, double ki, double kd, double kf);
    void configure_encoder_edges_per_rev(unsigned int ticks_per_rev);     // Note: only effects motion profiling, not GetSelectedSensorPositions

    /**
     * Configure wheel diameter for motion profiling. Must be in inches
     */
    void configure_wheel_diameter(double diameter);

    /**
     * Configure the Motion Profile Update Rate in milliseconds. This should match the Pathfinder
     * generation rate.
     */
    void configure_mp_update_rate(unsigned int milliseconds);

    /**
     * Load a Pathfinder Path partially.
     * \param \c segments The pathfinder segments
     * \param \c length The number of pathfinder segments to process
     * \return Has the path been fully loaded? If false, this will need
     *          to be called again
     */
    bool load_pathfinder(Segment *segments, int length);

    /**
     * Reset the Motion Profile buffer (reset pathfinder load status).
     * Call this instead of ClearMotionProfileTrajectories()
     */
    void reset_mp();

    /**
     * Start motion profiling (must be called after load_pathfinder() or SetControlMode(MotionProfile))
     */
    void enable_mp();

    /**
     * Hold the current motion profile point (should be called when last point)
     */
    void hold_mp();

    /**
     * Disable the motion profile
     */
    void disable_mp();

    /**
     * Process the Motion Profile
     * \return The motion profile status. Check .isLast() to check if path is finished
     */
    MotionProfileStatus process_mp();

  private:
    ControlMode _ctrl_mode = ControlMode::PercentOutput;
    double _val;
    int _st_stage;

    // motion
    unsigned int ticks_per_rev, _mp_load_offset = 0;
    double wheel_diameter;
  };

} // ns curtinfrc
