#include "config.hpp"

#include "AdcReader.hpp"

AdcReader::AdcReader(void)
{
  // setup pins as analog inputs (i.e. tri-state)
  DDRC  &= ~( _BV(PC3) | _BV(PC2) | _BV(PC1) | _BV(PC0) ); // input
  PORTC &= ~( _BV(PC3) | _BV(PC2) | _BV(PC1) | _BV(PC0) ); // disable pull-ups
  // initialize
  ADMUX  = _BV(ADLAR);
  // ADMUX / MUX3..MUX0
  ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADPS2) | _BV(ADPS1); // enable, start covnersion, set prescaler to 64 (i.e. 125kHz ADC clock)
  static_assert( F_CPU==8L*1000L*1000L, "CPU frequency has been changed, thus prescaler needs to be re-calbibrated" );
  // skip first results
  getRead();
}

uint8_t AdcReader::getRead(void)
{
  // just in case, wait for the results
  while( !isReadReady() ) {};
  ADCSRA |= _BV(ADIF);
  // return the result
  return ADCH;
}

void AdcReader::startReadAdc(uint8_t mux)
{
  // select channel
  ADMUX  &= ~( _BV(MUX3) | _BV(MUX2) | _BV(MUX1) | _BV(MUX0) );
  ADMUX  |= mux;
  // start conversion
  ADCSRA |= _BV(ADSC);
}
