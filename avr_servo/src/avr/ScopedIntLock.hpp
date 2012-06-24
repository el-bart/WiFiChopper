#ifndef INCLUDE_SCOPEDINTLOCK_HPP_FILE
#define INCLUDE_SCOPEDINTLOCK_HPP_FILE

#include "config.hpp"
#include <avr/interrupt.h>

#include "Noncopyable.hpp"


struct ScopedIntLock: private Noncopyable
{
  ScopedIntLock(void)
  {
    cli();
  }
  ~ScopedIntLock(void)
  {
    sei();
  }
}; // struct ScopedIntLock

#endif
