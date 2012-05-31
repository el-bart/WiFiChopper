/*
 * USART.cpp
 *
 */
#include "config.hpp"

#include <avr/io.h>
#include <avr/interrupt.h>

#include "uassert.hpp"
#include "USART.hpp"
#include "Queue.hpp"

#define USART_UBRR(baud,f) ( ((f)/((baud)*16L)) -1 )

// TODO: add test for baud-rate error and show error if value is over 1%

//
// I/O queues
//
namespace
{
Queue g_inQ;
Queue g_outQ;
} // unnamed namespace


static inline void sendDataImpl(void)
{
  uassert( UCSRA & _BV(UDRE) );
  UDR=g_outQ.pop();
}

// USART RX completed interrupt
ISR(USART_RXC_vect)
{
  const uint8_t c=UDR;          // read form hardware ASAP
  if( g_inQ.full() )            // if queue is full, drop last element
    g_inQ.pop();
  g_inQ.push(c);                // enqueue new byte
}

// USART TX completed interrupt
ISR(USART_TXC_vect)
{
  if( !g_outQ.empty() )         // if have something to send
    sendDataImpl();
}

// USART data register is empty interrupt
ISR(USART_UDRE_vect)
{
  UCSRB&=~_BV(UDRIE);           // data registry empty - disable interrupt
  if( g_outQ.size()>0 )         // if data register is empty and we have data to send
    sendDataImpl();             // we can send it now
}


void USART::init(void)
{
  // clock devider register (computed from baud rate and oscilator frequency)
  UBRRH=(uint8_t)( (USART_UBRR(USART_BAUD, F_CPU)>>8) & 0x00FF );
  UBRRL=(uint8_t)( (USART_UBRR(USART_BAUD, F_CPU)>>0) & 0x00FF );
  // set single speed
  UCSRA =0x00;
  UCSRA&=~_BV(U2X);

  // enable interrupts
  UCSRB|= _BV(RXCIE);   // RX complete
  UCSRB|= _BV(TXCIE);   // TX complete
  // enable transciever
  UCSRB|= _BV(RXEN);    // RX enable
  UCSRB|= _BV(TXEN);    // TX enable

  // configure 8-bit transmition mode
  // NOTE: there is a bug in spec at this point - UCSZ[01] must be 0 not 1 here!
  UCSRB&=~_BV(UCSZ2);   // 0
  UCSRC&=~_BV(UCSZ1);   // 0 (!)
  UCSRC&=~_BV(UCSZ0);   // 0 (!)
  // configure no parity mode
  UCSRC&=~_BV(UPM1);
  UCSRC&=~_BV(UPM0);
  // configure one stop bit
  UCSRC&=~_BV(USBS);

  // other configuration options
  // TODO: should be UCSRC|= _BV(UMSEL);   // synchronous mode
  UCSRC&=~_BV(URSEL);   // synchronous mode
  UCSRC&=~_BV(UCPOL);   // clock polarity mode

  // configure proper pins as in (RX) and out (TX)
  DDRD &=~_BV(PD0);     // RX as in
  PORTD|= _BV(PD0);     // RX high with pull-up
  DDRD |= _BV(PD1);     // TX as out
}

void USART::send(uint8_t b)
{
  while( g_outQ.full() );           // wait for space in queue
  g_outQ.push(b);                   // enqueue next char to send
  if( g_outQ.size()==1 )            // this might be first char to send
    UCSRB|=_BV(UDRIE);              // signal on data registry empty.
                                    // if transmition has not yet started this will
                                    // send initial (first) byte as soon as USART is ready
}

void USART::send(uint8_t *b, size_t size)
{
  uassert(b!=NULL);
  for(size_t i=0; i<size; ++i)
    send(b[i]);
}

size_t USART::inQueueSize(void)
{
  return g_inQ.size();              // return input queue size
}

uint8_t USART::receive(void)
{
  while( g_inQ.empty() );           // wait for the data
  return g_inQ.pop();               // return read data
}
