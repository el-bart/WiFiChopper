#include <utility>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "Net/Transciever.hpp"


namespace Net
{


Transciever::Transciever(Util::UniqueDescriptor&& rep, const Address& addr):
  rep_( std::move(rep) ),
  addr_(addr)
{
  if(rep_.get()==-1)
    throw InvalidDescriptor();
}


void Transciever::send(const uint8_t *buf, const uint8_t size)
{
  if( write(rep_.get(), buf, size)!=size )
    throw ErrorIO( addr_, strerror(errno) );
  // TODO
}


void Transciever::read(const uint8_t *buf, const uint8_t size, const std::chrono::milliseconds timeout)
{
  // TODO
}

}
