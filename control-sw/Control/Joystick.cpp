#include <algorithm>
#include <sstream>
#include <cstring>
#include <cerrno>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/joystick.h>

#include "Control/Joystick.hpp"
#include "Util/tabSize.hpp"


namespace Control
{

namespace
{

Util::UniqueDescriptor openDevice(const std::string& path)
{
  return Util::UniqueDescriptor{ open( path.c_str(), O_RDONLY ) };
}


template<typename T>
void callIoctl(const int fd, const std::string& dev, const int req, T t)
{
  if( ioctl(fd, req, t) == -1 )
    throw Util::Exception( UTIL_LOCSTRM << "ioctl(" << req << ") on device '" << dev << "' failed: " << strerror(errno) );
}


Joystick::Info readInfo(const std::string& path, const int fd)
{
  Joystick::Info info;

  int version;
  callIoctl(fd, path, JSIOCGVERSION, &version);
  info.driverVer_[0] = version >> 16;
  info.driverVer_[1] = (version >> 8) & 0xFF;
  info.driverVer_[2] = version & 0xFF;

  uint8_t axis;
  callIoctl(fd, path, JSIOCGAXES, &axis);
  if( axis < 2 )
    throw Util::Exception( UTIL_LOCSTRM << "device reports to have only " << axis << " axis, while at least 2 are required" );
  info.axis_ = axis;

  uint8_t buttons;
  callIoctl(fd, path, JSIOCGBUTTONS, &buttons);
  if( buttons < 1 )
    throw Util::Exception( UTIL_LOCSTRM << "device reports to have only " << buttons << " buttons, while at least 1 is required" );
  info.buttons_ = buttons;

  callIoctl(fd, path, JSIOCGNAME( Util::tabSize(info.name_) ), info.name_);

  return info;
}


std::string createName(const Joystick::Info& info)
{
  // create output name out of gathered data
  std::stringstream ss;
  ss << info.name_ << " (drv. ver. " << info.driverVer_[0] << "." << info.driverVer_[1] << "." << info.driverVer_[2] << "): "
     << info.axis_ << "ax. + " << info.buttons_ << "but.";
  return ss.str();
}

} // unnamed namespace


Joystick::Joystick(const std::string& path):
  Input( openDevice(path), path ),
  info_( readInfo( path, rawDescriptor() ) ),
  name_( createName(info_) )
// axis_( new int[info_.axis_] )
//  buttons_( new char[info_.buttons_] )
{ }


void Joystick::updateImpl(void)
{
  // TODO
}


const std::string& Joystick::nameImpl(void)
{
  return name_;
}

}
