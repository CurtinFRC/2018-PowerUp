#include "curtinfrc/math.h"
#include "WPILib.h"
#include "DriveBase.h"
#include "IO.h"
#include "Lift.h"
#include "Claw.h"
#include "Intake.h"
#include <iostream>

using namespace frc; // WPILib classes/functions

class Robot : public IterativeRobot {
public:
  DriveControl *drive;
  LiftControl *lift;
  ClawControl *claw;
  IntakeControl *intake;

  Robot() { }

  void RobotInit() {
    IO::get_instance();
    drive = new DriveControl();
    lift = new LiftControl();
    intake = new IntakeControl();
  }

  void AutonomousInit() { }
  void AutonomousPeriodic() { }

  void TeleopInit() { }
  void TeleopPeriodic() {
    double output_left = math::square_keep_sign(IO::get_left_y());
    double output_right = math::square_keep_sign(IO::get_right_y());
    drive->set_left(output_left);
    drive->set_right(output_right);

    lift->send_to_robot(IO::get_left_trigger() - IO::get_right_trigger());
  }

  void TestInit() { }
  void TestPeriodic() { }
};

START_ROBOT_CLASS(Robot)
