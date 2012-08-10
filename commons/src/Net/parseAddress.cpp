#include <cstdlib>

#include "Net/parseAddress.hpp"
#include "Net/Resolver.hpp"


namespace Net
{

Net::Address parseAddress(const char* hostStr, const char* portStr)
{
  const Net::Resolver resolver(hostStr);
  const uint16_t      port = atoi(portStr);
  return Net::Address( resolver[0], port );
}

}
