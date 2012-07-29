#include "config.hpp"       // this file must be included as first one!

#include "Input.hpp"
#include "USART.hpp"
#include "utils.hpp"


Input::Input(void):
  pos_(0),
  applied_(false)
{
  cmd_[0] = 0;
}


void Input::step(const AdcReadsWrapper& adc)
{
  // if nothing to receive, just skip this step
  if( !USART::hasData() )
    return;

  // got something - read it char from the port
  const char c=USART::receive();

  // process new char
  if(c=='\n' || c=='\r')
  {
    // if command was already applied, just skip next new lines
    if(applied_)
      return;
    // end of line means processing is applied
    proc_.process( cmd_, adc.getLast() );
    applied_ = true;
    // reset command buffer
    cmd_[0]  = 0;
    pos_     = 0;
  }
  else
  {
    // reset buffer, if it is about to overflow
    if( pos_+1u>=tabSize(cmd_) )
      pos_=0;
    // if not processing, add char to the buffer
    cmd_[pos_] = c;
    ++pos_;
    cmd_[pos_] = 0;
    applied_   = false;
  }
}
