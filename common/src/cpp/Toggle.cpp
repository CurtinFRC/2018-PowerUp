#include "curtinfrc/toggle.h"

using namespace curtinfrc;

Toggle::Toggle(Trigger _mode) {
  trigger_mode = _mode;
}

bool Toggle::apply(bool statement) {
  return (mode != (mode = statement)) && (trigger_mode == Trigger::True ? statement : true) && (trigger_mode == Trigger::False ? !statement : true);
}
