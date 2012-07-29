#ifndef INCLUDE_PROCESSOR_HPP_FILE
#define INCLUDE_PROCESSOR_HPP_FILE

#include "Tokenizer.hpp"
#include "AdcReadsWrapper.hpp"


class Processor
{
public:
  void process(char *buf, const AdcReadsWrapper::Reads &reads);

private:
  void errorFlash(const char *msg=nullptr);
  void handleHello(Tokenizer& tokenizer);
  void handleAccel(Tokenizer& tokenizer, const AdcReadsWrapper::Reads &reads);
  void handleEng(Tokenizer& tokenizer);
  void handleEngSet(Tokenizer& tokenizer);
  void handleVin(Tokenizer& tokenizer, const AdcReadsWrapper::Reads &reads);
};

#endif
