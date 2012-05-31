/*
 * main.cpp
 *
 */
#include "config.hpp"       // this file must be included as first one!

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>     // <avr/delay.h> once

#include "USART.hpp"
#include "uassert.hpp"


// handle unknown interruption(s)
ISR(BADISR_vect)
{
  uassert(!"unhandled interrupt");
}


void sendHex(uint8_t v)
{
  uassert(v<16);
  const char *lut="0123456789abcdef";
  USART::send(lut[v]);
}

void sendChar(uint8_t v)
{
  sendHex((v&0xF0)>>4);
  sendHex((v&0x0F)>>0);
}

void sendV(uint16_t v)
{
  sendChar((v&0xFF00)>>8);
  sendChar((v&0x00FF)>>0);
}


//
// MAIN
//
int main(void)
{
  // one big TODO !

  USART::init();                                    // configure serial interface
  sei();                                            // enable interrupts globally

  for(;;);      // TODO: just for now
  return 0;
} // main()

