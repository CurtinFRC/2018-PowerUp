#include "curtinfrc/strategy/strategy.h"
#include "components/Drive.h"

using namespace curtinfrc;
using namespace components;

namespace autonomous {
  class BaselineStrategy : public Strategy {
  public:
    BaselineStrategy(Drive *drive) {
      d = drive;
    }

    void start() override {

    }

    void tick(double time) override {
      done = d->DriveDistance(0.2, 0.05, false);
    }

    void stop() override {

    }
  private:
    Drive *d;
  };
}