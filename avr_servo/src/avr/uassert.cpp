#include "config.hpp"

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "uassert.hpp"

// TODO: c&p code
#define USART_UBRR(baud,f) ( ((f)/((baud)*16L)) -1 )

namespace
{
void init_usart(void)
{
  // clock divider register (computed from baud rate and oscilator frequency)
  UBRRH=(uint8_t)( (USART_UBRR(USART_BAUD, F_CPU)>>8) & 0x00FF );
  UBRRL=(uint8_t)( (USART_UBRR(USART_BAUD, F_CPU)>>0) & 0x00FF );

  UCSRB|= _BV(TXEN);    // TX enable
  DDRD |= _BV(PD1);     // TX as out
}

void debug_send_char(uint8_t c)
{
  // wait for transmition to be over
  while( !(UCSRA & (1<<UDRE)) )
  { /* wait */ };
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
  cli();        // disable interrupts
  init_usart(); // enable usart in a required mode

  // loop forever sending asseration details and blinking the led
  while(true)
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

    // wait a while
    _delay_ms(250);
    _delay_ms(250);
  } // while(true)
} // uassertInternalImplementation()

} // namespace detail
