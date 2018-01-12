#pragma once

#include "curtinfrc/motors/CurtinTalonSRX.h"

namespace curtinfrc {
    template <unsigned int N_MOTORS>
    class Drivetrain {
    public:
        Drivetrain(CurtinTalonSRX *l[N_MOTORS], CurtinTalonSRX *r[N_MOTORS]) {
            for (int i = 0; i < N_MOTORS; i++) {
                left[i] = l[i];
                right[i] = r[i];
            }
        }

        void set_left(double value) {
            for (int i = 0; i < N_MOTORS; i++) {
                left[i]->Set(value);
            }
        }

        void set_right(double value) {
            for (int i = 0; i < N_MOTORS; i++) {
                right[i]->Set(value);
            }
        }

    private:
        CurtinTalonSRX *left[N_MOTORS], *right[N_MOTORS];
    };
}