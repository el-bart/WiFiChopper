#include "config.hpp"       // this file must be included as first one!

#include "Input.hpp"
#include "USART.hpp"
#include "utils.hpp"


void Input::step(const AdcReadsWrapper& adc)
{
  char* cmd = USART::getNextCommand();      // get next command
  if( cmd == nullptr )                      // if no command just return
    return;
  proc_.process( cmd, adc.getLast() );      // if new command is available, process it
}
