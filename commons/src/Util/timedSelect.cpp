#include <algorithm>
#include <cstring>
#include <cerrno>
#include <cmath>
#include <sys/select.h>

#include "Util/timedSelect.hpp"

namespace Util
{

bool timedSelect(int fd, double timeout)
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
    throw Exception( UTIL_LOCSTRM << "select returned an error: " << strerror(errno) );

  // return info if timeout has occured or not
  return ret!=0;
}


std::vector<int> timedSelect(std::vector<int> fds, double timeout)
{
  // no elements, nothing to do...
  if( fds.empty() )
    return fds;

  const long     sec  = static_cast<long>( floor(timeout) );
  const long     usec = static_cast<long>( (timeout-sec)*1000*1000 );
  struct timeval tv   = { sec, usec };

  // select proper descriptor
  int    maxFd = -1;
  fd_set rfds;
  FD_ZERO(&rfds);
  for(int fd: fds)
  {
    FD_SET(fd, &rfds);
    maxFd = std::max(fd, maxFd);
  }

  // and wait...
  const int ret = select( maxFd+1, &rfds, nullptr, nullptr, &tv );
  if( ret==-1 )
    throw Exception( UTIL_LOCSTRM << "select returned an error: " << strerror(errno) );

  // translate these elements, that are not selected to -1
  for(int& fd: fds)
    if( !FD_ISSET(fd, &rfds) )
      fd = -1;
  return fds;
}

}
