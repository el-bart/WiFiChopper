#ifndef INCLUDE_POWERSAVE_HPP_FILE
#define INCLUDE_POWERSAVE_HPP_FILE

#include "config.hpp"
#include <avr/sleep.h>

/** \brief power-consumption reducing calls.
 */
struct PowerSave
{
  /** \brief enter idle mode.
   */
  static void idle(void)
  {
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_mode();
  }
}; // struct PowerSave

#endif
