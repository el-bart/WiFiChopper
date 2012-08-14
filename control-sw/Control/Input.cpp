#include <algorithm>
#include <cstring>
#include <cerrno>

#include "Control/Input.hpp"


namespace Control
{

Input::DeviceOpenFailed::DeviceOpenFailed(const std::string& path):
  Util::Exception( UTIL_LOCSTRM << "unable to open input device '" << path << "': " << strerror(errno) )
{ }

Input::Input(Util::UniqueDescriptor fd, std::string path):
  fd_( std::move(fd) ),
  path_( std::move(path) ),
  movement_{0, 0},
  throttle_(0)
{
  if( fd_.get()==-1 )
    throw DeviceOpenFailed(path_);
}

}
