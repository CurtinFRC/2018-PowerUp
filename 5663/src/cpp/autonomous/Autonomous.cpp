#include "autonomous/Autonomous.h"
#include <String.h>
using namespace components;

// Constructor for Autonomous class
Autonomous::Autonomous(Drive drive, Lift lift, Manipulator man, Ramp ramp) {
  drive.Stop();
}

// Choose the best autonomous routine
void Autonomous::ChooseRoutine(int autoMode, int startingPosition) {
  gamedata = DriverStation::GetInstance().GetGameSpecificMessage(); //Get specific match data
  SmartDashboard::PutString("Alliance Switch:", &gamedata[0]);
  SmartDashboard::PutString("Scale:", &gamedata[1]);
  SmartDashboard::PutString("Enemy Switch:", &gamedata[2]);  //Put data on shuffleboard
}

// Routine: Initial (1) > Switch (left)
void Autonomous::S1L() {
  //1-LS
}

// Routine: Initial (2) > Switch (left)
void Autonomous::S2L() {
  //2-LF
}

// Routine: Initial (3) > Switch (left)
void Autonomous::S3L() {
  //3-RS RS-RB RB-LB LB-LS
}

// Routine: Initial (1) > Switch (right)
void Autonomous::S1R() {
  //1-LS LS-LB LB-RB RB-RS
}

// Routine: Initial (2) > Switch (right)
void Autonomous::S2R() {
  //2-RF
}

// Routine: Initial (3) > Switch (right)
void Autonomous::S3R() {
  //3-RS
}
