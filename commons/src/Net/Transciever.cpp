#include <utility>
#include <unistd.h>

#include "Net/Transciever.hpp"


namespace Net
{


Transciever::Transciever(Util::UniqueDescriptor&& rep):
  rep_( std::move(rep) )
{
  // TODO
}


void Transciever::send(const uint8_t *buf, const uint8_t size)
{
  // TODO
}


void Transciever::read(const uint8_t *buf, const uint8_t size)
{
  // TODO
}

}
