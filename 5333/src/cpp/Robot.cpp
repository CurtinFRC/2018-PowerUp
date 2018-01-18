#include "curtinfrc/math.h"
#include "curtinfrc/drivetrain.h" // Shared drivetrain in commons
#include "WPILib.h"
// #include <pathfinder.h>

#include "IO.h"
#include "Belev.h"
#include "Map.h"
#include "Claw.h"
#include "Intake.h"
#include "ControlMap.h"
// #include "Auto.h"

#include <string>
#include <SmartDashboard/SmartDashboard.h>
#include <iostream>

using namespace frc; // WPILib classes/functions
using namespace std;

class Robot : public IterativeRobot {
public:
  Drivetrain<2> *drive;
  double throttle;
  bool left_bumper_toggle, right_bumper_toggle;

  // AutoControl *auto_;

  BelevatorControl *belev;
  ClawControl *claw;
  IntakeControl *intake;

  IO *io;

  Robot() { }

  void RobotInit() {
    io = IO::get_instance(); // Refer to IO

    drive = new Drivetrain<2>(io->left_motors, io->right_motors);
    belev = new BelevatorControl();
    intake = new IntakeControl();
    claw = new ClawControl();

    throttle = 0.6;
    left_bumper_toggle = right_bumper_toggle = false;
  }

  void AutonomousInit() { }
  void AutonomousPeriodic() {
    drive->set_left(0);
    drive->set_right(0);
  }

  void TeleopInit() {
    SmartDashboard::PutString("Test:", "A");
  }
  void TeleopPeriodic() {
    // Only move if joystick is not in deadzone
    if(abs(ControlMap::left_drive_power()) > Map::Controllers::deadzone) {
      double output_left = math::square_keep_sign(ControlMap::left_drive_power());
      drive->set_left(output_left * throttle);
    } else {
      drive->set_left(0);
    }

    if(abs(ControlMap::right_drive_power()) > Map::Controllers::deadzone) {
      double output_right = math::square_keep_sign(ControlMap::right_drive_power());
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
        throttle = max(throttle, 0.1);
        cout << "Throttle changed to " << throttle << endl;
      }
    } else if (right_bumper_toggle != ControlMap::throttle_increment()) {
      right_bumper_toggle = ControlMap::throttle_increment();
      if (right_bumper_toggle) {
        throttle += 0.1;
        throttle = min(throttle, 1.0);
        cout << "Throttle changed to " << throttle << endl;
      }
    }

    claw->send_to_robot(
      ControlMap::claw_state() ? DoubleSolenoid::Value::kForward : DoubleSolenoid::Value::kReverse
    ); // Note: The claw solenoid is reversed
    // 14 changes to 5 cylinders reduce upstream from 120 to 60
  }

  void TestInit() { }
  void TestPeriodic() { }
};

START_ROBOT_CLASS(Robot)
