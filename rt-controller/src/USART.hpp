/*
 * USART.hpp
 *
 */
#ifndef INCLUDE_USART_HPP_FILE
#define INCLUDE_USART_HPP_FILE

#include <stdio.h>
#include <inttypes.h>

/** \brief set of operations on serial interface.
 */
struct USART
{
  /** \brief initialize USART.
   */
  static void init(void);

  /** \brief send singe byte.
   *  \param b byte to send.
   */
  static void send(uint8_t b);
  /** \brief send array.
   *  \param b    pointer to the array to be sent.
   *  \param size array'y size.
   */
  static void send(uint8_t *b, size_t size);

  /** \brief get input queue size.
   *  \reutrn size of the input queue.
   */
  static size_t inQueueSize(void);
  /** \brief get one byte form the usart.
   *  \return data read.
   */
  static uint8_t receive(void);
}; // struct USART

#endif
