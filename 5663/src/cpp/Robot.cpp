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
#include <Timer.h>
// #include <networktables/NetworkTable.h>
// #include <networktables/NetworkTableInstance.h>
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
using namespace nt;

class Robot : public IterativeRobot {
  UsbCamera camera;
  XboxController *xbox, *xbox2;
  Joystick *station;
  PowerDistributionPanel *pdp;
  SendableChooser<int*> *AutoChooser; // Choose auto mode
  SendableChooser<int*> *StartingPosition; // Choose starting position
  SendableChooser<int*> *AutoWait;
  Drive *drive;
  Lift *lift;
  Ramp *ramp;
  Manipulator *man;
  Climber *climber;
  Compressor *compressor;
  Autonomous *auton;
  I2C *arduino;
  Timer *timer;

public:
  uint8_t message = 72;
  double maxspeed = 1;
  bool pressedTurn = false;

  void RobotInit() {
    camera = CameraServer::GetInstance()->StartAutomaticCapture();
    camera.SetResolution(640, 480);

    xbox = new XboxController(0);
    xbox2 = new XboxController(1);
    station = new Joystick(2);

    pdp = new PowerDistributionPanel(0);

    AutoChooser = new SendableChooser<int*>;
    StartingPosition = new SendableChooser<int*>;
    AutoWait = new SendableChooser<int*>;

    drive = new Drive(1, 2, 3,  //left
                      6, 5, 4,  //right
                      0, 1);    //solenoid
    lift = new Lift(8, 7);
    ramp = new Ramp(4, 5, 6, 7); // CHECK THESE!!!
    man = new Manipulator(0, 2, 3);

    compressor = new Compressor(0);
    compressor->SetClosedLoopControl(true);

    auton = new Autonomous(drive, lift, man);

    arduino = new I2C(arduino->kOnboard, 100);
    arduino->WriteBulk(&message, 1);

    timer = new Timer(); timer->Start();

    AutoChooser->AddDefault("Cross Baseline",(int*) 0);
    AutoChooser->AddObject("Single Switch",(int*) 1);
    AutoChooser->AddObject("Single Scale",(int*) 2);
    //SmartDashboard::PutData("AutoChooser", AutoChooser);

    StartingPosition->AddObject("Left (1)", (int*) 1);
    StartingPosition->AddDefault("Middle (2)", (int*) 2);
    StartingPosition->AddObject("Right (3)", (int*) 3);
    SmartDashboard::PutData("StartingPosition", StartingPosition);

    AutoWait->AddDefault("0S",(int*) 0);
    AutoWait->AddObject("1S",(int*) 1);
    AutoWait->AddObject("2S",(int*) 2);
    AutoWait->AddObject("3S",(int*) 3);
    AutoWait->AddObject("4S",(int*) 4);
    AutoWait->AddObject("5S",(int*) 5);
    AutoWait->AddObject("6S",(int*) 6);
    AutoWait->AddObject("7S",(int*) 7);
    AutoWait->AddObject("8S",(int*) 8);
    AutoWait->AddObject("9S",(int*) 9);
    AutoWait->AddObject("10S",(int*) 10);
    SmartDashboard::PutData("AutoWait", AutoWait);
  }

  void AutonomousInit() {
    drive->SetFastGear();
    drive->Stop();
    drive->ResetEncoder();
    lift->ResetEncoder();
    lift->SetLowPosition();

    auton->SetStageOne((int)AutoChooser->GetSelected(), (int)StartingPosition->GetSelected(), (int)AutoWait->GetSelected());
    auton->ChooseStage();
  }

  void AutonomousPeriodic() {
    message = 72;
    SmartDashboard::PutBoolean("transaction", arduino->Transaction(&message, 1, NULL, 0));
    auton->RunPeriodic();
  }

  void TeleopInit() {
    drive->SetFastGear();
    drive->Stop();
    lift->Stop();
    man->SetIntakeSpeed(0);
  }

  void TeleopPeriodic() {
//———[controller 1]—————————————————————————————————————————————————————————————
  //———[drivetrain]—————————————————————————————————————————————————————————————
    if(lift->GetLiftPosition() > 11000) drive->SetSlowGear();

    if(lift->GetLiftPosition() > 14000) maxspeed = 0.4;
    else if(9000 <= lift->GetLiftPosition() && lift->GetLiftPosition() <= 14000) {
      maxspeed = -(1.0/300.0) * (45.0 - ( sqrt(15.0) * sqrt(15835.0-lift->GetLiftPosition()) ) );
    }
    else if(lift->GetLiftPosition() < 9000) maxspeed = 1;

    if(xbox->GetAButton() && lift->GetLiftPosition() < 9000 && !pressedTurn) {
      pressedTurn = drive->TurnAngle(1, 180);
    }
    else {
      drive->TankDrive(-xbox->GetY(xbox->kLeftHand) + xbox->GetTriggerAxis(xbox->kLeftHand)*0.5,
        -xbox->GetY(xbox->kRightHand) + xbox->GetTriggerAxis(xbox->kRightHand)*0.5, true, maxspeed);
      drive->turning = false;
      if(!xbox->GetAButton()) pressedTurn = false;
    }

    if(xbox->GetYButtonPressed() || xbox->GetBumperPressed(xbox->kRightHand) || xbox->GetBumperPressed(xbox->kLeftHand)) {
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
    }

    lift->SetSpeed(-xbox2->GetY(xbox2->kRightHand));

  //———[manipulator]————————————————————————————————————————————————————————————
    if(xbox2->GetBumper(xbox2->kLeftHand)) {
      man->Release();
    } else {
      man->Restrain();
      man->SetIntakeSpeed(-xbox2->GetY(xbox2->kLeftHand));
    }

  //———[ramp]———————————————————————————————————————————————————————————————————
    if(xbox->GetBumper(xbox->kLeftHand) && xbox->GetBumper(xbox->kRightHand) && xbox2->GetBumper(xbox2->kLeftHand) && xbox2->GetBumper(xbox2->kRightHand)) {
      if(timer->GetMatchTime() < 30) ramp->ConfirmIntentionalDeployment();
    }

  //———[climber]————————————————————————————————————————————————————————————————


  //———[periodic]———————————————————————————————————————————————————————————————
    message = 76;
    SmartDashboard::PutBoolean("transaction", arduino->Transaction(&message, 1, NULL, 0));
    SmartDashboard::PutBoolean("rampsReady", (timer->GetMatchTime() < 30));
    SmartDashboard::PutData("AutoWait", AutoWait);
    drive->RunPeriodic();
    lift->RunPeriodic();
    man->RunPeriodic();

//———[driver station]—————————————————————————————————————————————————————————
    bool ledButton = station->GetRawButton(0);

    bool liftOverride = station->GetRawButton(3);
    bool intakeOverride = station->GetRawButton(4);
    bool zeroLift = station->GetRawButton(5);

    lift->OverrideLift(liftOverride);
    man->OverrideIntake(intakeOverride);

    if(zeroLift) lift->ResetEncoder();

  }

};

START_ROBOT_CLASS(Robot)
