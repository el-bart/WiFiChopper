#ifndef INCLUDE_USART_HPP_FILE
#define INCLUDE_USART_HPP_FILE

#include "config.hpp"
#include <inttypes.h>

#include "Noncopyable.hpp"
#include "CommQueues.hpp"

/** \brief set of operations on serial interface.
 */
class USART: private Noncopyable
{
public:
  /** \brief initialize USART.
   */
  USART(QueueSend &qSend, QueueRecv &qRecv);
  ~USART(void);

  /** \brief signal begining of the sending data.
   */
  void sendData(void);

  void enable(void);
  void disable(void);
}; // class USART

#endif
