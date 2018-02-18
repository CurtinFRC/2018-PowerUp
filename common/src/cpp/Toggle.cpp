#include "Toggle.h"

Toggle::Toggle(Trigger _mode) {
  trigger_mode = _mode;
  mode = _mode == Trigger::False;
}

bool Toggle::apply(bool statement) {
  return (mode != (mode = statement)) && (trigger_mode == Trigger::True ? statement : true) && (trigger_mode == Trigger::False ? !statement : true);
}
