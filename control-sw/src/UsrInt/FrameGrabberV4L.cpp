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
#include <libv4l2.h>

#include "UsrInt/FrameGrabberV4L.hpp"
#include "Util/timedSelect.hpp"

using namespace std;


namespace UsrInt
{

namespace
{

int callIoctlImpl(const char* file, const unsigned line, int fd, int request, void* arg, bool checkForError = true)
{
  int r;
  // ignore interrupts
  do
  { r = v4l2_ioctl(fd, request, arg); }
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


FrameGrabberV4L::FrameGrabberV4L(std::string devPath, const size_t width, const size_t height):
  devPath_( std::move(devPath) ),
  dev_( openDevice() )
{
  init( width, height );
  startCapture();
}


cv::Mat FrameGrabberV4L::grabImpl(void)
{
  // check if the device is ready
  // TODO: hardcoded timeout value....
  if( !Util::timedSelect( dev_.get(), 1.0 ) )
    throw Util::Exception( UTIL_LOCSTRM << "timeout occured while waiting for the image" );

  // get buffer from the device
  v4l2_buffer buf;
  zeroMemory(buf);
  buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  buf.memory = V4L2_MEMORY_MMAP;
  callIoctl( dev_.get(), VIDIOC_DQBUF, &buf );

  // get proper buffer
  if( buf.index >= buffers_.size() )
    throw std::logic_error("index returned by ioctl() is out of bound - wtf?!");
  MMem& mm = buffers_[buf.index];

  // convert to RGB
  cv::Mat tmp = toRGB( mm.mem(), buf.length );

  // return buffer to the driver
  // TODO: this place is not exception safe - if toRGB() throws this buffer will never be
  //       enqueued back to the driver (handle will not be lost however)
  callIoctl( dev_.get(), VIDIOC_QBUF, &buf );

  // return image to the user
  return tmp;
}


cv::Size FrameGrabberV4L::sizeImpl(void)
{
  return cv::Size( width_, height_ );
}


Util::UniqueDescriptor FrameGrabberV4L::openDevice(void) const
{
  Util::UniqueDescriptor fd( v4l2_open( devPath_.c_str(), O_RDWR | O_NONBLOCK ) );
  if( fd.get() == -1 )
    throw Util::Exception( UTIL_LOCSTRM << "unable to open video device '" << devPath_ << "': " << strerror(errno) );
  return fd;
}


void FrameGrabberV4L::init(const size_t width, const size_t height)
{
  // check device's capabilities
  v4l2_capability cap;
  callIoctl( dev_.get(), VIDIOC_QUERYCAP, &cap );
  if( !( cap.capabilities & V4L2_CAP_VIDEO_CAPTURE ) )
    throw Util::Exception( UTIL_LOCSTRM << "'" << devPath_ << "' is not a V4Lv2 capture device" );
  if( !( cap.capabilities & V4L2_CAP_STREAMING ) )
    throw Util::Exception( UTIL_LOCSTRM << "'" << devPath_ << "' is not able to stream data (mmap)" );

  // crop c(r)ap... - errors are ignored here
  v4l2_cropcap cropcap;
  zeroMemory(cropcap);
  cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  callIoctlImpl( __FILE__, __LINE__, dev_.get(), VIDIOC_CROPCAP, &cropcap, false );    // query
  // crop setup - errors are ignored here
  v4l2_crop crop;
  crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  crop.c    = cropcap.defrect;                          // default
  callIoctlImpl( __FILE__, __LINE__, dev_.get(), VIDIOC_S_CROP, &crop, false );        // set

  // setup format
  constexpr auto pixelFormat = V4L2_PIX_FMT_RGB24;
  v4l2_format fmt;
  zeroMemory(fmt);
  fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  fmt.fmt.pix.width       = width;                            // driver may change it
  fmt.fmt.pix.height      = height;                           // ...
  fmt.fmt.pix.pixelformat = pixelFormat;
  fmt.fmt.pix.field       = V4L2_FIELD_NONE;
  fmt.fmt.pix.colorspace  = V4L2_COLORSPACE_SRGB;
  callIoctl( dev_.get(), VIDIOC_S_FMT, &fmt );
  // write down info
  width_        = fmt.fmt.pix.width;
  height_       = fmt.fmt.pix.height;
  bytesPerLine_ = fmt.fmt.pix.bytesperline;
  if( fmt.fmt.pix.pixelformat != pixelFormat )
    throw Util::Exception( UTIL_LOCSTRM << "libv4l2 was not able to output in " << pixelFormat << " mode - aborting..." );

  // initialize buffers
  v4l2_requestbuffers req;
  zeroMemory(req);
  req.count  = 5;
  req.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory = V4L2_MEMORY_MMAP;
  callIoctl( dev_.get(), VIDIOC_REQBUFS, &req );
  const size_t reqBufCnt = 5;
  if( req.count < reqBufCnt )
    throw Util::Exception( UTIL_LOCSTRM << "requesting bufferes failed: got only " << req.count << " out of requested " << reqBufCnt << " buffers" );
  buffers_.reserve( req.count );
  // mmap buffers
  for(size_t i = 0; i < req.count; ++i)
  {
    // request next buffer
    v4l2_buffer buf;
    zeroMemory(buf);
    buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index  = i;
    callIoctl( dev_.get(), VIDIOC_QUERYBUF, &buf );
    // mmap buffer
    MMem mm( buf.length, PROT_READ|PROT_WRITE, MAP_SHARED, dev_.get(), buf.m.offset );
    // hol buffer locally
    buffers_.push_back( std::move(mm) );
  }

  assert( buffers_.size() == req.count );
}


void FrameGrabberV4L::startCapture(void)
{
  // add all buffer to the queue of the driver
  for(size_t i = 0; i < buffers_.size(); ++i)
  {
    v4l2_buffer buf;
    zeroMemory(buf);
    buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index  = i;
    callIoctl( dev_.get(), VIDIOC_QBUF, &buf );
  }

  // do the evolution! :)
  v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  callIoctl( dev_.get(), VIDIOC_STREAMON, &type );
}


cv::Mat FrameGrabberV4L::toRGB(void* mem, const size_t length) const
{
  assert( width_*height_*3 <= length );
  assert( width_*3 <= bytesPerLine_ );
  const cv::Size size(width_, height_);
  const cv::Mat  tmp( size, CV_8UC3, mem, bytesPerLine_ );  // catch this for easier processing
  cv::Mat        out;               // output image
  cvtColor(tmp, out, CV_RGB2BGR);   // tranformation from RGB (delivered by driver) to BGR (used internally by OpenCV)
  return out;
}

}
