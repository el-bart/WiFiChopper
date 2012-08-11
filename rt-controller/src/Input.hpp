#ifndef INCLUDE_INPUT_HPP_FILE
#define INCLUDE_INPUT_HPP_FILE

#include "config.hpp"

#include "AdcReadsWrapper.hpp"
#include "Processor.hpp"


class Input
{
public:
  void step(const AdcReadsWrapper& adc);

private:
  Processor proc_;
};

#endif
