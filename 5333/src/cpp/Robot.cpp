#include "curtinfrc/math.h"
#include "curtinfrc/drivetrain.h" // Shared drivetrain in commons
#include "WPILib.h"
#include "IO.h"
#include "Belev.h"
#include "Map.h"
#include "Claw.h"
#include "Intake.h"

#include <string>
#include <SmartDashboard/SmartDashboard.h>
#include <iostream>

using namespace frc; // WPILib classes/functions
using namespace std;

class Robot : public IterativeRobot {
public:
  Drivetrain<2> *drive;
  double deadzone = 0.04; // Prevents robot from moving within this zone
  double throttle;
  bool left_bumper_toggle, right_bumper_toggle;

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
  void AutonomousPeriodic() { }

  void TeleopInit() {
    SmartDashboard::PutString("Test:", "A");
  }
  void TeleopPeriodic() {
    // Only move if joystick is not in deadzone
    if(abs(io->get_left_Y()) > deadzone) {
      double output_left = math::square_keep_sign(io->get_left_Y());
      drive->set_left(output_left * throttle);
    } else {
      drive->set_left(0);
    }

    if(abs(io->get_right_Y()) > deadzone) {
      double output_right = math::square_keep_sign(io->get_right_Y());
      drive->set_right(output_right * throttle);
    } else {
      drive->set_right(0);
    }

    belev->send_to_robot(io->get_right_trigger() - io->get_left_trigger()); // Right controls up, left controls down
    // claw->send_to_robot(io->get_());
    // intake->send_to_robot(io->get_right_bumper());

    // Throttle Control
    if (left_bumper_toggle != io->get_left_bumper()) { // Prevent registering as multiple presses
      left_bumper_toggle = io->get_left_bumper();
      if (left_bumper_toggle) { // Left bumper decreases throttle, while right increases throttle
        throttle -= 0.1;
        throttle = max(throttle, 0.1);
        cout << "Throttle changed to " << throttle << endl;
      }
    } else if (right_bumper_toggle != io->get_right_bumper()) {
      right_bumper_toggle = io->get_right_bumper();
      if (right_bumper_toggle) {
        throttle += 0.1;
        throttle = min(throttle, 1.0);
        cout << "Throttle changed to " << throttle << endl;
      }
    }

    claw->send_to_robot(
      io->get_left_stick() || io->get_right_stick() ? DoubleSolenoid::Value::kForward : DoubleSolenoid::Value::kReverse
    ); // Note: The claw solenoid is reversed
    // 14 changes to 5 cylinders reduce upstream from 120 to 60
  }

  void TestInit() { }
  void TestPeriodic() { }
};

START_ROBOT_CLASS(Robot)
