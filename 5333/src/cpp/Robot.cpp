#include "curtinfrc/math.h"
#include "curtinfrc/drivetrain.h" // Shared drivetrain in commons
#include "WPILib.h"
#include "IO.h"
#include "Lift.h"
// #include "Claw.h"
// #include "Intake.h"
#include <iostream>

using namespace frc; // WPILib classes/functions
using namespace std;

class Robot : public IterativeRobot {
public:
  Drivetrain<2> *drive;
  double throttle;
  bool toggle_left_bumper, toggle_right_bumper;

  LiftControl *lift;
  // ClawControl *claw;
  // IntakeControl *intake;

  IO *io;

  Robot() { }

  void RobotInit() {
    io = IO::get_instance(); // Refer to IO

    drive = new Drivetrain<2>(io->left_motors, io->right_motors);
    lift = new LiftControl();
    // intake = new IntakeControl();
    // claw = new ClawControl();

    throttle = 0.6;
    toggle_left_bumper = toggle_right_bumper = false;
  }

  void AutonomousInit() { }
  void AutonomousPeriodic() { }

  void TeleopInit() { }
  void TeleopPeriodic() {
    double output_left = math::square_keep_sign(io->get_left_y());
    double output_right = math::square_keep_sign(io->get_right_y());
    drive->set_left(output_left * throttle);
    drive->set_right(output_right * throttle);

    lift->send_to_robot(io->get_right_trigger() - io->get_left_trigger()); // Right controls up, left controls down
    // claw->send_to_robot(io->get_());
    // intake->send_to_robot(io->get_right_bumper());

    // Throttle Control
    if(toggle_left_bumper != io->get_left_bumper()) { // Prevent registering as multiple presses
      toggle_left_bumper = io->get_left_bumper();
      if(toggle_left_bumper) { // Left bumper decreases throttle, while right increases throttle
        throttle -= 0.1;
        throttle = max(throttle, 0.1);
        cout << "Throttle changed to " << throttle << endl;
      }
    } else if(toggle_right_bumper != io->get_right_bumper()) {
      toggle_right_bumper = io->get_right_bumper();
      if(toggle_right_bumper) {
        throttle += 0.1;
        throttle = min(throttle, 1.0);
        cout << "Throttle changed to " << throttle << endl;
      }
    }
  }

  void TestInit() { }
  void TestPeriodic() { }
};

START_ROBOT_CLASS(Robot)
