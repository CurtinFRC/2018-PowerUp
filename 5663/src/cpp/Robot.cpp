#include "WPILib.h"
#include "AHRS.h"
#include "curtinfrc/math.h"
#include <iostream>
#include <string>
#include <SmartDashboard/SmartDashboard.h>
#include <PowerDistributionPanel.h>
#include "ctre/Phoenix.h"
#include <PIDOutput.h>
#include "components/Drive.h"

using namespace frc;
using namespace curtinfrc;
using namespace std;
using namespace components;

class Robot : public IterativeRobot {
  XboxController *xbox;
  PowerDistributionPanel *pdp;
  SendableChooser<int*> *AutoChooser; // Choose auto mode
// Solenoids for pneumatics, I'm just using them for ball shifters atm;
  // AHRS *ahrs;
  PIDController *turn;
  Drive *drive;
public:
  // Configuration settings: (maybe make a sperate file for these?)
  double deadzone = 0.04; //Stop the robot being a sneaky snail
  // Regular variables
  string gameData;
  int Auto, gearMode;
  void RobotInit() {
    xbox = new XboxController(0);
    pdp = new PowerDistributionPanel(0);
    AutoChooser = new SendableChooser<int*>;

    AutoChooser->AddDefault("Cross Baseline",(int*) 0);
    AutoChooser->AddObject("Auto 1",(int*) 1);
    AutoChooser->AddObject("Auto 2",(int*) 2);

    drive = new Drive(1, 2, 3, 4, 5, 6);
  }

  void AutonomousInit() {
    gameData = DriverStation::GetInstance().GetGameSpecificMessage(); //Get specific match data
    SmartDashboard::PutString("Alliance Switch:", &gameData[0]);
    SmartDashboard::PutString("Scale:", &gameData[1]);
    SmartDashboard::PutString("Enemy Switch:", &gameData[2]);  //Put data on shuffleboard
    Auto = (int) AutoChooser->GetSelected(); //What auto mode you wanna do
  }
  void AutonomousPeriodic() {
    // gameData will be an array with 3 characters, eg. "LRL"
    // check https://wpilib.screenstepslive.com/s/currentCS/m/getting_started/l/826278-2018-game-data-details
    //SetSlowGear();
  }

  void TeleopInit() {
    drive->SetFastGear();
  }
  void TeleopPeriodic() {
    if(xbox->GetYButtonPressed()) {
      drive->ToggleGear();
    }
    drive->RunPeriodic();
  }

  void TestInit() {

  }

  void TestPeriodic() {
  }
};

START_ROBOT_CLASS(Robot)
