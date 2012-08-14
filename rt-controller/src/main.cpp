#include "config.hpp"       // this file must be included as first one!
#include <avr/io.h>
#include <avr/interrupt.h>

#include "USART.hpp"
#include "Watchdog.hpp"
#include "AdcReadsWrapper.hpp"
#include "Input.hpp"
#include "CommLostProtector.hpp"


//
// MAIN
//
int main(void)
{
  Watchdog        wdg;      // prepare watchdog for work (keep this object as the first one to init!)
  wdg.enable();             // enable watchdog
  wdg.reset();
  AdcReadsWrapper adc;      // A/D converter with wrapped reads
  wdg.reset();
  USART::init();            // configure serial interface
  wdg.reset();
  Input           input;    // input data collect-and-response facility
  wdg.reset();
  sei();                    // enable interrupts

  // main loop (never ending)
  while(true)
  {
    wdg.reset();            // watchdog reset
    adc.step();             // ADC processing step
    input.step(adc);        // process input data
    // handle reset, if communication is lost. note that this mechanism is optional
    // and has to be explicitly enabled by used using 'enableclp' command.
    CommLostProtector::reactIfTimeout();
  }

  // this code is never reached
  return 0;
} // main()
