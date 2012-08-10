#include <iterator>
#include <algorithm>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <sys/select.h>

#include "Net/Channel.hpp"
#include "Net/timedSelect.hpp"
#include "Util/TimeoutClock.hpp"
#include "Util/tabSize.hpp"

namespace Net
{

Channel::Channel(Address addr, Util::UniqueDescriptor sock):
  addr_( std::move(addr) ),
  sock_( std::move(sock) )
{
  if( sock_.get() < 0 )
    throw InvalidSocket(addr_);
}


void Channel::write(const Data &in)
{
  size_t done = 0;
  size_t left = in.size();
  while( left > 0 )
  {
    const int ret = ::write( sock_.get(), in.data()+done, left );
    if(ret<=0)
      throw CallError(addr_, "write");
    done += ret;
    left -= ret;
  }
}


void Channel::read(Data &out, const double timeout)
{
  // wait...
  const Util::TimeoutClock tout(timeout);
  if( !timedSelect( addr_, sock_.get(), tout.remaining() ) )
    throw Timeout(addr_);
  // ok - we have some data
  uint8_t buf[8*1024];
  const int cnt = ::read( sock_.get(), buf, Util::tabSize(buf) );
  if( cnt<=0 )
    throw CallError(addr_, "read");
  // copy to the output buffer
  out.reserve( out.size() + cnt );
  copy( buf, buf+cnt, std::back_insert_iterator<Data>(out) );
}

}
