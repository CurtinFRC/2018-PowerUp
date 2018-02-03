#pragma once

#include <fstream>
#include <stdint.h>
#include <cstdarg>

namespace curtinfrc {
  class Logger {
  public:
    Logger(std::string filename, std::string headers) { // Make instance of Logger for given file
      _outfile.open("/home/lvuser/" + filename + ".csv"); // Generates file in user directory
      _outfile << "timestamp," << headers << std::endl;
      call = 0;
    }

    void write(uint64_t time_micro, int num_of_args, ...) {
      va_list args;
      va_start(args, num_of_args); // num_of_args is required because there is no easy way to determine va_list length (╯°□°）╯︵ ┻━┻

      time_milli = static_cast<double>(time_micro / 1000); // Convert to milliseconds
      for(int i = 0; i < num_of_args; i++) { // Append commas between columns
        _outfile << va_arg(args, std::string);
        _outfile << (i != (num_of_args - 1) ? "," : "\n");
      }

      call++;
      if (call % 10 == 0) { // Stores 10 then pushes in bulk for faster writing
        _outfile.flush();
      }
    }
  private:
    std::ofstream _outfile;
    std::string logger_name;
    double time_milli;
    int call;
  };
}

// Last flush calls still need to be handled *
