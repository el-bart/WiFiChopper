#ifndef INCLUDE_UTIL_TIMEOUTCLOCK_HPP_FILE
#define INCLUDE_UTIL_TIMEOUTCLOCK_HPP_FILE

#include "Util/ClockTimer.hpp"

namespace Util
{

class TimeoutClock
{
public:
  explicit TimeoutClock(double timeout):
    timeout_(timeout)
  { }

  double remaining(void) const
  {
    const auto e = clk_.elapsed();
    if( timeout_ <= e )
      return 0.0;
    return timeout_ - e;
  }

private:
  ClockTimerRT clk_;
  double       timeout_;
};

}

#endif
