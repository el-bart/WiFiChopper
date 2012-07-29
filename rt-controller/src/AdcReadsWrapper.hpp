#ifndef INCLUDE_ADCREADSWRAPPER_HPP_FILE
#define INCLUDE_ADCREADSWRAPPER_HPP_FILE

#include "config.hpp"

#include "AdcReader.hpp"


/** \brief wrapper object for reading and buffering ADC results.
 */
class AdcReadsWrapper
{
public:
  struct Reads
  {
    Reads(void):
      ox_(0),
      oy_(0),
      oz_(0),
      vin_(0)
    { }

    uint8_t ox_;
    uint8_t oy_;
    uint8_t oz_;
    uint8_t vin_;
  };

  /** \brief init and prepare first read data.
   */
  AdcReadsWrapper(void);

  /** \brief perform one step for reading. call this function often.
   */
  void step(void);

  /** \brief get result of the average of the last 1 or 2 read(s).
   */
  Reads getLast(void) const { return last_; }

private:
  AdcReader adc_;
  uint8_t   step_;
  Reads     now_;
  Reads     last_;
}; // AdcReadsWrapper

#endif
