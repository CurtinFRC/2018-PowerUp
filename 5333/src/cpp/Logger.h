#include <fstream> // Operates on external files
#include <stdint.h>

class LogControl { // Probably pneumatics
public:
  LogControl(std::string name) : logger_name(name) {} // Contructor to initialse a logger for the given header
  void init();

  void LogControl::log_write(const string &action, uint64_t time_micro);
private:
  std::string logger_name;
  double time_milli;
  // starttime
};
