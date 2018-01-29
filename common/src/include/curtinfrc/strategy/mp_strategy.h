#pragma once

#include "curtinfrc/motors/CurtinTalonSRX.h"
#include "AHRS.h"
#include "curtinfrc/strategy/strategy.h"

#include "WPILib.h"
#include <fstream>

namespace curtinfrc {

  enum class MotionProfileMode {
    TALON_MP, PATHFINDER
  };

  struct MotionProfileConfig {
    double enc_ticks_per_rev, wheel_diameter, kp, kd, kv, ka;  // note: ka unused on MotionProfileMode::TALON_MP
    MotionProfileMode mode;
  };

  class MotionProfileStrategy : public Strategy {
  public:
    // Note: AHRS can be nullptr for MotionProfileMode::TALON_MP
    MotionProfileStrategy(
      CurtinTalonSRX *esc_left, CurtinTalonSRX *esc_right, 
      AHRS *ahrs, 
      const char *file_left, const char *file_right, 
      MotionProfileConfig config
    ) : _escl(esc_left), _escr(esc_right), _ahrs(ahrs), _cfg(config) {
      _segments_length = Pathfinder::pathfinder_load_file(file_left, &_segments_left[0]);
      Pathfinder::pathfinder_load_file(file_right, &_segments_right[0]);
      _followl = _followr = { 0, 0, 0, 0, 0 };
    }

    void start() override;
    void tick(double time) override;
    void tick_talonmp();
    void tick_pathfinder();
    void stop() override;

  private:
    CurtinTalonSRX *_escl, *_escr;
    AHRS *_ahrs;
    Segment _segments_left[8192], _segments_right[8192];
    int _segments_length;
    MotionProfileConfig _cfg;

    EncoderFollower _followl, _followr;
    EncoderConfig _ecfg_l, _ecfg_r;

    frc::Notifier *_notifier;
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