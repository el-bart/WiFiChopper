#include "config.hpp"
#include <avr/io.h>
#include <avr/interrupt.h>

#include "Timer1.hpp"


Timer1::Timer1(void)
{
  // keep stopped by default
  stop();
}

void Timer1::start(void)
{
  TCNT1=0x0000;         // zero counter before start
  TCCR1B&=~_BV(CS12);   // set prescaller to 8
  TCCR1B|= _BV(CS11);   // ...
  TCCR1B&=~_BV(CS10);   // ...
}

void Timer1::stop(void)
{
  TCCR1B&=~_BV(CS12);   // disable counter
  TCCR1B&=~_BV(CS11);   // ...
  TCCR1B&=~_BV(CS10);   // ...
}
