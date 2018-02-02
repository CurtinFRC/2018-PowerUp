// Shared headers
#include "curtinfrc/math.h"
#include "curtinfrc/drivetrain.h" // Shared drivetrain
#include "curtinfrc/logger.h" // To log activity
#include "curtinfrc/vision/vision.h"
#include "WPILib.h"
// #include <pathfinder.h>

// Robot part classes
#include "IO.h"
#include "Belev.h"
#include "Map.h"
#include "Claw.h"
#include "Intake.h"
#include "ControlMap.h"
#include "Auto.h"

// Other required libraries
#include <string>
#include <SmartDashboard/SmartDashboard.h>
#include <iostream>
#include <stdint.h>

using namespace frc; // WPILib classes/functions
using namespace std;

class Robot : public IterativeRobot {
public:
  Drivetrain<2> *drive;
  curtinfrc::VisionSystem *vision;
  double throttle;
  bool left_bumper_toggle, right_bumper_toggle;
  uint64_t time_fpga; // divide it to milliseconds, then cast as double
  vector<string> drive_states(2);

  AutoControl *auto_;

  BelevatorControl *belev;
  ClawControl *claw;
  IntakeControl *intake;

  LogControl *log_drive;
  LogControl *log_belev;
  LogControl *log_claw;
  LogControl *log_intake;
  // Add logs for auto state?

  IO *io;

  Robot() { }

  void RobotInit() {
    io = IO::get_instance(); // Refer to IO

    auto_ = new AutoControl();

  	vision = new VisionSystem();
  	vision->start();

    drive = new Drivetrain<2>(io->left_motors, io->right_motors);
    belev = new BelevatorControl();
    intake = new IntakeControl();
    claw = new ClawControl();

    throttle = 0.6;
    left_bumper_toggle = right_bumper_toggle = false;

    // Create instances of Logger and pass header names
    string drive_header[2] = {"left_speed","right_speed"};
    Logger("log_drive", drive_header[]);
    string belev_header[1] = {"belev_motor"};
    Logger("log_belev", belev_header[]);
    string claw_header[2] = {"left_piston","right_piston"};
    Logger("log_claw", claw_header[]);
    string intake_header[2] = {"left_wheel","right_wheel"}; // Might both be set speeds with low torque
    Logger("log_intake", intake_header[]);

    string drive_states[2];
    string belev_states[1];
    string claw_states[2];
    string intake_states[2];
  }

  void AutonomousInit() {
    auto_->init();
  }
  void AutonomousPeriodic() {
    auto_->tick();

    time_fpga = ::frc::RobotController::GetFPGATime(); // Pass timestamp to each
    log_drive->log_write(time_fpga, drive_states);
    log_belev->log_write(time_fpga, belev_states);
    log_claw->log_write(time_fpga, claw_states);
    log_intake->log_write(time_fpga, intake_states);
  }

  void TeleopInit() {
    SmartDashboard::PutNumber("Throttle:", throttle);
    ControlMap::init();
  }
  void TeleopPeriodic() {
    // Only move if joystick is not in deadzone
    if(abs(ControlMap::left_drive_power()) > Map::Controllers::deadzone) {
      // double output_left = math::square_keep_sign(ControlMap::left_drive_power());
      double output_left = ControlMap::left_drive_power();
      drive->set_left(output_left * throttle);
    } else {
      drive->set_left(0);
    }

    if(abs(ControlMap::right_drive_power()) > Map::Controllers::deadzone) {
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

    // Send to logger
    time_fpga = ::frc::RobotController::GetFPGATime();
  }

  void TestInit() { }
  void TestPeriodic() { }
};

START_ROBOT_CLASS(Robot)
