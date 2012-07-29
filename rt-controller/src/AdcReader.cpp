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
  ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADPS1) | _BV(ADPS0);
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
