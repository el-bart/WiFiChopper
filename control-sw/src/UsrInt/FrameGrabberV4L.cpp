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
  startCapture();
}


cv::Mat FrameGrabberV4L::grabImpl(void)
{
  // check if the device is ready
  // TODO: hardcoded timeout value....
  if( !Util::timedSelect( dev_.get(), 4.0 ) )
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
  cv::Mat tmp = toRGB(mm);

  // return buffer to the driver
  // TODO: this place is not exception safe - if toRGB() throws this buffer will never be
  //       enqueued back to the driver (handle will not be lost however)
  callIoctl( dev_.get(), VIDIOC_QBUF, &buf );

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
  v4l2_capability cap;
  callIoctl( dev_.get(), VIDIOC_QUERYCAP, &cap );
  if( !( cap.capabilities & V4L2_CAP_VIDEO_CAPTURE ) )
    throw Util::Exception( UTIL_LOCSTRM << "'" << devPath_ << "' is not a V4Lv2 capture device" );
  if( !( cap.capabilities & V4L2_CAP_STREAMING ) )
    throw Util::Exception( UTIL_LOCSTRM << "'" << devPath_ << "' is not able to stream data (mmap)" );

#if 0
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
#endif

  // setup format
  v4l2_format fmt;
  zeroMemory(fmt);
  fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  fmt.fmt.pix.width       = 0;                              // autodetect
  fmt.fmt.pix.height      = 0;                              // autodetect
  //fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;              // shitty, but usually works... :/
  fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24;
  fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
  callIoctl( dev_.get(), VIDIOC_S_FMT, &fmt );
  // set width and height according to the parameters read
  width_  = fmt.fmt.pix.width;
  height_ = fmt.fmt.pix.height;

  // initialize buffers
  v4l2_requestbuffers req;
  zeroMemory(req);
  req.count  = 3;
  req.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory = V4L2_MEMORY_MMAP;
  callIoctl( dev_.get(), VIDIOC_REQBUFS, &req );
  if( req.count < 2 )
    throw Util::Exception( UTIL_LOCSTRM << "requesting bufferes failed: got only " << req.count << " buffers" );
std::cerr << "1BUFFERS size: " << buffers_.size() << std::endl;
  buffers_.reserve( req.count );
std::cerr << "2BUFFERS size: " << buffers_.size() << std::endl;
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
std::cerr << "LEN: " << buf.length << " at offset " << buf.m.offset << std::endl;                           
    MMem mm( buf.length, PROT_READ|PROT_WRITE, MAP_SHARED, dev_.get(), buf.m.offset );
    // hol buffer locally
std::cerr << "xBUFFERS size: " << buffers_.size() << std::endl;
    buffers_.push_back( std::move(mm) );
std::cerr << "yBUFFERS size: " << buffers_.size() << std::endl;
  }
std::cerr << "BUFFERS size: " << buffers_.size() << std::endl;
std::cerr << "BUFFERS: " << req.count << std::endl;

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
std::cerr << "quering buffer " << i << std::endl;                       
    callIoctl( dev_.get(), VIDIOC_QBUF, &buf );
  }

  // do the evolution! :)
  v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  callIoctl( dev_.get(), VIDIOC_STREAMON, &type );
}


namespace
{
inline uint8_t normalize(const double v)
{
  constexpr int minVal = 0x00;
  constexpr int maxVal = 0xFF;
  const double  nv     = std::max<double>( minVal, std::min<double>( v, maxVal ) );
  return static_cast<uint8_t>(nv);
}

inline void yuv422torgb888(uint8_t* out, const uint8_t y, const uint8_t u, const uint8_t v)
{
  out[0] = normalize( 1*y +       0*u + 1.13983*v );
  out[1] = normalize( 1*y - 0.39465*u - 0.58060*v );
  out[2] = normalize( 1*y + 2.03211*u -       0*v );
}
} // unnamed namespace

cv::Mat FrameGrabberV4L::toRGB(MMem& mm) const
{
  cv::Mat tmp(height_, width_, CV_8UC3, mm.mem(), true);
  return tmp;

  cv::Mat dst(height_, width_, CV_8UC3);    // RGB24 output image
  const size_t   imgBufSize = width_ * height_ * 3;   // WxHxRGB
  const size_t   end        = imgBufSize - (imgBufSize%6);   // to prevent reaching memory out of bound
  const uint8_t* pData      = static_cast<const uint8_t*>( mm.mem() );
  uint8_t*       out        = static_cast<uint8_t*>( dst.data );
  for(size_t i = 0, j=0; i < end; i+=6, j+=4)
  {
    const uint8_t y1 = pData[j+0];
    const uint8_t u  = pData[j+1];
    const uint8_t y2 = pData[j+2];
    const uint8_t v  = pData[j+3];
    yuv422torgb888(out+i+0, y1, u, v);
    yuv422torgb888(out+i+3, y2, u, v);
    /*
    out[i+0] = pData[j+0] + pData[j+3]*((1 - 0.299)/0.615);
    out[i+1] = pData[j+0] - pData[j+1]*((0.114*(1-0.114))/(0.436*0.587)) - pData[j+3]*((0.299*(1 - 0.299))/(0.615*0.587));
    out[i+2] = pData[j+0] + pData[j+1]*((1 - 0.114)/0.436);

    out[i+3] = pData[j+2] + pData[j+3]*((1 - 0.299)/0.615);
    out[i+4] = pData[j+2] - pData[j+1]*((0.114*(1-0.114))/(0.436*0.587)) - pData[j+3]*((0.299*(1 - 0.299))/(0.615*0.587));
    out[i+5] = pData[j+2] + pData[j+1]*((1 - 0.114)/0.436);
    */
  }

  return dst;

#if 0
  cv::Mat src(height_, width_, CV_8UC2, mm.mem() );
  cv::Mat dst(height_, width_, CV_8UC3);                                // create new output image
  //IplImage src = frame;
  //cv::Mat src(height_, width_, , mm.mem()
  //IplImage* src = cvCreateImageHeader( cvSize(width_, height_), dep, channels );
  /*
  IplImage src;                                                         // source image description (to be filled)
  cvInitImageHeader( &src, cvSize(width_, height_), IPL_DEPTH_8U, 2);   // init heder w/o allocating data
  cvSetData( &src, mm.mem(), width_*2 );                                // add data pointer
  cv::Mat dst(height_, width_, CV_8UC3);                                // create new output image
  IplImage dstIpl = dst;                                                // perform no the fly convertion
  */
  cv::cvtColor( src, dst, CV_YCrCb2RGB );                            // convert
  src.data = nullptr;
  return dst;                                                           // return
  //cvCvtColor(src, dsc, CV_YCbCr2RGB) 
  // TODO
#endif
}

}
