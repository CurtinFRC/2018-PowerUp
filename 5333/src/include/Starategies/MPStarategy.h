#pragma once

#include "curtinfrc/drivetrain.h"
#include "curtinfrc/strategy/mp_strategy.h"

#include <string>

using namespace curtinfrc;

class MPStarategy {
public:
   static std::shared_ptr<Strategy> make_strat(Drivetrain *drive, std::string mp_file_base);
};

/* Robot:
  Length = 3 feet
  Width = 2.36 feet

  Robot should be 1.15 from scale
*/

/* Field locations
  Driverstation:
    1 = (1.5, 22.0)
    2 = (1.5, 11.0)
    3 = (1.5, 6.0)

  Scale:
    Left    = (24.2, 21.8, 315)
    Right   = (24.2, 5.2, 45)
  Switch:
    Left    = (14, 21.15)
    Right   = (14, 5.85)
*/
