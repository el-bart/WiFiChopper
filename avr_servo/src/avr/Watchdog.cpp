#include "config.hpp"
#include <avr/wdt.h>

#include "Watchdog.hpp"

Watchdog::Watchdog(void)
{
  disable();
}

Watchdog::~Watchdog(void)
{
  disable();
}

void Watchdog::enable(void)
{
  // since whole cycle is 20ms, it means loosing ~22 cycles. if it happens,
  // it can be safely assumed something went VERY wrong.
  wdt_enable(WDTO_250MS);
}

void Watchdog::reset(void)
{
  wdt_reset();
}

void Watchdog::disable(void)
{
  wdt_disable();
}
