#include "IO/ServerBuilder.hpp"


namespace IO
{

ServerBuilder::ServerBuilder(const Net::Address& addr, Key key):
  s_(addr),
  key_( std::move(key) )
{
}

LineCommPtr ServerBuilder::buildImpl(void)
{
  return LineCommPtr( new LineCommNetwork( s_.accept(), key_ ) );
}

}
