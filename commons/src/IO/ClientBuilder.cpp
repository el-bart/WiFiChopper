#include <algorithm>

#include "IO/ClientBuilder.hpp"
#include "Net/connectToServer.hpp"


namespace IO
{

ClientBuilder::ClientBuilder(const Net::Address& addr, Key key):
  addr_(addr),
  key_( std::move(key) )
{ }

LineComm ClientBuilder::buildImpl(void)
{
  Net::Channel chn = Net::connectToServer(addr_);
  return LineComm( std::move(chn), key_ );
}

}
