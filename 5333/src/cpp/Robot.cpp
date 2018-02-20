// Shared headers
#include "curtinfrc/math.h"
#include "curtinfrc/drivetrain.h"
#include "curtinfrc/strategy/mp_strategy.h"
#include "WPILib.h"

// Robot part classes
#include "IO.h"
#include "Belev.h"
#include "Map.h"
#include "ControlMap.h"
#include "Auto.h"

// Starategies
#include "Starategies/DriveStarategy.h"
#include "Starategies/AutoBelevStarategy.h"
#include "Starategies/BelevStarategy.h"
#include "Starategies/IntakeStarategy.h"
#include "Starategies/TurnStarategy.h"

// Other required libraries
#include <string>
#include <SmartDashboard/SmartDashboard.h>
#include <iostream>
#include <stdint.h>
#include <cmath>

using namespace frc; // WPILib classes/functions
using namespace std;

class Robot : public TimedRobot {
public:
  Drivetrain *drive;
  BelevatorControl *belev;
  IO *io;

  AutoControl *auto_;

  int belev_ticks;

  Robot() { }

  void RobotInit() {
    CameraServer::GetInstance()->StartAutomaticCapture();

    io = IO::get_instance(); // Refer to IO
    drive = new Drivetrain(io->left_motors[0], io->right_motors[0], io->left_motors[0], io->right_motors[0]);
    belev = new BelevatorControl();

    auto_ = new AutoControl(drive);

    belev_ticks = 0;
  }

  void AutonomousInit() {
    cout << "Auto Init" << endl;
    auto_->init();
  }
  void AutonomousPeriodic() {
    auto_->tick();
    drive->strategy_controller().periodic();
    drive->log_write(); // Make this bit call only on mutates later *
    // belev->log_write();
  }

  void TeleopInit() {
    cout << "Teleop Init" << endl;
    ControlMap::init();

    auto drive_strat = std::make_shared<DriveStarategy>(drive);
    drive->strategy_controller().set_active(drive_strat);

    auto belev_strat = make_shared<BelevStarategy>(belev);
    belev->strategy_controller().set_active(belev_strat);
  }
  void TeleopPeriodic() {
    SmartDashboard::PutNumber("Yaw", io->navx->GetYaw());

    drive->strategy_controller().periodic();

    belev->strategy_controller().periodic();
    belev->tick();
    belev->claw(ControlMap::intake_claw_state());
    belev->winch_mode(ControlMap::winch_shifter_state() ? BelevatorControl::Gear::High : BelevatorControl::Gear::Low);

    double intake_throttle = 0.5 * (IO::get_instance()->right_joy->GetRawAxis(3) - 1);
    belev->intake(ControlMap::intake_motor_power() * intake_throttle);
    SmartDashboard::PutNumber("Intake Throttle", -intake_throttle);
  }

  void TestInit() {
    auto strat = std::make_shared<TurnStarategy>(drive, 90, 0.5);
    drive->strategy_controller().set_active(strat);
  }

  void TestPeriodic() {
    drive->strategy_controller().periodic();
  }
};

START_ROBOT_CLASS(Robot)
