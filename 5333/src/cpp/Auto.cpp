#include "AHRS.h"
#include "Auto.h"
#include "Map.h"
#include "IO.h"
#include <pathfinder.h>

using namespace curtinfrc;

int AutoControl::init() {
  segment_length = Pathfinder::pathfinder_load_file("/home/lvuser/paths/test_left.csv", &segments_left[0]);
  Pathfinder::pathfinder_load_file("/home/lvuser/paths/test_right.csv", &segments_right[0]);

  leftfollower.last_error = 0; leftfollower.segment = 0; leftfollower.finished = 0;
  rightfollower.last_error = 0; rightfollower.segment = 0; rightfollower.finished = 0;
  config = { 0, Map::Robot::encoder_ticks_per_rot, Map::Robot::wheel_circ, Map::PID::Navx::kp, Map::PID::Navx::ki, Map::PID::Navx::kd, 1.0 / Map::Robot::max_v, 0.0};

  return 0;
}

void AutoControl::tick() {
  CurtinTalonSRX *left_motor = IO::get_instance()->left_motors[0], *right_motor = IO::get_instance()->right_motors[0];

  double turn = 0.8 * (-1.0/80.0) * (fmod(r2d(leftfollower.heading) - IO::get_instance()->navx->GetAngle() + 180, 360) - 180);

  left_motor->Set(pathfinder_follow_encoder(config, &leftfollower, segments_left, segment_length, left_motor->GetSelectedSensorPosition(0)) + turn);
  right_motor->Set(pathfinder_follow_encoder(config, &rightfollower, segments_right, segment_length, right_motor->GetSelectedSensorPosition(0)) - turn);
}
