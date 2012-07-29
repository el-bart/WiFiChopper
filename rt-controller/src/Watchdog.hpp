#ifndef INCLUDE_WATCHDOG_HPP_FILE
#define INCLUDE_WATCHDOG_HPP_FILE

#include "config.hpp"

/** \brief watchdog operating class.
 */
class Watchdog
{
public:
  /** \brief initialies watchdog timer.
   *  \note watchdog starts in disabled mode.
   */
  Watchdog(void);
  /** \brief do cleanup. disables watchdog.
   */
  ~Watchdog(void);
  /** \brief enables watchdog. it has to be done explicitly, to prevent restart during init.
   */
  void enable(void);
  /** \brief reset timer, confirming system is alive.
   */
  void reset(void);

private:
  void disable(void);
}; // class Watchdog

#endif
