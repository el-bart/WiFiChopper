#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <cerrno>
#include <cstring>

#include "UsrInt/Display.hpp"


namespace UsrInt
{

namespace
{
int makeFlipDir(bool fh, bool fv)
{
  if(fh && fv)  // H + V
    return -1;
  if(fh)        // H only
    return 0;
  if(fv)        // V only
    return 1;
  return 0x42;  // doesn't matter ;)
}


std::string prepareUniqueMovieName(void)
{
  // get current time
  const time_t tt = time(nullptr);
  struct tm    bt;
  if( gmtime_r(&tt, &bt) != &bt )
    throw Util::Exception( UTIL_LOCSTRM << "gmtime_r() failed to decomose time: " << strerror(errno) );
  // create filename
  std::stringstream ss;
#define PAD_ZERO std::setfill('0') << std::setw(2)
  ss << "flight_record_"                                                                        // common begin
     << 1900+bt.tm_year << "-" << PAD_ZERO << 1+bt.tm_mon << "-" << PAD_ZERO << bt.tm_mday      // date
     << "_"                                                                                     // separator
     << PAD_ZERO << bt.tm_hour << ":" << PAD_ZERO << bt.tm_min << ":" << PAD_ZERO << bt.tm_sec  // time
     << ".avi";                                                                                 // extension
#undef PAD_ZERO
  // return file name
  return ss.str();
}
} // unnamed namespace


Display::Display(FrameGrabberPtr fg, bool flipH, bool flipV):
  fg_( std::move(fg) ),
  vw_( prepareUniqueMovieName(), *fg_ ),
  txt_( 250, 30, CV_RGB(0,255,0), 1.3 ),
  flip_( flipH || flipV ),
  flipDir_( makeFlipDir(flipH, flipV) )
{ }


void Display::update(const Info& info)
{
  // get next frame
  cv::Mat in = fg_->grab(0.100);        // 0.1[s] is max we can wait
  cv::Mat frame;                        // rotated frame
  // perform rotation, if needed
  if(flip_)
    cv::flip(in, frame, flipDir_);
  else
    frame = in;

  // add battery info
  {
    std::stringstream ss;
    ss << std::setprecision(2) << std::fixed;
    ss << "Batt.:  " << info.batteryVoltage_ << "V";
    txt_.addLine( frame, 0, ss.str() );
  }

  // add accelereation vector
  {
    std::stringstream ss;
    ss << std::setprecision(2) << std::fixed;
    ss << "Accel.: " << info.acceleration_.x_ << " "
                     << info.acceleration_.y_ << " "
                     << info.acceleration_.z_;
    txt_.addLine( frame, 1, ss.str() );
  }

  // add acceleration value
  {
    std::stringstream ss;
    ss << std::setprecision(2) << std::fixed;
    ss << "Accel.: " << info.acceleration_.norm() << "G";
    txt_.addLine( frame, 2, ss.str() );
  }

  // display new frame
  imshow( "WiFiChopper", frame );

  // write it to the file
  vw_.writeFrame(frame);
}

}
