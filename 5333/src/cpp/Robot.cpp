#include "WPILib.h"
#include "curtinfrc/math.h"
#include "DriveBase.h"
#include "IO.h"
#include "Arm.h"
#include <iostream>

using namespace frc; // WPILib classes/functions

class Robot : public IterativeRobot {
public:
  DriveControl *drive;
  ArmControl *arm;

  Robot() { }

  void RobotInit() {
    IO::get_instance();
    drive = new DriveControl();
    arm = new ArmControl();
  }

  void AutonomousInit() { }
  void AutonomousPeriodic() { }

  void TeleopInit() { }
  void TeleopPeriodic() {
    arm->send_to_robot(
      IO::get_instance()->xbox->GetTriggerAxis(XboxController::JoystickHand::kRightHand)
      - IO::get_instance()->xbox->GetTriggerAxis(XboxController::JoystickHand::kRightHand)
    );
  }

  void TestInit() { }
  void TestPeriodic() { }
};

START_ROBOT_CLASS(Robot)
