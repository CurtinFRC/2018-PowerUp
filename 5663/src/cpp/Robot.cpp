#include "WPILib.h"
#include "AHRS.h"
#include "curtinfrc/math.h"
#include <iostream>
#include <string>
#include <SmartDashboard/SmartDashboard.h>
#include <PowerDistributionPanel.h>
#include "ctre/Phoenix.h"
#include <PIDOutput.h>

using namespace frc;
using namespace curtinfrc;
using namespace std;

class Robot : public IterativeRobot {
  XboxController *xbox;
  PowerDistributionPanel *pdp;
  SendableChooser<int*> *AutoChooser; // Choose auto mode
  TalonSRX *left1, *left2, *left3,
    *right1, *right2, *right3;
  DoubleSolenoid *rightGear, *leftGear; // Solenoids for pneumatics, I'm just using them for ball shifters atm;
  AHRS *ahrs;
  PIDController *turn;
public:
  // Configuration settings: (maybe make a sperate file for these?)
  double deadzone = 0.04; //Stop the robot being a sneaky snail
  // Regular variables
  string gameData;
  int Auto, gearMode;
  void RobotInit() {
    xbox = new XboxController(0);
    out = new gyroPID();
    pdp = new PowerDistributionPanel(0);
    AutoChooser = new SendableChooser<int*>;
    left1 = new TalonSRX(1); left2 = new TalonSRX(2); left3 = new TalonSRX(3);
    right1 = new TalonSRX(4); right2 = new TalonSRX(4); right3 = new TalonSRX(5);
    leftGear = new DoubleSolenoid(0,0,1); rightGear = new DoubleSolenoid(0,2,3);
    AutoChooser->AddDefault("Cross Baseline",(int*) 0);
    AutoChooser->AddObject("Auto 1",(int*) 1);
    AutoChooser->AddObject("Auto 2",(int*) 2);
  }

  void Drive(double l, double r) {  //probably a wpi class to do this for us but I couldnt find one so.
    if(-deadzone < l && l < deadzone) l == 0;
    if(-deadzone < r && r < deadzone) r == 0;
    l *= abs(l);
    r *= abs(r); // square inputs
     left1->Set(ControlMode::PercentOutput,l); left2->Set(ControlMode::PercentOutput,l); left3->Set(ControlMode::PercentOutput,l);
     right1->Set(ControlMode::PercentOutput,l); right2->Set(ControlMode::PercentOutput,l); right2->Set(ControlMode::PercentOutput,l);
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
  }

  void TeleopInit() {
    rightGear->Set(rightGear->kForward);
    leftGear->Set(leftGear->kForward); //Set gear to default
    gearMode = rightGear->kForward;
  }
  void TeleopPeriodic() {
    Drive(xbox->GetY(xbox->kLeftHand),xbox->GetY(xbox->kRightHand));
    if(xbox->GetBumperPressed(xbox->kLeftHand) == true) {
      if(rightGear->Get() == rightGear->kForward) gearMode = rightGear->kReverse;
      else gearMode = rightGear->kForward;
    }
    if(gearMode == rightGear->kForward) {
      rightGear->Set(rightGear->kForward);
      leftGear->Set(rightGear->kForward);
    } else {
      rightGear->Set(rightGear->kReverse);
      leftGear->Set(rightGear->kReverse);
    }
  }

  void TestInit() {

  }

  void TestPeriodic() {
  }
};

START_ROBOT_CLASS(Robot)
