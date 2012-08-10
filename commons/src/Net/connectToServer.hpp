#ifndef INCLUDE_NET_CONNECTTOSERVER_HPP_FILE
#define INCLUDE_NET_CONNECTTOSERVER_HPP_FILE

#include "Net/Channel.hpp"
#include "Net/Address.hpp"
#include "Net/Exception.hpp"

namespace Net
{

Channel connectToServer(const Address& addr);

}

#endif
