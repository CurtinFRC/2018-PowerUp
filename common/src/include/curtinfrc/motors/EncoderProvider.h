#pragma once

#include <stdint.h>

namespace curtinfrc {
  class EncoderProvider {
  public:
    virtual uint64_t GetEncoder() = 0;
  };
}