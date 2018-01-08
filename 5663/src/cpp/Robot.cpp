#include "WPILib.h"
#include "curtinfrc/math.h"
#include <iostream>
#include <string>
#include <SmartDashboard/SmartDashboard.h>
#include <PowerDistributionPanel.h>
#include "ctre/Phoenix.h"

using namespace frc;
using namespace curtinfrc;
using namespace std;

class Robot : public IterativeRobot {
  XboxController *xbox;
  PowerDistributionPanel *pdp;
  SendableChooser<int*> *AutoChooser; // Choose auto mode
  TalonSRX *left1, *left2, *left3,
    *right1, *right2, *right3;
public:
  // Configuration settings: (maybe make a sperate file for these?)
  double deadzone = 0.04; //Stop the robot being a sneaky snail
  // Regular variables
  string gameData;
  bool A,B,X,Y,LB,RB,back,start,LS,RS;
  double LX,LY,RX,RY,LT,RT,Dpad;
  int Auto;
  void RobotInit() {
    xbox = new XboxController(0);
    pdp = new PowerDistributionPanel(0);
    AutoChooser = new SendableChooser<int*>;
    left1 = new TalonSRX(1); left2 = new TalonSRX(2); left3 = new TalonSRX(3);
    right1 = new TalonSRX(4); right2 = new TalonSRX(4); right3 = new TalonSRX(5);
    AutoChooser->AddDefault("Cross Baseline",(int*) 0);
    AutoChooser->AddObject("Auto 1",(int*) 1);
    AutoChooser->AddObject("Auto 2",(int*) 2);
    getValues();
    updateDash();
  }

  void Drive(double l, double r) {  //probably a wpi class to do this for us but I couldnt find one so.
    if(-deadzone < l && l < deadzone) l == 0;
    if(-deadzone < r && r < deadzone) r == 0;
    l *= abs(l);
    r *= abs(r); // square inputs
    left1->Set(ControlMode::PercentOutput,l); left2->Set(ControlMode::PercentOutput,l); left3->Set(ControlMode::PercentOutput,l);
    right1->Set(ControlMode::PercentOutput,l); right2->Set(ControlMode::PercentOutput,l); right2->Set(ControlMode::PercentOutput,l);
  }

  void getValues() { //Set variables for the xbox controller values for easy coding (put in seperate file?)
    A = xbox->GetRawButton(1);
    B = xbox->GetRawButton(2);
    X = xbox->GetRawButton(3);
    Y = xbox->GetRawButton(4);
    LB = xbox->GetRawButton(5);
    RB = xbox->GetRawButton(6);
    back = xbox->GetRawButton(7);
    start = xbox->GetRawButton(8);
    LS = xbox->GetRawButton(9);
    RS = xbox->GetRawButton(10);
    LX = xbox->GetRawAxis(0);
    LY = xbox->GetRawAxis(1);
    LT = xbox->GetRawAxis(2);
    RT = xbox->GetRawAxis(3);
    RX = xbox->GetRawAxis(4);
    RY = xbox->GetRawAxis(5);
    Dpad = xbox->GetPOV();
  }

  void updateDash() { //Put new controller values to the dashboard (put in seperate file?)
    SmartDashboard::PutNumber("D-Pad", Dpad);
    SmartDashboard::PutNumber("Current Draw", pdp->GetTotalCurrent());
    SmartDashboard::PutBoolean("A", A);
    SmartDashboard::PutBoolean("B", B);
    SmartDashboard::PutBoolean("X", X);
  	SmartDashboard::PutBoolean("Y", Y);
  	SmartDashboard::PutBoolean("Left Bumper", LB);
    SmartDashboard::PutBoolean("Right Bumper", RB);
    SmartDashboard::PutBoolean("Back", back);
    SmartDashboard::PutBoolean("Start", start);
    SmartDashboard::PutBoolean("Left Stick", LS);
  	SmartDashboard::PutBoolean("Right Stick", RS);
    SmartDashboard::PutNumber("Left Stick X", LX);
    SmartDashboard::PutNumber("Right Stick X", RX);
    SmartDashboard::PutNumber("Left Stick Y", LY);
    SmartDashboard::PutNumber("Right Stick Y", RY);
    SmartDashboard::PutNumber("Left Trigger", LT);
    SmartDashboard::PutNumber("Right Trigger", RT);
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

  void TeleopInit() { }
  void TeleopPeriodic() {
    void getValues();
    void updateDash();
    Drive(LY,RY);
  }

  void TestInit() { }
  void TestPeriodic() { }
};

START_ROBOT_CLASS(Robot)
