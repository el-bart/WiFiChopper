#ifndef INCLUDE_USART_HPP_FILE
#define INCLUDE_USART_HPP_FILE

#include "config.hpp"
#include <avr/io.h>
#include <avr/pgmspace.h>


class USART
{
public:
  static void init(void);

  static char* getNextCommand(void);

  /** \brief send singe byte.
   *  \param b byte to send.
   */
  static void send(const char b)
  {
    sendDataImpl(b);
  }

  /** \brief sends given string.
   *  \param str string to be sent.
   */
  static void send(const char *str)
  {
    if(str==nullptr)
      return;
    for(; *str!='\0'; ++str)
      sendDataImpl(*str);
  }

  /** \brief sends given string, placed in FLASH not in RAM.
   *  \param str string present in the FLASH, to be sent.
   */
  static void sendFlash(const char *str)
  {
    if(str==nullptr)
      return;
    for(; pgm_read_byte(str)!='\0'; ++str)
      sendDataImpl( pgm_read_byte(str) );
  }

private:
  inline static void sendDataImpl(const char c)
  {
    while( !(UCSRA & _BV(UDRE)) ) {};   // wait until we can send
    UDR = c;                            // send character
  }
}; // class USART

#endif
