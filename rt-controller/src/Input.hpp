#ifndef INCLUDE_INPUT_HPP_FILE
#define INCLUDE_INPUT_HPP_FILE

#include "config.hpp"
#include <inttypes.h>

#include "AdcReadsWrapper.hpp"
#include "Processor.hpp"


class Input
{
public:
  Input(void);

  void step(const AdcReadsWrapper& adc);

private:
  Processor proc_;
  // data gathering facility
  char      cmd_[INPUT_BUF_SIZE];
  uint8_t   pos_;
  bool      applied_;
};

#endif
