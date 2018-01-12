#include "curtinfrc/math.h"
#include "curtinfrc/drivetrain.h" // Shared drivetrain in commons
#include "WPILib.h"
#include "IO.h"
#include "Lift.h"
// #include "Claw.h"
// #include "Intake.h"
#include <iostream>

using namespace frc; // WPILib classes/functions

class Robot : public IterativeRobot {
public:
  Drivetrain<2> *drive;
  LiftControl *lift;
  // ClawControl *claw;
  // IntakeControl *intake;

  IO *io;

  Robot() { }

  void RobotInit() {
    drive = new Drivetrain<2>(io->left_motors, io->right_motors);
    lift = new LiftControl();
    // intake = new IntakeControl();
    // claw = new ClawControl();
    io = IO::get_instance(); // Refer to IO
  }

  void AutonomousInit() { }
  void AutonomousPeriodic() { }

  void TeleopInit() { }
  void TeleopPeriodic() {
    double output_left = math::square_keep_sign(io->get_left_y());
    double output_right = math::square_keep_sign(io->get_right_y());
    drive->set_left(output_left);
    drive->set_right(output_right);

    lift->send_to_robot(io->get_right_trigger() - io->get_left_trigger()); // Right controls up, left controls down
    // claw->send_to_robot(io->get_());
    // intake->send_to_robot(io->get_left_bumper());
  }

  void TestInit() { }
  void TestPeriodic() { }
};

START_ROBOT_CLASS(Robot)
