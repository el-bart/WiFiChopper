#include "config.hpp"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>

#include "CommLostProtector.hpp"


namespace
{
volatile uint8_t  g_cycles  = 0;
constexpr uint8_t second    = F_CPU/1024/256;  // prescaller is set to 1024 while counter interrupts every 256 such a cycles
constexpr uint8_t maxCycles = 1*second;        // maximum time w/o command
static_assert( maxCycles <= 0xFF/2, "values are too big - use uin16_t instead" );
} // unnamed namespace

// interruption on T0 overflow
ISR(TIMER0_OVF_vect)
{
  ++g_cycles;
}


void CommLostProtector::enable(void)
{
  g_cycles = 0;
  // use Timer0 for counting
  TCCR0 |= _BV(CS02) | _BV(CS00);               // set preskaler to 1024
  TIMSK |= _BV(TOIE0);                          // enable interrupt from T0
}


void CommLostProtector::reset(void)
{
  g_cycles = 0;
}


void CommLostProtector::reactIfTimeout(void)
{
  // no timeout?
  if( g_cycles < maxCycles )
    return;
  // ok - we're timed out...
  for(;;) { /* wait for wartchdog reset... */ }
}
