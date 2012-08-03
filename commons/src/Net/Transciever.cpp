#include <utility>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "Net/Transciever.hpp"


namespace Net
{


Transciever::Transciever(Util::UniqueDescriptor&& rep):
  rep_( std::move(rep) )
{
  if(rep_.get()==-1)
    throw InvalidDescriptor();
}


void Transciever::send(const uint8_t *buf, const uint8_t size)
{
  if( write(rep_.get(), buf, size)!=size )
    throw ErrorIO( strerror(errno) );
  // TODO
}


void Transciever::read(const uint8_t *buf, const uint8_t size, const std::chrono::milliseconds timeout)
{
  // TODO
}

}
