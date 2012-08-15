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


Joystick::Info readInfo(const std::string& path, const int fd, const Joystick::AxisMap& am)
{
  // sanity check
  if( !am.useXY_ && !am.useTh_ )
    throw Util::Exception( UTIL_LOCSTRM << "thing is in use - configuration does not make sense..." );

  // output structure
  Joystick::Info info;

  // read driver version number
  int version;
  callIoctl(fd, path, JSIOCGVERSION, &version);
  info.driverVer_[0] = version >> 16;
  info.driverVer_[1] = (version >> 8) & 0xFF;
  info.driverVer_[2] = version & 0xFF;

  // check required number of axies
  unsigned minAxies = 0;
  if( am.useXY_ )
    minAxies = std::max( am.ox_.num_, am.oy_.num_ );
  if( am.useTh_ )
    minAxies = std::max( minAxies, am.th_.num_ );
  ++minAxies;   // numbering start with zero
  // read number of axies
  uint8_t axis;
  callIoctl(fd, path, JSIOCGAXES, &axis);
  if( axis < minAxies )
    throw Util::Exception( UTIL_LOCSTRM << "device reports to have only " << axis << " axis, while at least " << minAxies << " are required" );
  info.axis_ = axis;

  // get number of buttons
  uint8_t buttons;
  callIoctl(fd, path, JSIOCGBUTTONS, &buttons);
  if( buttons < 1 )
    throw Util::Exception( UTIL_LOCSTRM << "device reports to have only " << buttons << " buttons, while at least 1 is required" );
  info.buttons_ = buttons;

  // get name
  callIoctl(fd, path, JSIOCGNAME( Util::tabSize(info.name_) ), info.name_);

  // return result
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


Joystick::Joystick(const std::string& path, AxisMap axisMap):
  Input( openDevice(path), path ),
  info_( readInfo( path, rawDescriptor(), axisMap ) ),
  name_( createName(info_) ),
  axisMap_( axisMap ),
  axis_( new int[info_.axis_] ),
  buttons_( new char[info_.buttons_] )
{
  bzero( axis_.get(),    info_.axis_    );
  bzero( buttons_.get(), info_.buttons_ );
}


void Joystick::updateImpl(void)
{
  // read
  js_event js;
  if( read( rawDescriptor(), &js, sizeof(js_event)) != sizeof(js_event) )
    throw Util::Exception( UTIL_LOCSTRM << "error reading device '" << path() << "': " << strerror(errno) );

  // update maps
  switch( js.type & (~JS_EVENT_INIT) )
  {
    case JS_EVENT_BUTTON:
         assert( js.number < info_.buttons_ && "button not reported by info!" );
         buttons_[js.number] = js.value;
         break;
    case JS_EVENT_AXIS:
         assert( js.number < info_.axis_ && "axis not reported by info!" );
         axis_[js.number] = js.value;
         break;
    default:
         // just ignore other calls
         break;
  }

  // set movement
  if( axisMap_.useXY_ )
  {
    // OX
    double ox = (axis_[axisMap_.ox_.num_]/(0xFFFF/2.0));
    if( axisMap_.ox_.invert_ )
      ox = -ox;
    // OY
    double oy = (axis_[axisMap_.oy_.num_]/(0xFFFF/2.0));
    if( axisMap_.oy_.invert_ )
      oy = -oy;
    // update
    setMovement( Movement{ox, oy} );
  }
  // set throttle
  if( axisMap_.useTh_ )
  {
    constexpr double maxVal= 0xFFFF;
    constexpr double half  = maxVal/2.0;
    double th = axis_[axisMap_.th_.num_] / half;    // map to [0..1]
    if( axisMap_.th_.invert_ )                      // axis invertion requested
      th = - th;
    if( th < 0.0 )                                  // there is no backward throttle
      th = 0.0;
    setThrottle( th );
  }
}


const std::string& Joystick::nameImpl(void)
{
  return name_;
}

}
