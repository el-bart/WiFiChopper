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
  // this is enough to decide that we're not operating
  wdt_enable(WDTO_120MS);
}

void Watchdog::reset(void)
{
  wdt_reset();
}

void Watchdog::disable(void)
{
  wdt_disable();
}
