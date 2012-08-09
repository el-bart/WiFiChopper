#ifndef INCLUDE_UTIL_CLOCK_HPP_FILE
#define INCLUDE_UTIL_CLOCK_HPP_FILE

#include <chrono>

namespace Util
{

class Clock
{
private:
  typedef std::chrono::high_resolution_clock Type;

public:
  Clock(void):
    start_( Type::now() )
  { }

  template<typename Duration>
  Duration elapsed(void) const
  {
    const Type::time_point now   = Type::now();
    const auto             diff  = now-start_;
    const Duration         final = std::chrono::duration_cast<Duration>(diff);
    return final;
  }

private:
  Type::time_point start_;
};

}

#endif
