#ifndef INCLUDE_NET_TIMERSELECT_HPP_FILE
#define INCLUDE_NET_TIMERSELECT_HPP_FILE

#include "Net/Address.hpp"
#include "Net/Exception.hpp"

namespace Net
{

bool timedSelect(const Address &addr, int fd, double timeout);

}

#endif
