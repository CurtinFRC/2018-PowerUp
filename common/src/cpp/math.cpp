#include "curtinfrc/math.h"

double curtinfrc::math::square_keep_sign(double input) {
    return input == 0 ? 0 : (input > 0 ? input*input : -1*input*input);
}