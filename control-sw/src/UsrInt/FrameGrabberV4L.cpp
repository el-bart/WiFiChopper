#include <iostream>
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
cerr << ".";                    
    const ssize_t ret = v4l2_read( dev_.get(), img.get(), left );
    if( ret == -1 )
      throw Util::Exception( UTIL_LOCSTRM << "read of " << imageBufferSize_ << " bytes failed: " << strerror(errno) );
    cerr<<"("<<ret<<")";
    left -= ret;
  }
  while( left > 0 );
cerr << "ok!" << endl;              

  // convert to RGB
  cv::Mat tmp = toRGB( std::move(img) );

  // return image to the user
  return tmp;
}


Util::UniqueDescriptor FrameGrabberV4L::openDevice(void) const
{
  Util::UniqueDescriptor fd( v4l2_open( devPath_.c_str(), O_RDWR | O_NONBLOCK ) );
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
    // is it a video capturing device indeed?
    if( !( cap.capabilities & V4L2_CAP_VIDEO_CAPTURE ) )
      throw Util::Exception( UTIL_LOCSTRM << "'" << devPath_ << "' is not a V4Lv2 capture device" );
    // can we read/write it?
    if( !( cap.capabilities & V4L2_CAP_READWRITE ) )
      throw Util::Exception( UTIL_LOCSTRM << "'" << devPath_ << "' is not able to stream data (via read() syscall)" );
    else
      cout << "can read()/write()" << endl;
    // can we mmap it?
    if( !( cap.capabilities & V4L2_CAP_STREAMING ) )
      throw Util::Exception( UTIL_LOCSTRM << "'" << devPath_ << "' is not able to stream data (via read() syscall)" );
    else
      cout << "can mmap() (i.e. stream)" << endl;
  }

  // input enumeration and selection
  {
    // enumerate over all of the devices
    int inputCount = 0;
    for(int index=0; ; ++index)
    {
      v4l2_input input;
      zeroMemory(input);
      input.index = index;
      if( callIoctlImpl( __FILE__, __LINE__, dev_.get(), VIDIOC_ENUMINPUT, &input, false ) < 0 )    // is there another input?
        break;
      ++inputCount;
      // output this on the screen
      cout << "INPUT " << index << ":" << endl;
      cout << "\tname: " << input.name << endl;
      cout << "\ttype: ";
      if( input.type & V4L2_INPUT_TYPE_CAMERA ) cout << "camera ";
      if( input.type & V4L2_INPUT_TYPE_TUNER  ) cout << "tuner ";
      cout << endl;
      cout << "\tstat: ";
      if( input.status & V4L2_IN_ST_NO_POWER  ) cout << "no_power ";
      if( input.status & V4L2_IN_ST_NO_SIGNAL ) cout << "no_signal ";
      if( input.status & V4L2_IN_ST_NO_COLOR  ) cout << "no_color ";
      cout << endl;
      cout << "\tstds: " << input.std << endl;
    }

    // just select the first input in a row
    int inputNo = 0;
    cout << "choosing input " << inputNo << endl;
    callIoctl( dev_.get(), VIDIOC_S_INPUT, &inputNo );
  }

  // standard enumeration and selection
  {
    // enumerate over all of the devices
    bool        hasStandard   = false;
    v4l2_std_id standardId    = 0x42;   // ==wtf ;)
    int         standardCount = 0;
    for(int index=0; ; ++index)
    {
      v4l2_standard standard;
      zeroMemory(standard);
      standard.index = index;
      if( callIoctlImpl( __FILE__, __LINE__, dev_.get(), VIDIOC_ENUMSTD, &standard, false ) < 0 )    // is there another input?
        break;
      hasStandard = true;
      standardId  = standard.id;
      ++standardCount;
      // output this on the screen
      // TODO: if( input.std & standard.id )  to see if it is supported or not
      cout << "STANDARD " << index << ":" << endl;
      cout << "\tid:   " << standard.id << endl;
      cout << "\tname: " << standard.name << endl;
      cout << "\tfrpr: " << standard.frameperiod.numerator / double(standard.frameperiod.denominator) << endl;
    }

    if( hasStandard )
    {
      // just select the first input in a row
      cout << "choosing standard " << standardId << endl;
      callIoctl( dev_.get(), VIDIOC_S_STD, &standardId );
    }
    else
      cout << "no standards found" << endl;
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
    fmt.fmt.pix.field       = V4L2_FIELD_NONE;
    fmt.fmt.pix.colorspace  = V4L2_COLORSPACE_SRGB;
    callIoctl( dev_.get(), VIDIOC_S_FMT, &fmt );
    // set sizes according to the parameters read
    cout << "IMAGE FORMAT NEGOCIATED:" << endl;
    cout << "\tsize  = " << fmt.fmt.pix.width << "x" << fmt.fmt.pix.height << endl;
    cout << "\tbpl   = " << fmt.fmt.pix.bytesperline << endl;
    const uint32_t pix    = fmt.fmt.pix.pixelformat;
    const char     tab[5] = { static_cast<char>((pix>>0)&0xFF), static_cast<char>((pix>>8)&0xFF), static_cast<char>((pix>>16)&0xFF), static_cast<char>((pix>>24)&0xFF), 0 };
    cout << "\tpixf  = " << tab << endl;
    cout << "\tfield = " << fmt.fmt.pix.field << endl;
    cout << "\timgsz = " << fmt.fmt.pix.sizeimage << endl;
    cout << "\tcspc  = " << fmt.fmt.pix.colorspace << endl;
    // write down info
    width_           = fmt.fmt.pix.width;
    height_          = fmt.fmt.pix.height;
    bytesPerLine_    = fmt.fmt.pix.bytesperline;
    imageBufferSize_ = fmt.fmt.pix.sizeimage;
    assert( width_ <= bytesPerLine_ && "data line is shorter than image line!" );
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
