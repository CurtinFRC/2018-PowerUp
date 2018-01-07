#include "WPILib.h"

#include "curtinfrc/math.h"

#include <iostream>

using namespace frc;
using namespace curtinfrc;

class Robot : public IterativeRobot {
    void RobotInit() {
        std::cout << "0.5^2 = " << math::square_keep_sign(0.5) << std::endl;
    }

    void AutonomousInit() { }
    void AutonomousPeriodic() { }
    
    void TeleopInit() { }
    void TeleopPeriodic() { }

    void TestInit() { }
    void TestPeriodic() { }
};

START_ROBOT_CLASS(Robot)