#ifndef INCLUDE_TIMER0_HPP_FILE
#define INCLUDE_TIMER0_HPP_FILE

#include "config.hpp"
#include <avr/io.h>

#include "Noncopyable.hpp"

/** \brief 8-bit timer0 wrapper.
 */
class Timer2: private Noncopyable
{
public:
  Timer2(void);
  uint8_t currentStep(void) const;
  void resetStep(void);
}; // class Timer2

#endif
