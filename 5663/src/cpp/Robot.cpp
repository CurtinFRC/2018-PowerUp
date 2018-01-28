#include "WPILib.h"
#include "AHRS.h"
#include "curtinfrc/math.h"
#include <iostream>
#include <string>
#include <SmartDashboard/SmartDashboard.h>
#include <PowerDistributionPanel.h>
#include <PIDOutput.h>
#include <I2C.h>
#include <pathfinder.h>

#include "components/Drive.h"
#include "components/Lift.h"
#include "components/Ramp.h"
#include "components/Manipulator.h"
#include "components/Climber.h"

#include "autonomous/Autonomous.h"

using namespace frc;
using namespace curtinfrc;
using namespace std;
using namespace components;
using namespace cs;
using namespace autonomous;

class Robot : public IterativeRobot {
  UsbCamera camera;
  XboxController *xbox, *xbox2;
  PowerDistributionPanel *pdp;
  SendableChooser<int*> *AutoChooser; // Choose auto mode
  SendableChooser<int*> *StartingPosition; // Choose starting position
  SendableChooser<int*> *ControlModeChooser; // Choose control mode
  Drive *drive;
  Lift *lift;
  Ramp *ramp;
  Manipulator *man;
  Climber *climber;
  Compressor *compressor;
  Autonomous *auton;
  I2C *arduino;

public:
  int AutoStage;
  uint8_t message = 72;

  void RobotInit() {
    camera = CameraServer::GetInstance()->StartAutomaticCapture();
    camera.SetResolution(640, 480);

    xbox = new XboxController(0);
    xbox2 = new XboxController(1);

    pdp = new PowerDistributionPanel(0);

    AutoChooser = new SendableChooser<int*>;
    AutoChooser->AddDefault("Cross Baseline",(int*) 0);
    AutoChooser->AddObject("Single Switch",(int*) 1);
    AutoChooser->AddObject("Single Scale",(int*) 2);

    StartingPosition = new SendableChooser<int*>;
    StartingPosition->AddObject("Left (1)", (int*) 1);
    StartingPosition->AddDefault("Middle (2)", (int*) 2);
    StartingPosition->AddObject("Right (3)", (int*) 3);

    ControlModeChooser = new SendableChooser<int*>;
    ControlModeChooser->AddDefault("Dual",(int*) 0);
    ControlModeChooser->AddObject("Single (Debug)",(int*) 1);

    drive = new Drive(1, 2, 5, 8, 7, 6);
    lift = new Lift(3, 4);
    ramp = new Ramp(2, 3);
    man = new Manipulator(9, 0, 1);

    compressor = new Compressor(0);
    compressor->SetClosedLoopControl(true);

    auton = new Autonomous(*drive, *lift, *man, *ramp);

    arduino = new I2C(arduino->kOnboard, 100);

    arduino->WriteBulk(&message, 1);
  }

  void AutonomousInit() {


    drive->SetSlowGear();
    lift->SetLowPosition();
    auton->ChooseRoutine((int)AutoChooser->GetSelected(), (int)StartingPosition->GetSelected());
  }

  void AutonomousPeriodic() {
    message = 72;
    SmartDashboard::PutBoolean("transaction", arduino->Transaction(&message, 1, NULL, 0));
    auton->RunPeriodic();
  }

  void TeleopInit() {
    drive->SetFastGear();
  }

  void TeleopPeriodic() {
    message = 76;
   SmartDashboard::PutBoolean("transaction", arduino->Transaction(&message, 1, NULL, 0));
//———[controller 1]—————————————————————————————————————————————————————————————
  //———[drivetrain]—————————————————————————————————————————————————————————————
    drive->TankDrive(xbox->GetY(xbox->kRightHand), xbox->GetY(xbox->kLeftHand), true);
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

  //———[climber]————————————————————————————————————————————————————————————————


  //———[periodic]———————————————————————————————————————————————————————————————
    drive->RunPeriodic();
    lift->RunPeriodic();
    man->RunPeriodic();
  }
};

START_ROBOT_CLASS(Robot)
