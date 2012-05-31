/*
 * Queue.hpp
 *
 */
#ifndef INCLUDE_QUEUE_HPP_FILE
#define INCLUDE_QUEUE_HPP_FILE

#include "config.hpp"

#include <stdio.h>
#include <inttypes.h>

/** \brief data FIFO queue of a compile-time-defined size.
 */
class Queue
{
public:
  /** \brief create queue.
   */
  Queue(void);

  /** \brief push data to the queue.
   *  \param b byte to be pushed.
   */
  void push(uint8_t b);

  /** \brief gets current queue size.
   */
  size_t size(void) const;
  /** \brief returnes true if queue is full, false otherwise.
   */
  bool full(void) const
  {
    return size()==QUEUE_MAX_SIZE;
  }
  /** \brief returnes true if queue is empty, false otherwise.
   */
  bool empty(void) const
  {
    return size()==0u;
  }

  /** \brief peeks given byte in the queue.
   *  \param pos position to peek byte from.
   *  \reutrn byte on a givne position.
   */
  uint8_t peek(size_t pos) const;
  /** \brief pops byte from queue.
   *  \return byte read from the queue.
   */
  uint8_t pop(void);
  /** \brief pops given elements count from the queue.
   *  \param n number of bytes to pop.
   */
  void popCount(size_t n);

private:
  // NOTE: these variables better be volatile, since they are in use in interrupts.
  volatile uint8_t queue_[QUEUE_MAX_SIZE];
  volatile size_t  begin_;
  volatile size_t  size_;
}; // class Queue

#endif
