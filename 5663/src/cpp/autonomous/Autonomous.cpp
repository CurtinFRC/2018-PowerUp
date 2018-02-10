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

// Set starting position and auto Mode
void Autonomous::SetStageOne(int mode, int startingPosition) {
  gameData = DriverStation::GetInstance().GetGameSpecificMessage();

  switch(mode) {
    case 0:
      stage1 = [=](){return this->Baseline();};
      break;

    case 1:
      if(gameData[0] == 'L') {
        if(startingPosition == 1) stage1 = [=](){return this->S1L();};
        else if(startingPosition == 2) stage1 = [=](){return this->S2L();};
        else autoFunction = [=](){return this->S3L();};
      } else {
        if(startingPosition == 1) stage1 = [=](){return this->S1R();};
        else if(startingPosition == 2) stage1 = [=](){return this->S2R();};
        else stage1 = [=](){return this->S3R();};
      }
  }

}

void Autonomous::SetStageTwo(int mode) {
  switch(mode) {

  }
}

void Autonomous::SetStageThree(int mode) {
  switch(mode) {

  }
}

// Choose the best autonomous routine
void Autonomous::ChooseStage() {
  autoState = 0;
  switch(currentStage) {
      case 0:
        autoFunction = [=](){return this->BackDrive();};
        break;
      case 1:
        autoFunction = stage1;
        break;
      case 2:
        autoFunction = stage2;
        break;
      case 3:
        autoFunction = stage3;
        break;
  }
}

void Autonomous::RunPeriodic() {
  gameData = DriverStation::GetInstance().GetGameSpecificMessage(); //Get specific match data
  // gameData will be an array with 3 characters, eg. "LRL"
  // check https://wpilib.screenstepslive.com/s/currentCS/m/getting_started/l/826278-2018-game-data-details
  SmartDashboard::PutString("Alliance Switch:", &gameData[0]);
  SmartDashboard::PutString("Scale:", &gameData[1]);
  SmartDashboard::PutString("Enemy Switch:", &gameData[2]);  //Put data on shuffleboard
  SmartDashboard::PutNumber("Auto State:", autoState);
  if(autoFunction()) {
    currentStage++;
    ChooseStage();
  }
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

bool Autonomous::BackDrive() {
  switch (autoState) {
    case 0:
      if(autoDrive->DriveDistance(0.5, -0.05, 100)) autoState++;
      break;
    default:
      autoDrive->Stop();
      return true;
    }
}

bool Autonomous::Baseline() {
  switch (autoState) {
    case 0:
      autoLift->SetMidPosition();
      if(autoDrive->DriveDistance(1, 3.0, 6000)) autoState++;  //Change to 4secs after testing
      break;
    default:
      autoDrive->Stop();
      return true;
  }
}

// Routine: Initial (1) > Switch (left)
bool Autonomous::S1L() {
  switch (autoState) {
    case 0:
      if(autoDrive->DriveDistance(1, 3.2, 10000)) autoState++;
      break;
    case 1:
      if(autoDrive->TurnAngle(1, 40, 2000)) autoState++;
      break;
    case 2:
      autoMan->SetIntakeSpeed(1);
      break;
    default:
      autoDrive->Stop();
      return true;
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
