#ifndef INCLUDE_ADCREADER_HPP_FILE
#define INCLUDE_ADCREADER_HPP_FILE

#include "config.hpp"

#include <avr/io.h>
#include <inttypes.h>


/** \brief class for reading A/D converter values.
 *
 *  startRead*() methods initialize A/D conversion for a given ADC input.
 *  while it is started end can be checked with isReadReady() method.
 *  when it returns true, getRead() must be used to obtain value and
 *  clear flags. to get value with blocking read just call startRead*() for
 *  a given value and then getRead() - call will block, waiting for the
 *  result.
 */
class AdcReader
{
public:
  /** \brief enables ADC converter on required ports.
   */
  AdcReader(void);

  void startReadOX(void)  { startReadAdc( 0 ); }
  void startReadOY(void)  { startReadAdc( _BV(MUX0) ); }
  void startReadOZ(void)  { startReadAdc( _BV(MUX1) ); }
  void startReadVin(void) { startReadAdc( _BV(MUX1) | _BV(MUX0) ); }

  bool isReadReady(void) const { return (ADCSRA & _BV(ADIF) ); }
  uint8_t getRead(void);

private:
  void startReadAdc(uint8_t mux);
};

#endif
