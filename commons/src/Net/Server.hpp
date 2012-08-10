#ifndef INCLUDE_NET_SERVER_HPP_FILE
#define INCLUDE_NET_SERVER_HPP_FILE

#include "Util/UniqueDescriptor.hpp"
#include "Net/Address.hpp"
#include "Net/Channel.hpp"
#include "Net/Exception.hpp"


namespace Net
{

class Server
{
public:
  explicit Server(Address addr);

  Channel accept(void);

private:
  Util::UniqueDescriptor openSocket(void) const;

  Address                addr_;
  Util::UniqueDescriptor srv_;
};

}

#endif
