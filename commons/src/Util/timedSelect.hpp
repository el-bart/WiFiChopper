#ifndef INCLUDE_UTIL_TIMERSELECT_HPP_FILE
#define INCLUDE_UTIL_TIMERSELECT_HPP_FILE

#include <vector>

#include "Util/Exception.hpp"

namespace Util
{
bool timedSelect(int fd, double timeout);

std::vector<int> timedSelect(std::vector<int> fds, double timeout);
}

#endif
