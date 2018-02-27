#pragma once

namespace curtinfrc {
class Toggle {
  public:
    enum class Trigger {
      True,
      False,
      Both
    };

    Toggle(Trigger _mode = Trigger::Both);
    bool apply(bool statement);

private:
    Trigger trigger_mode;
    bool mode;
};
} // namespace curtinfrc
