#include "curtinfrc/math.h"
#include "curtinfrc/drivetrain.h" // Shared drivetrain in commons
#include "curtinfrc/strategy/mp_strategy.h"
#include "WPILib.h"
// #include <pathfinder.h>

#include "IO.h"
#include "Belev.h"
#include "Map.h"
#include "Intake.h"
#include "ControlMap.h"
#include "Auto.h"
#include "DriveStarategy.h"

#include <string>
#include <SmartDashboard/SmartDashboard.h>
#include <iostream>
#include <cmath>

using namespace frc; // WPILib classes/functions
using namespace std;

class Robot : public TimedRobot {
public:
  Drivetrain<2> *drive;

  AutoControl *auto_;

  BelevatorControl *belev;
  ClawControl *claw;
  IntakeControl *intake;

  IO *io;

  Robot() { }

  void RobotInit() {
    io = IO::get_instance(); // Refer to IO

    auto_ = new AutoControl();

    drive = new Drivetrain<2>(io->left_motors, io->right_motors);
    belev = new BelevatorControl();
    intake = new IntakeControl();
    claw = new ClawControl();
  }

  void AutonomousInit() {
    cout << "Auto Init" << endl;
    auto io = IO::get_instance();
    io->navx->ZeroYaw();// Note: wheelbase width: 0.72
    MotionProfileConfig cfg = {
      1440, 6,                                // enc_ticks, wheel_diam
      12.0 / 0.2, 0,                             // kp (1 / full_speed_threshold_distance), kd
      3.34, 0.76,                 // kv, ka
      3 * (1.0/80.0),
      curtinfrc::MotionProfileMode::PATHFINDER
    };
    auto strat = make_shared<curtinfrc::MotionProfileStrategy>(
      io->left_motors[0], io->right_motors[0],
      io->navx,
      "/home/lvuser/paths/test_left.csv", "/home/lvuser/paths/test_right.csv",
      cfg
    );
    drive->strategy_controller().set_active(strat);
  }
  void AutonomousPeriodic() {
    drive->strategy_controller().periodic();
  }

  void TeleopInit() {
    cout << "Teleop Init" << endl;
    ControlMap::init();

    auto strat = make_shared<DriveStarategy>(drive);
    drive->strategy_controller().set_active(strat);
  }
  void TeleopPeriodic() {
    drive->strategy_controller().periodic();

    belev->send_to_robot(ControlMap::belevator_motor_power());

    intake->send_to_robot(ControlMap::claw_state());
    // 14 changes to 5 cylinders reduce upstream from 120 to 60
  }

  void TestInit() {
    auto io = IO::get_instance();
    auto strat = std::make_shared<curtinfrc::MotionProfileTunerStrategy>(
      io->left_motors[0], io->right_motors[0],
      io->navx, 1440, 6
    );
    drive->strategy_controller().set_active(strat);
  }

  void TestPeriodic() {
    drive->strategy_controller().periodic();
  }
};

START_ROBOT_CLASS(Robot)
