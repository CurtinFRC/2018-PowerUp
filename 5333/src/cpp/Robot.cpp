#include "curtinfrc/math.h"
#include "curtinfrc/drivetrain.h" // Shared drivetrain in commons
#include "curtinfrc/strategy/mp_strategy.h"
#include "WPILib.h"
// #include <pathfinder.h>

#include "IO.h"
#include "Belev.h"
#include "Map.h"
#include "Claw.h"
#include "Intake.h"
#include "ControlMap.h"
#include "Auto.h"

#include <string>
#include <SmartDashboard/SmartDashboard.h>
#include <iostream>
#include <cmath>

using namespace frc; // WPILib classes/functions
using namespace std;

class Robot : public TimedRobot {
public:
  Drivetrain<2> *drive;
  double throttle;
  bool left_bumper_toggle, right_bumper_toggle;

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

    throttle = 0.6;
    left_bumper_toggle = right_bumper_toggle = false;
  }

  void AutonomousInit() {
    std::cout << "Auto Init" << std::endl;
    auto io = IO::get_instance();
    io->navx->ZeroYaw();
    // Note: wheelbase width: 0.72
    MotionProfileConfig cfg = {
      1440, 6,                                  // enc_ticks, wheel_diam
      1 / 0.2, 0.2,                             // kp (1 / full_speed_threshold_distance), kd
      3.34 / 12.0, 0.911 / 12,                  // kv, ka
      curtinfrc::MotionProfileMode::PATHFINDER
    };
    auto strat = std::make_shared<curtinfrc::MotionProfileStrategy>(
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
