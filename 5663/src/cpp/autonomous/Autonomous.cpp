#include "autonomous/Autonomous.h"

using namespace autonomous;
using namespace components;

// Constructor for Autonomous class
Autonomous::Autonomous(Drive drive, Lift lift, Manipulator man, Ramp ramp) {
  autoLift = new Lift(lift);
  autoDrive = new Drive(drive);
  autoMan = new Manipulator(man);
}

// Choose the best autonomous routine
void Autonomous::ChooseRoutine(int autoMode, int startingPosition) {
  AutoStage = 0;
  gameData = DriverStation::GetInstance().GetGameSpecificMessage(); //Get specific match data
  // gameData will be an array with 3 characters, eg. "LRL"
  // check https://wpilib.screenstepslive.com/s/currentCS/m/getting_started/l/826278-2018-game-data-details
  SmartDashboard::PutString("Alliance Switch:", &gameData[0]);
  SmartDashboard::PutString("Scale:", &gameData[1]);
  SmartDashboard::PutString("Enemy Switch:", &gameData[2]);  //Put data on shuffleboard

  switch (autoMode) {
    case 0:
      autoFunction = [=](){return this->Baseline();};
      break;

    case 1:
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
  autoDrive->RunPeriodic();
  autoLift->RunPeriodic();

  switch(AutoStage) {
    case 0:
      if(autoFunction()) AutoStage++;
      break;
    case 1:
      ChooseRoutine(5,5);
      break;
  }

}

bool Autonomous::Baseline() {
  autoDrive->DriveDistance(1.0, 0.2, false);
}

// Routine: Initial (1) > Switch (left)
bool Autonomous::S1L() {
  //1-LS
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
