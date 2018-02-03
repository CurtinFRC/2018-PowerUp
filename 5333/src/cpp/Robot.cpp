// Shared headers
#include "curtinfrc/math.h"
#include "curtinfrc/drivetrain.h" // Shared drivetrain
#include "curtinfrc/strategy/mp_strategy.h"
#include "WPILib.h"
// #include <pathfinder.h>

// Robot part classes
#include "IO.h"
#include "Belev.h"
#include "Map.h"
#include "Claw.h"
#include "Intake.h"
#include "ControlMap.h"

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
  double throttle;
  bool left_bumper_toggle, right_bumper_toggle;

  BelevatorControl *belev;
  ClawControl *claw;
  IntakeControl *intake;

  IO *io;

  Robot() { }

  void RobotInit() {
    io = IO::get_instance(); // Refer to IO

    drive = new Drivetrain(io->left_motors[0], io->right_motors[0], io->left_motors[0], io->right_motors[0]);
    belev = new BelevatorControl();
    claw = new ClawControl();
    intake = new IntakeControl();

    throttle = 0.6;
    left_bumper_toggle = right_bumper_toggle = false;
  }

  void AutonomousInit() {
    std::cout << "Auto Init" << std::endl;
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
    claw->log_write();
    intake->log_write();
  }

  void TeleopInit() {
    SmartDashboard::PutNumber("Throttle:", throttle);
    ControlMap::init();
    drive->strategy_controller().set_active(nullptr);
  }
  void TeleopPeriodic() {
    // Only move if joystick is not in deadzone
    if(fabs(ControlMap::left_drive_power()) > Map::Controllers::deadzone) {
      // double output_left = math::square_keep_sign(ControlMap::left_drive_power());
      double output_left = ControlMap::left_drive_power();
      drive->set_left(output_left * throttle);
    } else {
      drive->set_left(0);
    }

    if(fabs(ControlMap::right_drive_power()) > Map::Controllers::deadzone) {
      // double output_right = math::square_keep_sign(ControlMap::right_drive_power());
      double output_right = ControlMap::right_drive_power();
      drive->set_right(output_right * throttle);
    } else {
      drive->set_right(0);
    }

    belev->send_to_robot(ControlMap::belevator_motor_power()); // Right controls up, left controls down
    // claw->send_to_robot(io->get_());
    // intake->send_to_robot(io->get_right_bumper());

    // Throttle Control
    if (left_bumper_toggle != ControlMap::throttle_decrement()) { // Prevent registering as multiple presses
      left_bumper_toggle = ControlMap::throttle_decrement();
      if (left_bumper_toggle) { // Left bumper decreases throttle, while right increases throttle
        throttle -= 0.1;
        throttle = round(max(throttle, 0.1) * 10) / 10;
        cout << "Throttle changed to " << throttle << endl;
        SmartDashboard::PutNumber("Throttle:", throttle);
      }
    } else if (right_bumper_toggle != ControlMap::throttle_increment()) {
      right_bumper_toggle = ControlMap::throttle_increment();
      if (right_bumper_toggle) {
        throttle += 0.1;
        throttle = round(min(throttle, 1.0) * 10) / 10;
        cout << "Throttle changed to " << throttle << endl;
        SmartDashboard::PutNumber("Throttle:", throttle);
      }
    }

    claw->send_to_robot(
      ControlMap::claw_state() ? DoubleSolenoid::Value::kForward : DoubleSolenoid::Value::kReverse
    ); // Note: The claw solenoid is reversed
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
