#ifndef INCLUDE_NET_PARSEADDRESS_HPP_FILE
#define INCLUDE_NET_PARSEADDRESS_HPP_FILE

#include "Net/Address.hpp"

namespace Net
{
Net::Address parseAddress(const char* hostStr, const char* portStr);
}

#endif
