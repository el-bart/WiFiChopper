#include <cmath>
#include <sys/select.h>

#include "Util/timedSelect.hpp"

namespace Util
{

bool timedSelect(const int fd, const double timeout, std::function<void(void)> onError)
{
  const long     sec  = static_cast<long>( floor(timeout) );
  const long     usec = static_cast<long>( (timeout-sec)*1000*1000 );
  struct timeval tv   = { sec, usec };
  // select proper descriptor
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET(fd, &rfds);
  // and wait...
  const int ret = select( fd+1, &rfds, nullptr, nullptr, &tv );
  if( ret==-1 )
  {
    onError();
    return false;   // this line should not be reached
  }
  // return info if timeout has occured or not
  return ret!=0;
}

}
