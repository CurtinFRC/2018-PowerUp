#include "WPILib.h"
#include "curtinfrc/math.h"
#include "DriveBase.h"
#include "IO.h"
#include <iostream>

using namespace frc; // WPILib classes/functions

class Robot : public IterativeRobot {
public:
  DriveControl *drive;

  Robot() { }

  void RobotInit() {
    IO::get_instance();
    drive = new DriveControl();
  }

  void AutonomousInit() { }
  void AutonomousPeriodic() { }

  void TeleopInit() { }
  void TeleopPeriodic() { }

  void TestInit() { }
  void TestPeriodic() { }
};

START_ROBOT_CLASS(Robot)
