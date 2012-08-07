#include "config.hpp"
#include <avr/io.h>

#include "servoNoToMask.hpp"

void servoNoToMask(const uint8_t servoNo, Entry &out)
{
  if(servoNo<6) // PORTB?
  {
    const uint8_t tmp=servoNo;
    out.maskPB_&=~_BV(tmp);
    return;
  }
  if(servoNo<12) // PORTC?
  {
    const uint8_t tmp=servoNo-6;
    out.maskPC_&=~_BV(tmp);
    return;
  }
  if(servoNo<17) // PORTD?
  {
    const uint8_t tmp=servoNo-12+3;
    out.maskPD_&=~_BV(tmp);
    return;
  }
  // if we're here, it means invalid servo number. entry will be skipped...
} // servoNoToMask()
