#include "config.hpp"
#include <avr/io.h>

#include "EngSpeed.hpp"


EngSpeed::EngSpeed(void)
{
  // rear engine enable as PWM / OC2
  setRear( RearEng{true,0} );       // stop rear engine
  DDRB  |= _BV(PB3) | _BV(PB0);     // configure output pins of port B
  DDRD  |= _BV(PD7);                // configure output pin of port D
  TCCR2 |= _BV(WGM21) | _BV(WGM20); // fast PWM mode, TOP=0xFF
  TCCR2 |= _BV(COM21);              // non-inverting PWM on OC2, clear on cmp-match, set on BOTTOM
  // NOTE: follwing settings gives 8MHz/(256*256)~=122Hz frequesncy, with variable fill-out.
  static_assert( F_CPU==8L*1000L*1000L, "CPU frequency has been changed, thus prescaler needs to be re-calbibrated" );
  TCCR2 |= _BV(CS22) | _BV(CS21);   // set preskaler to 256

  // main engine 1 enable as PWM / OC1A
  setMain1(0);                      // stop engine at the begining
  DDRB  |= _BV(PB1);                // configure output pin
  TCCR1A|= _BV(COM1A1);             // clear on TOP, set on MAX
  TCCR1A&=~_BV(COM1A0);             // ...
  TCCR1A&=~_BV(FOC1A);              // turn off force-output-compare

  // main engine 2 enable as PWM / OC1B
  setMain2(0);                      // stop engine at the begining
  DDRB  |= _BV(PB2);                // configure output pin
  TCCR1A|= _BV(COM1B1);             // clear on TOP, set on MAX
  TCCR1A&=~_BV(COM1B0);             // ...
  TCCR1A&=~_BV(FOC1B);              // turn off force-output-compare

  // configure timer
  TCCR1B|= _BV(WGM13);              // fast-PWM mode, TOP==ICR1
  TCCR1B|= _BV(WGM12);              // ...
  TCCR1A|= _BV(WGM11);              // ...
  TCCR1A&=~_BV(WGM10);              // ...
  // NOTE: following settings gives 8MHz/2^16==122Hz frequency,with variable fill-out.
  // configure TOP to MAX
  ICR1H=0xFF;
  ICR1L=0xFF;
  static_assert( F_CPU==8L*1000L*1000L, "CPU frequency has been changed, thus prescaler needs to be re-calbibrated" );
  TCCR1B&=~_BV(CS12);               // set prescaler to clk/1
  TCCR1B&=~_BV(CS11);               // ...
  TCCR1B|= _BV(CS10);               // ...
}


void EngSpeed::stop(void)
{
  setMain1(0);
  setMain2(0);
  setRear( RearEng{true, 0} );
}


uint8_t EngSpeed::getMain1(void) const
{
  return OCR1AH;
}

void EngSpeed::setMain1(const uint8_t v)
{
  OCR1AH=v;
  OCR1AL=v;
}


uint8_t EngSpeed::getMain2(void) const
{
  return OCR1BH;
}

void EngSpeed::setMain2(const uint8_t v)
{
  OCR1BH=v;
  OCR1BL=v;
}


EngSpeed::RearEng EngSpeed::getRear(void) const
{
  const RearEng out = { (PORTB & _BV(PB0))?true:false, OCR2 };
  return out;
}

void EngSpeed::setRear(const RearEng e)
{
  // set speed
  OCR2 = e.value_;
  // if speed is zero, disable both outputs
  if(e.value_==0)
  {
    PORTB &= ~_BV(PB0);
    PORTD &= ~_BV(PD7);
    return;
  }

  // set direction
  if(e.forward_)
  {
    PORTB |=  _BV(PB0);
    PORTD &= ~_BV(PD7);
  }
  else
  {
    PORTB &= ~_BV(PB0);
    PORTD |=  _BV(PD7);
  }
}
