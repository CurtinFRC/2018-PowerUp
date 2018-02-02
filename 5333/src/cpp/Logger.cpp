/* #include "Logger.h"
#include <fstream>
#include <stdint.h>

using namespace std;

void LogControl::init(const string &header) { // Set up file for given parameters
  file = new ofstream("log_" + logger_name + ".csv", ios_base::out); // Generate new file
  file << "timestamp," << header << endl; // Set columns for each parameter
}

void LogControl::log_write(uint64_t time_micro, const vector<string> &states) { // Format and write to log, given info
  time_milli = static_cast<double>(time_micro / 1000); // Convert to milliseconds
  // Append commas into vector
  int vsize = state.size();
  string state_string;
  for(int i = 0; i <= vsize; i++) {
    state_string += state[i] + ",";
  }
  file << timemilli << "," << states_string << endl; // Append to log file

  call++;
  if (call % 10 == 0) {  // Stores 10 then pushes in bulk for faster writing
    file.flush()
  }
}

// handle last flush calls later */
