/*
 * uassert.cpp
 *
 */
#include "config.hpp"

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "uassert.hpp"

namespace
{
void debug_send_char(uint8_t c)
{
  // wait for transmition to be over
  while( !(UCSRA & (1<<UDRE)) );
  // send char
  UDR=c;
}

void debug_send_hex(uint8_t h)
{
  h&=0x0F;  // just in case
  if(h<10)
    debug_send_char('0'+h);
  else
    debug_send_char('A'+(h-10));
}
} // unnamed namespace

namespace detail
{

void uassertInternalImplementation(uint16_t line, const char *file)
{
  cli();    // disable interrupts

  // loop forever sending asseration details and blinking the led
  while(1)
  {
    //
    // send debug data to terminal
    //

    // send begin string:
    for(const char *tmp="asseration failed @ "; *tmp!=0; ++tmp)
      debug_send_char(*tmp);
    // send file name
    for(const char *tmp=file; *tmp!=0; ++tmp)
      debug_send_char(*tmp);
    // send separator
    for(const char *tmp=":0x"; *tmp!=0; ++tmp)
      debug_send_char(*tmp);
    // send line number if hex
    for(int i=3; i>=0; --i)
      debug_send_hex( line>>(i*4) );
    // send end-of-line
    for(const char *tmp="\r\n"; *tmp!=0; ++tmp)
      debug_send_char(*tmp);

    //
    // blink led
    //

    PORTD|= _BV(5); // LED on
    _delay_ms(250);
    PORTD&=~_BV(5); // LED off
    _delay_ms(250);

  } // while(true)
} // uassertInternalImplementation()

} // namespace detail
