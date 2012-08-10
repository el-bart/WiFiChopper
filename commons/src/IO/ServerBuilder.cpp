#include "IO/ServerBuilder.hpp"


namespace IO
{

ServerBuilder::ServerBuilder(const Net::Address& addr, Key key):
  s_(addr),
  key_( std::move(key) )
{
}

LineComm ServerBuilder::buildImpl(void)
{
  return LineComm( s_.accept(), key_ );
}

}
