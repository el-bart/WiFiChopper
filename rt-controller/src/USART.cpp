#include "config.hpp"
#include <stdio.h>
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "USART.hpp"
#include "ScopedIntLock.hpp"

#define USART_UBRR(baud,f) ( ((f)/((baud)*16L)) -1 )

// TODO: add test for baud-rate error and show error if value is over 1%

//
// I/O queues and helper funcitons
//
namespace
{
constexpr uint8_t NO_READY_COMMAND = 0xFF;  // special mark

// buffers, counters, stuff...
char    g_recv[2][QUEUE_RECV_SIZE+1];   // command buffers (including space for '0')
uint8_t g_recvNew;                      // index of new command buffer
uint8_t g_recvNewSize;                  // size of new command
uint8_t g_recvReady;                    // command ready to be ready or NO_READY_COMMAND
// buffer to be used by user, externally
char    g_userBuf[QUEUE_RECV_SIZE+1];   // data are copyied here
} // unnamed namespace


// USART RX completed interrupt
ISR(USART_RXC_vect)
{
  // read form hardware ASAP
  const char b = UDR;

  // got end of command?
  if(b=='\n' || b=='\r')
  {
    if(g_recvNewSize==0)                    // skip series of EOLs
      return;
    g_recv[g_recvNew][g_recvNewSize] = 0;   // terminate bufer with 0
    if(g_recvReady==NO_READY_COMMAND)       // can signal new data?
    {
      g_recvReady = g_recvNew;              // mark this buffer as ready
      g_recvNew   = (g_recvNew+1)%2;        // choose next command buffer
    }
    g_recvNewSize        = 0;               // new buffer is empty
    g_recv[g_recvNew][0] = 0;               // reset content (just in case)
    return;                                 // all is done here
  }

  // regular char - add it to the buffer
  if(g_recvNewSize==QUEUE_RECV_SIZE)        // command too long?
  {
    g_recvNewSize        = 0;               // mark buffer as empty
    g_recv[g_recvNew][0] = 0;               // reset content (just in case)
  }

  // regular char, that fits in the buffer - add it!
  g_recv[g_recvNew][g_recvNewSize] = b;
  ++g_recvNewSize;
}


void USART::init(void)
{
  // mark all buffers as empty
  g_recv[0][0]  = 0;
  g_recv[1][0]  = 0;
  // use first buffer in row
  g_recvNew     = 0;
  g_recvNewSize = 0;
  // no data ready for reception
  g_recvReady   = NO_READY_COMMAND;

  // clock divider register (computed from baud rate and oscilator frequency)
  UBRRH=(uint8_t)( (USART_UBRR(USART_BAUD, F_CPU)>>8) & 0x00FF );
  UBRRL=(uint8_t)( (USART_UBRR(USART_BAUD, F_CPU)>>0) & 0x00FF );

  // enable transciever
  UCSRB|= _BV(RXEN);    // RX enable
  UCSRB|= _BV(TXEN);    // TX enable

  // configure proper pins as in (RX) and out (TX)
  DDRD &=~_BV(PD0);     // RX as in
  PORTD|= _BV(PD0);     // RX high with pull-up
  DDRD |= _BV(PD1);     // TX as out

  // enable interrupts
  UCSRB|= _BV(RXCIE);   // RX complete
  //UCSRB|= _BV(TXCIE);   // TX complete
}

char* USART::getNextCommand(void)
{
  ScopedIntLock sil;                        // disable interrupts here
  if( g_recvReady == NO_READY_COMMAND )     // if no command is available, just return nullptr
    return nullptr;
  // copy data to output buffer
  {
    char* in  = g_recv[g_recvReady];     // source
    char* out = g_userBuf;               // destination
    for(; *in!=0; ++in, ++out)              // copy each byte
      *out = *in;                           // copy data
    *out = 0;                               // add terminating zero
  }
  g_recvReady = NO_READY_COMMAND;           // signal that buffer is ready to be reused
  // return it to the user
  return g_userBuf;
}
