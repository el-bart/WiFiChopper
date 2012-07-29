#include "config.hpp"       // this file must be included as first one!
#include <avr/io.h>

#include "USART.hpp"
#include "Watchdog.hpp"
#include "AdcReadsWrapper.hpp"


//
// MAIN
//
int main(void)
{
  Watchdog        wdg;      // prepare watchdog for work (keep this object as the first one to init!)
  wdg.enable();             // enable watchdog
  AdcReadsWrapper adcw;     // A/D converter with wrapped reads
  wdg.reset();
  USART::init();            // configure serial interface
  wdg.reset();

  // main loop - never ends
  while(true)
  {
    wdg.reset();            // watchdog reset
    adcw.step();            // ADC processing step

    // TODO...
  }

  return 0;
} // main()

