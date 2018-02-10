#include "autonomous/Autonomous.h"

using namespace autonomous;
using namespace components;
using namespace curtinfrc;

// Constructor for Autonomous class
Autonomous::Autonomous(Drive *drive, Lift lift, Manipulator man, Ramp ramp) {
  autoDrive = drive;
  Lift autoLift = lift;
  Manipulator autoMan = man;
  timer = new Timer();
  timer->Start();
}

// Choose the best autonomous routine
void Autonomous::ChooseRoutine(int autoMode, int startingPosition) {
  SmartDashboard::PutString("Auto Mode:", "null");
  SmartDashboard::PutBoolean("ran baseline", false);
  autoState = 0;
  gameData = DriverStation::GetInstance().GetGameSpecificMessage(); //Get specific match data
  // gameData will be an array with 3 characters, eg. "LRL"
  // check https://wpilib.screenstepslive.com/s/currentCS/m/getting_started/l/826278-2018-game-data-details
  SmartDashboard::PutString("Alliance Switch:", &gameData[0]);
  SmartDashboard::PutString("Scale:", &gameData[1]);
  SmartDashboard::PutString("Enemy Switch:", &gameData[2]);  //Put data on shuffleboard

  switch (autoMode) {
    case 0:
      autoFunction = [=](){return this->S1L();};
      SmartDashboard::PutString("Auto Mode:", "S1L");
      break;

    case 1:
      SmartDashboard::PutString("Auto Mode:", "next stage");
      if(gameData[0] == 'L') {
        if(startingPosition == 1) autoFunction = [=](){return this->S1L();};
        else if(startingPosition == 2) autoFunction = [=](){return this->S2L();};
        else autoFunction = [=](){return this->S3L();};
      } else {
        if(startingPosition == 1) autoFunction = [=](){return this->S1R();};
        else if(startingPosition == 2) autoFunction = [=](){return this->S2R();};
        else autoFunction = [=](){return this->S3R();};
      }
  }
}

void Autonomous::RunPeriodic() {
  SmartDashboard::PutNumber("Auto State:", autoState);
  autoFunction();
}

bool Autonomous::Wait(int delay) {
  if(waiting) {
    if(timer->HasPeriodPassed(delay)) {
        waiting = false;
        return true;
    }
  } else {
    timer->Reset();
    waiting = true;
  }
}

bool Autonomous::Baseline() {
   switch (autoState) {
     case 0:
      if(autoDrive->DriveDistance(0.2, -0.05, 2000)) autoState++;
      break;
     case 1:
      if(autoDrive->DriveDistance(0.2, 0.5, 10000)) autoState++;
      break;
    default:
      autoDrive->Stop();
      break;
  }
}

// Routine: Initial (1) > Switch (left)
bool Autonomous::S1L() {
  switch (autoState) {
    case 0:
      if(autoDrive->DriveDistance(0.5, -0.01, 2000)) autoState++;
      break;
    case 1:
      if(autoDrive->DriveDistance(0.5, 3.0, 10000)) autoState++;
      break;
    case 2:
      autoDrive->Stop();
      break;
  }
}

// Routine: Initial (2) > Switch (left)
bool Autonomous::S2L() {
  //2-LF
}

// Routine: Initial (3) > Switch (left)
bool Autonomous::S3L() {
  //3-RS RS-RB RB-LB LB-LS
}

// Routine: Initial (1) > Switch (right)
bool Autonomous::S1R() {
  //1-LS LS-LB LB-RB RB-RS
}

// Routine: Initial (2) > Switch (right)
bool Autonomous::S2R() {
  //2-RF
}

// Routine: Initial (3) > Switch (right)
bool Autonomous::S3R() {
  //3-RS
}
