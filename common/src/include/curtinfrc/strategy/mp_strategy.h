#pragma once

#include "curtinfrc/motors/CurtinTalonSRX.h"
#include "curtinfrc/motors/MotionProfiling.h"
#include "curtinfrc/strategy/strategy.h"
#include "curtinfrc/drivetrain.h"

#include <SpeedController.h>
#include <Notifier.h>
#include <AHRS.h>
#include <fstream>

namespace curtinfrc {

  class MotionProfileStrategy : public Strategy {
  public:
    MotionProfileStrategy(
      std::shared_ptr<MotionProfilingMode> mode,
      frc::SpeedController *esc
    ) : _mode(mode), _esc(esc) { }

    void start() override;
    void tick(double time) override;
    void stop() override;

  private:
    std::shared_ptr<MotionProfilingMode> _mode;
    frc::SpeedController *_esc;
    bool _running;

    frc::Notifier *_notifier;
  };

  class DrivetrainMotionProfileStrategy : public Strategy {
  public:
    DrivetrainMotionProfileStrategy(
      std::shared_ptr<MotionProfilingMode> mode_left, std::shared_ptr<MotionProfilingMode> mode_right,
      Drivetrain *drivetrain,
      AHRS *ahrs,        // Set to nullptr to not use the gyroscope
      double ahrs_kP
    ) : _mode_left(mode_left), _mode_right(mode_right), _drivetrain(drivetrain), _ahrs(ahrs), _ahrs_kP(ahrs_kP) { }

    void start() override;
    void tick(double time) override;
    void stop() override;

  private:
    std::shared_ptr<MotionProfilingMode> _mode_left, _mode_right;
    Drivetrain *_drivetrain;
    AHRS *_ahrs;
    double _ahrs_kP;
    frc::Notifier *_notifier;
    bool _running;
  };

  /**
   * Simple testing strategy to find the Motion Profiling coefficients kv and ka.
   */
  class MotionProfileTunerStrategy : public Strategy {
  public:
    MotionProfileTunerStrategy(
      CurtinTalonSRX *esc_left, CurtinTalonSRX *esc_right,
      AHRS *ahrs, double enc_ticks_per_rev, double wheel_diameter
    ) : _escl(esc_left), _escr(esc_right), _ahrs(ahrs), _tpr(enc_ticks_per_rev), _wd(wheel_diameter * 0.0254) {
      _outfile.open("/home/lvuser/mp_tuner.csv");
      _outfile_accel.open("/home/lvuser/mp_tuner_accel.csv");
    }

    void start() override;
    void tick(double time) override;
    void stop() override;

  private:
    CurtinTalonSRX *_escl, *_escr;
    AHRS *_ahrs;
    double _tpr, _wd;

    std::ofstream _outfile, _outfile_accel;
    double _ahrs_initial, _throttle, _trackwidth, _last_time, _set_time, _last_vel;
    int _state;
  };

} // namespace curtinfrc