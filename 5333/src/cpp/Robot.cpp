// Shared headers
#include "curtinfrc/math.h"
#include "curtinfrc/drivetrain.h"
#include "curtinfrc/strategy/mp_strategy.h"
#include "WPILib.h"

// Robot part classes
#include "IO.h"
#include "Belev.h"
#include "Map.h"
#include "Intake.h"
#include "Winch.h"
#include "ControlMap.h"
#include "DriveStarategy.h"

// Other required libraries
#include <string>
#include <SmartDashboard/SmartDashboard.h>
#include <iostream>
#include <stdint.h>
#include <cmath>

using namespace frc; // WPILib classes/functions
using namespace std;

class Robot : public TimedRobot {
public:
  Drivetrain *drive;

  BelevatorControl *belev;
  WinchControl *winch;
  IntakeControl *intake;

  IO *io;

  Robot() { }

  void RobotInit() {
    io = IO::get_instance(); // Refer to IO

    drive = new Drivetrain(io->left_motors[0], io->right_motors[0], io->left_motors[0], io->right_motors[0]);
    belev = new BelevatorControl();
    intake = new IntakeControl();
    winch = new WinchControl();
  }

  void AutonomousInit() {
    cout << "Auto Init" << endl;
    auto io = IO::get_instance();
    io->navx->ZeroYaw();

    // Note: wheelbase width: 0.72
    MotionProfileConfig mcfg = {
      1440, 6,                  // enc ticks, wheel diameter inches
      1.0 / 0.2, 0, 0,          // P, I, D
      3.34 / 12.0, 0.76 / 12.0  // kV, kA
    };
    double kt = 3 * (1.0 / 80.0);

    auto mode_left = std::make_shared<PathfinderMPMode>(
      io->left_motors[0], mcfg, "/home/lvuser/paths/test_left.csv"
    );
    auto mode_right = std::make_shared<PathfinderMPMode>(
      io->right_motors[0], mcfg, "/home/lvuser/paths/test_right.csv"
    );
    auto strat = std::make_shared<DrivetrainMotionProfileStrategy>(
      mode_left, mode_right, drive,
      io->navx, kt
    );
    drive->strategy_controller().set_active(strat);
  }
  void AutonomousPeriodic() {
    drive->strategy_controller().periodic();
    drive->log_write(); // Make this bit call only on mutates later *
    belev->log_write();
    intake->log_write();
    //winch->log_write();
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

    intake->send_to_robot(ControlMap::intake_claw_state());

    winch->send_to_robot(ControlMap::winch_power());
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
