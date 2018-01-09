#include "WPILib.h"
#include "AHRS.h"
#include "curtinfrc/math.h"
#include <iostream>
#include <string>
#include <SmartDashboard/SmartDashboard.h>
#include <PowerDistributionPanel.h>
#include "ctre/Phoenix.h"

using namespace nt;
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
public:
  // Configuration settings: (maybe make a sperate file for these?)
  double deadzone = 0.04; //Stop the robot being a sneaky snail
  // Regular variables
  string gameData;
  bool A,B,X,Y,LB,RB,back,start,LS,RS;
  double LX,LY,RX,RY,LT,RT,Dpad;
  int Auto, gearMode;
  void RobotInit() {
    xbox = new XboxController(0);
    pdp = new PowerDistributionPanel(0);
    AutoChooser = new SendableChooser<int*>;
    left1 = new TalonSRX(1); left2 = new TalonSRX(2); left3 = new TalonSRX(3);
    right1 = new TalonSRX(4); right2 = new TalonSRX(4); right3 = new TalonSRX(5);
    leftGear = new DoubleSolenoid(1,0,1); rightGear = new DoubleSolenoid(1,2,3);
    ahrs = new AHRS(I2C::Port::kMXP);
    ahrs->EnableLogging(true);
    //slowGear = rightGear->kReverse;
    //fastGear = leftGear->kForward; // Assign named variables to the 2 gear modes just for easier assignment
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

  void TeleopInit() {
    rightGear->Set(rightGear->kForward);
    leftGear->Set(leftGear->kForward); //Set gear to default
    gearMode = rightGear->kForward;
  }
  void TeleopPeriodic() {
    void getValues();
    void updateDash();
    Drive(LY,RY);
    // if(xbox->GetBumperPressed(xbox->kLeftHand) == true) {
    //   if(rightGear->Get() == rightGear->kForward) gearMode = rightGear->kReverse;
    //   else gearMode = rightGear->kForward;
    // }
    // if(gearMode == rightGear->kForward) {
    //   rightGear->Set(rightGear->kForward);
    //   leftGear->Set(rightGear->kForward);
    // } else {
    //   rightGear->Set(rightGear->kReverse);
    //   leftGear->Set(rightGear->kReverse);
    // }
    if ( !ahrs ) return;

        bool reset_yaw_button_pressed = DriverStation::GetInstance().GetStickButton(0,1);
        if ( reset_yaw_button_pressed ) {
            ahrs->ZeroYaw();
        }

        SmartDashboard::PutBoolean( "IMU_Connected",        ahrs->IsConnected());
        SmartDashboard::PutNumber(  "IMU_Yaw",              ahrs->GetYaw());
        SmartDashboard::PutNumber(  "IMU_Pitch",            ahrs->GetPitch());
        SmartDashboard::PutNumber(  "IMU_Roll",             ahrs->GetRoll());
        SmartDashboard::PutNumber(  "IMU_CompassHeading",   ahrs->GetCompassHeading());
        SmartDashboard::PutNumber(  "IMU_Update_Count",     ahrs->GetUpdateCount());
        SmartDashboard::PutNumber(  "IMU_Byte_Count",       ahrs->GetByteCount());
        SmartDashboard::PutNumber(  "IMU_Timestamp",        ahrs->GetLastSensorTimestamp());

        /* These functions are compatible w/the WPI Gyro Class */
        SmartDashboard::PutNumber(  "IMU_TotalYaw",         ahrs->GetAngle());
        SmartDashboard::PutNumber(  "IMU_YawRateDPS",       ahrs->GetRate());

        SmartDashboard::PutNumber(  "IMU_Accel_X",          ahrs->GetWorldLinearAccelX());
        SmartDashboard::PutNumber(  "IMU_Accel_Y",          ahrs->GetWorldLinearAccelY());
        SmartDashboard::PutBoolean( "IMU_IsMoving",         ahrs->IsMoving());
        SmartDashboard::PutNumber(  "IMU_Temp_C",           ahrs->GetTempC());
        SmartDashboard::PutBoolean( "IMU_IsCalibrating",    ahrs->IsCalibrating());

        SmartDashboard::PutNumber(  "Velocity_X",           ahrs->GetVelocityX() );
        SmartDashboard::PutNumber(  "Velocity_Y",           ahrs->GetVelocityY() );
        SmartDashboard::PutNumber(  "Displacement_X",       ahrs->GetDisplacementX() );
        SmartDashboard::PutNumber(  "Displacement_Y",       ahrs->GetDisplacementY() );

        /* Display Raw Gyro/Accelerometer/Magnetometer Values                       */
        /* NOTE:  These values are not normally necessary, but are made available   */
        /* for advanced users.  Before using this data, please consider whether     */
        /* the processed data (see above) will suit your needs.                     */

        SmartDashboard::PutNumber(  "RawGyro_X",            ahrs->GetRawGyroX());
        SmartDashboard::PutNumber(  "RawGyro_Y",            ahrs->GetRawGyroY());
        SmartDashboard::PutNumber(  "RawGyro_Z",            ahrs->GetRawGyroZ());
        SmartDashboard::PutNumber(  "RawAccel_X",           ahrs->GetRawAccelX());
        SmartDashboard::PutNumber(  "RawAccel_Y",           ahrs->GetRawAccelY());
        SmartDashboard::PutNumber(  "RawAccel_Z",           ahrs->GetRawAccelZ());
        SmartDashboard::PutNumber(  "RawMag_X",             ahrs->GetRawMagX());
        SmartDashboard::PutNumber(  "RawMag_Y",             ahrs->GetRawMagY());
        SmartDashboard::PutNumber(  "RawMag_Z",             ahrs->GetRawMagZ());
        SmartDashboard::PutNumber(  "IMU_Temp_C",           ahrs->GetTempC());
        /* Omnimount Yaw Axis Information                                           */
        /* For more info, see http://navx-mxp.kauailabs.com/installation/omnimount  */
        AHRS::BoardYawAxis yaw_axis = ahrs->GetBoardYawAxis();
        SmartDashboard::PutString(  "YawAxisDirection",     yaw_axis.up ? "Up" : "Down" );
        SmartDashboard::PutNumber(  "YawAxis",              yaw_axis.board_axis );

        /* Sensor Board Information                                                 */
        SmartDashboard::PutString(  "FirmwareVersion",      ahrs->GetFirmwareVersion());

        /* Quaternion Data                                                          */
        /* Quaternions are fascinating, and are the most compact representation of  */
        /* orientation data.  All of the Yaw, Pitch and Roll Values can be derived  */
        /* from the Quaternions.  If interested in motion processing, knowledge of  */
        /* Quaternions is highly recommended.                                       */
        SmartDashboard::PutNumber(  "QuaternionW",          ahrs->GetQuaternionW());
        SmartDashboard::PutNumber(  "QuaternionX",          ahrs->GetQuaternionX());
        SmartDashboard::PutNumber(  "QuaternionY",          ahrs->GetQuaternionY());
        SmartDashboard::PutNumber(  "QuaternionZ",          ahrs->GetQuaternionZ());

  }

  void TestInit() { }
  void TestPeriodic() { }
};

START_ROBOT_CLASS(Robot)
