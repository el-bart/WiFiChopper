#ifndef INCLUDE_NET_TIMERSELECT_HPP_FILE
#define INCLUDE_NET_TIMERSELECT_HPP_FILE

#include "Util/timedSelect.hpp"
#include "Net/Address.hpp"
#include "Net/Exception.hpp"

namespace Net
{

inline bool timedSelect(const Address& addr, const int fd, const double timeout)
{
  return Util::timedSelect( fd, timeout, [&addr]{ throw CallError(addr, "select"); } );
}

}

#endif
