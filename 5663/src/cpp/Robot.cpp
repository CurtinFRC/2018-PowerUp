#include "WPILib.h"
#include "AHRS.h"
#include "curtinfrc/math.h"
#include <iostream>
#include <string>
#include <SmartDashboard/SmartDashboard.h>
#include <PowerDistributionPanel.h>
#include <PIDOutput.h>

#include "components/Drive.h"
#include "components/Lift.h"
#include "components/Ramp.h"
#include "components/Manipulator.h"

using namespace frc;
using namespace curtinfrc;
using namespace std;
using namespace components;

class Robot : public IterativeRobot {
  XboxController *xbox, *xbox2;
  PowerDistributionPanel *pdp;
  SendableChooser<int*> *AutoChooser; // Choose auto mode
  SendableChooser<int*> *ControlModeChooser; // Choose control mode
  Drive *drive;
  Lift *lift;
  Ramp *ramp;
  Manipulator *man;
  Compressor *compressor;
public:
  string gameData;
  int Auto;

  void RobotInit() {
    xbox = new XboxController(0);
    xbox2 = new XboxController(1);

    pdp = new PowerDistributionPanel(0);

    AutoChooser = new SendableChooser<int*>;
    AutoChooser->AddDefault("Cross Baseline",(int*) 0);
    AutoChooser->AddObject("Auto 1",(int*) 1);
    AutoChooser->AddObject("Auto 2",(int*) 2);

    ControlModeChooser = new SendableChooser<int*>;
    ControlModeChooser->AddDefault("Dual",(int*) 0);
    ControlModeChooser->AddObject("Single (Debug)",(int*) 1);

    drive = new Drive(1, 2, 3, 4, 5, 6);
    lift = new Lift(7, 8);
    ramp = new Ramp(2, 3);
    man = new Manipulator(9, 0, 1);

    compressor = new Compressor(0);
    compressor->SetClosedLoopControl(true);
  }

  void AutonomousInit() {
    drive->SetSlowGear();
    gameData = DriverStation::GetInstance().GetGameSpecificMessage(); //Get specific match data
    SmartDashboard::PutString("Alliance Switch:", &gameData[0]);
    SmartDashboard::PutString("Scale:", &gameData[1]);
    SmartDashboard::PutString("Enemy Switch:", &gameData[2]);  //Put data on shuffleboard
    Auto = (int) AutoChooser->GetSelected(); //What auto mode you wanna do
  }

  void AutonomousPeriodic() {
    // gameData will be an array with 3 characters, eg. "LRL"
    // check https://wpilib.screenstepslive.com/s/currentCS/m/getting_started/l/826278-2018-game-data-details
    lift->SetSpeed(0);
  }

  void TeleopInit() {
    drive->SetFastGear();
  }

  void TeleopPeriodic() {

//———[controller 1]—————————————————————————————————————————————————————————————
  //———[drivetrain]—————————————————————————————————————————————————————————————
    drive->TankDrive(xbox->GetY(xbox->kLeftHand), xbox->GetY(xbox->kRightHand));
    if(xbox->GetYButtonPressed()) {
      drive->ToggleGear();
    }

//———[controller 2]—————————————————————————————————————————————————————————————
  //———[lift]———————————————————————————————————————————————————————————————————
    if(xbox2->GetAButton()) {
      lift->SetLowPosition();
    } else if(xbox2->GetBButton()) {
      lift->SetMidPosition();
    } else if(xbox2->GetYButton()) {
      lift->SetHighPosition();
    } else if(xbox2->GetXButton()) {
      lift->ResetEncoder();
    }
    lift->SetSpeed(xbox2->GetY(xbox2->kRightHand));

  //———[manipulator]————————————————————————————————————————————————————————————
    if(xbox2->GetBumper(xbox2->kLeftHand)) {
      man->Release();
    } else {
      man->Restrain();
    }

    man->SetIntakeSpeed(xbox2->GetY(xbox2->kLeftHand));

  //———[ramp]———————————————————————————————————————————————————————————————————
    if(xbox->GetBumper(xbox->kLeftHand) && xbox->GetBumper(xbox->kRightHand) && xbox2->GetBumper(xbox2->kLeftHand) && xbox2->GetBumper(xbox2->kRightHand)) {
      ramp->ConfirmIntentionalDeployment();
    }

  //———[periodic]———————————————————————————————————————————————————————————————————
    drive->RunPeriodic();
    lift->RunPeriodic();
    man->RunPeriodic();
  }
};

START_ROBOT_CLASS(Robot)
