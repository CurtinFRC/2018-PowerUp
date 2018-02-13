#include "autonomous/Autonomous.h"

using namespace autonomous;
using namespace components;

// Constructor for Autonomous class
Autonomous::Autonomous(Drive *drive, Lift *lift, Manipulator *man) {
  autoDrive = drive;
  autoLift = lift;
  autoMan = man;
  timer = new Timer();
  timer->Start();
  AutoWait = new SendableChooser<int*>;
  AutoWait->AddDefault("0S",(int*) 0);
  AutoWait->AddObject("1S",(int*) 1000);
  AutoWait->AddObject("2S",(int*) 2000);
  AutoWait->AddObject("3S",(int*) 3000);
  AutoWait->AddObject("4S",(int*) 4000);
  AutoWait->AddObject("5S",(int*) 5000);
  AutoWait->AddObject("6S",(int*) 6000);
  AutoWait->AddObject("7S",(int*) 7000);
  AutoWait->AddObject("8S",(int*) 8000);
  AutoWait->AddObject("9S",(int*) 9000);
  AutoWait->AddObject("10S",(int*) 10000);
}

// Set starting position and auto Mode
void Autonomous::SetStageOne(int mode, int startingPosition) {
  gameData = DriverStation::GetInstance().GetGameSpecificMessage();

  switch(mode) {
    case 0: //Baseline
      stage1 = [=](){return this->Baseline();};
      break;

    case 1: //Switch
      if(gameData[0] == 'L') {
        if(startingPosition == 1) stage1 = [=](){return this->Sw1L();};
        else if(startingPosition == 2) stage1 = [=](){return this->Sw2L();};
        else autoFunction = [=](){return this->Sw3L();};
      } else {
        if(startingPosition == 1) stage1 = [=](){return this->Sw1R();};
        else if(startingPosition == 2) stage1 = [=](){return this->Sw2R();};
        else stage1 = [=](){return this->Sw3R();};
      }

    case 2: //Scale
      if(gameData[0] == 'L') {
        if(startingPosition == 1) stage1 = [=](){return this->Sc1L();};
        else if(startingPosition == 2) stage1 = [=](){return this->Sc2L();};
        else stage1 = [=](){return this->Sc3L();};
      } else {
        if(startingPosition == 1) stage1 = [=](){return this->Sc1R();};
        else if(startingPosition == 2) stage1 = [=](){return this->Sc2R();};
        else stage1 = [=](){return this->Sc3R();};
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

bool Autonomous::Stop() {
  autoDrive->Stop();
  //autoMan->SetIntakeSpeed(0);
  return true;
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
  return false;
}

bool Autonomous::BackDrive() {
  switch (autoState) {
    case 0:
      if(autoDrive->DriveDistance(0.5, -0.02)) autoState++;
      break;
    case 1:
      if(Wait((int)AutoWait->GetSelected())) autoState++;
      break;
    default:
      Stop();
      return true;
    }
    return false;
}

bool Autonomous::Baseline() {
  switch (autoState) {
    case 0:
      autoLift->SetMidPosition();
      if(autoDrive->DriveDistance(1, 3.0)) autoState++;  //Change to 4secs after testing
      break;
    default:
      Stop();
      return true;
  }
  return false;
}

// Routine: Initial (1) > Switch (left)
bool Autonomous::Sw1L() {
  switch (autoState) {
    case 0:
      autoLift->SetMidPosition();
      if(autoDrive->DriveDistance(1, 3.7)) autoState++;
      break;
    case 1:
      if(autoDrive->TurnAngle(1, 90)) autoState++;
      break;
    case 2:
      if(autoDrive->DriveDistance(1, 0.9, 1000)) autoState++;
      break;
    case 3:
      autoMan->SetIntakeSpeed(1);
      if(Wait(500)) autoState++;
      break;
    case 4:
      if(autoDrive->DriveDistance(1, -0.5)) autoState++;
      break;
    case 5:
      autoLift->SetLowPosition();
      break;
    default:
      Stop();
      return true;
  }
  return false;
}

// Routine: Initial (2) > Switch (left)
bool Autonomous::Sw2L() {
  switch(autoState) {
    case 0:
      if(autoDrive->DriveDistance(1, 0.5)) autoState++;
      break;
    case 1:
      if(autoDrive->TurnAngle(1, -45)) autoState++;
      break;
    case 2:
      if(autoDrive->DriveDistance(1, 3.0)) autoState++;
      break;
    case 3:
      if(autoDrive->TurnAngle(1, 45)) autoState++;
      break;
    case 4:
      if(autoDrive->DriveDistance(1, 0.8, 1500)) autoState++;
      break;
    case 5:
      autoMan->SetIntakeSpeed(1);
      if(Wait(500)) autoState++;
      break;
    case 6:
      if(autoDrive->DriveDistance(1, -0.5)) autoState++;
      break;
    case 7:
      autoLift->SetLowPosition();
      break;
    default:
      Stop();
      return true;
  }
  return false;
}

// Routine: Initial (3) > Switch (left)
bool Autonomous::Sw3L() {
  switch(autoState) {
    case 0:
      break;
    default:
      Stop();
      return true;
  }
  return false;
}

// Routine: Initial (1) > Switch (right)
bool Autonomous::Sw1R() {
  switch(autoState) {
    case 0:
      break;
    default:
      Stop();
      return true;
  }
  return false;
}

// Routine: Initial (2) > Switch (right)
bool Autonomous::Sw2R() {
  switch(autoState) {
    case 0:
      autoLift->SetMidPosition();
      if(autoDrive->DriveDistance(1, 0.5)) autoState++;
      break;
    case 1:
      if(autoDrive->TurnAngle(1, 30)) autoState++;
      break;
    case 2:
      if(autoDrive->DriveDistance(1, 2.0)) autoState++;
      break;
    case 3:
      if(autoDrive->TurnAngle(1, -30)) autoState++;
      break;
    case 4:
      if(autoDrive->DriveDistance(1, 0.5, 1000)) autoState++;
      break;
    case 5:
      autoMan->SetIntakeSpeed(1);
      if(Wait(500)) autoState++;
      break;
    case 6:
      if(autoDrive->DriveDistance(1, -0.5)) autoState++;
      break;
    case 7:
      autoLift->SetLowPosition();
      break;
    default:
      Stop();
      return true;
  }
  return false;
}

// Routine: Initial (3) > Switch (right)
bool Autonomous::Sw3R() {
  switch (autoState) {
    case 0:
      autoLift->SetMidPosition();
      if(autoDrive->DriveDistance(1, 3.7)) autoState++;
      break;
    case 1:
      if(autoDrive->TurnAngle(1, -90)) autoState++;
      break;
    case 2:
      if(autoDrive->DriveDistance(1, 0.9, 1000)) autoState++;
      break;
    case 3:
      autoMan->SetIntakeSpeed(1);
      if(Wait(500)) autoState++;
      break;
    case 4:
      if(autoDrive->DriveDistance(1, -0.5)) autoState++;
      break;
    case 5:
      autoLift->SetLowPosition();
      break;
    default:
      Stop();
      return true;
  }
  return false;
}

bool Autonomous::Sc1L() {
  switch (autoState) {
    case 0:
      if(autoDrive->DriveDistance(1, 8.0)) autoState++;
      break;
    case 1:
      if(autoDrive->TurnAngle(1, -90)) autoState++;
      break;
    case 2:
      if(autoDrive->DriveDistance(1, -2.0, 2500)) autoState++;
      break;
    case 3:
      autoLift->SetHighPosition();
      if(Wait(1000)) autoState++;
      break;
    case 4:
      if(autoDrive->DriveDistance(0.5, 0.3)) autoState++;
      break;
    case 5:
      autoMan->SetIntakeSpeed(1);
      if(Wait(300)) autoState++;
    default:
      Stop();
      return true;
  }
  return false;
}

bool Autonomous::Sc2L() {
  switch (autoState) {
    case 0:
      break;
    default:
      Stop();
      return true;
  }
  return false;
}

bool Autonomous::Sc3L() {
  switch (autoState) {
    case 0:
      break;
    default:
      Stop();
      return true;
  }
  return false;
}

bool Autonomous::Sc1R() {
  switch (autoState) {
    case 0:
      break;
    default:
      Stop();
      return true;
  }
  return false;
}

bool Autonomous::Sc2R() {
  switch (autoState) {
    case 0:
      break;
    default:
      Stop();
      return true;
  }
  return false;
}

bool Autonomous::Sc3R() {
  switch (autoState) {
    case 0:
      break;
    default:
      Stop();
      return true;
  }
  return false;
}
