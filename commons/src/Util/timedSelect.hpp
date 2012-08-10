#ifndef INCLUDE_UTIL_TIMERSELECT_HPP_FILE
#define INCLUDE_UTIL_TIMERSELECT_HPP_FILE

#include <functional>

namespace Util
{
bool timedSelect(int fd, double timeout, std::function<void(void)> onError);
}

#endif
