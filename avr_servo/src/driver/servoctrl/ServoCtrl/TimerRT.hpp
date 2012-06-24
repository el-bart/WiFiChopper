/*
 * TimerRT.hpp
 *
 */
#ifndef INCLUDE_SERVOCTRL_TIMERRT_HPP_FILE
#define INCLUDE_SERVOCTRL_TIMERRT_HPP_FILE

/* public header */

#include <ctime>

#include "ServoCtrl/Exception.hpp"

namespace ServoCtrl
{
namespace detail
{

/** \brief generic timer template to be used with different clocks.
 *
 *  possible clocks are:
 *
 *  CLOCK_REALTIME
 *    System-wide real-time clock.  Setting this clock requires appropriate
 *    privileges.
 *
 *  CLOCK_MONOTONIC
 *    Clock that cannot be set and represents monotonic time since some
 *    unspecified starting point.
 *
 *  CLOCK_MONOTONIC_RAW (since Linux 2.6.28; Linux-specific)
 *    Similar to CLOCK_MONOTONIC, but provides access to a raw hardware-based
 *    time that is not subject to NTP adjustments.
 *
 *  CLOCK_PROCESS_CPUTIME_ID
 *    High-resolution per-process timer from the CPU.
 *
 *  CLOCK_THREAD_CPUTIME_ID
 *    Thread-specific CPU-time clock.
 */
template<clockid_t TClock>
class ClockTimer
{
public:
  /** \brief start timer.
   */
  ClockTimer(void)
  {
    restart();
  }

  /** \brief restart timer.
   */
  void restart(void)
  {
    if( clock_gettime(TClock, &start_)!=0 )
      throw Exception{"clock_gettime(): cannot read timer"};
  }

  /** \brief returns time elapsed since timer's start.
   *  \return time since start, measured in seconds (and its fractions).
   */
  double elapsed(void) const
  {
    struct timespec now;
    if( clock_gettime(TClock, &now)!=0 )
      throw Exception{"clock_gettime(): cannot read timer"};

    const double sec =now.tv_sec -start_.tv_sec;
    const double nsec=now.tv_nsec-start_.tv_nsec;

    return sec+nsec/(1000*1000*1000);
  }

  /** \brief gets timer's resolution.
   *  \return minimal time that can be measured by this timer
   */
  static double resolution(void)
  {
    struct timespec res;
    if( clock_getres(TClock, &res)!=0 )
      throw Exception{"clock_getres(): cannot get timer's resolution"};
    return res.tv_sec+static_cast<double>(res.tv_nsec)/(1000*1000*1000);
  }

private:
  struct timespec start_;
}; // class ClockTimer

} // namespace detail


typedef detail::ClockTimer<CLOCK_REALTIME> TimerRT;

} // namespace ServoCtrl

#endif
