/* #include <fstream> // Operates on external files
#include <stdint.h>

using namespace std;

class LogControl { // Probably pneumatics
public:
  LogControl(std::string name) : logger_name(name) {} // Contructor to initialse a logger for the given header
  void init(const string &header);
  void log_write(uint64_t time_micro, const vector<string> &states);
private:
  string logger_name;
  double time_milli;
  // starttime
}; */
