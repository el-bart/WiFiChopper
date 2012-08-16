#include <algorithm>
#include <cstring>
#include <cinttypes>
#include <cerrno>
#include <cassert>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/videodev2.h>

#include "UsrInt/FrameGrabberV4L.hpp"
#include "Util/timedSelect.hpp"


namespace UsrInt
{

namespace
{

int callIoctlImpl(const char* file, const unsigned line, int fd, int request, void* arg, bool checkForError = true)
{
  int r;
  // ignore interrupts
  do
  { r = ioctl(fd, request, arg); }
  while( r == -1 && EINTR == errno );
  // check for errors?
  if( checkForError && r == -1 )
    throw Util::Exception( Util::ErrStrm{} << file << ":" << line << ": error calling ioctl(" << request << "): " << strerror(errno) );
  // return resulting values
  return r;
}


// ugly, but reserves file:line info...
#define callIoctl(fd, req, arg) callIoctlImpl(__FILE__, __LINE__, fd, req, arg)


template<typename T>
void zeroMemory(T& t) { bzero( &t, sizeof(t) ); }

} // unnamed namespace


FrameGrabberV4L::FrameGrabberV4L(std::string devPath):
  devPath_( std::move(devPath) ),
  dev_( openDevice() )
{
  init();
}


cv::Mat FrameGrabberV4L::grabImpl(void)
{
  // check if the device is ready
  // TODO: hardcoded timeout value....
  if( !Util::timedSelect( dev_.get(), 4.0 ) )
    throw Util::Exception( UTIL_LOCSTRM << "timeout occured while waiting for the image" );

  // get buffer from the device
  ImageBuffer  img( new uint8_t[imageBufferSize_] );
  size_t       left = imageBufferSize_;
  do
  {
    const ssize_t ret = read( dev_.get(), img.get(), left );
    if( ret == -1 )
      throw Util::Exception( UTIL_LOCSTRM << "read of " << imageBufferSize_ << " bytes failed: " << strerror(errno) );
    left -= ret;
  }
  while( left > 0 );

  // convert to RGB
  cv::Mat tmp = toRGB( std::move(img) );

  // return image to the user
  return tmp;
}


Util::UniqueDescriptor FrameGrabberV4L::openDevice(void) const
{
  Util::UniqueDescriptor fd( open( devPath_.c_str(), O_RDWR | O_NONBLOCK ) );
  if( fd.get() == -1 )
    throw Util::Exception( UTIL_LOCSTRM << "unable to open video device '" << devPath_ << "': " << strerror(errno) );
  return fd;
}


void FrameGrabberV4L::init(void)
{
  // check device's capabilities
  {
    v4l2_capability cap;
    callIoctl( dev_.get(), VIDIOC_QUERYCAP, &cap );
    if( !( cap.capabilities & V4L2_CAP_VIDEO_CAPTURE ) )
      throw Util::Exception( UTIL_LOCSTRM << "'" << devPath_ << "' is not a V4Lv2 capture device" );
    if( !( cap.capabilities & V4L2_CAP_READWRITE ) )
      throw Util::Exception( UTIL_LOCSTRM << "'" << devPath_ << "' is not able to stream data (via read() syscall)" );
  }

  // crop, cap and crap... this can fail - doesn't matter. errors will be ignored here
  {
    // crop c(r)ap...
    v4l2_cropcap cropcap;
    zeroMemory(cropcap);
    cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    callIoctlImpl( __FILE__, __LINE__, dev_.get(), VIDIOC_CROPCAP, &cropcap, false );    // set
    // crop setup
    v4l2_crop crop;
    crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    crop.c    = cropcap.defrect;                          // default
    callIoctlImpl( __FILE__, __LINE__, dev_.get(), VIDIOC_S_CROP, &crop, false );        // set
  }

  // setup format
  {
    v4l2_format fmt;
    zeroMemory(fmt);
    fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;      // :)
    fmt.fmt.pix.width       = 1200;                             // TODO...
    fmt.fmt.pix.height      = 1200;                             // TODO...
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24;               // use RGB color plane
    fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
    callIoctl( dev_.get(), VIDIOC_S_FMT, &fmt );
    // set sizes according to the parameters read
    width_           = fmt.fmt.pix.width;
    height_          = fmt.fmt.pix.height;
    bytesPerLine_    = std::max( fmt.fmt.pix.bytesperline, width_*3 );  // fix buggy driver's results, if needed
    imageBufferSize_ = height_ * bytesPerLine_;
    assert( width_ <= bytesPerLine_ && "data line is shorter than image line!" );
    assert( width_ * height_ * 3 <= imageBufferSize_ && "output image buffer is too small" );
  }
}


cv::Mat FrameGrabberV4L::toRGB(ImageBuffer img) const
{
  const auto size = cv::Size(width_, height_);
  cv::Mat out( size, CV_8UC3, img.get(), bytesPerLine_ );       // get data buffer
  img.release();                                                // ownership is passed
  return out;
}

}
