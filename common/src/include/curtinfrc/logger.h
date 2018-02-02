#include <fstream>
#include <stdint.h>

template <unsigned int N_COLS>
class Logger {
public:
  Logger(std::string filename, std::string cols[N_COLS]) { // Make instance of Logger for given file
    _outfile.open("/home/lvuser/" + filename + ".csv"); // Generates file in user directory
    _outfile << "timestamp,";

    for (int i = 0; i < N_COLS; i++) { // Append commas between columns
      _outfile << cols[i];
      _outfile << (i != (N_COLS - 1) ? "," : std::endl);
    }
    call = 0;
  }

  void log_write(uint64_t time_micro, std::string values[N_COLS]) {
    time_milli = static_cast<double>(time_micro / 1000); // Convert to milliseconds
    for (int i = 0; i < N_COLS; i++) { // Append commas between columns
      _outfile << values[i];
      _outfile << (i != (N_COLS - 1) ? "," : endl);
    }

    call++;
    if (call % 10 == 0) { // Stores 10 then pushes in bulk for faster writing
      file.flush()
    }
  }
private:
  ofstream _outfile;
  string logger_name;
  double time_milli;
  int call;
};

// Last flush calls still need to be handled *
