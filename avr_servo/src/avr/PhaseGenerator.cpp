#include "config.hpp"
#include <avr/io.h>

#include "PhaseGenerator.hpp"

PhaseGenerator::PhaseGenerator(void):
  defMaskPB_( uint8_t(~( _BV(PB0) | _BV(PB1) | _BV(PB2) | _BV(PB3) | _BV(PB4) | _BV(PB5) )) ),
  defMaskPC_( uint8_t(~( _BV(PC0) | _BV(PC1) | _BV(PC2) | _BV(PC3) | _BV(PC4) | _BV(PC5) )) ),
  defMaskPD_( uint8_t(~(            _BV(PD3) | _BV(PD4) | _BV(PD5) | _BV(PD6) | _BV(PD7) )) )
{
  // setup output pins
  DDRB|=~defMaskPB_;
  DDRC|=~defMaskPC_;
  DDRD|=~defMaskPD_;
}

void PhaseGenerator::rise(void) const
{
  PORTB|=~defMaskPB_;
  PORTC|=~defMaskPC_;
  PORTD|=~defMaskPD_;
}

void PhaseGenerator::generate(Timer1 &t1, const EntryTable &et) const
{
  // restart timer
  t1.stop();
  t1.start();
  // generate proper phases
  for(uint8_t i=0; i<SERVO_COUNT; ++i)
  {
    // wait until proper time will come
    while( et[i].time_ > static_cast<uint8_t>(t1.get()/8) )
    { /* wait... */ };

    // ok - update ports!
    PORTB&=et[i].maskPB_|defMaskPB_;
    PORTC&=et[i].maskPC_|defMaskPC_;
    PORTD&=et[i].maskPD_|defMaskPD_;
  }

  // ensure app ports are down
  fall();
  // timer 1 is no longer needed
  t1.stop();
}

void PhaseGenerator::fall(void) const
{
  PORTB&=defMaskPB_;
  PORTC&=defMaskPC_;
  PORTD&=defMaskPD_;
}
