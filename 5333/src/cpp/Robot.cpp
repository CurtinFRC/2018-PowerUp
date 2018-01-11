#include "WPILib.h"
#include "DriveBase.h"
#include "IO.h"
#include "Lift.h"
#include <iostream>

using namespace frc; // WPILib classes/functions

class Robot : public IterativeRobot {
public:
  DriveControl *drive;
  LiftControl *lift;

  Robot() { }

  void RobotInit() {
    IO::get_instance();
    drive = new DriveControl();
    lift = new LiftControl();
  }

  void AutonomousInit() { }
  void AutonomousPeriodic() { }

  void TeleopInit() { }
  void TeleopPeriodic() {
    lift->send_to_robot(
      IO::get_instance()->xbox->GetTriggerAxis(XboxController::JoystickHand::kRightHand)
      - IO::get_instance()->xbox->GetTriggerAxis(XboxController::JoystickHand::kRightHand)
    );
  }

  void TestInit() { }
  void TestPeriodic() { }
};

START_ROBOT_CLASS(Robot)
