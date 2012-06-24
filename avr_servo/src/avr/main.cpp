/*
 * AVR_servo
 *
 * servo controller driver, written for AVR uC.
 *
 */
#include "config.hpp"       // this file must be included as first one!

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "USART.hpp"
#include "Timer1.hpp"
#include "Timer2.hpp"
#include "Watchdog.hpp"
#include "PowerSave.hpp"
#include "ChronoTable.hpp"
#include "CommProtocol.hpp"
#include "PhaseGenerator.hpp"
#include "uassert.hpp"


// handle unknown interruption(s)
ISR(BADISR_vect)
{
  uassert(!"unhandled interrupt");
  for(;;) {};   // watchdog will do the rest
}


namespace
{
inline Positions getDefaultPositions(const PersistentSettings &s)
{
  Positions out;
  for(uint8_t i=0; i<SERVO_COUNT; ++i)
    out[i]=s.posDef().read(i);
  return out;
} // getDefaultPositions()

inline void waitUntilStep(const Timer2 &t2, const uint8_t step)
{
  while( t2.currentStep()<step )
    PowerSave::idle();
} // waitUntilStep()
} // unnamed namespace


//
// MAIN
//
int main(void)
{
  Watchdog           wdg;                                           // initialize watchdog (disable by default)
  PersistentSettings settings;                                      // persistent settings
  ChronoTable        chronoTable( getDefaultPositions(settings) );  // chronology table
  QueueSend          qSend;                                         // output queue
  QueueRecv          qRecv;                                         // input queue
  USART              usart(qSend, qRecv);                           // prepare USART to work
  Timer1             t1;                                            // configure T1
  Timer2             t2;                                            // configure T2
  PhaseGenerator     phaseGen;                                      // controling facility
  CommProtocol       proto(qSend, qRecv, settings);                 // protocol parser
#ifdef NDEBUG
  // watchdog should be enabled only in release modes
  wdg.enable();                                                     // enable watchdog
#endif
  sei();                                                            // allow interrupts globally

  const unsigned int cycleLen=20*2;
  while(true)
  {

    //
    // phase I: first rising edge (hard-RT)
    //
    phaseGen.rise();
    waitUntilStep(t2, 1);

    //
    // phase II: phase generation (hard-RT)
    //
    phaseGen.generate( t1, chronoTable.currentEntries() );
    phaseGen.fall();

    //
    // phase III: doing I/O processing
    //
    usart.enable();
    proto.process( chronoTable.currentPos() );
    usart.sendData();

    //
    // phase IV: computing output data for phase generation
    //
    chronoTable.update();                   // NOTE: this may take few ms...

    //
    // phase V: waiting for next-to-last cycle and disable USART
    //
    waitUntilStep(t2, cycleLen-1);
    usart.disable();                        // disable USART, to ensure no delays in hard-RT part

    //
    // phase VI: do final tasks and close the cycle
    //
    waitUntilStep(t2, cycleLen);            // wait until final step is reached
    wdg.reset();                            // reset watchdog
    t2.resetStep();                         // reset step counter
    uassert( t2.currentStep()==cycleLen );  // if this fails, it means computations last too long...
                                            // NOTE: it may fail, when heavy stress is presented in debug mode.
  } // while(true)

  // code never reaches here
  return 0;
} // main()

