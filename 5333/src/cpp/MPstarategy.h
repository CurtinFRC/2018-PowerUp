#pragma once

#include "curtinfrc/drivetrain.h"
#include "curtinfrc/strategy/mp_strategy.h"

#include <string>

using namespace curtinfrc;

class MPStarategy {
public:
   static std::shared_ptr<Strategy> make_strat(Drivetrain *drive, std::string left_mp_file, std::string right_mp_file);
};
