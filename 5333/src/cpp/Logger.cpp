#include "Logger.h"
#include <fstream>
#include <stdint.h>

using namespace std;

void LogControl::init(const string &text) { // Set up file for given header
  file = new ofstream("log_" + logger_name + ".csv", ios_base::out); // Generate new file
  file << "action, timestamp" << endl; // Set columns for each parameter
}

void LogControl::log_write(const string &action, uint64_t time_micro) { // Format and write to log, given info
  time_milli = static_cast<double>(time_micro / 1000); // Convert to milliseconds
  file << action << "," << timemilli << endl; // Append to log file

  call++;
  if (call % 10 == 0) {  // Stores 10 then pushes in bulk for faster writing
    file.flush()
  }
}

// handle last flush calls later
