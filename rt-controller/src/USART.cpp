/*
 * USART.cpp
 *
 */
#include "config.hpp"

#include <avr/io.h>
#include <avr/pgmspace.h>

#include "USART.hpp"

#define USART_UBRR(baud,f) ( ((f)/((baud)*16L)) -1 )

// TODO: add test for baud-rate error and show error if value is over 1%


namespace
{
inline void sendDataImpl(const char c)
{
  while( !(UCSRA & _BV(UDRE)) ) {}; // wait until we can send
  UDR = c;                          // send character
}
} // unnamed namespace


void USART::init(void)
{
  // clock devider register (computed from baud rate and oscilator frequency)
  UBRRH=(uint8_t)( (USART_UBRR(USART_BAUD, F_CPU)>>8) & 0x00FF );
  UBRRL=(uint8_t)( (USART_UBRR(USART_BAUD, F_CPU)>>0) & 0x00FF );

  // enable transciever
  UCSRB|= _BV(RXEN);    // RX enable
  UCSRB|= _BV(TXEN);    // TX enable

  // configure proper pins as in (RX) and out (TX)
  DDRD &=~_BV(PD0);     // RX as in
  PORTD|= _BV(PD0);     // RX high with pull-up
  DDRD |= _BV(PD1);     // TX as out
}


void USART::send(char b)
{
  sendDataImpl(b);
}


void USART::send(const char *str)
{
  if(str==nullptr)
    return;
  for(; *str!='\0'; ++str)
    sendDataImpl(*str);
}


void USART::sendFlash(const char *str)
{
  if(str==nullptr)
    return;
  for(; pgm_read_byte(str)!='\0'; ++str)
    sendDataImpl( pgm_read_byte(str) );
}


bool USART::hasData(void)
{
  return (UCSRA & _BV(RXC));        // check if data has arrived
}


char USART::receive(void)
{
  while( !hasData() ) {}            // wait until data is ready
  return UDR;                       // read the char and return it
}
