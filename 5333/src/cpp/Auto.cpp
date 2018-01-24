#include "Auto.h"
#include "Map.h"
#include "IO.h"
#include <pathfinder.h>

using namespace curtinfrc;

int AutoControl::init() {
  CurtinTalonSRX *left_motor = IO::get_instance()->left_motors[0], *right_motor = IO::get_instance()->right_motors[0];

  segment_length = Pathfinder::pathfinder_load_file("/home/lvuser/paths/test_left.csv", &segments_left[0]);
  Pathfinder::pathfinder_load_file("/home/lvuser/paths/test_right.csv", &segments_right[0]);

  left_motor->reset_mp();
  left_motor->SetIntegralAccumulator(0, 0, 0);
  left_motor->configure_pidf(_kp, _ki, _kd, _kf);
  left_motor->configure_encoder_edges_per_rev(Map::Robot::encoder_ticks_per_rot);
  left_motor->configure_wheel_diameter(Map::Robot::wheel_diameter);
  left_motor->configure_mp_update_rate(20);
  left_motor->load_pathfinder(segments_left, segment_length);

  right_motor->reset_mp();
  right_motor->SetIntegralAccumulator(0, 0, 0);
  right_motor->configure_pidf(_kp, _ki, _kd, _kf);
  right_motor->configure_encoder_edges_per_rev(Map::Robot::encoder_ticks_per_rot);
  right_motor->configure_wheel_diameter(Map::Robot::wheel_diameter);
  right_motor->configure_mp_update_rate(20);
  right_motor->load_pathfinder(segments_right, segment_length);

  return 0;
}

void AutoControl::tick() {
  CurtinTalonSRX *left_motor = IO::get_instance()->left_motors[0], *right_motor = IO::get_instance()->right_motors[0];

  left_motor->load_pathfinder(segments_left, segment_length);
  right_motor->load_pathfinder(segments_right, segment_length);

  CurtinTalonSRX::MotionProfileStatus status_left = left_motor->process_mp(), status_right = right_motor->process_mp();

  if (status_left.isLast)   left_motor->hold_mp();
  else                      left_motor->enable_mp();

  if (status_right.isLast)  right_motor->hold_mp();
  else                      right_motor->enable_mp();
}
