#pragma once

class Toggle {
public:
  enum class Trigger {
    True,
    False,
    Both
  };

  Toggle(Trigger _mode = Trigger::Both);
  bool apply(bool statement);
  bool mode;

private:
  Trigger trigger_mode;
};
