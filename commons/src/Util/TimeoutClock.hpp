#ifndef INCLUDE_UTIL_TIMEOUTCLOCK_HPP_FILE
#define INCLUDE_UTIL_TIMEOUTCLOCK_HPP_FILE

#include "Util/Clock.hpp"

namespace Util
{

template<typename Duration>
class TimeoutClock
{
public:
  explicit TimeoutClock(Duration timeout):
    timeout_(timeout)
  { }

  Duration remaining(void) const
  {
    const auto e = clk_.elapsed<Duration>();
    if( timeout_ < e )
      return Duration(0);
    return timeout_ - e;
  }

private:
  Clock    clk_;
  Duration timeout_;
};

}

#endif
