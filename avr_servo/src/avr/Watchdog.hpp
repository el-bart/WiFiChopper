#ifndef INCLUDE_WATCHDOG_HPP_FILE
#define INCLUDE_WATCHDOG_HPP_FILE

#include "config.hpp"
#include "Noncopyable.hpp"

/** \brief watchdog operating class.
 */
class Watchdog: private Noncopyable
{
public:
  /** \brief initialies watchdog timer.
   *  \note watchdog starts in disabled mode. it is so since, after the
   *        reset it is still active and this would cause problems, while reading EEPROM
   *        values (it takes some time). to enable it enable() must be called explicitly.
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
